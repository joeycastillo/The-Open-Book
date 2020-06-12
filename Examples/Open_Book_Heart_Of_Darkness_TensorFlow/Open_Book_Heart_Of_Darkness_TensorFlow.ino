#include <OpenBook.h>
#include <MenuSystem.h>
#include <TensorFlowLite.h>
#include <Arduino.h>

#include "text.h"

// definitions for book and layout
#define BOOK_TITLE "Heart of\nDarkness"
#define BOOK_AUTHOR "Joseph Conrad"
#define BOOK_TEXT heart_of_darkness
#define MAX_LINES 20

// globals
OpenBook *book;
int page = 0;
int lastPage = -1;
bool menuUpdated = false;

#include "main_functions.h"

#include "audio_provider.h"
#include "command_responder.h"
#include "feature_provider.h"
#include "micro_features_micro_model_settings.h"
#include "micro_features_tiny_conv_micro_features_model_data.h"
#include "recognize_commands.h"
#include "tensorflow/lite/experimental/micro/kernels/micro_ops.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/experimental/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* model_input = nullptr;
FeatureProvider* feature_provider = nullptr;
RecognizeCommands* recognizer = nullptr;
int32_t previous_time = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 10 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

#include "menu.h"

void nextPage(MenuComponent* p_menu_component = NULL) {
    page++;
}

void previousPage(MenuComponent* p_menu_component = NULL) {
  page--;
}

MenuSystem ms(my_renderer);
MenuItem mm_mi1(" Previous ", &previousPage);
MenuItem mm_mi2(" Next ", &nextPage);

void setup()
{
  Serial.begin(115200);

  // the OpenBook class provides an interface for the peripherals of either the Open Book or eBook Wing.
  book = new OpenBook();
  
  book->configureScreen();
  // pause before initializing Babel; on the wing they share a bus and I've seen things get garbled if you don't give it a hot second.
  delay(500);
  // Babel requires a GD25Q16C chip flashed with the latest babel.bin file. See BurnBabelBurn for details: https://github.com/joeycastillo/babel/blob/master/examples/BurnBabelBurn/BurnBabelBurn.ino
  book->configureBabel();
  
  #if defined(ODDLY_SPECIFIC_OPEN_BOOK)
  // the buttons on the Open Book board are on a shift register
  book->configureShiftButtons();
  #else // on the wing, they're on an I2C port expander.
  book->configureI2CButtons();
  #endif
  
  book->getTypesetter()->setTextColor(EPD_BLACK);
  book->getTypesetter()->setLayoutArea(16, 16, 264, 368);
  book->getTypesetter()->setWordWrap(true);
  book->getDisplay()->setRotation(0);

  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);




  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_tiny_conv_micro_features_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //
  // tflite::ops::micro::AllOpsResolver resolver;
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver micro_mutable_op_resolver;
  micro_mutable_op_resolver.AddBuiltin(
      tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
      tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
  micro_mutable_op_resolver.AddBuiltin(
      tflite::BuiltinOperator_FULLY_CONNECTED,
      tflite::ops::micro::Register_FULLY_CONNECTED());
  micro_mutable_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
                                       tflite::ops::micro::Register_SOFTMAX());

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_mutable_op_resolver, tensor_arena, kTensorArenaSize,
      error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  model_input = interpreter->input(0);
  if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != kFeatureSliceCount) ||
      (model_input->dims->data[2] != kFeatureSliceSize) ||
      (model_input->type != kTfLiteUInt8)) {
    error_reporter->Report("Bad input tensor parameters in model");
    return;
  }

  // Prepare to access the audio spectrograms from a microphone or other source
  // that will provide the inputs to the neural network.
  // NOLINTNEXTLINE(runtime-global-variables)
  static FeatureProvider static_feature_provider(kFeatureElementCount,
                                                 model_input->data.uint8);
  feature_provider = &static_feature_provider;

  static RecognizeCommands static_recognizer(error_reporter);
  recognizer = &static_recognizer;

  previous_time = 0;
}

void loop()
{
  handleVoice();
  handleButtons();
  drawPageIfNeeded();
}

void handleVoice() {
  // Fetch the spectrogram for the current time.
  const int32_t current_time = LatestAudioTimestamp();
  int how_many_new_slices = 0;
  TfLiteStatus feature_status = feature_provider->PopulateFeatureData(
      error_reporter, previous_time, current_time, &how_many_new_slices);
  if (feature_status != kTfLiteOk) {
    error_reporter->Report("Feature generation failed");
    return;
  }
  previous_time = current_time;
  // If no new audio samples have been received since last time, don't bother
  // running the network model.
  if (how_many_new_slices == 0) {
    return;
  }

  // Run the model on the spectrogram input and make sure it succeeds.
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed");
    return;
  }

  // Obtain a pointer to the output tensor
  TfLiteTensor* output = interpreter->output(0);
  // Determine whether a command was recognized based on the output of inference
  const char* found_command = nullptr;
  uint8_t score = 0;
  bool is_new_command = false;
  TfLiteStatus process_status = recognizer->ProcessLatestResults(
      output, current_time, &found_command, &score, &is_new_command);
  if (process_status != kTfLiteOk) {
    error_reporter->Report("RecognizeCommands::ProcessLatestResults() failed");
    return;
  }
  // Do something based on the recognized command. The default implementation
  // just prints to the error console, but you should replace this with your
  // own function for a real application.
  RespondToCommand(error_reporter, current_time, found_command, score,
                   is_new_command);
}

void handleButtons() {
  uint8_t buttons = book->readButtons();
  if (buttons & OPENBOOK_BUTTONMASK_PREVIOUS) {
     previousPage();
  }
  if (buttons & OPENBOOK_BUTTONMASK_NEXT) {
    nextPage();
  }
  if (buttons & OPENBOOK_BUTTONMASK_LEFT) {
      ms.prev();
      ms.display();
      menuUpdated = true;
  }
  if (buttons & OPENBOOK_BUTTONMASK_RIGHT) {
      ms.next();
      ms.display();
      menuUpdated = true;
  }
  if (buttons & OPENBOOK_BUTTONMASK_SELECT) {
      ms.select();
      ms.display();
  }

  if (page < 0) page = 0;
}

void drawPageIfNeeded() {
  if (page != lastPage || menuUpdated)
  {
    menuUpdated = false;
    Adafruit_EPD *display = book->getDisplay();
    // the Babel typesetter has its own reference to the display, which it uses to display multilingual text.
    BabelTypesetter *typesetter = book->getTypesetter();
    BabelDevice *babel = typesetter->getBabel();

    display->clearBuffer();
    ms.display();
    lastPage = max(page, 0);
    if (page == 0)
    {
      display->fillScreen(EPD_WHITE);
      typesetter->setTextColor(EPD_BLACK);
      typesetter->setTextSize(3);
      typesetter->setCursor(16, 16);
      typesetter->print("\n");
      typesetter->print(BOOK_TITLE);
      typesetter->print("\n\n");
      typesetter->setTextSize(2);
      typesetter->print(BOOK_AUTHOR);
    } else
    {
      display->fillScreen(EPD_WHITE);
      typesetter->setTextColor(EPD_BLACK);
      typesetter->setTextSize(1);
      int start = (page - 1) * MAX_LINES;
      typesetter->setCursor(16, 16);
      for(int i = start; i < start + MAX_LINES; i++)
      {
        typesetter->print((char *)BOOK_TEXT[i]);
        typesetter->print("\n");
      }
    }
    display->display();
  }
}

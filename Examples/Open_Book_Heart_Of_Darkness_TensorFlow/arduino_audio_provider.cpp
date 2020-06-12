/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "audio_provider.h"
#include "micro_features_micro_model_settings.h"
#include <Arduino.h>
#include <Adafruit_ZeroTimer.h>

static Adafruit_ZeroTimer zerotimer = Adafruit_ZeroTimer(4);

void TC4_Handler() {
  Adafruit_ZeroTimer::timerHandler(4);
}

#define USE_EXTERNAL_MIC A10  // D2 on pybadge
//#define USE_EDGEBADGE_PDMMIC
//#define AUDIO_OUT A0         // uncomment to 'echo' audio to A0 for debugging
#define DEFAULT_BUFFER_SIZE 512

#if defined(USE_EDGEBADGE_PDMMIC)
  #include <Adafruit_ZeroPDMSPI.h>
  #define PDM_SPI            SPI2    // PDM mic SPI peripheral
  #define TIMER_CALLBACK     SERCOM3_0_Handler
  Adafruit_ZeroPDMSPI pdmspi(&PDM_SPI);
#endif

namespace {
bool g_is_audio_initialized = false;
// An internal buffer able to fit 16x our sample size
constexpr int kAudioCaptureBufferSize = DEFAULT_BUFFER_SIZE * 16;
int16_t g_audio_capture_buffer[kAudioCaptureBufferSize];
// A buffer that holds our output
int16_t g_audio_output_buffer[kMaxAudioSampleSize];
// Mark as volatile so we can check in a while loop to see if
// any samples have arrived yet.
volatile int32_t g_latest_audio_timestamp = 0;
// Our callback buffer for collecting a chunk of data
volatile int16_t recording_buffer[DEFAULT_BUFFER_SIZE];

volatile int max_audio = -32768, min_audio = 32768;
}  // namespace


void CaptureSamples();

void TIMER_CALLBACK() {
  static bool ledtoggle = false;
  static uint32_t audio_idx = 0;
  int32_t sample = 0;

#if defined(USE_EDGEBADGE_PDMMIC)
  uint16_t read_pdm;
  if (!pdmspi.decimateFilterWord(&read_pdm)) {
    return; // not ready for data yet!
  }
  sample = read_pdm;
#endif
  
  if (audio_idx >= DEFAULT_BUFFER_SIZE) {
    CaptureSamples();
    max_audio = -32768, min_audio = 32768;
    audio_idx = 0;
  }

  // tick tock test
  //digitalWrite(LED_BUILTIN, ledtoggle);
  //ledtoggle = !ledtoggle;
#if defined(USE_EXTERNAL_MIC)
  sample = analogRead(USE_EXTERNAL_MIC);
  sample -= 2047; // 12 bit audio unsigned  0-4095 to signed -2048-~2047
#endif
#if defined(USE_EDGEBADGE_PDMMIC)
  sample -= 32676; // from 0-65535 to -32768 to 32768
#endif
#if defined(AUDIO_OUT)
  analogWrite(AUDIO_OUT, sample+2048); 
#endif

  recording_buffer[audio_idx] = sample;
  max_audio = max(max_audio, sample);
  min_audio = min(min_audio, sample);
  audio_idx++;
}

// borrowed from Arcada
bool timerCallback(float freq, void (*callback)()) {
  Serial.printf("Desired freq: %f Hz\n", freq);
  uint16_t divider = 1;
  uint16_t compare = 0;
  tc_clock_prescaler prescaler = TC_CLOCK_PRESCALER_DIV1;

  if ((freq < 24000000) && (freq > 800)) {
    divider = 1;
    prescaler = TC_CLOCK_PRESCALER_DIV1;
    compare = 48000000/freq;
  } else if (freq > 400) {
    divider = 2;
    prescaler = TC_CLOCK_PRESCALER_DIV2;
    compare = (48000000/2)/freq;
  } else if (freq > 200) {
    divider = 4;
    prescaler = TC_CLOCK_PRESCALER_DIV4;
    compare = (48000000/4)/freq;
  } else if (freq > 100) {
    divider = 8;
    prescaler = TC_CLOCK_PRESCALER_DIV8;
    compare = (48000000/8)/freq;
  } else if (freq > 50) {
    divider = 16;
    prescaler = TC_CLOCK_PRESCALER_DIV16;
    compare = (48000000/16)/freq;
  } else if (freq > 12) {
    divider = 64;
    prescaler = TC_CLOCK_PRESCALER_DIV64;
    compare = (48000000/64)/freq;
  } else if (freq > 3) {
    divider = 256;
    prescaler = TC_CLOCK_PRESCALER_DIV256;
    compare = (48000000/256)/freq;
  } else if (freq >= 0.75) {
    divider = 1024;
    prescaler = TC_CLOCK_PRESCALER_DIV1024;
    compare = (48000000/1024)/freq;
  } else {
    return false;
  }
  Serial.printf("Divider %d / compare %d -> %d Hz\n", 
         divider, compare, (48000000/compare));
  
  zerotimer.enable(false);
  zerotimer.configure(prescaler, // prescaler
          TC_COUNTER_SIZE_16BIT,   // bit width of timer/counter
          TC_WAVE_GENERATION_MATCH_PWM // frequency or PWM mode
          );

  zerotimer.setCompare(0, compare);
  zerotimer.setCallback(true, TC_CALLBACK_CC_CHANNEL0, callback);
  zerotimer.enable(true);
  return true;
}

TfLiteStatus InitAudioRecording(tflite::ErrorReporter* error_reporter) {
  //while (!Serial) yield();
  Serial.begin(115200);

  // Hook up the callback that will be called with each sample
#if defined(USE_EXTERNAL_MIC)
  timerCallback(kAudioSampleFrequency, TIMER_CALLBACK);
  analogReadResolution(12);
#endif
#if defined(USE_EDGEBADGE_PDMMIC)
  pdmspi.begin(kAudioSampleFrequency);
  Serial.print("Final PDM frequency: "); Serial.println(pdmspi.sampleRate);
#endif
#if defined(AUDIO_OUT)  
  analogWriteResolution(12);
#endif
  // Block until we have our first audio sample
  while (!g_latest_audio_timestamp) {
  }

  return kTfLiteOk;
}



void CaptureSamples() {
  // This is how many bytes of new data we have each time this is called
  const int number_of_samples = DEFAULT_BUFFER_SIZE;
  // Calculate what timestamp the last audio sample represents
  const int32_t time_in_ms =
      g_latest_audio_timestamp +
      (number_of_samples / (kAudioSampleFrequency / 1000));
  // Determine the index, in the history of all samples, of the last sample
  const int32_t start_sample_offset =
      g_latest_audio_timestamp * (kAudioSampleFrequency / 1000);
  // Determine the index of this sample in our ring buffer
  const int capture_index = start_sample_offset % kAudioCaptureBufferSize;
  // Read the data to the correct place in our buffer, note 2 bytes per buffer entry
  memcpy(g_audio_capture_buffer + capture_index, (void *)recording_buffer, DEFAULT_BUFFER_SIZE*2);
  // This is how we let the outside world know that new audio data has arrived.
  g_latest_audio_timestamp = time_in_ms;

  int peak = (max_audio - min_audio);
//  Serial.printf("pp %d\n", peak);
  //int normalized = map(peak, 20, 2000, 0, 65535);
}

TfLiteStatus GetAudioSamples(tflite::ErrorReporter* error_reporter,
                             int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  // Set everything up to start receiving audio
  if (!g_is_audio_initialized) {
    TfLiteStatus init_status = InitAudioRecording(error_reporter);
    if (init_status != kTfLiteOk) {
      return init_status;
    }
    g_is_audio_initialized = true;
  }
  // This next part should only be called when the main thread notices that the
  // latest audio sample data timestamp has changed, so that there's new data
  // in the capture ring buffer. The ring buffer will eventually wrap around and
  // overwrite the data, but the assumption is that the main thread is checking
  // often enough and the buffer is large enough that this call will be made
  // before that happens.

  // Determine the index, in the history of all samples, of the first
  // sample we want
  const int start_offset = start_ms * (kAudioSampleFrequency / 1000);
  // Determine how many samples we want in total
  const int duration_sample_count =
      duration_ms * (kAudioSampleFrequency / 1000);
  for (int i = 0; i < duration_sample_count; ++i) {
    // For each sample, transform its index in the history of all samples into
    // its index in g_audio_capture_buffer
    const int capture_index = (start_offset + i) % kAudioCaptureBufferSize;
    // Write the sample to the output buffer
    g_audio_output_buffer[i] = g_audio_capture_buffer[capture_index];
  }

  // Set pointers to provide access to the audio
  *audio_samples_size = kMaxAudioSampleSize;
  *audio_samples = g_audio_output_buffer;

  return kTfLiteOk;
}

int32_t LatestAudioTimestamp() { return g_latest_audio_timestamp; }

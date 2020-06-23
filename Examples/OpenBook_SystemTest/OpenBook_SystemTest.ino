#include <OpenBook.h>
#include <Adafruit_SPIFlash.h>
#include <SdFat.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MP3.h>
#include "vivaldi.h" // Violin Concerto in G minor, RV 315

OpenBook *book = new OpenBook();
Adafruit_FlashTransport_QSPI qspiTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
Adafruit_SPIFlash qspiFlash(&qspiTransport);
Adafruit_FlashTransport_SPI spiTransport(OPENBOOK_BCS, &SPI);
Adafruit_SPIFlash babelFlash(&spiTransport);
SdFat sd;
Adafruit_NeoPixel pixel(1, OPENBOOK_NEOPIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_MP3 player;

uint8_t *currentPtr;
uint32_t thisManyBytesLeft;

void setup() {
  char cmd;
  
  int qspiFlashPassed = -1;
  int babelFlashPassed = -1;
  int cardDetectPassed = -1;
  int sdCardPassed = -1;
  int buttonsPassed = -1;
  int pixelPassed = -1;
  int headphonesPassed = -1;
  int inlineButtonPassed = -1;
  int micAmpPassed = -1;
  
  int startTime;
  int micValue;

  book->configureShiftButtons();
  pinMode(OPENBOOK_MIC_SHUTDOWN, OUTPUT);
  digitalWrite(OPENBOOK_MIC_SHUTDOWN, HIGH);
  
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Open Book System Test");
  Serial.println("=====================");
  Serial.println("This sketch runs interactively in the Arduino serial monitor. \nWhen prompted to take an action, take that action.");
  Serial.println("Some tests reqiure that you answer a question. \nEnter a single letter in response to each prompt.");
  Serial.println();

  Serial.println("Testing power...");
  pinMode(OPENBOOK_BATTERY_MONITOR, INPUT);
  pinMode(OPENBOOK_USB_MONITOR, INPUT);
  int batmonPassed = analogRead(OPENBOOK_BATTERY_MONITOR) > 300;
  int busmonPassed = analogRead(OPENBOOK_USB_MONITOR) > 300;

  Serial.println("Testing internal storage...");
  qspiFlash.begin();
  qspiFlashPassed = qspiFlash.size() == 2097152 || qspiFlash.size() == 4194304;

  babelFlash.begin();
  babelFlashPassed = babelFlash.size() == 2097152 || babelFlash.size() == 4194304;

  Serial.println("-----------------------------------");
  Serial.println("If you have an SD card in the slot, remove it now, then enter 'C' to continue.");
  Serial.println("To skip testing of the SD card, enter 'S'");
  if (waitForAnswer('C') == -1) goto BUTTONS;
  
  cardDetectPassed = book->sdCardState() == OPEN_BOOK_SD_CARD_NOT_PRESENT;
  Serial.println("Now insert the SD card, and enter 'C' to continue.");
  waitForAnswer('C');
  cardDetectPassed &= book->sdCardState() == OPEN_BOOK_SD_CARD_PRESENT;

  sdCardPassed = sd.begin(4, SD_SCK_MHZ(50));

BUTTONS:
  Serial.println("-----------------------------------");
  Serial.println("Do you wish to test the buttons? Enter 'Y' to test, or 'N' to skip.");
  if (waitForAnswer('Y', 'N') != 1) goto PIXEL;
  buttonsPassed = true;
  Serial.println("Press the 'Previous Page' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_PREVIOUS);
  Serial.println("Press the 'Next Page' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_NEXT);
  Serial.println("Now, let's move on to the direction pad in the middle.");
  Serial.println("Press the 'Left' button on the D-pad until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_LEFT);
  Serial.println("Press the 'Down' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_DOWN);
  Serial.println("Press the 'Up' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_UP);
  Serial.println("Press the 'Right' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_RIGHT);
  Serial.println("Press the 'Select' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_SELECT);
  Serial.println("Press the 'Lock' button until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  buttonsPassed &= waitForButton(OPENBOOK_BUTTONMASK_LOCK);

PIXEL:
  Serial.println("-----------------------------------");
  Serial.println("Testing Neopixel...");
  pixel.begin();
  pixel.setPixelColor(0, pixel.Color(150, 150, 0));
  pixel.show();
  Serial.println("Is the Neopixel showing a yellow color?");
  Serial.println("Enter 'Y' for Yes, 'N' for No, or 'S' to skip this test.");
  pixelPassed = waitForAnswer('Y', 'N'); // returns -1 if skipped, no need for a goto here.
  pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  pixel.show();

  Serial.println("-----------------------------------");
  Serial.println("If you wish to test audio output, the headset button or the microphone, \nplug a 3.5mm headset in to the headphone jack now.");
  Serial.println("Do you wish to audio output? Enter 'Y' to test, or 'N' to skip.");
  if (waitForAnswer('Y', 'N') != 1) goto INLINE_BUTTON;

  Serial.println("-----------------------------------");
  Serial.println("Testing headphones...");
  analogWrite(OPENBOOK_AUDIO_R, 2048);
  analogWrite(OPENBOOK_AUDIO_L, 2048);
  currentPtr = (uint8_t*)vivaldi_mp3;
  thisManyBytesLeft = sizeof(vivaldi_mp3);
  player.begin();
  player.setSampleReadyCallback(writeDacs);
  player.setBufferCallback(getMoreData);
  player.play();
  startTime = millis();
  while(millis() < startTime + 2000) player.tick();
  player.pause();
  Serial.println("Did you hear audio in both the left and right channels?");
  Serial.println("Enter 'Y' for Yes or 'N' for No.");
  headphonesPassed = waitForAnswer('Y', 'N');

INLINE_BUTTON:
  Serial.println("-----------------------------------");
  Serial.println("Do you wish to test the inline button? Enter 'Y' to test, or 'N' to skip.");
  if (waitForAnswer('Y', 'N') != 1) goto MICAMP;
  pinMode(OPENBOOK_MIC_RAW, INPUT_PULLUP);
  delay(500);
  Serial.println("Tap the inline button on the headset until you see 'OK' appear on the console. \nIf 'OK' does not appear, enter 'F' in the console to mark the test failed.");
  while(digitalRead(OPENBOOK_MIC_RAW) == 1) {
    if (Serial.available()) {
      if (toupper(Serial.read()) == 'F') {
        inlineButtonPassed = false;
        goto MICAMP;
      }
    }
  }
  inlineButtonPassed = true;
  Serial.println("OK\n");

MICAMP:
  Serial.println("-----------------------------------");
  Serial.println("Do you wish to the microphone amplifier?");
  Serial.println("This test will play back the output from the mic amp into the headphones \nfor five seconds, so you can listen for an echo of your voice.");
  Serial.println("Enter 'Y' to test, or 'N' to skip.");
  if (waitForAnswer('Y', 'N') != 1) goto RESULTS;
  pinMode(OPENBOOK_MIC_RAW, INPUT);
  delay(500);
  digitalWrite(OPENBOOK_MIC_SHUTDOWN, LOW);
  pinMode(A10, INPUT);
  Serial.println("Make some sounds and listen for playback in the headphones.");
  startTime = millis();
  while(millis() < startTime + 5000) {
    micValue = analogRead(OPENBOOK_MIC_AMPLIFIED);
    analogWrite(OPENBOOK_AUDIO_R, micValue * 4);
    analogWrite(OPENBOOK_AUDIO_L, micValue * 4);
  }
  digitalWrite(OPENBOOK_MIC_SHUTDOWN, HIGH);
  Serial.println("Did you hear audio from the microphone played back in the headphones?");
  Serial.println("Enter 'Y' for Yes or 'N' for No.");
  micAmpPassed = waitForAnswer('Y', 'N');

RESULTS:
  Serial.println("\n\n\n\n");
  Serial.println("   OPEN BOOK TEST RESULTS");
  Serial.println("============================");
  Serial.print("Battery Monitior......"); printlnResult(batmonPassed);
  Serial.print("VBUS Monitior........."); printlnResult(busmonPassed);
  Serial.print("QSPI Flash Chip......."); printlnResult(qspiFlashPassed);
  Serial.print("Babel Flash Chip......"); printlnResult(babelFlashPassed);
  Serial.print("SD Card Detect........"); printlnResult(cardDetectPassed);
  Serial.print("SD Card..............."); printlnResult(sdCardPassed);
  Serial.print("Buttons..............."); printlnResult(buttonsPassed);
  Serial.print("Neopixel.............."); printlnResult(pixelPassed);
  Serial.print("Headphones............"); printlnResult(headphonesPassed);
  Serial.print("Inline Button........."); printlnResult(inlineButtonPassed);
  Serial.print("Mic Amp..............."); printlnResult(micAmpPassed);
}

void loop() {
}

void printlnResult(int status) {
  switch (status) {
    case -1:
      Serial.println("Skipped");
      break;
    case 0:
      Serial.println("** FAILED **");
      break;
    case 1:
      Serial.println("Passed");
      break;
  }
  
}

int waitForAnswer(char affirmative) {
  return waitForAnswer(affirmative, 0);
}

int waitForAnswer(char affirmative, char negative) {
  char command = 0;
  while (true) {
    while (!Serial.available());
    char cmd = toupper(Serial.read());
    if (cmd == 'S') return -1;
    if (cmd == affirmative) return 1;
    if (negative && cmd == negative) return 0;
  }
}

bool waitForButton(uint8_t button) {
  while(book->readButtons() != button) {
    if (Serial.available()) {
      if (toupper(Serial.read()) == 'F') {
        return false;
      }
    }
  }

  Serial.println("OK\n");
  return true;
}

void writeDacs(int16_t l, int16_t r){
  analogWrite(OPENBOOK_AUDIO_R, map(r, -32768, 32767, 0, 4095));
  analogWrite(OPENBOOK_AUDIO_L, map(l, -32768, 32767, 0, 4095));
}

int getMoreData(uint8_t *writeHere, int thisManyBytes){
  int toWrite = min(thisManyBytesLeft, thisManyBytes);
  memcpy(writeHere, currentPtr, toWrite);
  currentPtr += toWrite;
  thisManyBytesLeft -= toWrite;
  return toWrite;
}

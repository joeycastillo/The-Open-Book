/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "OpenBook_IL91874.h"

#define EPD_CS     9
#define EPD_DC     10
#define EPD_RESET  11
#define EPD_BUSY   12

#define SRAM_CS    -1

OpenBook_IL91874 display(264, 176, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS);


void setup() {
  Serial.begin(115200);

  display.begin();
  display.setRotation(3);

  // large block of text
  display.clearBuffer();
  display.fillRect(0, 0, 176, 264, EPD_WHITE);
  display.setCursor(0, 0);
  testdrawtext("Compile time:", EPD_BLACK, EPD_WHITE);
  display.println(F(__TIME__));
  testdrawtext("BLACK on WHITE.", EPD_BLACK, EPD_WHITE);
  testdrawtext("DARK on WHITE.", EPD_DARK, EPD_WHITE);
  testdrawtext("LIGHT on WHITE.", EPD_LIGHT, EPD_WHITE);
  testdrawtext("WHITE on BLACK.", EPD_WHITE, EPD_BLACK);
  testdrawtext("LIGHT on BLACK.", EPD_LIGHT, EPD_BLACK);
  testdrawtext("DARK on BLACK.", EPD_DARK, EPD_BLACK);
  testdrawtext("DARK on LIGHT.", EPD_DARK, EPD_LIGHT);
  testdrawtext("LIGHT on DARK.", EPD_LIGHT, EPD_DARK);

  display.fillRect( 10, 200, 30, 30, EPD_BLACK);
  display.fillRect( 52, 200, 30, 30, EPD_DARK);
  display.fillRect( 94, 200, 30, 30, EPD_LIGHT);
  display.fillRect(136, 200, 30, 30, EPD_WHITE);

  display.drawRect( 10, 200, 30, 30, EPD_BLACK);
  display.drawRect( 52, 200, 30, 30, EPD_BLACK);
  display.drawRect( 94, 200, 30, 30, EPD_BLACK);
  display.drawRect(136, 200, 30, 30, EPD_BLACK);

  display.display();
  display.setDisplayMode(OPEN_BOOK_DISPLAY_MODE_GRAYSCALE);
  delay(5000);
  display.display();
}

void loop() {
  //don't do anything!
}


void testdrawtext(char *text, uint16_t color, uint16_t background) {
  display.setTextColor(color, background);
  display.println(text);
}

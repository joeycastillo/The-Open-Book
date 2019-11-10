#include <GxEPD2_BW.h>
#include "bitmap.h"

#define MAX_DISPAY_BUFFER_SIZE 15000ul
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8))
GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=77*/ 44, /*DC=*/ 45, /*RST=*/ 46, /*BUSY=*/ 47));

void setup()
{
  Serial.begin(115200);
  delay(100);
  display.init(115200);
  display.setFullWindow();
  display.firstPage();
  display.fillScreen(GxEPD_WHITE);
  display.drawInvertedBitmap(0, 0, OpenBookSplash, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
  while (display.nextPage());
  delay(2000);
  display.powerOff();
}

void loop()
{
}

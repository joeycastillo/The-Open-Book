#include <OpenBook.h>
#include "bitmap.h"

OpenBook *book;

void setup()
{
  Serial.begin(115200);
  book = new OpenBook();
  book->configureScreen();
  delay(1000);
  Adafruit_EPD *display = book->getDisplay();
  display->drawBitmap(0, 0, OpenBookSplash, 400, 300, EPD_BLACK);
  display->display();
}

void loop()
{
}

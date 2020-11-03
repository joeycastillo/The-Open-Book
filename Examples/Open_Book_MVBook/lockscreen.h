#include <Adafruit_NeoPixel.h>

void drawGrayscale(Adafruit_GFX *display, int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h) {
    int16_t byteWidth = (w*2 + 7) / 8;
    uint8_t byte = 0;
    uint16_t color;

    display->startWrite();
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i < w * 2; i += 2 ) {
            if(i & 7) byte <<= 2;
            else      byte   = bitmap[j * byteWidth + i / 8];
            switch((byte & 0xC0) >> 6) {
                case 0:
                    color = EPD_BLACK;
                    break;
                case 1:
                    color = EPD_DARK;
                    break;
                case 2:
                    color = EPD_LIGHT;
                    break;
                case 3:
                    color = EPD_WHITE;
                    break;
                default:
                    break;
            }
            display->writePixel(x+i/2, y, color);
        }
    }
    display->endWrite();
}

void doLock() {
    OpenBook_IL0398 *display = book->getDisplay();

    // display a pretty screen when turned off.
    display->setDisplayMode(OPEN_BOOK_DISPLAY_MODE_GRAYSCALE);
    display->setRotation(1);
    display->fillScreen(EPD_WHITE);
    display->display();
    drawGrayscale(display, 0, 0, LockScreen, 400, 300);

    // this hint works to turn the device off, but will also remind the
    // user to turn the device back on when they pick it up next.
    book->getDisplay()->setCursor(248, 290);
    book->getDisplay()->setTextColor(EPD_WHITE, EPD_BLACK);
    book->getDisplay()->setTextSize(1);
    book->getDisplay()->println("Slide the power switch ->");
    display->display();

    // show a yellow color to remind the user to turn off the device.
    Adafruit_NeoPixel pixel(1, OPENBOOK_NEOPIXEL, NEO_GRB + NEO_KHZ800);
    pixel.begin();
    pixel.setPixelColor(0, pixel.Color(35, 30, 0));
    pixel.show();

    // completely turn off the SAMD51
    PM->SLEEPCFG.bit.SLEEPMODE = 0x7;
    while(PM->SLEEPCFG.bit.SLEEPMODE != 0x7);
}

/*!
 * @file OpenBook_IL0398.h
 *
 * Forked from Adafruit_IL0398.h; copyright notce below.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
 * Open Book additions by Joey Castillo.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef OSO_OpenBook_IL0398_h
#define OSO_OpenBook_IL0398_h

#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

#include "Adafruit_EPD.h"

typedef enum OpenBookDisplayMode {
    OPEN_BOOK_DISPLAY_MODE_DEFAULT,
    OPEN_BOOK_DISPLAY_MODE_QUICK,
    OPEN_BOOK_DISPLAY_MODE_PARTIAL,
    OPEN_BOOK_DISPLAY_MODE_GRAYSCALE
}
OpenBookDisplayMode;

/**************************************************************************/
/*!
    @brief  Class for interfacing with IL0398 EPD drivers
*/
/**************************************************************************/
class OpenBook_IL0398 : public Adafruit_EPD {
 public:
    OpenBook_IL0398(int width, int height, int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t MISO, int8_t BUSY = -1);
    OpenBook_IL0398(int width, int height, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t BUSY = -1, SPIClass *spi = &SPI);

    void begin(bool reset=true);
    void powerUp();
    void update();
    void powerDown();
    void setDisplayMode(OpenBookDisplayMode displayMode);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void display();
    void displayPartial(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
protected:
    void init(OpenBookDisplayMode displayMode);
    uint8_t writeRAMCommand(uint8_t index);
    void setRAMAddress(uint16_t x, uint16_t y);
    void busy_wait();
    void setWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    
    OpenBookDisplayMode currentDisplayMode = OPEN_BOOK_DISPLAY_MODE_DEFAULT;
private:
    static const unsigned char LUT_VCOM_FULL[];
    static const unsigned char LUT_W[];
    static const unsigned char LUT_B[];

    static const unsigned char LUT_VCOM_PARTIAL[];
    static const unsigned char LUT_WW_PARTIAL[];
    static const unsigned char LUT_WB_PARTIAL[];
    static const unsigned char LUT_BW_PARTIAL[];
    static const unsigned char LUT_BB_PARTIAL[];

    static const unsigned char LUT_VCOM_GRAYSCALE[];
    static const unsigned char LUT_WW_GRAYSCALE[];
    static const unsigned char LUT_WB_GRAYSCALE[];
    static const unsigned char LUT_BW_GRAYSCALE[];
    static const unsigned char LUT_BB_GRAYSCALE[];
};

#endif // OSO_OpenBook_IL0398_h

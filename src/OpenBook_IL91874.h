/*!
 * @file OpenBook_IL91874.h
 *
 * Forked from Adafruit_IL91874.h; copyright notce below.
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

#ifndef OSO_OpenBook_IL91874_h
#define OSO_OpenBook_IL91874_h

#include "Adafruit_EPD.h"
#include <Arduino.h>

#define IL91874_DTM2 0x13
#define IL91874_PARTIAL_RES 0x90
#define IL91874_PARTIALIN 0x91
#define IL91874_PARTIALOUT 0x92

// TODO: don't duplicate this for every screen, move to a common header for EPD drivers.
typedef enum OpenBookDisplayMode {
    OPEN_BOOK_DISPLAY_MODE_DEFAULT,
    OPEN_BOOK_DISPLAY_MODE_QUICK,
    OPEN_BOOK_DISPLAY_MODE_PARTIAL,
    OPEN_BOOK_DISPLAY_MODE_GRAYSCALE
}
OpenBookDisplayMode;

/**************************************************************************/
/*!
    @brief  Class for interfacing with IL0373 EPD drivers
*/
/**************************************************************************/
class OpenBook_IL91874 : public Adafruit_EPD {
public:
    OpenBook_IL91874(int width, int height, int8_t SID, int8_t SCLK, int8_t DC,
                   int8_t RST, int8_t CS, int8_t SRCS, int8_t MISO,
                   int8_t BUSY = -1);
    OpenBook_IL91874(int width, int height, int8_t DC, int8_t RST, int8_t CS,
                   int8_t SRCS, int8_t BUSY = -1, SPIClass *spi = &SPI);

    void begin(bool reset = true);
    void powerUp();
    void update();
    void powerDown();
    void setDisplayMode(OpenBookDisplayMode displayMode);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void displayPartial(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
protected:
    void init(OpenBookDisplayMode displayMode);
    uint8_t writeRAMCommand(uint8_t index);
    void setRAMAddress(uint16_t x, uint16_t y);
    void busy_wait();

    OpenBookDisplayMode currentDisplayMode = OPEN_BOOK_DISPLAY_MODE_DEFAULT;
private:
    static const unsigned char LUT_VCOM_QUICK[];
    static const unsigned char LUT_W_QUICK[];
    static const unsigned char LUT_B_QUICK[];

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

#endif
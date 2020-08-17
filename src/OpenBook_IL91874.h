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

#define IL91874_PANEL_SETTING 0x00
#define IL91874_POWER_SETTING 0x01
#define IL91874_POWER_OFF 0x02
#define IL91874_POWER_OFF_SEQUENCE 0x03
#define IL91874_POWER_ON 0x04
#define IL91874_POWER_ON_MEASURE 0x05
#define IL91874_BOOSTER_SOFT_START 0x06
#define IL91874_DEEP_SLEEP 0x07
#define IL91874_DTM1 0x10
#define IL91874_DATA_STOP 0x11
#define IL91874_DISPLAY_REFRESH 0x12
#define IL91874_DTM2 0x13
#define IL91874_PDTM1 0x14
#define IL91874_PDTM2 0x15
#define IL91874_PDRF 0x16
#define IL91874_LUT1 0x20
#define IL91874_LUTWW 0x21
#define IL91874_LUTBW 0x22
#define IL91874_LUTWB 0x23
#define IL91874_LUTBB 0x24
#define IL91874_PLL 0x30
#define IL91874_CDI 0x50
#define IL91874_RESOLUTION 0x61
#define IL91874_VCM_DC_SETTING 0x82

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
protected:
    void init(OpenBookDisplayMode displayMode);
    uint8_t writeRAMCommand(uint8_t index);
    void setRAMAddress(uint16_t x, uint16_t y);
    void busy_wait();

    OpenBookDisplayMode currentDisplayMode = OPEN_BOOK_DISPLAY_MODE_DEFAULT;
private:
    static const unsigned char LUT_VCOM_GRAYSCALE[];
    static const unsigned char LUT_WW_GRAYSCALE[];
    static const unsigned char LUT_WB_GRAYSCALE[];
    static const unsigned char LUT_BW_GRAYSCALE[];
    static const unsigned char LUT_BB_GRAYSCALE[];
};

#endif
/*!
 * @file Adafruit_IL0398.h
 *
 * Forked from Adafruit_IL0398.h; copyright notce below.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
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

#define IL0398_PANEL_SETTING 0x00
#define IL0398_POWER_SETTING 0x01
#define IL0398_POWER_OFF 0x02
#define IL0398_POWER_OFF_SEQUENCE 0x03
#define IL0398_POWER_ON 0x04
#define IL0398_POWER_ON_MEASURE 0x05
#define IL0398_BOOSTER_SOFT_START 0x06
#define IL0398_DEEP_SLEEP 0x07
#define IL0398_DTM1 0x10
#define IL0398_DATA_STOP 0x11
#define IL0398_DISPLAY_REFRESH 0x12
#define IL0398_DTM2 0x13
#define IL0398_PDTM1 0x14
#define IL0398_PDTM2 0x15
#define IL0398_PDRF 0x16
#define IL0398_LUT1 0x20
#define IL0398_LUTWW 0x21
#define IL0398_LUTBW 0x22
#define IL0398_LUTWB 0x23
#define IL0398_LUTBB 0x24
#define IL0398_PLL 0x30
#define IL0398_TEMPCALIBRATE 0x40
#define IL0398_TEMPSELECT 0x41
#define IL0398_TEMPWRITE 0x42
#define IL0398_TEMPREAD 0x43
#define IL0398_VCOM 0x50
#define IL0398_LOWPOWERDETECT 0x51
#define IL0398_TCON 0x60
#define IL0398_RESOLUTION 0x61
#define IL0398_GSSTSETTING 0x65
#define IL0398_REVISION 0x70
#define IL0398_GETSTATUS 0x71
#define IL0398_AUTOVCOM 0x80
#define IL0398_READVCOM 0x81
#define IL0398_VCM_DC_SETTING 0x82
#define IL0398_PARTWINDOW 0x90
#define IL0398_PARTIALIN 0x91
#define IL0398_PARTIALOUT 0x92
#define IL0398_PROGRAMMODE 0xA0
#define IL0398_ACTIVEPROGRAM 0xA1
#define IL0398_READOTP 0xA2
#define IL0398_CASCADESET 0xE0
#define IL0398_POWERSAVING 0xE3
#define IL0398_FORCETEMP 0xE5

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

/*!
 * @file OpenBook_IL91874.cpp
 *
 * Forked from Adafruit_IL91874.cpp; copyright notce below.
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

#include "OpenBook_IL91874.h"
#include "Adafruit_EPD.h"

#define BUSY_WAIT 500

/**************************************************************************/
/*!
    @brief constructor if using external SRAM chip and software SPI
    @param width the width of the display in pixels
    @param height the height of the display in pixels
    @param SID the SID pin to use
    @param SCLK the SCLK pin to use
    @param DC the data/command pin to use
    @param RST the reset pin to use
    @param CS the chip select pin to use
    @param SRCS the SRAM chip select pin to use
    @param MISO the MISO pin to use
    @param BUSY the busy pin to use
*/
/**************************************************************************/
OpenBook_IL91874::OpenBook_IL91874(int width, int height, int8_t SID,
                                   int8_t SCLK, int8_t DC, int8_t RST,
                                   int8_t CS, int8_t SRCS, int8_t MISO,
                                   int8_t BUSY)
    : Adafruit_EPD(width, height, SID, SCLK, DC, RST, CS, SRCS, MISO, BUSY) {

  buffer1_size = ((uint32_t)width * (uint32_t)height) / 8;
  buffer2_size = buffer1_size;

  if (SRCS >= 0) {
    use_sram = true;
    buffer1_addr = 0;
    buffer2_addr = buffer1_size;
    buffer1 = buffer2 = NULL;
  } else {
    buffer1 = (uint8_t *)malloc(buffer1_size);
    buffer2 = (uint8_t *)malloc(buffer2_size);
  }
}

// constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
/**************************************************************************/
/*!
    @brief constructor if using on-chip RAM and hardware SPI
    @param width the width of the display in pixels
    @param height the height of the display in pixels
    @param DC the data/command pin to use
    @param RST the reset pin to use
    @param CS the chip select pin to use
    @param SRCS the SRAM chip select pin to use
    @param BUSY the busy pin to use
*/
/**************************************************************************/
OpenBook_IL91874::OpenBook_IL91874(int width, int height, int8_t DC, int8_t RST,
                                   int8_t CS, int8_t SRCS, int8_t BUSY, SPIClass *spi)
    : Adafruit_EPD(width, height, DC, RST, CS, SRCS, BUSY, spi) {

  buffer1_size = ((uint32_t)width * (uint32_t)height) / 8;
  buffer2_size = buffer1_size;

  if (SRCS >= 0) {
    use_sram = true;
    buffer1_addr = 0;
    buffer2_addr = buffer1_size;
    buffer1 = buffer2 = NULL;
  } else {
    buffer1 = (uint8_t *)malloc(buffer1_size);
    buffer2 = (uint8_t *)malloc(buffer2_size);
  }
}

/**************************************************************************/
/*!
    @brief wait for busy signal to end
*/
/**************************************************************************/
void OpenBook_IL91874::busy_wait(void) {
  if (_busy_pin >= 0) {
    while (!digitalRead(_busy_pin)) {
      delay(1); // wait for busy low
    }
  } else {
    delay(BUSY_WAIT);
  }
}

/**************************************************************************/
/*!
    @brief begin communication with and set up the display.
    @param reset if true the reset pin will be toggled.
*/
/**************************************************************************/
void OpenBook_IL91874::begin(bool reset) {
  singleByteTxns = true;
  Adafruit_EPD::begin(reset);

  setBlackBuffer(0, false);
  setColorBuffer(1, false);

  powerDown();
}

/**************************************************************************/
/*!
    @brief signal the display to update
*/
/**************************************************************************/
void OpenBook_IL91874::update() {
  EPD_command(IL91874_DISPLAY_REFRESH);

  busy_wait();
  if (_busy_pin <= -1) {
    delay(16000);
  }
}

const unsigned char OpenBook_IL91874::LUT_VCOM_GRAYSCALE[] =
{
0x00	,0x00,
0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
const unsigned char OpenBook_IL91874::LUT_WW_GRAYSCALE[] = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

const unsigned char OpenBook_IL91874::LUT_WB_GRAYSCALE[] = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

const unsigned char OpenBook_IL91874::LUT_BW_GRAYSCALE[] = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

const unsigned char OpenBook_IL91874::LUT_BB_GRAYSCALE[] = {
0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};


/**************************************************************************/
/*!
    @brief start up the display. Same as init, but here for compatibility with
           Adafruit_EPD; you can call OpenBook_IL91874::init with more options.
*/
/**************************************************************************/
void OpenBook_IL91874::powerUp() {
    this->init(this->currentDisplayMode);
}

/**************************************************************************/
/*!
    @brief start up the display
    @param displayMode an enum indicating which refresh waveform should be loaded in. Default is to use the factory-supplied waveform.
    @warning You always need to do one full refresh before enabling partial mode.
*/
/**************************************************************************/
void OpenBook_IL91874::init(OpenBookDisplayMode displayMode) {
  uint8_t buf[5];

  hardwareReset();

  if (displayMode == OPEN_BOOK_DISPLAY_MODE_GRAYSCALE) {
    buf[0] = 0xBF; // grayscale using custom LUT
  }	else {
    buf[0] = 0x9F; // black and white only, use manufacturer LUT
  }
  EPD_command(IL91874_PANEL_SETTING, buf, 1);

  buf[0] = 0x90; // OSC clock frequency. Good Display's grayscale demo says this value = 100 Hz, but the datasheet says it's 50. Shrug emoji.
  EPD_command(IL91874_PLL, buf, 1);

  buf[0] = 0x03; // use on-board booster to generate VGH/VGL and VSH/VSL.
  buf[1] = 0x00; // VGH=16v, VGL=-16v, VCOMH=VSH+VCOMDC, VCOML=VSL+VCOMDC
  buf[2] = 0x2b; // VSH=11v
  buf[3] = 0x2b; // VSL=-11v
  EPD_command(IL91874_POWER_SETTING, buf, 4);

  buf[0] = 0x07; // phase A: 6.58uS, strength 1, soft start 10ms
  buf[1] = 0x07; // phase B: 6.58uS, strength 1, soft start 10ms
  buf[2] = 0x17; // phase A: 6.58uS, strength 3, soft start 10ms
  EPD_command(IL91874_BOOSTER_SOFT_START, buf, 3);

// these commands were in the Adafruit driver but I don't know what they do. Something about the boost circuit?
  buf[0] = 0x60;
  buf[1] = 0xA5;
  EPD_command(0xF8, buf, 2);

  buf[0] = 0x89;
  buf[1] = 0xA5;
  EPD_command(0xF8, buf, 2);

  buf[0] = 0x90;
  buf[1] = 0x00;
  EPD_command(0xF8, buf, 2);

  buf[0] = 0x93;
  buf[1] = 0x2A;
  EPD_command(0xF8, buf, 2);

  buf[0] = 0x73;
  buf[1] = 0x41;
  EPD_command(0xF8, buf, 2);

  buf[0] = 0x08; // VCOMDC=-0.5v
  EPD_command(IL91874_VCM_DC_SETTING, buf, 1);

  buf[0] = 0x07; // Low bits have to do with display timing. High bits controls border color; leaving it floating.
  EPD_command(IL91874_CDI, buf, 1);

  switch (displayMode) {
    case OPEN_BOOK_DISPLAY_MODE_GRAYSCALE:
        EPD_command(IL91874_LUT1, LUT_VCOM_GRAYSCALE, sizeof(LUT_VCOM_GRAYSCALE));
        EPD_command(IL91874_LUTWW, LUT_WW_GRAYSCALE, sizeof(LUT_WW_GRAYSCALE));
        EPD_command(IL91874_LUTBW, LUT_BW_GRAYSCALE, sizeof(LUT_BW_GRAYSCALE));
        EPD_command(IL91874_LUTWB, LUT_WB_GRAYSCALE, sizeof(LUT_WB_GRAYSCALE));
        EPD_command(IL91874_LUTBB, LUT_BB_GRAYSCALE, sizeof(LUT_BB_GRAYSCALE));
        break;
    default:
        // only supporting grayscale for now, partial/quick modes are TODO.
        break;
  }

  buf[0] = (HEIGHT >> 8) & 0xFF;
  buf[1] = HEIGHT & 0xFF;
  buf[2] = (WIDTH >> 8) & 0xFF;
  buf[3] = WIDTH & 0xFF;
  EPD_command(IL91874_RESOLUTION, buf, 4);

  buf[0] = 0x00;
  EPD_command(IL91874_PDRF, buf, 1); // Disable partial display refresh.

  this->currentDisplayMode = displayMode;

  EPD_command(IL91874_POWER_ON);
  busy_wait();
}

/**************************************************************************/
/*!
    @brief wind down the display
*/
/**************************************************************************/
void OpenBook_IL91874::powerDown() {
  // power off
  uint8_t buf[1];

  EPD_command(IL91874_POWER_OFF);
  busy_wait();

  // Only deep sleep if we can get out of it
  if (_reset_pin >= 0) {
    buf[0] = 0xA5;
    EPD_command(IL91874_DEEP_SLEEP, buf, 1);
  }
}

/**************************************************************************/
/*!
    @brief Sets the display waveforms for a particular display mode
    @param displayMode the desired mode from except for OPEN_BOOK_DISPLAY_MODE_PARTIAL.
    @note For IL91874, only default and grayscale modes are supported so far.

*/
/**************************************************************************/
void OpenBook_IL91874::setDisplayMode(OpenBookDisplayMode displayMode) {
    if (displayMode != OPEN_BOOK_DISPLAY_MODE_PARTIAL) {
        this->init(displayMode);
    }
}

/**************************************************************************/
/*!
    @brief draw a single pixel on the screen
	@param x the x axis position
	@param y the y axis position
	@param color the color of the pixel
*/
/**************************************************************************/
void OpenBook_IL91874::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  uint8_t *pByte1;
  uint8_t *pByte2;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
    case 1:
    EPD_swap(x, y);
    x = WIDTH - x - 1;
    break;
    case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
    case 3:
    EPD_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  uint16_t addr = ( (uint32_t)(WIDTH - 1 - x) * (uint32_t)HEIGHT + y)/8;

  if (use_sram) {
    uint8_t byte1 = sram.read8(blackbuffer_addr + addr);
    uint8_t byte2 = sram.read8(colorbuffer_addr + addr);
    pByte1 = &byte1;
    pByte2 = &byte2;
  } else {
    pByte1 = black_buffer + addr;
    pByte2 = color_buffer + addr;
  }

  switch (color) {
    case EPD_BLACK:
        *pByte1 |= (1 << (7 - (y%8)));
        *pByte2 |= (1 << (7 - (y%8)));
      break;
    case EPD_DARK:
        *pByte1 &= ~(1 << (7 - (y%8)));
        *pByte2 |= (1 << (7 - (y%8)));
        break;
    case EPD_LIGHT:
        *pByte1 |= (1 << (7 - (y%8)));
        *pByte2 &= ~(1 << (7 - (y%8)));
        break;
    case EPD_WHITE:
      *pByte1 &= ~(1 << (7 - (y%8)));
      *pByte2 &= ~(1 << (7 - (y%8)));
        break;
  }

  if (use_sram) {
    sram.write8(addr, *pByte1);
    sram.write8(addr + buffer1_size, *pByte2);
  }
}

/**************************************************************************/
/*!
    @brief Send the specific command to start writing to EPD display RAM
    @param index The index for which buffer to write (0 or 1 or tri-color
   displays) Ignored for monochrome displays.
    @returns The byte that is read from SPI at the same time as sending the
   command
*/
/**************************************************************************/
uint8_t OpenBook_IL91874::writeRAMCommand(uint8_t index) {
  if (index == 0) {
    return EPD_command(IL91874_DTM2, false);
  }
  if (index == 1) {
    return EPD_command(IL91874_DTM1, false);
  }
  return 0;
}

/**************************************************************************/
/*!
    @brief Some displays require setting the RAM address pointer
    @param x X address counter value
    @param y Y address counter value
*/
/**************************************************************************/
void OpenBook_IL91874::setRAMAddress(uint16_t x, uint16_t y) {
  // on this chip we do nothing
}
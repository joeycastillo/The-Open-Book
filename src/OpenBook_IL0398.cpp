/*!
 * @file Adafruit_IL0398.cpp
 *
 * Forked from Adafruit_IL0398.cpp; copyright notce below.
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

#include "OpenBook_IL0398.h"

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
OpenBook_IL0398::OpenBook_IL0398(int width, int height, 
				 int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, 
				 int8_t CS, int8_t SRCS, int8_t MISO, int8_t BUSY) : 
  Adafruit_EPD(width, height, SID, SCLK, DC, RST, CS, SRCS, MISO, BUSY) {

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
OpenBook_IL0398::OpenBook_IL0398(int width, int height, 
				 int8_t DC, int8_t RST, 
				 int8_t CS, int8_t SRCS, int8_t BUSY, SPIClass *spi) :
  Adafruit_EPD(width, height, DC, RST, CS, SRCS, BUSY, spi) {

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
void OpenBook_IL0398::busy_wait(void)
{
  if (_busy_pin > -1) {
    do {
      EPD_command(IL0398_GETSTATUS);
      delay(10);
    } while (digitalRead(_busy_pin)); //wait for busy low
    delay(200);
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
void OpenBook_IL0398::begin(bool reset)
{
  Adafruit_EPD::begin(reset);
  setBlackBuffer(1, true);
  setColorBuffer(1, true);

  setRotation(1);
  powerDown();
}

/**************************************************************************/
/*!
    @brief signal the display to update
*/
/**************************************************************************/
void OpenBook_IL0398::update()
{
  EPD_command(IL0398_DISPLAY_REFRESH);
  delay(100);

  busy_wait();
  if (_busy_pin <= -1) {
    delay(5000);
  }
}


/**************************************************************************/
/*!
    @brief Custom lookup tables for full screen updates. Public domain (?) from Waveshare.
    @note Video explainer: https://www.youtube.com/watch?v=MsbiO8EAsGw and more info: 
          https://benkrasnow.blogspot.com/2017/10/fast-partial-refresh-on-42-e-paper.html
    @warning YOU CAN PERMANENELY DAMAGE YOUR DISPLAY BY ABUSING THESE LOOKUP TABLES! 
             Seriously, the controller is incredibly programmable and you can force all 
             kinds of voltages and timings onto the screen that it wasn't meant to deal 
             with. TWEAK AT YOUR OWN RISK!
*/
/**************************************************************************/

/*  This looks a little like the dark arts, but basically: refreshing an e-paper screen is
    a matter of setting a sequence of voltages across the pixels on the screen and VCOM, 
    the common voltage of the electrode up top. You might hear this referred to as a 
    "waveform", in that the flashing of high and low voltages creates a waveform that 
    drives the display. When you update the screen, the controller walks through a series 
    of steps or states. The panel has some programmed in, but we can also specify our own.
    It's not much more complicated than that: these tables are just instructions for which 
    voltages to set and when.
    
    The actual voltages are set in powerUp via the power setting register; our values are:
     * VDH:    11v
     * VDL:    -11v
     * VCM_DC: -1.5v

    This is the format of the LUT for the IL0398: each line in the lookup table is one
    state, consisting of six bytes:
    Byte 1: Voltage level select. A series of four two-bit values: 
            D7 D6   D5 D4   D3 D2   D1 D0
            [LS0]   [LS1]   [LS2]   [LS3]
            These are different for the different LUTs so I'll explain them below.
    Byte 2: Number of frames to hold LS0.
    Byte 3: Number of frames to hold LS1.
    Byte 4: Number of frames to hold LS2.
    Byte 5: Number of frames to hold LS3.
    Byte 6: Number of times to repeat this state.

    If you see a row full of zeroes, basically nothing happens, since it's holding the 
    desired voltage for zero frames, zero times.

    Also note that while we technically have to send over five tables (W->W, W->B, B->W, 
    B->B, VCOM), for a full refresh there's no difference between a pixel changing color 
    and remaining the same, so I'm only including three and we send two of them twice. 
    Partial refresh lookup tables are a TODO item and will require all five to be defined.
*/

/*  For the per-pixel tables, voltage level select is as follows: 
    00b: GND 
    01b: VDH 
    10b: VDL 
*/
const unsigned char OpenBook_IL0398::LUT_W[] PROGMEM =
{
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02, // state 0
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02, // state 1
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01, // state 2
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02, // state 3
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // state 4
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // state 5
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // state 6
};
// so to unpack this table (which is for pixels that will be white after the refresh):
// State 0: 0x40 = 0b01000000, meaning: hold the pixel at VDH for 0x17 (23) frames; repeat twice.
// State 1: 0x90 = 0b10010000: hold pixel at VDL for 23 frames, then at VDH for 23 frames; repeat twice.
// State 2: 0x40 = 0b01000000: hold pixel at VDH for 10 frames, then hold at GND for 1 frame; repeat once.
// State 3: 0xA0 = 0b10100000: hold pixel at VDL for 14 frames, and then do it again; repeat twice.

const unsigned char OpenBook_IL0398::LUT_B[] PROGMEM =
{
    0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         
};
// similarly for black pixels:
// State 0: 0x80 = 0b10000000: hold pixel at VDL for 23 frames; repeat twice.
// State 1: 0x90 = 0b10010000: hold pixel at VDL for 23 frames, then at VDH for 23 frames; repeat twice.
// State 2: 0x80 = 0b10000000: hold pixel at VDL for 10 frames, then hold at GND for 1 frame; repeat once.
// State 3: 0x50 = 0b01010000: hold pixel at VDH for 14 frames, and then do it again; repeat twice.

/*  For the VCOM lookup table, voltage level select is as follows: 
    00b: VCM_DC
    01b: VDH+VCM_DC (VCOMH)
    10b: VDL+VCM_DC (VCOML)
    11b: Floating
*/
const unsigned char OpenBook_IL0398::LUT_VCOM[] PROGMEM =
{
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // the VCOM lookup table has two extra values, which we set to 0:
    0x00, // ST_XON: if bit (1<<state) is set, all gates are on for that state
    0x00, // ST_CHV: if bit (1<<state) is set, VCOM high voltage is set for that state. I do not know what this means but don't even want to try it.
};
// and just to round it out, VCOM's status:
// State 0:   0x40 = 0b01000000 meaning hold VCOM at VCOMH for 23 frames and repeat twice.
// State 1-3: for the rest of the refresh we just hold VCOM at VCM_DC.

// You can see where this sequence describes the flashing effect that you see on a full panel refresh.
// * in phase 0, VCOM is at 9.5v, white pixels are at 11v (high) and black pixels are at -11v (low). You see kind of a half and half of what's on the screen now and what you're about to display.
// then VCOM is set to -1.5v and:
// * in phase 1, all the pixels go high, and then all the pixels go low, twice. Basically coaxing the ink out of where it was and into a known state.
// * in phase 2, the white pixels briefly go high, and the black pixels briefly go low. You get an inverted version of what you're about to display. Then all the pixels go to ground for one frame.
// * Finally, in phase 3, the white pixels go low and the black pixels go high, and we hold them there for 56 frames (14*2*2). This is the final state and where the pixels end up staying.

/**************************************************************************/
/*!
    @brief start up the display
*/
/**************************************************************************/
void OpenBook_IL0398::powerUp()
{
  uint8_t buf[4];

  hardwareReset();

  buf[0] = 0x03;    // Panel will generate VDH and VDL (1<<0) and VGH and VGL (1<<1)
  buf[1] = 0x00;    // VCOMH=VDH+VCOMDC and VCOML=VDL+VCOMDC; VGH and VGL are 16v and -16v respectively
  buf[2] = 0x2b;    // VDH= 11V
  buf[3] = 0x2b;    // VDL=-11V
  EPD_command(IL0398_POWER_SETTING, buf, 4);

  buf[0] = 0x17;    // phase A: soft start 10ms, driving strength 3, off time 6.58us
  buf[1] = 0x17;    // phase B: soft start 10ms, driving strength 3, off time 6.58us
  buf[2] = 0x17;    // phase C: driving strength 3, off time 6.58us
  EPD_command(IL0398_BOOSTER_SOFT_START, buf, 3);

  buf[0] = 0x3F; // (1<<4) sets display to monochrome; (1<<5) enables custom LUTs
  EPD_command(IL0398_PANEL_SETTING, buf, 1);
  
  buf[0] = 0x3A; // 100 Hz
  EPD_command(IL0398_PLL, buf, 1);

  buf[0] = (HEIGHT >> 8) & 0xFF;
  buf[1] = HEIGHT & 0xFF;
  buf[2] = (WIDTH >> 8) & 0xFF;
  buf[3] = WIDTH & 0xFF;
  EPD_command(IL0398_RESOLUTION, buf, 4);
  
  buf[0] = 0x12; // VCOM_DC = -1.5v
  EPD_command(IL0398_VCM_DC_SETTING, buf, 1);
  
  buf[0] = 0xD7; // 0x57 for black border. 0x97 for white border. 0xD7 for floating border.
  EPD_command(IL0398_VCOM, buf, 1);
  
  EPD_command(IL0398_POWER_ON);
  busy_wait();

  EPD_command(IL0398_LUT1, LUT_VCOM, sizeof(LUT_VCOM));
  EPD_command(IL0398_LUTWW, LUT_W, sizeof(LUT_W));
  EPD_command(IL0398_LUTBW, LUT_W, sizeof(LUT_W));
  EPD_command(IL0398_LUTWB, LUT_B, sizeof(LUT_B));
  EPD_command(IL0398_LUTBB, LUT_B, sizeof(LUT_B));

  delay(20);
}

/**************************************************************************/
/*!
    @brief wind down the display
*/
/**************************************************************************/
void OpenBook_IL0398::powerDown()
{
  uint8_t buf[4];

  // power off
  buf[0] = 0xF7; // border floating
  EPD_command(IL0398_VCOM, buf, 1);
  EPD_command(IL0398_POWER_OFF);
  busy_wait();
  buf[0] = 0xA5; // deep sleep
  EPD_command(IL0398_DEEP_SLEEP, buf, 1);
  delay(100);
}




/**************************************************************************/
/*!
    @brief Send the specific command to start writing to EPD display RAM
    @param index The index for which buffer to write (0 or 1 or tri-color displays)
    Ignored for monochrome displays.
    @returns The byte that is read from SPI at the same time as sending the command
*/
/**************************************************************************/
uint8_t OpenBook_IL0398::writeRAMCommand(uint8_t index) {
  if (index == 0) {
    return EPD_command(EPD_RAM_BW, false);
  }
  if (index == 1) {
    return EPD_command(EPD_RAM_RED, false);
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
void OpenBook_IL0398::setRAMAddress(uint16_t x, uint16_t y) {
  // on this chip we do nothing
}

/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Original copyright (c) 2017 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "boards/board.h"
#include "mpconfigboard.h"
#include "hal/include/hal_gpio.h"
#include "shared-bindings/board/__init__.h"
#include "shared-bindings/busio/SPI.h"
#include "shared-bindings/displayio/FourWire.h"
#include "shared-bindings/time/__init__.h"
#include "shared-module/displayio/__init__.h"
#include "shared-module/displayio/mipi_constants.h"

displayio_fourwire_obj_t board_display_obj;

#define DELAY 0x80
#define HEIGHT 264
#define WIDTH 176

uint8_t start_sequence[] = {
    0x00, 0x01, 0x9f, // panel setting: B&W only, custom LUT
    0x30, 0x01, 0x3a, // PLL
    0x01, 0x04, 0x03, 0x00, 0x2b, 0x2b, // power setting
    0x06, 0x03, 0x07, 0x07, 0x17, // booster soft start
    0x82, 0x01, 0x08, // VCOM
    0x50, 0x01, 0x07, // CDI
    0x61, 0x04, (WIDTH >> 8) & 0xFF, WIDTH & 0xFF, (HEIGHT >> 8) & 0xFF, HEIGHT & 0xFF, // Resolution
    0x16, 0x01, 0x00, // PDRF
    0x04, 0x00, // main screen turn on
};

uint8_t stop_sequence[] = {
    0x02, 0x01, 0x17  // Power off
};

void board_init(void) {
    busio_spi_obj_t* spi = &displays[0].fourwire_bus.inline_bus;
    common_hal_busio_spi_construct(spi, &pin_PA16, &pin_PA19, NULL);
    common_hal_busio_spi_never_reset(spi);

    displayio_fourwire_obj_t* bus = &displays[0].fourwire_bus;
    bus->base.type = &displayio_fourwire_type;
    common_hal_displayio_fourwire_construct(bus,
        spi,
        &pin_PA22, // EPD_DC Command or data
        &pin_PA23, // EPD_CS Chip select
        &pin_PA21, // EPD_RST Reset
        1000000, 0, 0);

    displayio_epaperdisplay_obj_t* display = &displays[0].epaper_display;
    display->base.type = &displayio_epaperdisplay_type;
    common_hal_displayio_epaperdisplay_construct(display,
        bus,
        start_sequence,
        sizeof(start_sequence),
        stop_sequence,
        sizeof(stop_sequence),
        WIDTH, // width
        HEIGHT, // height
        320, // RAM width
        300, // RAM height
        0, // colstart
        0, // rowstart
        0, // rotation
        NO_COMMAND, // set_column_window_command
        NO_COMMAND, // set_row_window_command
        NO_COMMAND, // set_current_column_command
        NO_COMMAND, // set_current_row_command
        0x13, // write_black_ram_command
        true, // black_bits_inverted
        NO_COMMAND, // write_color_ram_command (can add this for grayscale eventually)
        true, // color_bits_inverted
        0x000000, // highlight_color
        0x12, // refresh_display_command
        1, // refresh_time
        &pin_PA12, // busy_pin
        false, // busy_state
        0, // seconds_per_frame
        true); // chip_select (don't always toggle chip select)
}

bool board_requests_safe_mode(void) {
    return false;
}

void reset_board(void) {
}
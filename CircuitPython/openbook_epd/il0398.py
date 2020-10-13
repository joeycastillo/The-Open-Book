# The MIT License (MIT)
#
# Copyright (c) 2019 Scott Shawcroft for Adafruit Industries LLC
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
"""
`openbook_il0398`
================================================================================

CircuitPython displayio drivers for IL0398 driven e-paper displays


* Author(s): Scott Shawcroft
* Author(s): Joey Castillo

Implementation Notes
--------------------

**Hardware:**

**Software and Dependencies:**

* Adafruit CircuitPython (5+) firmware for the supported boards:
  https://github.com/adafruit/circuitpython/releases

"""

import displayio

__version__ = "0.0.0-auto.0"
__repo__ = "https://github.com/joeycastillo/The-Open-Book.git"

_START_SEQUENCE = (
    b"\x01\x05\x03\x00\x2b\x2b\x03"  # power setting
    b"\x06\x03\x17\x17\x17"  # booster soft start
    b"\x00\x01\x1f"  # panel setting
    b"\x30\x01\x3a"  # PLL / Osc
    b"\x82\x01\x12"  # VCM_DC
    b"\x50\x01\xd7"  # VCOM register. 0x57 for black border. 0x97 for white border. 0xD7 for floating border.
    b"\x61\x04\x00\x00\x00\x00"  # Resolution
    b"\x04\x80\xc8"  # power on and wait 200 ms
)

_QUICK_SEQUENCE = (
    b"\x01\x05\x03\x00\x2b\x2b\x03"  # power setting
    b"\x06\x03\x17\x17\x17"  # booster soft start
    b"\x00\x01\x3f"  # panel setting
    b"\x30\x01\x3a"  # PLL / Osc
    b"\x82\x01\x12"  # VCM_DC
    b"\x50\x01\xd7"  # VCOM register. 0x57 for black border. 0x97 for white border. 0xD7 for floating border.
    # Look Up Tables
    # LUT1
    b"\x20\x2c"
    b"\x40\x17\x00\x00\x00\x01"
    b"\x00\x17\x17\x00\x00\x02"
    b"\x00\x0a\x01\x00\x00\x01"
    b"\x00\x0e\x0e\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00"
    # LUTWW
    b"\x21\x2a"
    b"\x40\x17\x00\x00\x00\x01"
    b"\x90\x17\x17\x00\x00\x02"
    b"\x40\x0a\x01\x00\x00\x01"
    b"\xa0\x0e\x0e\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTBW
    b"\x22\x2a"
    b"\x40\x17\x00\x00\x00\x01"
    b"\x90\x17\x17\x00\x00\x02"
    b"\x40\x0a\x01\x00\x00\x01"
    b"\xa0\x0e\x0e\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTWB
    b"\x23\x2a"
    b"\x80\x17\x00\x00\x00\x01"
    b"\x90\x17\x17\x00\x00\x02"
    b"\x80\x0a\x01\x00\x00\x01"
    b"\x50\x0e\x0e\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTBB
    b"\x24\x2a"
    b"\x80\x17\x00\x00\x00\x01"
    b"\x90\x17\x17\x00\x00\x02"
    b"\x80\x0a\x01\x00\x00\x01"
    b"\x50\x0e\x0e\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"

    b"\x61\x04\x00\x00\x00\x00"  # Resolution
    b"\x04\x80\xc8"  # power on and wait 200 ms
)

_STOP_SEQUENCE = (
    b"\x50\x01\xf7"  # CDI setting
    b"\x02\x80\xf0"  # Power off
    # TODO: Busy wait
    # b"\x07\x01\xa5" # Deep sleep
)

# pylint: disable=too-few-public-methods
class OpenBook_IL0398(displayio.EPaperDisplay):
    """IL0398 driver"""

    def __init__(self, bus, **kwargs):
        start_sequence = bytearray(_QUICK_SEQUENCE)

        width = kwargs["width"]
        height = kwargs["height"]
        if "rotation" in kwargs and kwargs["rotation"] % 180 != 0:
            width, height = height, width
        start_sequence[-7] = (width >> 8) & 0xFF
        start_sequence[-6] = width & 0xFF
        start_sequence[-5] = (height >> 8) & 0xFF
        start_sequence[-4] = height & 0xFF
        if "highlight_color" in kwargs:
            write_black_ram_command = 0x10
            write_color_ram_command = 0x13
        else:
            write_color_ram_command = 0x10
            write_black_ram_command = 0x13
        super().__init__(
            bus,
            start_sequence,
            _STOP_SEQUENCE,
            **kwargs,
            ram_width=400,
            ram_height=300,
            busy_state=False,
            write_black_ram_command=write_black_ram_command,
            write_color_ram_command=write_color_ram_command,
            black_bits_inverted=True, # invert screen to avoid allocating a bitmap for the white background
            refresh_display_command=0x12,
            seconds_per_frame=0.1,
        )

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
`adafruit_il91874`
================================================================================

CircuitPython `displayio` driver for IL91874-based ePaper displays


* Author(s): Scott Shawcroft
* Author(s): Joey Castillo

Implementation Notes
--------------------

**Hardware:**

* `Adafruit 2.7" Tri-Color ePaper Display Shield <https://www.adafruit.com/product/4229>`_

**Software and Dependencies:**

* Adafruit CircuitPython firmware (version 5+) for the supported boards:
  https://github.com/adafruit/circuitpython/releases

# * Adafruit's Bus Device library: https://github.com/adafruit/Adafruit_CircuitPython_BusDevice
# * Adafruit's Register library: https://github.com/adafruit/Adafruit_CircuitPython_Register
"""

import displayio

__version__ = "0.0.0-auto.0"
__repo__ = "https://github.com/adafruit/Adafruit_CircuitPython_IL91874.git"

_START_SEQUENCE = (
    b"\x00\x01\x9f"  # panel setting
    b"\x30\x01\x90"  # PLL
    b"\x01\x04\x03\x00\x2b\x2b"  # power setting
    b"\x06\x03\x07\x07\x17"  # booster soft start
    b"\xf8\x02\x60\xa5"  # mystery command in example code
    b"\xf8\x02\x89\xa5"  # mystery command in example code
    b"\xf8\x02\x90\x00"  # mystery command in example code
    b"\xf8\x02\x93\x2a"  # mystery command in example code
    b"\xf8\x02\x73\x41"  # mystery command in example code
    b"\x82\x01\x08"  # VCM DC
    b"\x50\x01\x07"  # CDI setting
    b"\x61\x04\x00\x00\x00\x00"  # Resolution
    b"\x16\x01\x00"  # PDRF
    b"\x04\x00"  # Power on
)

_QUICK_SEQUENCE = (
    b"\x00\x01\xbf"  # panel setting
    b"\x30\x01\x90"  # PLL
    b"\x01\x04\x03\x00\x2b\x2b"  # power setting
    b"\x06\x03\x07\x07\x17"  # booster soft start
    b"\xf8\x02\x60\xa5"  # mystery command in example code
    b"\xf8\x02\x89\xa5"  # mystery command in example code
    b"\xf8\x02\x90\x00"  # mystery command in example code
    b"\xf8\x02\x93\x2a"  # mystery command in example code
    b"\xf8\x02\x73\x41"  # mystery command in example code
    b"\x82\x01\x08"  # VCM DC
    b"\x50\x01\x07"  # CDI setting
    # Look Up Tables
    # LUT1
    b"\x20\x2c"
    b"\x00\x00"
    b"\x40\x0b\x00\x00\x00\x01"
    b"\x00\x0b\x0b\x00\x00\x01"
    b"\x00\x05\x01\x00\x00\x01"
    b"\x00\x07\x07\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTWW
    b"\x21\x2a"
    b"\x40\x0b\x00\x00\x00\x01"
    b"\x90\x0b\x0b\x00\x00\x01"
    b"\x40\x05\x01\x00\x00\x01"
    b"\xa0\x07\x07\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTBW
    b"\x22\x2a"
    b"\x40\x0b\x00\x00\x00\x01"
    b"\x90\x0b\x0b\x00\x00\x01"
    b"\x40\x05\x01\x00\x00\x01"
    b"\xa0\x07\x07\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTWB
    b"\x23\x2a"
    b"\x80\x0b\x00\x00\x00\x01"
    b"\x90\x0b\x0b\x00\x00\x01"
    b"\x80\x05\x01\x00\x00\x01"
    b"\x50\x07\x07\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    # LUTBB
    b"\x24\x2a"
    b"\x80\x0b\x00\x00\x00\x01"
    b"\x90\x0b\x0b\x00\x00\x01"
    b"\x80\x05\x01\x00\x00\x01"
    b"\x50\x07\x07\x00\x00\x01"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"
    b"\x00\x00\x00\x00\x00\x00"

    b"\x61\x04\x00\x00\x00\x00" # Resolution
    b"\x16\x01\x00"             # PDRF
    b"\x04\x00"                 # Power on
)

_STOP_SEQUENCE = b"\x02\x01\x17"  # Power off

# pylint: disable=too-few-public-methods
class TinyBook_IL91874(displayio.EPaperDisplay):
    """IL91874 display driver"""

    def __init__(self, bus, **kwargs):
        start_sequence = bytearray(_QUICK_SEQUENCE)

        width = kwargs["width"]
        height = kwargs["height"]
        if "rotation" in kwargs and kwargs["rotation"] % 180 != 0:
            width, height = height, width
        start_sequence[-9] = (width >> 8) & 0xFF
        start_sequence[-8] = width & 0xFF
        start_sequence[-7] = (height >> 8) & 0xFF
        start_sequence[-6] = height & 0xFF

        super().__init__(
            bus,
            start_sequence,
            _STOP_SEQUENCE,
            **kwargs,
            ram_width=320,
            ram_height=300,
            busy_state=False,
            write_black_ram_command=0x13,
            black_bits_inverted=True,
            refresh_display_command=0x12,
            always_toggle_chip_select=True,
            seconds_per_frame=0.1
        )
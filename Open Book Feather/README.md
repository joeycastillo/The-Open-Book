# Notes on the Open Book PCB files

The current working, tested revision is **Open Book A1 Rev 05** (**OSO-BOOK-A1-05**). For the sake of clarity, I've removed revisions 01 through 04; if you made one of these old boards, these notes remain.

As of August 2020, there is also also a DXF file `OSO-BOOK-A1-05 Solder Stencil.dxf` suitable for laser cutting your own solder stencil, courtesy of [Spencer Owen](https://www.tindie.com/stores/Semachthemonkey/). Many thanks!

## Open Book A1 Rev 01 Errata

I imagine you don't have this board; it was the first test, and was replaced with revision 2 almost immediately. It had a couple of big issues:

* The buttons on the shift register floated, because there were no pull-up resistors. This rendered the buttons unusable.
* The Neopixel, in addition to being in a really inconvenient place, was powered by VHI instead of the 3V regulator, which made it power up before everything else and shine bright blue until you took control of it. It also wouldn't turn off when the enable switch was thrown.

## Open Book A1 Rev 02 Errata

There are two errors on this revision's descriptive silkscreen: first, at the bottom, in the "Button Register" block, the shift register's pins are listed as:

    Pins: 
    BUTTON_LATCH: D48/PB31
    BUTTON_OUT:  D49/PB30
    BUTTON_CLOCK: D50/PB00

The correct mapping is:

    Pins: 
    BUTTON_LATCH: D50/PB00
    BUTTON_OUT:  D49/PB30
    BUTTON_CLOCK: D48/PB31

Second, the SAM D51 Microcontroller block erroneously lists the UART's SERCOM as SERCOM0. It is in fact SERCOM5.

## Open Book A1 Rev 03

Revision 3 was never fabricated; it simply fixed the first error above, but was otherwise identical to Revision 2. I wanted to correct the error in case anyone was building along at home, and bumped the revision to make it clear which version had the issue. It still has the incorrect SERCOM listed, though; that was not caught until later.

Note that the pin muxing changed after revision 3; if you have board revision A1-01, A1-02 or A1-03, you will need to set a flag when compiling your code. In the Arduino IDE, this setting is in `Tools -> Hardware Revision`. In CircuitPython, you can just use the digital pin numbers as labeled on your silkscreen to access the correct pins.

## Open Book A1 Rev 04

This board has the final pinout (no need to set the Hardware Revision flag), and adds several features:

* Microphone shutdown pin
* USB voltage monitor (detect when plugged in to external power)
* Battery full indicator

This is the last revision with the SERCOM0/SERCOM5 silkscreen error.

## Open Book A1 Rev 05

This is identical to Rev 04, but fixes the silkscreen error in the SAM D51 block. It also adds a RC filter to the USB cable shield (recommendation from the SAM D51 data sheet), and tweaks the through-hole button footprints for a slightly better fit.

This is the final version of the A1-type hardware. Having said that, when I run out of the A1-05 boards, I plan to update the silkscreen for any future run to remove deprecated SPI signal names (MOSI/MISO), and replace them with either SDO/SDI or COPI/CIPO.

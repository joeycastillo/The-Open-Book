# Notes on the Open Book PCB files

The current working, tested revision is **Open Book A1 Rev 02**. If you want to try your hand at building this, that's the board to get fabbed.

## Open Book A1 Rev 02 Errata

There is one error on the descriptive silkscreen: at the bottom, in the "Button Register" block, the shift register's pins are listed as:

    Pins: 
    BUTTON_LATCH: D48/PB31
    BUTTON_OUT:  D49/PB30
    BUTTON_CLOCK: D50/PB00

The correct mapping is:

    Pins: 
    BUTTON_LATCH: D50/PB00
    BUTTON_OUT:  D49/PB30
    BUTTON_CLOCK: D48/PB31

## Open Book A1 Rev 01

**Do not use this revision.** There are a couple of design errors:

* The buttons on the shift register float, because there are no pull-up resistors. This renders the buttons unusable.
* The Neopixel, in addition to being in a really inconvenient place, is powered by VHI instead of the 3V regulator, which seems to cause it to power up before everything else and shine bright blue until you take control of it. It also doesn't turn off when the enable switch is thrown.

## Open Book A1 Rev 03 (WIP)

This is a work in progress, tho Rev 2 worked so well I might leave it there. FWIW my thinking on this is to nix the SRAM chip ($--) since the SAMD51 has plenty of RAM. It also swaps the shift register for an I2C port expander ($++), for four reasons: 1, it matches the Wing's hardware design; 2, internal pull-ups mean no need to place two fiddly little resistor arrays; 3, it gives us the ability to interrupt on any button, and 4, it frees up pins that we can use for a mic amp shutdown and VBUS monitor (to know when we're connected to external power).

It also adds some creature comforts like a green "Battery Full" lamp, and an SWO pin on the SWD connector.
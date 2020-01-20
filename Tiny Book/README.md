# The Tiny Book

UNTESTED! Currently (1/20/2020) on order from OSH Park. The theory: make a less expensive, smaller version of the Open Book with as much of its functionality as possible. To that end: 

- Things that are basically the same:
    - Battery charging
    - Indicator LEDs
    - 2MB QSPI Flash
    - MicroSD slot
    - Enable switch
    - Reset button
    - Serial port
    - Raw and amplified mic input
    - Mic shutdown pin
    - VBAT and VBUS monitors
- Things that are similar, and the differences:
    - Microcontroller: it's still a SAMD51 with native USB and the same amount of Flash and RAM, but in a QFN48 package, so fewer pins (and sadly less hand-solderability).
    - E-Paper screen. Still has partial refresh, but it's smaller (2.7" instead of 4.2"), and it's on the main SPI bus instead of having a SERCOM to itself.
    - Instead of seven buttons on the front, there are five capacitive touch pads. I imagine using these to emulate the Open Book's five center buttons (possibly with on-screen soft button indicators), and I hope to have basic gesture detection of "swipe left" and "swipe right" to emulate the other two buttons.
    - The I2C port is now JST-SH (STEMMA-QT) due to size constraints.
    - Headphone jack is mono instead of stereo, due to cost and pin constraints.
    - Neopixel is right-angle edge-mounted, which honestly the Open Book should have done to begin with.
- Things that are gone, and strategies for doing without:
    - Lock button. I imagine we can have a menu option and a timeout to put the device in a low power state, and then use the reset button to wake it up.
    - Babel chip. Babel was written with an eye toward having different sources for Unifont data; I could see either using a Babel file on the SD card, or having a subset of glyphs in program memory for folks who don't need the whole BMP.
    - One STEMMA port instead of two. Wish I could have swung it, but there was no room and no spare pin besides.
    - Feather compatibility. This is a big loss, no way around it.

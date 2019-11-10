# The Open Book Project

THE PREMISE: As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

![image](/images/board.jpg)

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## A Note to Visitors: State of the Book

The most important thing I can reiterate in this README is that **This Is A Work In Progress!** The Open Book board is probably 90% of the way there, but the software required to actually be an eBook is in its infancy; I can put a few Arduino sketches up here, but the long-term goal involves building open source eBook software, and that's still a ways out. As of November 2019, this is where we are on the roadmap:

- [X] Hardware: Build Feather Wing to validate peripheral hardware design.
- [X] Hardware: Open Book Feather with SAMD51 processor, Flash memory, shift register for buttons, indicator LED, etc.
- [ ] Board support: UF2 Bootloader for Open Book (for now just use [the PyBadge LC bootloader](https://github.com/adafruit/uf2-samdx1/releases), it's similar enough that it "just works")
- [X] Board support: [Arduino core for Open Book](https://github.com/joeycastillo/ArduinoCore-samd)
- [ ] Board support: CircuitPython board definition for Open Book. Note that you can run CircuitPython now using [the PyBadge build](https://circuitpython.org/board/pybadge/), but it lacks pin definitions for the display, lock button, SRAM, Babel and mic input. Some of the other pins have been mapped to different functions, so it might be a little wacky.
- [ ] Software: [Universal language support with Babel](https://github.com/joeycastillo/babel). In progress; can draw glyphs in normal, bold, italic and larger sizes. Localized case mapping is also working. Next steps are word wrap and pagination.
  - [ ] Software: [Port Babel to CircuitPython](https://github.com/joeycastillo/circuitpython/tree/babel). This had been a baseline goal, but I think the minimum viable software is going to happen in C++ at least for now.
- [ ] External dependencies: modify [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD) and [GxEPD2](https://github.com/ZinggJM/GxEPD2) to support eInk screens on secondary SPI bus. (for now you can just replace all references to `SPI` with `SPI1`)
- [ ] Software: minimum viable menu system for listing and selecting books as text files on SD card.
- [ ] Software: minimum viable interface for reading a book, storing user's place on SD card.
- [ ] Hardware: Migrate Open Book PCB to KiCad.

## Hardware: The Open Book Feather

The Open Book is a Feather-compatible SAMD51-based board inspired greatly by the [Adafruit PyBadge](https://www.adafruit.com/product/4200). In addition to standard Feather features (3.3v logic, built in LiPo charging), it supports a variety of peripherals aimed at creating an accessible, universal text reading device.

![image](/images/book-rev1.jpg)

Main features:

 * 4.2" inch e-paper display with partial refresh, driven over a dedicated SPI bus.
 * Seven buttons (directional pad, select button and page turn buttons) accessible via a shift register, as well as an eighth button (lock) connected to an interrupt pin on the SAMD11.
 * A MicroSD slot on the main SPI bus
 * A 2MB flash chip and Neopixel for CircuitPython support.
 * A second 2MB Flash chip for universal language support with [Babel](https://github.com/joeycastillo/babel).
 * A 32kb SRAM chip for buffering the e-ink display.
 * Access to two digital or analog pins via STEMMA-compatible ports with current and voltage protection.
 * Access to an I2C interface via a third STEMMA-compatible port.
 * An audio combo jack with stereo audio output and raw microphone input.
 * A MAX4466 microphone amplifier for amplified microphone input.

As the name implies, the intent of this board is to create an accessible open source device for reading books. These peripherals were chosen with this use case in mind (screen for displaying words; MicroSD for storing texts to read; Flash chip for language support; headphone jack for playing back audio books; microphone input for voice control). Having said that, this board is capable of more; you could turn it into a music player or voice memo recorder; use the STEMMA ports to add sensors and turn it into an air quality monitor; plug in an [AirLift wing](https://www.adafruit.com/product/4264) and have it display sports scores or transit alerts. I'm really curious what uses folks will come up with.

More documentation will come, but for the moment, a couple of other notes about intent: the design of the board prioritizes inexpensive components, to try to make it as accessible as possible. It also uses larger surface mount components than you might be expecting, specifically to make it easier for folks to build at home without a fancy pick and place. Resistors and capacitors are all 0805 or larger to make hand-placement more friendly, and where a chip was available in TSSOP and SOIC packages, I went with the SOIC. The only tricky parts are the flex connector and the SAMD51, which both have 0.5mm pitch. There was really no way around those. If this seems a bit outside your wheelhouse, read on.

## Hardware: The E-Book Feather Wing

The eBook wing does less than the Open Book Feather, mainly because it's limited to using only the pins available via the Feather header. Still, it packs quite a few features:

* Same 4.2" screen, this time driven over the main SPI bus.
* Eight buttons on an I2C port expander (more expensive than the shift register, but we don't have three pins to spare).
* MicroSD slot on the main SPI bus.
* 2MB Flash chip for Babel (not connected by default)
* 32kb SRAM for screen buffering
* I2C and pins A1 and A2 broken out as STEMMA ports with current and voltage protection.
* Mono headphone jack driven by A0.

The wing also leaves pins 11, 12 and 13 available so that it can stack with the AirLift FeatherWing.

![image](/images/wing-rev2.jpg)

Initially the eBook Wing was intended to just validate the ideas going into the Open Book, but after building a few of the Open Book board, I think it has a place as its own thing. It's a much simpler build, with fewer parts to place and more space to rework if need be. It also supports probably 80% of the desired use cases: screen, storage and buttons, language support, audio out and expansion ports. You lose mic input, but you can remedy that with [a STEMMA-compatible microphone](https://www.adafruit.com/product/1063).

Basically, if you want to build your own book but don't have a reflow oven or aren't comfortable placing really fine-pitched parts, the wing is a way to DIY and let a [Feather M4](https://www.adafruit.com/product/3857) handle the power and compute stuff.

## Next Steps

Boards for the Book rev 2 and Wing rev 3 are on their way. Need to assemble and test those and then publish a BOM. Software-wise, next steps are finishing up the Babel typesetter and then getting started on the minimum viable book reader UI.
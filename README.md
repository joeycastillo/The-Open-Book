# The Open Book Project

**Quick Links**

* [Purchase the E-Book FeatherWing PCB on Tindie](https://www.tindie.com/products/19994/).
* Purchase the Open Book PCB on Tindie (coming very soon).
* [Setup guide for the E-Book Wing](https://github.com/joeycastillo/The-Open-Book/wiki/E-Book-Wing-Setup-Guide).
* [Setup guide and documentation for the Open Book](https://github.com/joeycastillo/The-Open-Book/wiki/Open-Book-Documentation).
* [Patreon page](https://www.patreon.com/joeycastillo), where I post occasional notes and thoughts that don't fit into [tweets](https://twitter.com/josecastillo).
* [Mailing list signup](http://eepurl.com/gKOpQ9); I haven't sent anything yet, but when I have something big to share, you'll get it there.

With that out of the way, on with the readme!

As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

![image](/images/board.jpg)

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## State of the Book

In January 2020, the Open Book was named winner of Hackaday's Take Flight with Feather contest! This means that at some point in the future, Adafruit will manufacture 100 of the boards, and they will be available for purchase at DigiKey. The COVID situation has impacted the timeline for this, but in the meantime, I've been trying my best to make the project as DIY friendly as possible.

In May 2020, I put 24 of [the bare E-Book FeatherWing PCB's on Tindie](https://www.tindie.com/products/19994/). Those all sold, and a second batch of 25 is available there now. Note that this is a bare PCB; you will still need to purchase components and assemble the board yourself.

As of June 2020, the final Open Book revision is ready! I ordered 50 earlier this month, assembled and tested one, and expect to have the other 49 on Tindie by the end of the month. Again, this will be a bare PCB; you will need to purchase everything in the BOM and assemble the board yourself.

Software support has made great strides; on the Arduino side, the Open Book IL0398 driver has waveforms for 2-bit grayscale mode and a "quick" mode for turning pages, as well as initial support for partial refresh (though the API for this will change). There is also a minimum viable e-book reading application ([MVBook](https://github.com/joeycastillo/The-Open-Book/tree/master/Examples/Open_Book_MVBook)) available in this repository, to showcase the basics of what this hardware is capable of.

The Open Book also [supports CircuitPython](https://circuitpython.org/board/openbook_m4/)!

As of June 2020, this is where we are on the roadmap:

- [X] Hardware: Build Feather Wing to validate peripheral hardware design.
- [X] Hardware: Open Book Feather with SAMD51 processor, Flash memory, shift register for buttons, indicator LED, etc.
- [X] Board support: [UF2 Bootloader for Open Book](https://github.com/adafruit/uf2-samdx1/releases)
- [X] Board support: [Arduino core for Open Book](https://github.com/joeycastillo/ArduinoCore-samd)
- [X] Board support: [CircuitPython board definition for Open Book](https://circuitpython.org/board/openbook_m4/).
- [X] Software: [Universal language support with Babel](https://github.com/joeycastillo/babel). Word wrap and pagination are WIP, but it's stable enough for general use.
  - [X] Software: [Port Babel to CircuitPython](https://github.com/joeycastillo/babel/tree/master/CircuitPython). Right-to-left support is still lacking; need to make a custom Label class for this.
- [X] External dependencies: modify [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD) to support e-paper screens on secondary SPI bus.
- [X] Software: [minimum viable software](https://github.com/joeycastillo/The-Open-Book/tree/master/Examples/Open_Book_MVBook) for listing and selecting books as text files on SD card, reading a work...
  - [ ] ...and storing the user's place in that work between reading sessions.
- [ ] Hardware: Migrate Open Book PCB to KiCad.

## Hardware: The Open Book Feather

The Open Book is a Feather-compatible SAMD51-based board inspired greatly by the [Adafruit PyBadge](https://www.adafruit.com/product/4200). In addition to standard Feather features (3.3v logic, built in LiPo charging), it supports a variety of peripherals aimed at creating an accessible, universal text reading device. The PCB can be ordered directly from OSH Park at [this link](https://oshpark.com/shared_projects/9h6jn9B1)

![image](/images/book-rev5.jpg)

Main features:

 * 4.2" inch e-paper display with partial refresh, driven over a dedicated SPI bus.
 * Seven buttons (directional pad, select button and page turn buttons) accessible via a shift register, as well as an eighth button (lock) connected to an interrupt pin on the SAMD51.
 * A MicroSD slot on the main SPI bus, with SD card detect.
 * LEDs to indicate charging and full status, plus a red LED for general blinking.
 * A 2MB flash chip and Neopixel for CircuitPython support.
 * A second 2MB Flash chip for universal language support with [Babel](https://github.com/joeycastillo/babel).
 * Access to two digital or analog pins via STEMMA-compatible ports with current and voltage protection.
 * Access to an I2C interface via a third STEMMA-compatible port.
 * Voltage monitors on both the battery and the USB power port.
 * An audio combo jack with stereo audio output and raw microphone input.
 * A MAX4468 microphone amplifier for amplified microphone input.

As the name implies, the intent of this board is to create an accessible open source device for reading books. These peripherals were chosen with this use case in mind (screen for displaying words; MicroSD for storing texts to read; Flash chip for language support; headphone jack for playing back audio books; microphone input for voice control). Having said that, this board is capable of more; you could turn it into a music player or voice memo recorder; use the STEMMA ports to add sensors and turn it into an air quality monitor; plug in an [AirLift wing](https://www.adafruit.com/product/4264) and have it display sports scores or transit alerts. I'm really curious what uses folks will come up with.

More documentation will come, but for the moment, a couple of other notes about intent: the design of the board prioritizes inexpensive components, to try to make it as affordable as possible. It also uses larger surface mount components than you might be expecting, specifically to make it easier for folks to build at home without a fancy pick and place. Resistors and capacitors are all 0805 or larger to make hand-placement more friendly, and where a chip was available in TSSOP and SOIC packages, I went with the SOIC. The only tricky parts are the flex connector and the SAMD51, which both have 0.5mm pitch. There was really no way around those. If this seems a bit outside your wheelhouse, read on.

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

![image](/images/wing-rev5.jpg)

Initially the eBook Wing was intended to just validate the ideas going into the Open Book, but after building a few of the Open Book board, I think it has a place as its own thing. It's a much simpler build, with fewer parts to place and more space to rework if need be. It also supports probably 80% of the desired use cases: screen, storage and buttons, language support, audio out and expansion ports. You lose mic input, but you can remedy that with [a STEMMA-compatible microphone](https://www.adafruit.com/product/1063).

Basically, if you want to build your own book but don't have a reflow oven or aren't comfortable placing really fine-pitched parts, the wing is a way to DIY and let a [Feather M4](https://www.adafruit.com/product/3857) handle the power and compute stuff.

## The BOMs

A couple of notes on this. First off, the Feather connectors are ludicrously expensive compared to the rest of the BOM. More for a row of connectors than an entire microcontroller?! Ridiculous. If you're doing this as a DIY project, try ordering [two of these 2x20 surface mount headers](https://www.adafruit.com/product/2187) and cutting them down to the size you want; it'll shave almost $4 off the BOM, and they're much slimmer than the ones linked here. If you want a slim connector like that one, without the hassle of cutting things to size, Samtec has [2x12](https://www.samtec.com/products/hle-112-02-g-dv-a-p) and [2x16](https://www.samtec.com/products/hle-116-02-g-dv-a-k) slim headers with similar pricing at low volumes; still trying to figure out how to make this more reasonable.

Second, the 1µF capacitors. Strictly speaking, only the ones for the e-ink display _need_ to be rated for 25V, but since pricing is so similar for 10V or 25V caps, I just grouped them all together so you don't have to worry about the distinction.

Finally, for the Flash chips, the GD25Q16C and GD25Q32C are 100% pin-compatible, which is to say that if you want more space for CircuitPython storage, feel free to use the larger chip for U4. It's twice the storage for an extra 25¢. You can also use the larger chip for Babel's U6; it won't function any differently right now, but I eventually plan to use that extra space to support the supplemental multilingual plane, including emoji!

OK, here's the rundown:

### Open Book Feather A1 Rev 05

I'm in the process of moving the BOM for this board to Kitspace, and will be updating this section shortly.

### E-Book FeatherWing A1 Rev 05

The E-Book Wing BOM is currently available at [this URL](https://kitspace.org/boards/github.com/joeycastillo/the-open-book/) but that will change once I add the second board to Kitspace; if you're still seeing this paragraph after July 1, check for an update.

## Next Steps

Almost 30 E-Book wing PCBs are shipped and in the hands of makers, and the Open Book PCB should be shipping soon. I know that Adafruit remains excited about the Take Flight with Feather run, but the COVID crisis has left the timing of that in question. With that said, the next steps are all software related. 

Over the course of the month of July, my goals are to:

* Produce a specification and design document for the e-book reader software
* Publish a guide for contributors
* Ship as many of the Open Book PCBs as folks want
* Start writing the software!

## Schematics

![image](/images/oso-book-a1-05-schematic-page1.png)

![image](/images/oso-book-a1-05-schematic-page2.png)

![image](/images/oso-book-a1-05-schematic-page3.png)

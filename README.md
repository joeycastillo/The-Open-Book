# The Open Book Project

**Quick Links**

* [Purchase the E-Book FeatherWing PCB on Tindie](https://www.tindie.com/products/19994/)
* [Purchase the Open Book PCB on Tindie](https://www.tindie.com/products/20599/)
* [Setup guide for the E-Book Wing](https://github.com/joeycastillo/The-Open-Book/wiki/E-Book-Wing-Setup-Guide) (and [Assembly Video](https://youtu.be/0Bsh1q2tqaU))
* [Setup guide and documentation for the Open Book](https://github.com/joeycastillo/The-Open-Book/wiki/Open-Book-Documentation)
* [Project page on Hackaday.io](https://hackaday.io/project/168761-the-open-book-feather)
* [Mailing list signup](http://eepurl.com/gKOpQ9); I haven't sent anything yet, but when I have something big to share, you'll get it there.

With that out of the way, on with the readme!

As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

![image](/images/board.jpg)

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## State of the Book

In January 2020, the Open Book was named winner of Hackaday's Take Flight with Feather contest! This means that at some point in the future, Adafruit will manufacture 100 of the boards, and they will be available for purchase at DigiKey. The COVID situation has impacted the timeline for this, but in the meantime, I've been trying my best to make the project as DIY friendly as possible.

In May 2020, I put [the bare E-Book FeatherWing PCB's on Tindie](https://www.tindie.com/products/19994/). Despite selling out a couple of times, I've kept restocking, and they remain available as of October 2020. Note that this is a bare PCB; you will still need to purchase components and assemble the board yourself.

In June 2020, I put [the Open Book PCB on Tindie](https://www.tindie.com/products/20599/) as well. The first run of 50 boards sold out in September, but as of October they are available again.

Software support has made great strides; on the Arduino side, the Open Book IL0398 driver has waveforms for 2-bit grayscale mode and a "quick" mode for turning pages, as well as initial support for partial refresh (though the API for this will change). There is also a minimum viable e-book reading application ([MVBook](https://github.com/joeycastillo/The-Open-Book/tree/master/Examples/Open_Book_MVBook)) available in this repository, to showcase the basics of what this hardware is capable of.

The Open Book also [supports CircuitPython](https://circuitpython.org/board/openbook_m4/)!

As of October 2020, this is where we are on the roadmap:

- [X] Hardware: Build Feather Wing to validate peripheral hardware design.
- [X] Hardware: Open Book Feather with SAMD51 processor, Flash memory, shift register for buttons, indicator LED, etc.
- [X] Board support: [UF2 Bootloader for Open Book](https://github.com/adafruit/uf2-samdx1/releases)
- [X] Board support: [Arduino core for Open Book](https://github.com/joeycastillo/ArduinoCore-samd)
- [X] Board support: [CircuitPython board definition for Open Book](https://circuitpython.org/board/openbook_m4/).
- [X] Software: [Universal language support with Babel](https://github.com/joeycastillo/babel). Word wrap and pagination are WIP, but it's stable enough for general use.
  - [X] Software: [Port Babel to CircuitPython](https://github.com/joeycastillo/babel/tree/master/CircuitPython). Right-to-left support is still lacking; need to make a custom Label class for this.
- [X] External dependencies: modify [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD) to support e-paper screens on secondary SPI bus.
- [X] Software: [minimum viable software](https://github.com/joeycastillo/The-Open-Book/tree/master/Examples/Open_Book_MVBook) for listing and selecting books as text files on SD card, reading a work...
  - [X] ...and storing the user's place in that work between reading sessions.
- [X] Hardware: Migrate Open Book PCB to KiCad. (So many thanks to [DCelectronics](https://github.com/DCelectronics) for the [pull request](https://github.com/joeycastillo/The-Open-Book/pull/38))!

## Hardware: The Open Book Feather

The Open Book is a Feather-compatible SAMD51-based board inspired greatly by the [Adafruit PyBadge](https://www.adafruit.com/product/4200). In addition to standard Feather features (3.3v logic, built in LiPo charging), it supports a variety of peripherals aimed at creating an accessible, universal text reading device. In addition to the Tindie store, you can order the PCB from OSH Park at [this link](https://oshpark.com/shared_projects/8h6dgwkn).

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

![image](/images/bowi-rev5.jpg)

Initially the eBook Wing was intended to just validate the ideas going into the Open Book, but after building a few of the Open Book board, I think it has a place as its own thing. It's a much simpler build, with fewer parts to place and more space to rework if need be. It also supports probably 80% of the desired use cases: screen, storage and buttons, language support, audio out and expansion ports. You lose mic input, but you can remedy that with [a STEMMA-compatible microphone](https://www.adafruit.com/product/1063).

Basically, if you want to build your own book but don't have a reflow oven or aren't comfortable placing really fine-pitched parts, the wing is a way to DIY and let a [Feather M4](https://www.adafruit.com/product/3857) handle the power and compute stuff.

## The BOMs

A couple of notes on this. First off, the Feather connectors are ludicrously expensive compared to the rest of the BOM. More for a row of connectors than an entire microcontroller?! Ridiculous. If you're doing this as a DIY project, try ordering [two of these 2x20 surface mount headers](https://www.adafruit.com/product/2187) and cutting them down to the size you want; it'll shave almost $4 off the BOM, and they're much slimmer than the ones linked here. If you want a slim connector like that one, without the hassle of cutting things to size, Samtec has [2x12](https://www.samtec.com/products/hle-112-02-g-dv-a-p) and [2x16](https://www.samtec.com/products/hle-116-02-g-dv-a-k) slim headers with similar pricing at low volumes; still trying to figure out how to make this more reasonable.

Second, the 1µF capacitors. Strictly speaking, only the ones for the e-ink display _need_ to be rated for 25V, but since pricing is so similar for 10V or 25V caps, I just grouped them all together so you don't have to worry about the distinction.

OK, here's the rundown:

### Open Book Feather A1 Rev 06

The Open Book BOM is [available on Kitspace](https://kitspace.org/boards/github.com/joeycastillo/the-open-book/open-book/).

### E-Book FeatherWing A1 Rev 06

The E-Book Wing BOM is also [available on Kitspace](https://kitspace.org/boards/github.com/joeycastillo/the-open-book/ebook-wing/).

## Next Steps

As of October 2020, more than 100 Open Book and E-Book FeatherWing PCBs have been shipped and are in the hands of makers, and several folks have posted photos of their completed builds! I know that Adafruit remains excited about the Take Flight with Feather run, but the ongoing COVID situation has left the timing of that in question.

As summer turned to fall, I got it in my head to try to build the Open Book's reader software using CircuitPython, which spawned the [circuitpyui project](https://github.com/joeycastillo/circuitpyui) in September. This project aims to create an event-driven UI framework on top of CircuitPython's displayio. I still think this is a great idea — and it actually works pretty well on TFT-based devices so far — but there was also some [democracy-related work](https://github.com/joeycastillo/voteforthedemocrat) that needed doing this fall, so I haven't made as much progress on this as I had hoped.

I have tried my best to document this project pretty relentlessly, from the silkscreen itself to the [documentation](https://github.com/joeycastillo/The-Open-Book/tree/master/eBook%20Feather%20Wing/Documentation%20Packet) [packets](https://github.com/joeycastillo/The-Open-Book/tree/master/eBook%20Feather%20Wing/Documentation%20Packet) that come packed with each PCB. Having said that, I also recognize that I have not done as good a job as I could have in creating guidelines for contributors or outlining the areas where outside help would have the most impact. This hasn't some stopped folks from very generously contributing their time and effort to the project! Still, I know that this is an area that I need to improve, and my hope is that as fall turns to winter, I can get the project to a state where folks feel more empowered to take part.

## Schematics

![image](/images/oso-book-a1-06-schematic-page1.png)

![image](/images/oso-book-a1-06-schematic-page2.png)

![image](/images/oso-book-a1-06-schematic-page3.png)

# The Open Book Project

THE PREMISE: As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

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

Reworking this README a bit, check back soon for more.

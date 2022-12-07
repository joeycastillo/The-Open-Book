# The Open Book: Project Reboot

**A note from the project creator:** The Open Book was my first real electronics project; the earliest designs date to late 2019. I have learned a lot since those early days, and as such, here three years later, I am hitting reset. At this time the repository contains a version of the Open Book designed around the Raspberry Pi Pico. It's optimized for low part count and easy hand assembly, but it runs on AAA batteries and may not be as svelte as some folks might prefer. At a later date, I hope to design a followup with built-in LiPo charging and a slimmer profile, but at this time, this version of the book is the quickest path to getting hardware in people's hands so that we can start hacking on firmware together.

For details on how to build, test and use Open Book, I've [made documentation available here](https://www.oddlyspecificobjects.com/projects/openbook/).

The original Open Book repository has been archived in a branch called [original](https://github.com/joeycastillo/The-Open-Book/tree/original).

[The Open Book firmware, called libros, is under development here](https://github.com/joeycastillo/libros). It's a goddamn mess in some ways, and in dire need of some documentation, but for the moment it does do the job of presenting a list of books stored on an SD card, and letting you read them. The canonical format for books is plain text with the book title on the first line, OR plain text plus some front matter and some ASCII control codes for chapter breaks and formatting, [as documented here](https://www.oddlyspecificobjects.com/projects/openbook/#advanced-text-formatting).

## Original Introduction

As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## State of the Book

At this time, the Pi Pico book is in decent shape if you want to try your hand at building it yourself. You will need to have two custom things fabricated: the **Open Book Main Board** (which you can get as a bare PCB) and the **Castellated E-Paper Driver** module (which you'll want to have done as a PCBA job). All the files you will need to send out for this can be found in the **Fabrication Files** folder in the project root:

* Upload `OSO-BOOK-C1-04-rounded.zip` to your PCB fabrication house of choice. It is designed to be a two-layer, 1 mm thick PCB, and you can use either an ENIG or lead-free HASL finish.
* If you plan to use JLCPCB's economic PCBA service, upload all three files in `OSO-BOOK-C2-01` to JLCPCB. Opt for a 1 mm thick lead-free HASL finish. Note that the board is slightly wider than it needs to be, just to meet the minimum size requirements for this service.
* If you plan to use PCBWay's PCBA service, upload all three files in `OSO-BOOK-C2-02` to PCBWay. Once again, opt for a 1 mm thick lead-free HASL finish. 

Other Parts: 

* Two of these [side mount buttons](https://www.digikey.com/en/products/detail/würth-elektronik/434351045816/5209090)
* One of these [side-mount switches](https://www.digikey.com/en/products/detail/c-k/JS102011SAQN/1640095)
* One [MEM2075 MicroSD card slot](https://www.digikey.com/en/products/detail/gct/MEM2075-00-140-01-A/9859614)
* One [GD25Q16C Flash chip](https://www.digikey.com/en/products/detail/gigadevice-semiconductor-hk-limited/GD25Q16CTIGR/9484675) with SOIC / SOP8 footprint.
* One [Keystone 1022 dual AAA battery holder](https://www.digikey.com/en/products/detail/keystone-electronics/1022/2137859) (you can get clones on Aliexpress for cheap)
* Two P-channel MOSFETS with SOT23 footprint (I use the DMG3415)
* Two 10kΩ resistors with 1206 footprint.
* Two 10µF capacitors with 1206 footprint (rated voltage >=6.3V).
* One 1µF capacitor with 0805 footprint (rated voltage >=6.3V).
* Seven through-hole slim tactile buttons (3mm by 6mm; [TL1107 type](https://www.digikey.com/en/products/detail/e-switch/TL1107AF130WQ/378976))
* One [GDEW042T2 grayscale e-paper display](https://buy-lcd.com/products/42inch-e-inkanel-spi-interface-buy-eaper-display). (Don't get the tri-color version; it'll end in heartbreak)
* And finally, one [Raspberry Pi Pico](https://www.digikey.com/en/products/detail/raspberry-pi/SC0915/13624793) board

The Open Book is open source hardware: you should feel free to build one yourself, order parts for ten and do a workshop at your local maker space, or even buy parts for fifty and sell them as kits. 

Please steal this book.

I plan to add more documentation in the new year, but until then, [this half-hour video walks through building one Open Book board in real-time](https://twitter.com/i/broadcasts/1OyKAVPjrvaGb).

### Forking and tweaking the boards

* Design files for the Open Book main board can be found in the `OSO-BOOK-C1` folder. It's a KiCad project.
* Design files for the castellated e-paper driver module can be found in the `OSO-BOOK-C2` folder. Alas, they are Eagle projects that predate my move to KiCad. There are two versions: an older version that was successfully fabricated with JLCPCB's economic PCBA service (`OSO-BOOK-C2-01`), and a newer version successfully fabricated using PCBWay's PCBA service (`OSO-BOOK-C2-02`). Both work great.

## License

 Different components of the project are licensed differently, see [LICENSE.md](https://github.com/joeycastillo/The-Open-Book/blob/main/LICENSE.md).

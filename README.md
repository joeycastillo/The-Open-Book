# The Open Book Project

THE PREMISE: As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## A Note to Visitors

The most important thing I can reiterate in this README is that **This Is A Work In Progress!** The eBook Wing is a first crack at the hardware, to validate things like the e-paper and audio driving circuitry, and to serve as a testbed for the software that will eventually drive the final product. The Wing is just an accessory board, and it requires a [Feather M4 Express](https://www.adafruit.com/product/3857) to do the heavy lifting. Moreover, the software required to actually be an eBook is in its infancy; I can put a few Arduino sketches up here, but the long-term goal involves building open source eBook software in CircuitPython, and that's still a ways out. 

The goal is to take the lessons learned on the Wing, and apply them to the final product, The Open Book, which will contain its own microcontroller and power circuitry, so that it can be a standalone Feather that you can use as-is or extend with other wings. I'm hoping to accomplish that by the end of the year. If you want to help bring this idea into the world, feel free to fork this and open a pull request! My general road map looks something like this: 

- [X] Hardware: Build Feather Wing to validate peripheral hardware design
  - [ ] Software: Native [GNU Unifont support](https://github.com/joeycastillo/Adafruit-GFX-Library/tree/master/unifontconvert) in CircuitPython (support all characters of all languages + RTL text layout, accents, diacritics)
  - [ ] Software: CircuitPython menu system for listing and selecting books on SD card
  - [ ] Software: CircuitPython interface for reading a book, storing user's place on SD card
- [ ] Hardware: Open Book Feather with SAMD51 processor, Flash memory, shift register for buttons, indicator LED, etc.
  - [ ] Board support: UF2 Bootloader for Open Book
  - [ ] Board support: CircuitPython board definition for Open Book
  - [ ] Secondary goal: Arduino core for Open Book

## The E-Book Feather Wing

The Feather ecosystem seemed like the ideal place to start building this. An eBook reader is an inherently portable device, and all Feather mainboards support LiPo battery operation and charging via USB. All Feather boards work with the Arduino IDE, which is super accessible for anyone to hack on, and newer boards also support CircuitPython, which lowers the bar to entry even further.

![image](/images/board.png)

### The BOM

| Item          | Schematic Ref | Cost  | Qty | Total |
| ------------- | ------------- | -----:|:---:| -----:|
| [10 µF Capacitor](https://www.digikey.com/short/prr548) | C1 C2 | $0.24 | 2 | $0.48 |
| [1 µF Capacitor](https://www.digikey.com/short/prr543) | C3 C4 C5 C10 C11 C12 | $0.10 | 6 | $0.60 |
| [10 µF Capacitor, 25V rated](https://www.digikey.com/short/prr59w) | C7 C8 C13 C14 C15 C16 | $0.19 | 6 | $1.14 |
| [4.7 µF Capacitor, 25V rated](https://www.digikey.com/short/prr5b9) | C9 | $0.19 | 1 | $0.19 |
| [100 µF Capacitor](https://www.digikey.com/short/prr545) | C6 | $0.70 | 1 | $0.70 |
| [10µH Inductor](https://www.digikey.com/short/prr5bv) | L1 | $0.64 | 1 | $0.64 |
| [1KΩ Resistor](https://www.digikey.com/short/prr5ff) | R1 R2 | $0.10 | 2 | $0.20 |
| [10KΩ Resistor](https://www.digikey.com/short/prr5f5) | R3 R4 R6 R7 | $0.10 | 4 | $0.40 |
| [100Ω Resistor](https://www.digikey.com/short/prr5f9) | R8 R9 | $0.10 | 2 | $0.20 |
| [0.47Ω Resistor](https://www.digikey.com/short/prr5fh) | R5 | $0.17 | 1 | $0.17 |
| [Ferrite Bead](https://www.digikey.com/short/prr5fd) | FB1 FB2 | $0.11 | 2 | $0.22 |
| [3.6V Zener Diode](https://www.digikey.com/short/prr5f3) | D1 D2 | $0.21 | 2 | $0.42 |
| [30V Schottky Diode](https://www.digikey.com/short/prr5f8) | D3 D4 D5 | $0.21 | 3 | $0.63 |
| [N-Channel MOSFET](https://www.digikey.com/short/prr5f0) | Q1 | $0.48 | 1 | $0.48 |
| [MCP23008 GPIO Expander](https://www.digikey.com/short/prr52n) | IC1 | $1.05 | 1 | $1.05 |
| [32KB SRAM Chip](https://www.digikey.com/short/prr52f) | IC2 | $1.14 | 1 | $1.14 |
| [2MB NOR Flash Chip](https://www.digikey.com/short/prr525) | IC3 | $0.56 | 1 | $0.56 |
| [3-position JST-PH connector](https://www.digikey.com/short/prr5pc) | A1 A2 ports | $0.69 | 2 | $1.38 |
| [4-position JST-PH connector](https://www.digikey.com/short/prr9wp) | I2C port | $0.79 | 1 | $0.79 |
| [24-position FFC connector](https://www.digikey.com/short/prr5wm) | - | $0.46 | 1 | $0.46 |
| [Right angle surface mount buttons](https://www.digikey.com/short/prr5pv) | - | $0.54 | 4 | $2.16 |
| [Thru-hole buttons](https://www.digikey.com/short/prr5pq) | - | $0.37 | 7 | $2.59 |
| [SPDT switch](https://www.digikey.com/short/prr5w1) | SW1 | $0.31 | 1 | $0.31 |
| [Headphone Jack](https://www.digikey.com/short/prr5np) | J1 | $1.24 | 1 | $1.24 |
| [4.2 inch e-ink panel](https://zh-tw.buyepaper.com/42inch-e-ink-panel-spi-interface-buy-e-paper-display-p0085-p0085.html) | - | $18.26 | 1 | $18.26 |
| [MicroSD Socket](https://www.adafruit.com/product/1660) | MICROSD | $1.95 | 1 | $1.95 |
| [2x20 SMT Headers](https://www.adafruit.com/product/2187) (to cut down) | - | $1.95 | 2 | $3.90 |
| **GRAND TOTAL** |  |  |  | $42.26 |

You'll also need to [order the board from OSH Park](https://oshpark.com/shared_projects/rzopLYdJ); it's $66.80 for three.

Note that you can omit some of these things; the headphone jack might not be necessary for your use case, and I'm unclear whether the SRAM chip is going to be 100% useful; in the end it's your book, make it with whatever bits you want

## Going Forward

Immediate goals as of 9/2019: I'm writing a native module for CircuitPython that reads the GNU Unifont from the Wing's flash chip, and uses that data for super fast text rendering of pixels out to a bitmap. Plus other Unicode features like bidirectional text, RTL mirroring, accents and diacritics, etc. When that's done, the book should be able to display text in all the languages of the world, in CircuitPython. 

After that, the goal will be to start writing the user interface in Python, while simultaneously making progress toward a final PCB layout for the Open Book. 

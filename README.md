# The Open Book Project

THE PREMISE: As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## Phase One: The E-Book Feather Wing

The Feather ecosystem seemed like the ideal place to start building this. An eBook reader is an inherently portable device, and all Feather mainboards support LiPo battery operation and charging via USB. All Feather boards work with the Arduino IDE, which is super accessible for anyone to hack on, and newer boards also support CircuitPython, which lowers the bar to entry even further.

![image](/images/board.png)

The eBook feather wing is designed as a set of modules that add features to the board. Some are more or less mandatory (to achieve a minimum of functionality); some are recommended (to achieve big picture design goals) and some are optional (to enable certain specific use cases). In all cases, the board outlines which modules live where and what they do.

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

### Mandatory Modules and bits

* The **e-paper screen** and supporting circuitry. You'll need a 24 position FFC connector with contacts on top, the Good Display 4.2 inch e-paper display, and all the components in the e-paper display block. Note that many of the capacitors in this block need to be rated for 25V.
* The **GPIO expander**. It lets you access all the buttons over I2C. You could skip this, and just solder from a few of the pads to the few remaining GPIO pins, but it's like $1, treat yourself. In the future I'm going to use a shift register for this purpose.
* The **Feather headers**, obviously, to connect a Feather mainboard to the wing.
* The through-hole **buttons** on the front, at least some of them. Oh and as a side note: these are thru-hole buttons because I wanted to keep all surface mount components on one side.

### Recommended Modules

* The **MicroSD socket**. This is, like, highly recommended. Where else are you going to store your library? Note that you will need a fine tip on your soldering iron or a hot air reflow station to solder this one in.
* The **JST-PH ports**, and parts in that block. These are so useful to make the book extensible. Imagine a gesture recognizer on the I2C port for someone with limited mobility, or an electret mic for voice control, or a strip of LED's as a reading light, or any number of things you haven't imagined yet!
* The **Enable switch**. It's a switch, you use it to turn the Feather off, which saves battery.

### Optional Modules and bits

* The **2MB Flash Chip**. I recommend using a [Feather M4 Express](https://www.adafruit.com/product/3857) with this wing. It's only $3 more than an M0, and it has its own Flash on a dedicated QSPI bus, making this chip totally unnecessary.
* The **32KB SRAM chip**. The Adafruit EPD library can use this to avoid allocating a buffer in RAM for the screen. Also useful if you're using their CircuitPython EPD stuff. can also make use of this chip. But it's not, strictly speaking, required.
* The **headphone jack** and supporting circuitry. It might be useful; I can imagine playing wav file audiobooks from an SD card for example, but you can omit it unless you have a specific audio use case in mind.
* The **surface mount buttons** (previous page, next page and reset). You can already access the previous and next page buttons on the front, so these are just nice-to-have. And because of the way the Feather connects, its reset button should always be exposed, so you really don't need another reset button unless you want it.

## Going Forward

Ordering the Rev B boards and hoping to assemble one by the end of the month to verify the design. After that, the Phase Two goal is to design a SAMD51-based Feather mainboard with this same capabilities (e-paper screen, buttons, SD card, etc).

I'll also have to start writing an open source firmware that can hold a library on an SD card and let people read, but hey, one thing at a time.

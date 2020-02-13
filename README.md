# The Open Book Project

A quick note: after many requests, I've set up [a Patreon page where folks can support the work](https://www.patreon.com/joeycastillo). I also plan to use it to post occasional notes and thoughts that don't fit into tweets. For less frequent updates about the Open Book and other Oddly Specific Objects, [sign up for the mailing list here](http://eepurl.com/gKOpQ9). I hate spam as much as the next guy (honestly probably a lot more) so I promise I'll only reach out when I have something to share that you'll want to hear about. Okay, on with the readme!

As a society, we need an open source device for reading. Books are among the most important documents of our culture, yet the most popular and widespread devices we have for reading — the Kobo, the Nook, the Kindle and even the iPad — are closed devices, operating as small moving parts in a set of giant closed platforms whose owners' interests are not always aligned with readers'.

![image](/images/board.jpg)

The Open Book aims to be a simple device that anyone with a soldering iron can build for themselves. The Open Book should be comprehensible: the reader should be able to look at it and understand, at least in broad strokes, how it works. It should be extensible, so that a reader with different needs can write code and add accessories that make the book work for them. It should be global, supporting readers of books in all the languages of the world. Most of all, it should be open, so that anyone can take this design as a starting point and use it to build a better book.

## A Note to Visitors: State of the Book

In January 2020, the Open Book was named winner of Hackaday's Take Flight with Feather contest! This means that at some point in the near future, Adafruit will manufacture 100 of the boards, and they will be available for purchase at DigiKey.

The Open Book Rev 4 board is at 99%; the only hardware issue is that there is a slight buzzing sound on the headphone jack when plugged in to USB; when tied directly to ground, the USB cable's shield acts like an antenna, picking up RF noise from the environment. I plan to address this with an RC filter between the USB shield and the ground plane.

Software support has made great strides; on the Arduino side, the Open Book IL0398 driver has waveforms for 2-bit grayscale mode and a "quick" mode for turning pages, as well as initial support for partial refresh (though the API for this will change).

The Open Book also [supports CircuitPython](https://circuitpython.org/board/openbook_m4/)!

As of January 2020, this is where we are on the roadmap:

- [X] Hardware: Build Feather Wing to validate peripheral hardware design.
- [X] Hardware: Open Book Feather with SAMD51 processor, Flash memory, shift register for buttons, indicator LED, etc.
- [X] Board support: [UF2 Bootloader for Open Book](https://github.com/adafruit/uf2-samdx1/releases)
- [X] Board support: [Arduino core for Open Book](https://github.com/joeycastillo/ArduinoCore-samd)
- [X] Board support: [CircuitPython board definition for Open Book](https://circuitpython.org/board/openbook_m4/).
- [X] Software: [Universal language support with Babel](https://github.com/joeycastillo/babel). Word wrap and pagination are WIP, but it's stable enough for general use.
  - [ ] Software: [Port Babel to CircuitPython](https://github.com/joeycastillo/circuitpython/tree/babel).
- [X] External dependencies: modify [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD) to support e-paper screens on secondary SPI bus.
- [ ] Software: [minimum viable software](https://github.com/joeycastillo/The-Open-Book/tree/master/Examples/Open_Book_MVBook) for listing and selecting books as text files on SD card, reading a book, storing user's place.
- [ ] Hardware: Migrate Open Book PCB to KiCad.

## Hardware: The Open Book Feather

The Open Book is a Feather-compatible SAMD51-based board inspired greatly by the [Adafruit PyBadge](https://www.adafruit.com/product/4200). In addition to standard Feather features (3.3v logic, built in LiPo charging), it supports a variety of peripherals aimed at creating an accessible, universal text reading device. The PCB can be ordered directly from OSH Park at [this link](https://oshpark.com/shared_projects/9h6jn9B1)

![image](/images/book-rev4.jpg)

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
 * A MAX4466 microphone amplifier for amplified microphone input.

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

![image](/images/wing-rev2.jpg)

Initially the eBook Wing was intended to just validate the ideas going into the Open Book, but after building a few of the Open Book board, I think it has a place as its own thing. It's a much simpler build, with fewer parts to place and more space to rework if need be. It also supports probably 80% of the desired use cases: screen, storage and buttons, language support, audio out and expansion ports. You lose mic input, but you can remedy that with [a STEMMA-compatible microphone](https://www.adafruit.com/product/1063).

Basically, if you want to build your own book but don't have a reflow oven or aren't comfortable placing really fine-pitched parts, the wing is a way to DIY and let a [Feather M4](https://www.adafruit.com/product/3857) handle the power and compute stuff.

## The BOMs

A couple of notes on this. First off, the Feather connectors are ludicrously expensive compared to the rest of the BOM. More for a row of connectors than an entire microcontroller?! Ridiculous. If you're doing this as a DIY project, try ordering [two of these 2x20 surface mount headers](https://www.adafruit.com/product/2187) and cutting them down to the size you want; it'll shave almost $4 off the BOM, and they're much slimmer than the ones linked here. If you want a slim connector like that one, without the hassle of cutting things to size, Samtec has [2x12](https://www.samtec.com/products/hle-112-02-g-dv-a-p) and [2x16](https://www.samtec.com/products/hle-116-02-g-dv-a-k) slim headers with similar pricing at low volumes; still trying to figure out how to make this more reasonable.

Second, the 1µF capacitors. Strictly speaking, only the ones for the e-ink display _need_ to be rated for 25V, but since pricing is so similar for 10V or 25V caps, I just grouped them all together so you don't have to worry about the distinction.

Finally, for the Flash chips, the GD25Q16C and GD25Q32C are 100% pin-compatible, which is to say that if you want more space for CircuitPython storage, feel free to use the larger chip for U4. It's twice the storage for an extra 25¢. You can also use the larger chip for Babel's U6; it won't function any differently right now, but I eventually plan to use that extra space to support the supplemental multilingual plane, including emoji!

OK, here's the rundown:

### Open Book Feather A1 Rev 04

|Parts|Qty|Device|Value|Cost @ 1 assembly|Cost @ 3 assemblies|Cost @ 10 assemblies|Cost @ 100 assemblies|URL|
|-----|---|------|-----|-----|-----|-----|------|-----|
|C1, C2, C4, C5, C9, C11, C25, C27, C35|9|0805 Capacitor|10uF|$0.85|$2.30|$7.65|$24.46|https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL21A106KQCLRNC/1276-2405-1-ND/3890491|
|C3, C6, C7, C8, C10, C12, C14, C15, C16, C17, C18, C19, C20, C21, C22, C23, C26, C28, C29|19|0805 Capacitor|1uF/25V|$1.50|$4.50|$6.69|$37.56|https://www.digikey.com/product-detail/en/CL21B105KAFNNNE/1276-1066-1-ND/3889152/|
|C13, C32|2|0805 Capacitor|0.1uF|$0.20|$0.60|$1.42|$6.32|https://www.digikey.com/product-detail/en/kemet/C0805C104M3RACTU/399-8000-1-ND/3471723|
|C24|1|0805 Capacitor|4.7uF/25V|$0.30|$0.90|$2.05|$11.61|https://www.digikey.com/product-detail/en/TMK212AB7475KG-T/587-2990-1-ND/2714183/|
|C30, C31|2|1210 Capacitor|100uF|$1.40|$4.20|$10.36|$71.06|https://www.digikey.com/product-detail/en/CL32A107MQVNNNE/1276-1092-1-ND/3889178/|
|C33|1|0805 Capacitor|0.01uF|$0.10|$0.30|$0.68|$3.04|https://www.digikey.com/product-detail/en/kemet/C0805C103K1RAC7210/399-17617-1-ND/8572617|
|C34|1|0805 Capacitor|100pF|$0.10|$0.30|$0.67|$2.98|https://www.digikey.com/product-detail/en/kemet/C0805C101J5GACTU/399-1122-1-ND/411397|
|R1, R3, R5, R6|4|0805 Resistor|10K|$0.36|$0.43|$1.44|$5.88|https://www.digikey.com/product-detail/en/yageo/RC0805FR-0710KL/311-10.0KCRCT-ND/730482|
|R2, R8, R9, R11, R26|5|0805 Resistor|100K|$0.38|$0.57|$1.90|$7.75|https://www.digikey.com/product-detail/en/yageo/RC0805FR-07100KL/311-100KCRCT-ND/730491|
|R4, R10, R15, R16, R21, R22, R25|7|0805 Resistor|1K|$0.32|$0.67|$2.24|$9.24|https://www.digikey.com/product-detail/en/yageo/RC0805JR-071KL/311-1.0KARCT-ND/731165|
|R7, R17, R18, R19, R20|5|0805 Resistor|100|$0.35|$0.53|$1.75|$7.00|https://www.digikey.com/product-detail/en/yageo/RC0805JR-07100RL/311-100ARCT-ND/731185|
|R12|1|0805 Resistor|0.47ohm|$0.17|$0.51|$1.49|$5.74|https://www.digikey.com/product-detail/en/cts-resistor-products/73L3R47J/73L3R47JCT-ND/1543531|
|R13, R14|2|EXB-V8V103JV|100K array|$0.28|$0.84|$2.44|$9.36|https://www.digikey.com/product-detail/en/panasonic-electronic-components/EXB-V8V103JV/Y4103CT-ND/155171|
|R23, R24|2|0805 Resistor|1M|$0.20|$0.60|$0.80|$3.26|https://www.digikey.com/product-detail/en/yageo/RC0805FR-071ML/311-1.00MCRCT-ND/730392|
|D1|1|Schottky diode SOD-123|MBR120|$0.39|$1.17|$2.95|$18.36|https://www.digikey.com/product-detail/en/on-semiconductor/MBR120ESFT1G/MBR120ESFT1GOSCT-ND/964525|
|D2, D3, D4|3|Schottky diode SOD-123|MBR0530|$0.93|$2.79|$7.62|$40.35|https://www.digikey.com/product-detail/en/MBR0530T1G/MBR0530T1GOSCT-ND/917966/|
|D5, D6|2|Zener Diode SOD123|3.6V|$0.40|$1.20|$3.32|$17.62|https://www.digikey.com/product-detail/en/MMSZ4685-TP/MMSZ4685-TPMSCT-ND/2041552/|
|L1|1|Inductor NR401|10uH|$0.64|$1.92|$5.59|$39.96|https://www.digikey.com/product-detail/en/vishay-dale/IFSC1515AHER100M01/541-1406-1-ND/2744259|
|Q1|1|MOSFET-P (SOT23-R)|DMG341|$0.42|$1.26|$3.17|$19.76|https://www.digikey.com/product-detail/en/diodes-incorporated/DMG3415U-7/DMG3415UDICT-ND/2052799|
|Q2|1|MOSFET-N (SOT23-3)|IRLML0100|$0.48|$1.44|$4.11|$30.66|https://www.digikey.com/product-detail/en/infineon-technologies/IRLML0100TRPBF/IRLML0100TRPBFCT-ND/2202230|
|FB1, FB2|2|0805 Ferrite Bead|Ferrite|$0.22|$0.66|$1.76|$10.04|https://www.digikey.com/product-detail/en/tdk-corporation/MMZ2012R102AT000/445-1555-1-ND/571885|
|LED|1|Chip LED 0805|RED|$0.29|$0.87|$2.15|$10.16|https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KRKT/160-1427-1-ND/386800|
|CHG|1|Chip LED 0805|AMBER|$0.31|$0.93|$2.28|$10.82|https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KSKT/160-1428-1-ND/386802|
|STATUS|1|3535 Neopixel|WS2812B3535|$4.95|$4.95|$4.95|$44.60|https://www.adafruit.com/product/2686|
|U1|1|AP2112 3.3v voltage regulator (SOT23-5)|AP2112-3.3|$0.47|$1.41|$3.55|$22.11|https://www.digikey.com/product-detail/en/diodes-incorporated/AP2112K-3.3TRG1/AP2112K-3.3TRG1DICT-ND/4505257|
|U2|1|MCP73831/2 LIPO Charger (SOT23-5)|MCP73831T-2ACI/OT|$0.56|$1.68|$5.60|$42.00|https://www.digikey.com/product-detail/en/microchip-technology/MCP73831T-2ACI-OT/MCP73831T-2ACI-OTCT-ND/1979802|
|U3|1|ATSAMD51J64-TQFP(10X10)|-|$4.18|$12.54|$41.80|$347.00|https://www.digikey.com/product-detail/en/ATSAMD51J19A-AUT/ATSAMD51J19A-AUTCT-ND/7390180/|
|U4, U6|2|SPI Flash (SOIC8_150MIL)|GD25Q16C (or GD25Q32C)|$1.12|$3.36|$10.92|$89.52|https://www.digikey.com/product-detail/en/gigadevice-semiconductor-hk-limited/GD25Q16CTIGR/1970-1010-1-ND/9484760|
|U5|1|8-bit parallel load shift register (SO16)|74AC165D|$0.37|$1.11|$3.02|$20.53|https://www.digikey.com/product-detail/en/nexperia-usa-inc/74HC165D653/1727-2788-1-ND/763378|
|U7|1|32KB SRAM (SOIC8_150MIL)|23K256-I/SN|$1.11|$3.33|$11.10|$111.00|https://www.digikey.com/product-detail/en/microchip-technology/23K256T-I-SN/23K256T-I-SNCT-ND/5818673|
|U8|1|MAX4466 Audio amp|MAX4466EXK+T|$0.89|$2.67|$7.82|$55.67|https://www.digikey.com/product-detail/en/MAX4466EXK%2bT/MAX4466EXK%2bCT-ND/5405773/|
|J1|1|4 conductor 3.5 mm Surface Mount Audio Jack|SJ-43514-SMT|$1.16|$3.48|$10.21|$88.09|https://www.digikey.com/product-detail/en/cui-inc/SJ-43514-SMT-TR/CP-43514SJCT-ND/669734?utm_source=snapeda&utm_medium=aggregator&utm_campaign=symbol|
|J2|1|10118193-0001LF|Micro USB B Type Receptacle|$0.43|$1.29|$4.05|$31.04|https://www.digikey.com/product-detail/en/amphenol-icc-fci/10118193-0001LF/609-4616-1-ND/2785380?utm_source=snapeda&utm_medium=aggregator&utm_campaign=symbol|
|X1|1|JST-PH 2 Pin connector|JST PH 2|$0.58|$1.74|$5.47|$41.94|https://www.digikey.com/product-detail/en/S2B-PH-SM4-TB(LF)(SN)/455-1749-1-ND/926846/|
|PORT_A8, PORT_A9|2|JST-PH 3 Pin connector|JST PH 3|$1.38|$4.14|$12.86|$98.58|https://www.digikey.com/product-detail/en/S3B-PH-SM4-TB(LF)(SN)/455-1750-1-ND/926847/|
|PORT_I2C|1|JST-PH 4 Pin connector|JST PH 4|$0.79|$2.37|$7.42|$56.90|https://www.digikey.com/product-detail/en/S4B-PH-SM4-TB(LF)(SN)/455-1751-1-ND/926848/|
|MICROSD|1|MicroSD socket|MICROSD|$1.95|$5.85|$17.60|$156.00|https://www.adafruit.com/product/1660|
|MS1|1|Feather Connector 24pos|-|$2.87|$8.61|$27.53|$229.42|https://www.digikey.com/product-detail/en/amphenol-icc-fci/89898-312LF/609-3046-ND/1536315|
|MS1|1|Feather Connector 32pos|-|$4.77|$14.31|$45.83|$381.92|https://www.digikey.com/product-detail/en/amphenol-icc-fci/89898-316LF/609-3050-ND/1534980|
|X2|1|Serial Wire (SW-DP) Connector|2x5 0.05 SWD"|$0.74|$2.22|$6.90|$52.90|https://www.digikey.com/product-detail/en/cnc-tech/3220-10-0300-00/1175-1629-ND/3883266|
|BTN_PREV, BTN_NEXT, BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_SEL|7|Thru-hole SPST Switch|MJTP1250|$3.56|$7.48|$23.11|$177.80|https://www.digikey.com/product-detail/en/TL1107AF130WQ/EG2510-ND/378976/|
|BTN_LOCK, BTN_NEXT_ALT, BTN_PREV_ALT, BTN_RESET|4|Surface-mount right-angle SPST switch|TL3330AF130QG|$1.08|$6.20|$20.29|$153.12|https://www.digikey.com/product-detail/en/TL3330AF130QG/EG4388CT-ND/1426858/|
|SW1|1|A10767 SPDT Switch|-|$0.31|$0.93|$3.00|$24.99|https://www.digikey.com/product-detail/en/te-connectivity-alcoswitch-switches/1825232-1/A107673-ND/4021554|
|U$2|1|eInk Display with 24-pin connector|-|$18.26|$54.78|$182.60|$1373.00|https://zh-tw.buyepaper.com/42inch-e-ink-panel-spi-interface-buy-e-paper-display-p0085.html|
||||**Total cost**|**$62.12**|**$173.94**|**$530.16**|**$4001.13**||
||||**Total per assembly**|**$62.12**|**$57.98**|**$53.02**|**$40.01**||

### E-Book FeatherWing

|Parts|Qty|Device|Value|Cost @ 1 assembly|Cost @ 3 assemblies|Cost @ 10 assemblies|Cost @ 100 assemblies|URL|
|-----|---|------|-----|-----|-----|-----|------|-----|
|C1, C2|2|0805 Capacitor|10uF|$0.24|$0.72|$1.70|$7.62|https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL21A106KQCLRNC/1276-2405-1-ND/3890491|
|C3, C4, C5, C7, C8, C10, C11, C12, C13, C14, C15, C16|12|0805 Capacitor|1uF/25V|$0.94|$2.84|$4.22|$23.72|https://www.digikey.com/product-detail/en/CL21B105KAFNNNE/1276-1066-1-ND/3889152/|
|C6|1|1210 Capacitor|100uF|$0.70|$1.40|$5.18|$35.53|https://www.digikey.com/product-detail/en/CL32A107MQVNNNE/1276-1092-1-ND/3889178/|
|C9|1|0805 Capacitor|4.7uF/25V|$0.30|$0.90|$2.05|$11.61|https://www.digikey.com/product-detail/en/TMK212AB7475KG-T/587-2990-1-ND/2714183/|
|R1, R2|2|0805 Resistor|1K|$0.20|$0.32|$0.64|$2.64|https://www.digikey.com/product-detail/en/yageo/RC0805JR-071KL/311-1.0KARCT-ND/731165|
|R3, R4, R6, R7|4|0805 Resistor|10K|$0.36|$0.43|$1.44|$5.88|https://www.digikey.com/product-detail/en/yageo/RC0805FR-0710KL/311-10.0KCRCT-ND/730482|
|R5|1|0805 Resistor|0.47ohm|$0.17|$0.51|$1.49|$5.74|https://www.digikey.com/product-detail/en/cts-resistor-products/73L3R47J/73L3R47JCT-ND/1543531|
|R8, R9|2|0805 Resistor|100|$0.20|$0.35|$0.70|$2.80|https://www.digikey.com/product-detail/en/yageo/RC0805JR-07100RL/311-100ARCT-ND/731185|
|D1, D2|2|Zener Diode SOD123|3.6V|$0.40|$1.20|$3.32|$17.62|https://www.digikey.com/product-detail/en/MMSZ4685-TP/MMSZ4685-TPMSCT-ND/2041552/|
|D3, D4, D5|3|Schottky diode SOD-123|MBR0530|$0.93|$2.79|$7.62|$40.35|https://www.digikey.com/product-detail/en/MBR0530T1G/MBR0530T1GOSCT-ND/917966/|
|L1|1|Inductor NR401|10uH|$0.64|$1.92|$5.59|$39.96|https://www.digikey.com/product-detail/en/vishay-dale/IFSC1515AHER100M01/541-1406-1-ND/2744259|
|Q1|1|MOSFET-N (SOT23-3)|IRLML0100|$0.48|$1.44|$4.11|$30.66|https://www.digikey.com/product-detail/en/infineon-technologies/IRLML0100TRPBF/IRLML0100TRPBFCT-ND/2202230|
|FB1, FB2|2|0805 Ferrite Bead|Ferrite|$0.22|$0.66|$1.76|$10.04|https://www.digikey.com/product-detail/en/tdk-corporation/MMZ2012R102AT000/445-1555-1-ND/571885|
|IC1|1|MCP23008/SO18W|MCP23008/SO|$1.02|$3.06|$10.20|$78.00|https://www.digikey.com/product-detail/en/MCP23008T-E%2fSO/MCP23008T-E%2fSOCT-ND/5358243|
|IC2|1|32KB SRAM (SOIC8_150MIL)|23K256-I/SN|$1.11|$3.33|$11.10|$111.00|https://www.digikey.com/product-detail/en/microchip-technology/23K256T-I-SN/23K256T-I-SNCT-ND/5818673|
|IC3|1|SPI Flash (SOIC8_150MIL)|GD25Q16C (or GD25Q32C)|$0.56|$1.68|$5.46|$44.76|https://www.digikey.com/product-detail/en/gigadevice-semiconductor-hk-limited/GD25Q16CTIGR/1970-1010-1-ND/9484760|
|J1|1|4 conductor 3.5 mm Surface Mount Audio Jack|SJ-43514-SMT|$1.16|$3.48|$10.21|$88.09|https://www.digikey.com/product-detail/en/cui-inc/SJ-43514-SMT-TR/CP-43514SJCT-ND/669734?utm_source=snapeda&utm_medium=aggregator&utm_campaign=symbol|
|I2C_PORT|1|JST-PH 4 Pin connector|JST PH 4|$0.79|$2.37|$7.42|$56.90|https://www.digikey.com/product-detail/en/S4B-PH-SM4-TB(LF)(SN)/455-1751-1-ND/926848/|
|A1_PORT, A2_PORT|2|JST-PH 3 Pin connector|JST PH 3|$1.38|$4.14|$12.86|$98.58|https://www.digikey.com/product-detail/en/S3B-PH-SM4-TB(LF)(SN)/455-1750-1-ND/926847/|
|MICROSD|1|MicroSD socket|MICROSD|$1.95|$5.85|$17.60|$156.00|https://www.adafruit.com/product/1660|
|MS1|1|Feather Connector 24pos|-|$2.87|$8.61|$27.53|$229.42|https://www.digikey.com/product-detail/en/amphenol-icc-fci/89898-312LF/609-3046-ND/1536315|
|MS1|1|Feather Connector 32pos|-|$4.77|$14.31|$45.83|$381.92|https://www.digikey.com/product-detail/en/amphenol-icc-fci/89898-316LF/609-3050-ND/1534980|
|BTN_LOCK, BTN_NEXT_ALT, BTN_PREV_ALT, BTN_RESET|4|Surface-mount right-angle SPST switch|TL3330AF130QG|$1.08|$6.20|$20.29|$153.12|https://www.digikey.com/product-detail/en/TL3330AF130QG/EG4388CT-ND/1426858/|
|BTN_PREV, BTN_NEXT, BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_SEL|7|Thru-hole SPST Switch|MJTP1250|$3.56|$7.48|$23.11|$177.80|https://www.digikey.com/product-detail/en/TL1107AF130WQ/EG2510-ND/378976/|
|SW1|1|A10767 SPDT Switch|-|$0.31|$0.93|$3.00|$24.99|https://www.digikey.com/product-detail/en/te-connectivity-alcoswitch-switches/1825232-1/A107673-ND/4021554|
|U$2|1|eInk Display with 24-pin connector|-|$18.26|$54.78|$182.60|$1373.00|https://zh-tw.buyepaper.com/42inch-e-ink-panel-spi-interface-buy-e-paper-display-p0085.html|
||||**Total cost**|**$44.60**|**$131.70**|**$417.03**|**$3207.75**||
||||**Total per assembly**|**$44.60**|**$43.90**|**$41.70**|**$32.08**||

## Next Steps

I've designed the Open Book A1 Rev 05 to address some small issues:

- There was a slight hum in the headphones when plugged in to USB. Addressed this by adding a resistor and a capacitor between the USB cable shield and the ground plane.
- The thru-hole button footprints weren't quite perfect, and prevented the buttons from mounting consistently flush with the board. A new button footprint should address that issue.
- I moved to new gull-wing buttons for reset and lock, which should improve their durability. Left the footprints for the old side-mount previous and next buttons as-is, but renamed them from BTN to DNP since their functions are covered by the thru-hole previous and next buttons. You can populate them if you want, but I'm planning to design enclosures as if they're not there.
- Moved the status Neopixel to the bottom, by the reset button, since it's especially useful for communicating bootloader status which often involves the reset function. It's also a right-angle LED now.
- Extended the inductor's solder pad out on either side, in the hopes of making hand-soldering easier (before, you could only solder that part with an oven or hot air).
- Finally, I fixed a minor typo in the silkscreen; UART is SERCOM5, not SERCOM0.

It's unclear whether I'll test this version of the board; the changes are so minor that I think it should "just work", and for the Take Flight with Feather run, there'll probably be a comopletely different revision that takes DFM concerns into account. I just wanted to make sure I updated the schematics in case folks wanted to build along at home.

The differences from the BOM above:

- The Neopixel is [this part](https://www.digikey.com/product-detail/en/inolux/IN-PI42TASPRPGPB/1830-1206-1-ND/9681286) instead of the one listed.
- You only need two instead of four right angle buttons, and it's [this part](https://www.digikey.com/product-detail/en/KSS321GLFS/401-1100-1-ND/417671/?itemSeq=315746150).
- You'll need one extra 1MΩ resistor (R27) and a 4.5nF capacitor (C36), both 0805-sized, for the shield connection.

## Schematics

![image](/images/oso-book-a1-05-schematic-page1.png)

![image](/images/oso-book-a1-05-schematic-page2.png)

![image](/images/oso-book-a1-05-schematic-page3.png)

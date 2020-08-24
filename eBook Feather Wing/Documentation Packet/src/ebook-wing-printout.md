<!--
This is the source for the booklet that ships with the E-Book Wing PCB on Tindie. It's mostly Markdown with a sprinkling of LaTeX.

To generate the PDF:

    pandoc ebook-wing-printout.md -N -V links-as-notes -V papersize:a5 -V geometry:margin=1in -V title="E-Book FeatherWing Documentation" -V toc-depth=1 --toc --template=template.tex --variable mainfont="Palatino" --variable sansfont="Helvetica" --variable monofont="Menlo" --variable fontsize=12pt --pdf-engine=xelatex -o ebook-wing-printout.pdf

I turn it into a booklet with the excellent (though not free) BookletCreator app for Mac: https://www.bookletcreator.com/

-->

\newpage\thispagestyle{empty}

&nbsp; <!-- this page intentionally left blank -->

\newpage

# Introduction

Hi there, and congrats on taking the plunge and deciding to build your own E-Book FeatherWing! The fact that you've sought out this kit and brought it into your life probably signals that you understand the gist of its purpose, but to recap: this is an open hardware device for reading that you are going to build yourself and understand from bottom to top.

This document is also available on the [Open Book wiki](https://github.com/joeycastillo/The-Open-Book/wiki/E-Book-Wing-Setup-Guide).

## What You Will Need

It's tough to know exactly what to put in this list, because there are many different ways to put this thing together. Having said that, there are some broad requirements, and some specific items that can fulfill those requirements, so I'll outline them here and then go into more specifics in the sections where we need those things.

* All of the parts on the E-Book Wing Bill of Materials (BOM), as well as the E-Book FeatherWing printed circuit board (PCB).
* Tools necessary for soldering the parts on to the board. The E-Book Wing is designed to be hand-solderable; you should just need a [soldering iron](https://adafru.it/180) with a [fine tip](https://adafru.it/1249), [fine-tipped tweezers](https://adafru.it/422) for placing components, [flux](https://adafru.it/2667) or a [flux pen](https://adafru.it/3468), [solder wick](https://adafru.it/4456) and a [solder sucker](https://adafru.it/148) (plus of course some [thin solder](https://adafru.it/1930)).
* A [magnifier loupe](https://www.digikey.com/short/zhvph4) will help with the one fine-pitched part (the 24-pin connector), but a magnifying glass or high magnification reading glasses would also suffice.
* A [multimeter](https://adafru.it/2034). The linked one on Adafruit's site is is quite nice, but you can also [pick one up for cheap at your local hardware store](https://www.harborfreight.com/7-function-digital-multimeter-63759.html).
* Some optional items that enable additional functionality: a [MicroSD card](https://adafru.it/1294) for flashing the global font data, a [lithium-Polymer battery](https://adafru.it/3898) for making the device portable, and an [external 1/8" headset](https://adafru.it/1966) if you want to use the E-Book Wing's audio features.

## BOM Notes

With the exception of the e-paper display, you can for the most part just go to [the E-Book Wing's Kitspace page](https://kitspace.org/boards/github.com/joeycastillo/the-open-book/ebook-wing/) and 1-click order the parts you need from Digikey. But there are a couple of parts that you can swap out to save a few bucks:

* The Feather connectors on the BOM are rather ludicrously expensive, so I've taken to ordering [two of these 2x20 surface mount headers](https://adafru.it/2187) and cutting them down to size, which saves $4 per board.
* If the listed part numbers for some passive components are out of stock, or you find alternatives that are cheaper, you can swap in parts with the same values. Just be extra careful with the 4.7µF capacitor and the 1µF capacitors in the e-paper display block; for the most part, they need to be rated for 25 volts.
* When shopping for the e-paper display, you may be tempted by Good Display's 4.2" tri-color EPD screens, which add either red or yellow to the standard black and white. **I suggest you don't do it.** The long refresh time (10-15 seconds) required for tri-color display modes makes this sort of display poorly suited for book reading.
* Finally, the 24-pin flex connector. The part on the BOM does work, but in my experience, Good Display tends to include a very high quality 24-pin connector for each display you order from them. I would recommend using that connector over the one in the BOM, but it's there just in case.

The E-Book Wing comes to you as a bare PCB. Coaxing life into this inanimate sheet of copper and fiberglass will require that you solder dozens of small parts onto the board. The vast majority of those parts are surface mount technology, or SMT. If you have assembled surface mount boards before, no problem! Use whatever method makes the most sense for you. If you have not, there are several strategies that I have used to assemble the E-Book Wing; I'll outline them here, so you can choose one that you're most comfortable with.

## Soldering Strategies

First off, you can solder this board together using a fine-tipped soldering iron and either solder paste or thin solder. [Collin Cunningham of Adafruit has an excellent video on the topic](https://youtu.be/QzoPxvIM2qE); that video covers soldering all of the kinds of devices you'll find on the Open Book board. 

You can also use hot air reflow to assemble the board. This involves placing solder paste on the board, and blowing hot air at the part until it reflows into place. [This video by Antti Kupila](https://youtu.be/vzoMEBmCNQQ) shows an example of this process. To do hot air reflow, you would also need:

* A [hot air soldering station](https://adafru.it/1869), and
* [Solder paste](https://adafru.it/3217). Personally, I use [this solder paste](https://www.digikey.com/short/zh1qmq), but that's a big syringe; the small tube of Maker Paste should be plenty.

The hot air soldering station is also very useful if you need to rework, i.e. removing a chip that was placed in the wrong orientation: you can blow hot air at the component and remove it with tweezers.

Speaking of hot air, the final method for soldering a board like the E-Book Wing is using a reflow oven. This method involves placing all the solder paste and all the components all at once, and then baking the board at high temperature to set everything in its place. I inherited [one of these](https://www.ebay.com/sch/?_nkw=T-962+reflow+oven) from a colleague, but you can also DIY one, [as shown in this tutorial from Dan Cogliano](https://learn.adafruit.com/ez-make-oven/). Truth be told though, unless you already have a reflow oven, this method is overkill for the E-Book Wing. Hand soldering, in my experience, is the way to go.

Whichever method you choose, there are a few things to be aware of:

* Be aware of the polarity of the diodes. Diodes have a gray line on their plastic body that should face the same direction as the line in the diode symbol on the silkscreen.
* Be aware of the different types of diodes used in this design. The Zener diodes in the 'Extra Ports' block are easy to mix up with the Schottky diodes in the 'E-Paper Display' block. Make sure to put the correct diodes in the correct places.
* When placing IC's, be conscious of the pin one alignment: there is a dot on each chip that should match with the dot on the board's silkscreen.

# Board Setup

Once you have populated the E-Book Wing board and soldered all the components into place, it's time to set up the software support for the board. You should have the following items on hand: 

* E-Book Feather Wing
* A Feather M4 Express
* A MicroSD Card
* A MicroSD Card Reader

## Development Environment Setup

We will be working with the [Arduino IDE](https://www.arduino.cc) for this project; if you do not already have Arduino on your machine, download and install it now.

You will also need to have Adafruit SAM board support installed. If you do not already have this board support package installed, you can [follow the instructions here](https://learn.adafruit.com/add-boards-arduino-v164/setup) to get this set up.

You will need several libraries installed. Most of them, we will install from the Arduino Library Manager. 

* **Adafruit GFX Library**
* **Adafruit BusIO**
* **Adafruit MCP23008 Library**
* **Adafruit EPD**
* **Adafruit SPIFlash**
* **SDFat - Adafruit Fork**
* **arduino-menusystem** (optional, needed for MVBook at the end)

Finally, you will need to install two libraries by copying them to your Arduino libraries folder: 

* **[The Open Book](https://github.com/joeycastillo/The-Open-Book)**
* **[Babel](https://github.com/joeycastillo/babel)**

You can either use git to clone these repositories in your Arduino libraries folder, or download the zip files and move the unzipped folders to the libraries folder. Note that the zip files may unzip as folders named "The-Open-Book-master" or "babel-master"; rename them to remove the "-master", so they are named "The-Open-Book" and "babel", and copy them to the libraries folder.

When you have all of these libraries installed, restart the Arduino IDE.

# Testing the Screen

The first sketch will validate that the screen is working. Before beginning this test, double check the fine-pitched flex connector at the top right of the board. Inspect it for any bridged pins. If you see any bridged pins, separate them by applying flux and dragging your (clean) soldering iron away from the bridged spot.

When you are ready to test the screen:

1. Go to File -> Examples -> Open Book and load the `Open_Book_Screen_Test` sketch.
1. Plug the Feather M4 in to the back of the E-Book Wing board using the headers. Make sure that the Feather is oriented so that long and the short row of headers match up with the long and short socket header on the board. 
1. Plug the assembled device in to your computer.
1. Select the Feather M4 Express from the list of boards in Tools -> Board. In Tools -> Ports, ensure that your Feather M4 is selected. If you do not see it in the list of ports, double tap the Reset button; this tends to fix a lot of issues with the board not appearing or responding.
1. Once you have selected the Feather M4, run the sketch! 

You should see the screen flash to life, and display the Open Book Project logo. 

## Troubleshooting

* If you do not see the Feather show up in the Arduino IDE, check the Enable switch. If it is switched toward the center of the board, it is in the OFF position, and the Feather is not receiving power. Switch it toward the outside of the board to turn it on. 
* If you do not see the screen refresh, double check that the Feather is mounted correctly, and that the pins are correctly aligned. You should also double check that the screen's flex cable is seated correctly; re-seat the cable if necessary.
* If the screen image looks weak, gray or shows lots of noise or static, double check the orientation and type of the diodes in the E-Paper Display block. All three diodes should bear a tiny marking "B3" (not "CM") on the top of their cases, and the gray line which indicates polarity should be facing upward, toward the top of the board. 
    * If one or more of the diodes is facing the wrong direction, desolder it and flip it around. 
    * If one or more of the diodes bears the marking "CM", you've placed one of the Zener diodes here instead of the Schottky diodes that belong in this spot. Find the Schottky diode (again, look for the B3 marking). You may have put it in the "Extra Ports" block. Desolder and swap them out. 

# Burning the Babel Flash chip

The secondary Flash chip on the E-Book Wing board is dedicated to language support, but it currently does not have any data on it. To use it for languages, you will need to burn the glyph and font data on to that chip. 

By default, the Babel Flash chip is not connected to any pin on the Feather, to make it easy for people to choose a pin that they want to use for it. In the case of the Feather M4 though, we have a spare pin on D4, so we'll use that one. 

1. Unplug the device from your computer and remove the Feather from the Wing. 
1. Locate the BCS solder jumper (the solder mask might be hard to read; consult the board diagram included with your board, or look for it right next to the "ER" in "FEATHER", near the socket header). 
1. Using your soldering iron, bridge the two pads of the BCS solder jumper. This connects BCS to pin D4, which is what the Babel libray will expect for a Feather M4. 
1. Re-seat the Feather in the socket header, and plug the device back in. 

That's the hardware setup. Next, you will need to get your card reader. The Babel library reads data from a 2 megabyte blob containing Unicode character data, Unifont glyphs and tables of localized case mapping and form shaping data. We are going to copy that blob to the MicroSD, and then run a sketch to burn that image to the Flash chip. 

The file you will burn is called `babel.bin`; it's located in the `babelconvert` folder inside the babel library you downloaded, or you can [download it directly here](https://github.com/joeycastillo/babel/raw/master/babelconvert/babel.bin).

1. Copy the `babel.bin` file to your MicroSD card, and eject the drive
1. Plug the MicroSD card in to the socket on the e-book wing. 
1. Go to File -> Examples -> Babel Universal Character Library and load the `BurnBabelBurn` sketch.
1. Run the sketch, and open the Serial Monitor (Tools -> Serial Monitor)

At this point you will see a menu of options for actions you can take. 

1. First, enter `E` and press 'Enter'. This will erase the Flash chip.
1. Next, enter `W` and press 'Enter'. You will see a series of messages indicating the writing of a page to Flash. Wait until they finish scrolling; you will see "Done!" appear when the process is complete.
1. Finally, enter `V` and press 'Enter'. This will verify that the burned data matches the blob on the SD card.

If the image verifies, you're done! You can delete the babel.bin file from the SD card; the Babel chip will retain this data indefinitely.

## Troubleshooting

* If the verification fails a few pages in, return to step 1 and try the whole process again. Also, if you have just gotten done soldering the chip, wait until it cools down completely; I've experienced errors when I tried to flash the Babel image right after pulling the board from the oven.
* If the process fails more than three times, try using a different brand of MicroSD card; I have had issues with off-brand cards, but no problems at all with Sandisk cards.
* If the Flash size appears as 0, and writing fails, double check the placement of the Flash chip. The dot indicating pin 1 position should be at the bottom left. If it is not, you can remove this chip this using hot air and tweezers, and re-place it correctly.
* If you get the message "Card failed, or not present", double check the connections of the MicroSD socket.

# Running the MVBook Software

The MVBook software is a simple menu system for displaying a few works, stored in a specific format, and allowing the user to select and page through them. It's not as full-featured as I hope it someday will be, but it is my hope that as we build more of a community around this hardware, we can advance better software support together.

The MVBook example folder includes a zip file called [books.zip](https://github.com/joeycastillo/The-Open-Book/raw/master/Examples/Open_Book_MVBook/books.zip) with a handful of public domain works you can copy to an SD card. The MVBook example folder also includes a Python script called [bookbinder.py](https://github.com/joeycastillo/babel/raw/master/babelconvert/babel.bin) that formats plain text files to function with MVBook. Ideally, the canonical format for reading will be plain text files, but for the moment, some of the work of pagination and embedding metadata needs to happen on a computer. 

1. Copy the works you want to read with MVBook to an SD card and insert it into the E-Book Wing. 
1. Go to File -> Examples -> Open Book and load the `Open_Book_MVBook` sketch.
1. Run the sketch!

You should see the screen refresh and display a listing of the books on the MicroSD. 

## Controls

* Use the direction pad's "Up" and "Down" arrow keys to navigate the list of works. 
* Use the direction pad's "Select" button in the middle to select a work in the list and enter reading mode.
* When reading a work, use the "Previous Page" and "Next page" buttons on the far left and right to navigate the work.
* To return to the main menu while reading a work, press the center "Select" button.

## Troubleshooting

* If the MVBook sketch does not run, ensure that you completed the steps in "Burning the Babel Flash chip" above, as MVBook depends on Babel for text rendering.
* If the interface is not responding, especially after you have left the device alone for for a while, ensure that the board has power by plugging it in or pressing the reset button. The e-paper display will retain whatever text was on the screen last even after the battery dies, which can be confusing.

# Ideas for Future Work

The Feather M4 has a DAC on A0 that the E-Book Wing uses to drive a headphone jack; you can use the [Adafruit Talkie library](https://github.com/adafruit/Talkie) for synthesized speech, or the [Adafruit MP3 library](https://github.com/adafruit/Adafruit_MP3) to play back audiobooks or podcasts.

MVBook only makes use of a handful of the available buttons; you could put together a much more interesting user interface by making use of the full D-Pad, or configuring the Lock button to emit an interrupt for low power sleep/wake operation.

The STEMMA ports along the left side offer a lot of flexibility! The A1 port (on the bottom) also has a DAC; you could add [a STEMMA speaker](https://www.adafruit.com/product/3885) if you don't want to use headphones, or add [a Neopixel strip](https://www.adafruit.com/product/2869) to either port to make a reading light! These ports also work to add a [microphone](https://www.adafruit.com/product/1063) for voice control (there's a demo of this, but may need some tweaking; look for `Open_Book_Heart_Of_Darkness_TensorFlow`). The STEMMA I2C port (left side center) lets you plug in all manner of sensors; for accessibility, you could add a [gesture sensor](https://www.adafruit.com/product/3595) to turn pages with a flick of the hand.

If you have a Feather with stacking headers, you can also add [an AirLift Feather Wing](https://www.adafruit.com/product/4264) to add WiFi to your book! The wing leaves pins 11, 12 and 13 free as well as the UART (pins 0 and 1), so it stacks with any wings that can use those pins or the I2C pins, such as (among others): the [DS3231 Precision RTC FeatherWing](https://www.adafruit.com/product/3028) (for telling time), [Ultimate GPS FeatherWing](https://www.adafruit.com/product/3133) (maybe a navigation device?), [ADXL343 + ADT7410 Sensor FeatherWing](https://www.adafruit.com/product/4147) (weather station?), or [INA219 FeatherWing](https://www.adafruit.com/product/3650) (workbench friend).

Congratulations on building your E-Book FeatherWing!

\newpage\thispagestyle{empty}\vspace*{\fill}\small

**E-Book FeatherWing Documentation**  
*CC-BY-SA 4.0, Joey Castillo*

\tiny

This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.  
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
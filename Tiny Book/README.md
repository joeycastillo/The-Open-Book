# The Tiny Book

Okay I'm going to be completely honest: I have wandered pretty far from the original prompt with this one. The theory was let's make a less expensive, smaller version of the Open Book with as much of its functionality as possible. With variant B, tiny book has instead become a this very quixotic experiment in form and intent, as well as a playground for new tech like [Good Display's touch screen 2.7" EPD](https://zh-tw.buyepaper.com/products/e-ink-touchscreen-27inch-spi-interface-china-e-ink-manufacturer). 

Variant A (with the cap touch slider) is still here but the touch screen kind of renders it moot. Variant B might be a dead end too, but I think it'll have been worth building for the lessons learned along the way. You could also probably redesign it around an ESP32 or NRF52840 and wind up with something more useful, but there are enough new things in this that I figured I'd stick with the SAMD51 for now since I know the chip so well.

![image](/images/tibo-variant-b.png)

- Features that are basically the same as the Open Book:
    - Battery charging
    - Indicator LEDs
    - 4MB QSPI Flash (up from 2MB)
    - MicroSD slot
    - Enable switch
    - Lock button
    - Reset button
    - Serial port
    - VBAT and VBUS monitors
    - Neopixel

- Features that are similar, and the differences:
    - E-Paper screen. Still has partial refresh, but it's smaller (2.7" instead of 4.2").
    - Instead of seven buttons on the front, the E-Paper display has a capacitive touch matrix that detects touches and gestures (and has an interrupt pin).
    - Microcontroller: it's still a SAMD51 with native USB and the same amount of Flash and RAM, but in a QFN48 package, so fewer pins (and sadly less hand-solderability).
    - The I2C port is now JST-SH (STEMMA-QT) due to size constraints.

- Things that are gone, and strategies for doing without:
    - One STEMMA port instead of two, but it's a really versatile pin. It's a DAC pin so you can speaker with it, or configure it as analog input for a mic or potentiometer, PWM it for an LED, run a Neopixel strip or use it as GPIO for a button.
    - Babel chip. The CircuitPython Babel library supports just putting babel.bin on the QSPI flash chip (and since it's twice the size, you'll still have the same amount of space left over for code).
    - Feather compatibility. Sorry. It would be rad to be able to support FeatherWings but there's just not enough room (or pins).

In the course of this experiment, I ran across a very old design concept that imagined a hinged device with a second e-paper screen on a separate board. So I threw one together, using a [9-pin connector](https://www.adafruit.com/product/4350) (like Adafruit uses on the [MONSTER M4SK](https://www.adafruit.com/product/4343)) to bring over SPI, I2C and one extra signal. A SAMD09 on the secondary board (running Adafruit's Seesaw firmware) gives us more pins for more toys.

- Features that got moved to the secondary board or nerfed.
    - Headphone jack (mono instead of stereo) now lives on the secondary board. I sense that the audio quality may not be as good, since the signal has to travel over a 9-pin connector right next to the I2C signals, but we'll find out.
    - Inline mic input moves over with the headphone jack, but alas there is no longer any provision for recording sound (I removed the mic preamp since there's no spare wire to send a mic signal back to the main board). You can still bias the mic input pin and detect up to three types of button presses from the inline button.

- New features on the secondary board
    - A second 2.7" e-paper display. Pads are there for a touch screen and it's in theory connected to pins on the SAMD09 that *could* support I2C. But this would require some kind of passthrough of I2C messages that I'd have to implement in the SAMD09 firmware. My plan is to use a cheaper non-touchscreen display and only have the main screen be a touch screen.
    - An extra status LED. I'm thinking blue?
    - Speaking of LED's: a PWM-enabled bank of five white LED's along the right (left?) side of the board. I call it a reading light, but they'll shine right at your face if you turn them on. There might be some way to fold the device that coaxes them into illuminating the main screen.
    - Speaking of folding the device: a voltage divider labeled "Hinge?" that I might be able to turn into some kind of hinge position detector using a flex PCB. I honestly have no idea.
    - A real-time clock, because I had room (and because I think this could actually make for a sweet clock).
    - A buzzer for making noises (because it might as well be a sweet alarm clock).
    - A light sensor (because I had a pin leftover).

oh also because i haven't written down the board nomenclature anywhere, this is what i mean by OSO-TIBO-B1-00:

- OSO = Oddly Specific Objects
- TIBO = TIny BOok, the project name
- B = Variant, A-Z, some major version of the board with a specific piece of tech or form factor that makes it unique.
- 1 = Subassembly, 1-9. I doubt I'll ever come up with a project that involves more than 9 PCBs.
- 00 = revision, 00-99. Usually starts at 01, but if I'm really unsure of something I'll start at 00.

I'll update this once the boards come in and I actually assemble one of these.
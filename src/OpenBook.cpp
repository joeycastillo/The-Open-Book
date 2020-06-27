#include "OpenBook.h"
#include "OpenBook_IL0398.h"

OpenBook::OpenBook() {
}

/**
 @brief Configures the e-ink screen.
 @param srcs Chip select pin for the SRAM. Pass in OPENBOOK_NOT_PRESENT if you
             have omitted this chip.
 @param ecs Chip select pin for the e-ink screen.
 @param edc Data/command pin for the e-ink screen.
 @param erst Reset pin for the e-ink screen.
 @param ebsy Busy pin for the e-ink screen.
 @param spi Address of the SPI bus for the screen and SRAM chip.
 @param width Native width of the display
 @param height Native height of the display
 @returns always returns true for now
 @note A medium-term term goal is to write our own driver for the screen and
       have it live in this repository. For now, we're using the Adafruit_IL0398
       driver. Refresh rate is honestly a bit slow, and I know this screen can
       refresh faster because the GxEPD2 library is downright snappy. GxEPD2
       also has LUTs for partial refresh, which I think we want to port over.
       Inspo:
        * Adafruit_EPD: https://github.com/adafruit/Adafruit_EPD
        * GxEPD2: https://github.com/ZinggJM/GxEPD2
        * IL0398 Datasheet: https://cdn.sparkfun.com/assets/f/a/9/3/7/4.2in_ePaper_Driver.pdf
*/
bool OpenBook::configureScreen(int8_t srcs, int8_t ecs, int8_t edc, int8_t erst, int8_t ebsy, SPIClass *spi, int width, int height) {
    OpenBook_IL0398 *display = new OpenBook_IL0398(width, height, edc, erst, ecs, srcs, ebsy, spi);
    display->begin();
    display->clearBuffer();
    display->setRotation(1);
    display->setColorBuffer(1, false);
    this->display = display;

    return true;
}

/**
 @brief Configures the buttons for devices with a shift register.
 @param latch Latch pin for the shift register
 @param data Data out pin for the shift register
 @param clock Clock pin for the shift register
 @returns true as long as you passed in valid pins.
 @note This is currently the button solution for the Open Book. The Lock button
       is NOT on the shift register, though; it's on a SAMD51 interrupt pin, and
       SD Card Detect is on the shift register. This feels very inelegant and I
       may rework this in the final revision.
*/
bool OpenBook::configureShiftButtons(int8_t active, int8_t latch, int8_t data, int8_t clock, int8_t lockButton) {
    if (latch < 0 || data < 0 || clock < 0) return false;

    pinMode(latch, OUTPUT);
    digitalWrite(latch, HIGH);
    pinMode(data, INPUT);
    pinMode(clock, OUTPUT);
    digitalWrite(clock, HIGH);
    pinMode(lockButton, INPUT_PULLUP);

    this->activeState = active;
    this->buttonLatch = latch;
    this->buttonData = data;
    this->buttonClock = clock;
    this->buttonInterrupt = lockButton;
    // todo: implement an interrupt handler when lock button is pressed

    // sometimes the first read returns a garbage value, so get that out of the way now.
    this->readButtonRegister();

    return true;
}

/**
 @brief Configures the buttons for devices with a MCP23008 I2C port expander.
 @param interrupt The port expander's interrupt pin.
 @returns always returns true for now
 @note This is currently the button solution for the wing. All buttons are on
       the port expander, including the Lock button.
*/
bool OpenBook::configureI2CButtons(int8_t active, int8_t interrupt) {
#if OPENBOOK_USES_IO_EXPANDER
    Adafruit_MCP23008 *ioExpander = new Adafruit_MCP23008();
    ioExpander->begin();
    for (int i = 0; i <= 7; i++) {
        ioExpander->pinMode(i, INPUT);
        if (active == LOW) ioExpander->pullUp(i, HIGH);
    }
    this->ioExpander = ioExpander;

    if (interrupt >= 0) {
        pinMode(interrupt, INPUT);
    }
    this->activeState = active;
    this->buttonInterrupt = interrupt;
    // todo: implement an interrupt handler for when buttons are pressed.
    // also add interrupt feature to https://github.com/adafruit/Adafruit-MCP23008-library

    return true;
#endif

    return false;
}

/**
 @brief Configures the Babel language expansion chip. You must call this after
        configuring the display, as Babel needs a reference to it to function.
 @param bcs Chip select for the Babel chip.
 @returns true if Babel was successfully set up.
 @note Open Book has a default pin for Babel. The e-book wing has a solder
       jumper for Babel on the "extra" pin; if your device has a usable pin
       there, we assume you've closed the jumper. Pass in a different value for
       Babel Chip Select if you've wired it to a different GPIO.
*/
bool OpenBook::configureBabel(int8_t bcs) {
    if (this->display == NULL || bcs < 0) return false;

    BabelTypesetterGFX *typesetter = new BabelTypesetterGFX(this->display, bcs, &SPI);
    typesetter->begin();
    this->typesetter = typesetter;

    return true;
}

/**
 @brief Configures pins for audio input and output.
 @param left DAC pin for the left or mono channel.
 @param right DAC pin for the right channel, or OPENBOOK_NOT_PRESENT if using
              the wing where audio output is mono.
 @param inlineMic analog pin for raw input from the inline mic, or
                  OPENBOOK_NOT_PRESENT if not available. Useful for getting
                  presses on the inline button. Android-compatible headsets have
                  three additional voltage levels you can read for additional
                  functionality: https://source.android.com/devices/accessories/headset/plug-headset-spec
 @param amplifiedMic analog pin with amplified mic input, coming from the
                     MAX4466, or OPENBOOK_NOT_PRESENT if this is not available.
                     Wing users can plug an Adafruit or Sparkfun electret mic
                     breakout board into a STEMMA port, and get amplified mic
                     input that way; pass in A1 or A2, depending on the port you
                     are using.
 @returns true as long as at least one audio pin was configured.
*/
bool OpenBook::configureAudio(int8_t left, int8_t right, int8_t inlineMic, int8_t amplifiedMic) {
    int8_t channels = 0;
    if(left >= 0) {
        channels++;
    }
    if(right >= 0) {
        channels++;
    }
    if(inlineMic >= 0) {
        pinMode(inlineMic, INPUT);
    }
    if(amplifiedMic >= 0) {
        pinMode(amplifiedMic, INPUT);
    }

    this->leftOutput = left;
    this->rightOutput = right;
    this->micInput = inlineMic;
    this->amplifiedInput = amplifiedMic;
    this->outputChannels = channels;

    return channels || (inlineMic >= 0) || (amplifiedMic >= 0);
}

/**
 @brief Reads button state and returns it as a byte.
 @returns a bitmask with 1 for every button that is pressed, and 0 for every
          button that is not.
 @note Don't worry whether the buttons are active low or active high; this
       method abstracts that away. 1 is pressed, 0 is not pressed.
*/
uint8_t OpenBook::readButtons() {
    uint8_t buttonState = 0;
#if OPENBOOK_USES_IO_EXPANDER
    if(this->ioExpander != NULL) {
        // read from I2C
        buttonState = this->ioExpander->readGPIO();
    }
#else
    if (this->buttonData > 0) {
        // read from shift register
        buttonState = this->readButtonRegister();
        // clear high bit which is the SD card detect
        buttonState &= 0x7f;
        // and replace high bit with state of the lock button
        if (this->buttonInterrupt >= 0 && !digitalRead(this->buttonInterrupt)) {
            buttonState |= 0x80; // set high bit if lock button is low (pressed)
        }
    }
#endif
    if (this->activeState == LOW) return ~buttonState; // low buttons are pressed, high buttons are being pulled up.
    else return buttonState; // high buttons are pressed, low buttons are being pulled down.
}

/**
 @brief Lets you know whether there is an SD card inserted.
 @returns one of the following three values:
          * OPEN_BOOK_SD_CARD_UNKNOWN if the device does not have a card detect signal
          * OPEN_BOOK_SD_CARD_PRESENT if the signal is available and a card is inserted
          * OPEN_BOOK_SD_CARD_NOT_PRESENT if the signal is available and no card is inserted
 @note This is designed to work with the SD card slot available on the Adafruit website.
       The one in the DigiKey 1-click BOM does not have card detect functionality; if you
       use that one, this function will always return the same value.
*/
OpenBookSDCardState OpenBook::sdCardState() {
    #if defined(ODDLY_SPECIFIC_OPEN_BOOK)
    if ((this->readButtonRegister() & OPENBOOK_BUTTONMASK_SDCD) == 0) return OPEN_BOOK_SD_CARD_NOT_PRESENT;
    return OPEN_BOOK_SD_CARD_PRESENT;
    #endif

    return OPEN_BOOK_SD_CARD_UNKNOWN;
}

/**
 @brief Private method to read button states from the shift register.
 @returns a bitmask with the state of the shift register.
*/
uint8_t OpenBook::readButtonRegister() {
    uint8_t registerData = 0;

    // pulse the latch to load the current state into the shift register.
    digitalWrite(this->buttonLatch, LOW);
    delayMicroseconds(1);
    digitalWrite(this->buttonLatch, HIGH);
    delayMicroseconds(1);

    for(int i = 0; i < 8; i++) {
        // left shift the bits read so far...
        registerData <<= 1;
        // ...then read the next state into the low bit of the byte...
        registerData |= digitalRead(this->buttonData);
        // ...and finally pulse clock to shift the next state into buttonData.
        digitalWrite(this->buttonClock, HIGH);
        delayMicroseconds(1);
        digitalWrite(this->buttonClock, LOW);
        delayMicroseconds(1);
    }

    return registerData;
}

/**
 @returns a reference to the e-paper display, or NULL if not configured.
*/
OpenBook_IL0398 * OpenBook::getDisplay() {
    return this->display;
}

/**
 @returns a reference to the Babel typesetter, or NULL if not configured.
*/
BabelTypesetterGFX * OpenBook::getTypesetter() {
    return this->typesetter;
}

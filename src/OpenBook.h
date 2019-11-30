#ifndef OSO_OpenBook_h
#define OSO_OpenBook_h

#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

#include "BabelTypesetterGFX.h"
#include "Adafruit_MCP23008.h"
#include "Adafruit_EPD.h"

#define OPENBOOK_KNOWN_HARDWARE 1

#define OPENBOOK_NOT_PRESENT (-1)

#define OPENBOOK_BUTTONMASK_LEFT (1)
#define OPENBOOK_BUTTONMASK_DOWN (2)
#define OPENBOOK_BUTTONMASK_UP (4)
#define OPENBOOK_BUTTONMASK_RIGHT (8)
#define OPENBOOK_BUTTONMASK_SELECT (16)
#define OPENBOOK_BUTTONMASK_PREVIOUS (32)
#define OPENBOOK_BUTTONMASK_NEXT (64)
#define OPENBOOK_BUTTONMASK_LOCK (128)


#if defined(ODDLY_SPECIFIC_OPEN_BOOK)
#define OPENBOOK_DISPLAY_BUS (&SPI1)
#define OPENBOOK_SRCS (-1) // RAM chip acting wacky on one of the prototype boards, disabling for now (53)
#define OPENBOOK_ECS (44)
#define OPENBOOK_EDC (45)
#define OPENBOOK_ERST (46)
#define OPENBOOK_EBSY (47)
#define OPENBOOK_SDCS (4)
#define OPENBOOK_BCS (52)
#define OPENBOOK_BATTERY_MONITOR (A6)
#define OPENBOOK_BUTTON_INTERRUPT (-1)
#define OPENBOOK_BUTTON_LATCH (48)
#define OPENBOOK_BUTTON_DATA (49)
#define OPENBOOK_BUTTON_CLOCK (50)
#define OPENBOOK_BUTTON_LOCK (51)
#define OPENBOOK_BUTTON_ACTIVE (HIGH)
#define OPENBOOK_AUDIO_L (A0)
#define OPENBOOK_AUDIO_R (A1)
#define OPENBOOK_MIC_RAW (A7)
#define OPENBOOK_MIC_AMPLIFIED (A10)

#elif defined(ADAFRUIT_FEATHER_M4_EXPRESS) // e-book wing on Feather M4
#define OPENBOOK_DISPLAY_BUS (&SPI)
#define OPENBOOK_SRCS (6)
#define OPENBOOK_ECS (9)
#define OPENBOOK_EDC (10)
#define OPENBOOK_ERST (A4)
#define OPENBOOK_EBSY (A3)
#define OPENBOOK_SDCS (5)
#define OPENBOOK_BCS (4)
#define OPENBOOK_BATTERY_MONITOR (-1)
#define OPENBOOK_BUTTON_INTERRUPT (A5)
#define OPENBOOK_BUTTON_LATCH (-1)
#define OPENBOOK_BUTTON_DATA (-1)
#define OPENBOOK_BUTTON_CLOCK (-1)
#define OPENBOOK_BUTTON_LOCK (-1)
#define OPENBOOK_BUTTON_ACTIVE (LOW)
#define OPENBOOK_AUDIO_L (A0)
#define OPENBOOK_AUDIO_R (-1)
#define OPENBOOK_MIC_RAW (-1)
#define OPENBOOK_MIC_AMPLIFIED (-1)

#else // TODO: pin mappings for other Feather boards.
#define OPENBOOK_KNOWN_HARDWARE 0
#endif


class OpenBook {
public:
    OpenBook();
#if OPENBOOK_KNOWN_HARDWARE
    bool configureScreen(int8_t srcs = OPENBOOK_SRCS, int8_t ecs = OPENBOOK_ECS, int8_t edc = OPENBOOK_EDC, int8_t erst = OPENBOOK_ERST, int8_t ebsy = OPENBOOK_EBSY, SPIClass *spi = OPENBOOK_DISPLAY_BUS, int width = 300, int height = 400);
    bool configureShiftButtons(int8_t latch = OPENBOOK_BUTTON_LATCH, int8_t data = OPENBOOK_BUTTON_DATA, int8_t clock = OPENBOOK_BUTTON_CLOCK, int8_t lockButton = OPENBOOK_BUTTON_LOCK);
    bool configureI2CButtons(int8_t interrupt = OPENBOOK_BUTTON_INTERRUPT);
    bool configureBabel(int8_t bcs = OPENBOOK_BCS);
    bool configureAudio(int8_t left = OPENBOOK_AUDIO_L, int8_t right = OPENBOOK_AUDIO_R, int8_t inlineMic = OPENBOOK_MIC_RAW, int8_t amplifiedMic = OPENBOOK_MIC_AMPLIFIED);
#else
    bool configureScreen(int8_t srcs, int8_t ecs, int8_t edc, int8_t erst, int8_t ebsy, SPIClass *spi, int width, int height);
    bool configureShiftButtons(int8_t active, int8_t latch, int8_t data, int8_t clock, int8_t lockButton);
    bool configureI2CButtons(int8_t active, int8_t interrupt);
    bool configureBabel(int8_t bcs);
    bool configureAudio(int8_t left, int8_t right, int8_t inlineMic, int8_t amplifiedMic);
#endif // OPENBOOK_KNOWN_HARDWARE

    uint8_t readButtons();
    Adafruit_EPD *getDisplay();
    BabelTypesetterGFX *getTypesetter();

protected:
    uint8_t readButtonRegister();
    Adafruit_EPD *display = NULL;
    BabelTypesetterGFX *typesetter = NULL;
    Adafruit_MCP23008 *ioExpander = NULL;
    int8_t buttonLatch, buttonData, buttonClock, buttonInterrupt, leftOutput, rightOutput, micInput, amplifiedInput, outputChannels;
};
#endif // OSO_OpenBook_h

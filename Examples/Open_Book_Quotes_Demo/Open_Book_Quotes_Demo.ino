/*
  Open Book Quotes Demo

  Demonstrates basic usage of the Open Book Arduino library.
  
  Displays four quotes in different languages on four pages. Navigate with the left/right or previous/next buttons.
  Use the up and down buttons to demonstrate Babel's localized case mapping by turning the quotes to uppercase or lowercase.
  Use with either a Feather M4 + eBook Wing, or the Open Book board.
*/

#include <OpenBook.h>

OpenBook *book;

void setup() {
  Serial.begin(115200);

  // the OpenBook class provides an interface for the peripherals of either the Open Book or eBook Wing.
  book = new OpenBook();
  
  book->configureScreen();
  // pause before initializing Babel; on the wing they share a bus and I've seen things get garbled if you don't give it a hot second.
  delay(500);
  // Babel requires a GD25Q16C chip flashed with the latest babel.bin file. See BurnBabelBurn for details: https://github.com/joeycastillo/babel/blob/master/examples/BurnBabelBurn/BurnBabelBurn.ino
  book->configureBabel();
  
  #if defined(ODDLY_SPECIFIC_OPEN_BOOK)
  // the buttons on the Open Book board are on a shift register
  book->configureShiftButtons();
  #else // on the wing, they're on an I2C port expander.
  book->configureI2CButtons();
  #endif
  
  book->getTypesetter()->textColor = EPD_BLACK;
  book->getTypesetter()->setLayoutArea(16, 16, 264, 368);
  book->getDisplay()->setRotation(0);
}

char *originals[] = {
"Voici mon secret. Il est très \nsimple: on ne voit bien qu'avec \nle cœur. L'essentiel est \ninvisible pour les yeux.\n\n",
"Бери́сь дру́жно, не бу́дет гру́зно.\n\n",
"La vida no es la que uno vivió, \nsino la que uno recuerda, y \ncómo la recuerda para contarla.\n\n",
"ἄνδρα μοι ἔννεπε, μοῦσα, \nπολύτροπον, ὃς μάλα πολλὰ πλάγχθη, \nἐπεὶ Τροίης ἱερὸν πτολίεθρον \nἔπερσεν.\n\n",
};

void loop() {
  static int i = 0;
  static bool firstRun = true;

  uint8_t buttons = book->readButtons();

  if (firstRun || buttons) {
    firstRun = false;
    if (buttons & OPENBOOK_BUTTONMASK_NEXT || buttons & OPENBOOK_BUTTONMASK_RIGHT) i++;
    if (buttons & OPENBOOK_BUTTONMASK_PREVIOUS|| buttons & OPENBOOK_BUTTONMASK_LEFT) i--;
    i = max(i, 0);
    i = min(i, 3);

    // the display is the Adafruit_GFX object we use to manipulate any pixels on the screen.
    Adafruit_EPD *display = book->getDisplay();
    // the Babel typesetter has its own reference to the display, which it uses to display multilingual text.
    BabelTypesetter *typesetter = book->getTypesetter();

    // determine the number of codepoints in the string (utf-8 can have one codepoint per 1-6 bytes)
    size_t len = typesetter->glyphStorage->utf8_codepoint_length(originals[i]);
    // allocate an appropriately sized buffer (codepoints outside the basic multilingual plane can be larger than a uint16_t, but for now Babel only supports the BMP)
    BABEL_CODEPOINT *buf = (uint16_t *)malloc(len);
    // parse UTF-8 to codepoints
    typesetter->glyphStorage->utf8_parse(originals[i], buf);

    // Localized case mapping demo. Converts buf in-place to uppercase or lowercase.
    if (buttons & OPENBOOK_BUTTONMASK_UP) typesetter->glyphStorage->to_uppercase(buf, len);
    if (buttons & OPENBOOK_BUTTONMASK_DOWN) typesetter->glyphStorage->to_lowercase(buf, len);

    // finally. clear the screen and show some text.
    display->clearBuffer();
    typesetter->bold = false;
    typesetter->italic = false;
    typesetter->setCursor(16, 16);
    typesetter->writeCodepoints(buf, len);
    typesetter->italic = true;
    typesetter->writeCodepoints(buf, len);
    typesetter->italic = false;
    typesetter->bold = true;
    typesetter->writeCodepoints(buf, len);
    typesetter->italic = true;
    typesetter->writeCodepoints(buf, len);

    display->display();

    free(buf);
  }
}

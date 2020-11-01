/*
 * MVBook for the Open Book
 * The Minimum Viable Book Reader
 * by Joey Castillo
 * 
 * MIT License
 */
#include <MenuSystem.h>
#include <OpenBook.h>
#include <SD.h>

// global reference to the Open Book hardware abstraction
OpenBook *book;

// global state
bool fullRefresh = true;
char *currentBook = NULL;
size_t currentLine = 0;
bool atEnd = false;
bool bookNeedsRefresh = false;
uint8_t buttons = 0;
int menuOffset = 0;

#include "menu.h"
#include "readbook.h"

void open_file(MenuComponent* menu_item) {
    currentBook = (char *)menu_item->get_name();
    currentLine = 2; // start of lines, after title and author
    bookNeedsRefresh = true;

    Serial.print("Opening ");
    Serial.println(currentBook);
}

void setup() {
    book = new OpenBook();
    Serial.begin(115200);
    Serial.println("MVBook Demo!");

    if (!SD.begin(OPENBOOK_SDCS)) {
        Serial.println("No SD?");
    }

    book->configureScreen();
    book->configureBabel();
    #if defined(ODDLY_SPECIFIC_OPEN_BOOK)
    // the buttons on the Open Book board are on a shift register
    book->configureShiftButtons();
    #else // on the wing, they're on an I2C port expander.
    book->configureI2CButtons();
    #endif

    book->getDisplay()->setRotation(0);

    File root = SD.open("/");

    int i = 0;
    File entry = root.openNextFile();
    Serial.println("Adding files...");
    while (entry) {
        if (!entry.isDirectory()) {
            uint64_t magic = 0;
            uint8_t terminator = 0;
            entry.read((void *)&magic, 8);
            if (magic == 5426643222204338255) { // the string "OPENBOOK"
              Serial.println(entry.name());
              char *filename = (char *)malloc(13);
              strcpy(filename, entry.name());
              MenuItem *item = new MenuItem(filename, &open_file);
              ms.get_root_menu().add_item(item);
            }
        }
        entry = root.openNextFile();
    }

    ms.get_root_menu().set_name("The Open Book");
    ms.display();
}

void loop() {
    buttons = book->readButtons();

    if(currentBook == NULL) {
        doMenu();
    } else {
        doReader();
    }
}

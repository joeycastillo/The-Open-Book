/*
 * MVBook for the Open Book
 * The Minimum Viable Book Reader
 * by Joey Castillo
 * 
 * MIT License
 */
#include <MenuSystem.h>
#include <OpenBook.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>

// global reference to the Open Book hardware abstraction
OpenBook *book;

// global reference to the Flash chip and file system
Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;
FatFile file;
FatFile root;

// global state
bool fullRefresh = true;
char *currentBook = NULL;
size_t currentLine = 0;
bool atEnd = false;
bool bookNeedsRefresh = false;
uint8_t buttons = 0;

#include "menu.h"
#include "readbook.h"

void open_file(MenuComponent* menu_item) {
    currentBook = (char *)menu_item->get_name();
    currentLine = 2; // start of lines, after title and author
    bookNeedsRefresh = true;
}

void setup() {
    book = new OpenBook();
    Serial.begin(115200);
    Serial.println("MVBook Demo!");

    flash.begin();
    fatfs.begin(&flash);
    
    book->configureScreen();
    book->configureBabel();
    book->configureShiftButtons();

    book->getDisplay()->setRotation(0);

    if (!root.open("/")) {
      Serial.println("open root failed");
    }
    root.rewind();

    int i = 0;
    Serial.println("Adding files...");
    while (i < 6 && file.openNext(&root, O_RDONLY)) {
        if (!file.isDir()) {
            uint64_t magic = 0;
            uint8_t terminator = 0;
            file.read((void *)&magic, 8);
            if (magic == 5426643222204338255) { // the string "OPENBOOK"
              char *filename = (char *)malloc(13);
              file.getName(filename, 13);
              Serial.println(filename);
              MenuItem *item = new MenuItem(filename, &open_file);
              ms.get_root_menu().add_item(item);
              i++;
            }
        }
        file.close();
    }
    Serial.println("Done!");

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

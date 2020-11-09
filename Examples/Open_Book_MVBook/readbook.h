void doReader() {
    if (buttons & OPENBOOK_BUTTONMASK_PREVIOUS) {
        if (currentLine > 22) {
            currentLine -= 22;
            atEnd = false;
            bookNeedsRefresh = true;
        }
    }
    if (buttons & OPENBOOK_BUTTONMASK_NEXT) {
        if (!atEnd) {
            currentLine += 22;
            bookNeedsRefresh = true;
        }
    }
    if (buttons & OPENBOOK_BUTTONMASK_SELECT) {
        currentBook = NULL;
        currentLine = 0;
        atEnd = false;
        fullRefresh = true;
        ms.display();

        return;
    }

    if (bookNeedsRefresh) {
        // update user's stored location
        File f = SD.open(currentProgressFile, O_WRITE | O_CREAT);
        f.write((byte *)&currentLine, sizeof(size_t));
        f.close();

        // refresh the screen
        bookNeedsRefresh = false;
        book->getDisplay()->clearBuffer();
        BabelTypesetter *typesetter = book->getTypesetter();
        typesetter->setTextColor(EPD_BLACK);
        typesetter->setLayoutArea(16, 16, 264, 368);
        File file = SD.open(currentBook);
        for(int i = 0; i < 22; i++) {
            uint64_t loc;
            uint32_t len;
            file.seek(8 + (i + currentLine) * 12);
            file.read(&loc, 8);
            file.read(&len, 4);

            // handle flags (high bit of len indicates last line)
            atEnd = len >> 15;
            len &= 0x7FFF;
            
            file.seek(loc);
            char *line = (char *)malloc(len + 1);
            file.read(line, len);
            line[len] = 0;
            typesetter->print(line);
            typesetter->print("\n");
            free(line);

            if (atEnd) {
                Serial.println("break");
                break;
            }
        }

        // draw the user's place in the book as a progress bar at the bottom
        uint64_t pos = file.position();
        uint64_t len = file.size();
        uint64_t textStart;
        file.seek(32);
        file.read(&textStart, 8);
        pos -= textStart;
        len -= textStart;
        uint16_t progress = (uint16_t)(300 * pos / len);
        book->getDisplay()->fillRect(0, 384, progress, 16, EPD_BLACK);

        book->getDisplay()->setDisplayMode(OPEN_BOOK_DISPLAY_MODE_QUICK);
        book->getDisplay()->display();
    }
}


/*  The MVBook File Format
 *  ----------------------
 *  
 *  This is not a _great_ file format, just a minimally viable one :)
 *  
 *  Bytes                 Purpose
 *   0..7                 The string "OPENBOOK" as a magic number indicating compatibility
 *   8..15                The location in the file of the book's title
 *  16..17                The length of the title
 *  18..25                The location in the file of the book's author
 *  26..27                The length of the author
 *  28..numLines * 12 - 1
 *  
 *  From there the header repeats 64-bit locations and 16-bit lengths for each line of text.
 *  The last entry should have the high bit of its length set to indicate that it is the last line.
 *  Then it's just a concatenation of the title, author and lines at the indicated offsets.
 *  
 *  I used a Python script to generate obk files for this sketch; you can find it here:
 *  https://gist.github.com/joeycastillo/39de5625ebdf88c3515454cbdbfe941a
 */

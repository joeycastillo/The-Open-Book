uint16_t selectionIndicatorOrigin = 0;
uint16_t selectionIndicatorHeight = 0;

class MyRenderer : public MenuComponentRenderer {
public:
    void render(Menu const& menu) const {
        book->getDisplay()->fillScreen(EPD_WHITE);
        menu.render(*this);
        for (int i = 0; i < menu.get_num_components(); ++i) {
            MenuComponent const* component = menu.get_menu_component(i);
            component->render(*this);
        }
        if (fullRefresh) {
            book->getDisplay()->setDisplayMode(OPEN_BOOK_DISPLAY_MODE_DEFAULT);
            // display the menu WITHOUT the selection indicator. The default waveform pushes a lot of
            // ink to the front, so if the indicator displays here, an after-image will remain even
            // after the user scrolls to the next item.
            book->getDisplay()->display();
            // now do a partial refresh of the indicator area; the default waveform sucked a lot of
            // ink to the back, this equalizes it a bit and makes the indicator appear bolder when 
            // we display it below.
            book->getDisplay()->displayPartial(72, 280, 320, 8);
            fullRefresh = false;
        }
        book->getDisplay()->fillRect(12, selectionIndicatorOrigin, 8, selectionIndicatorHeight, EPD_BLACK);
        // also note that the partial update method is a mess right now; it should rotate this to the
        // screen orientation and expand to 8-byte boundaries, but right now i'm passing in raw coordinates
        // from the hardware's POV (and making sure they're multiples of 8 to avoid glitchiness)
        book->getDisplay()->displayPartial(72, 280, 320, 8);
    }

    void render_menu_item(MenuItem const& menu_item) const {
        FatFile file = fatfs.open((char *)menu_item.get_name(), FILE_READ);
        file.seekSet(8);
        uint64_t title_loc;
        uint32_t title_len;
        uint64_t author_loc;
        uint32_t author_len;
        file.read(&title_loc, 8);
        file.read(&title_len, 4);
        file.read(&author_loc, 8);
        file.read(&author_len, 4);

        char *title = (char *)malloc(title_len + 1);
        char *author = (char *)malloc(author_len + 1);

        file.seekSet(title_loc);
        file.read(title, title_len);
        title[title_len] = 0;
        file.seekSet(author_loc);
        file.read(author, author_len);
        author[author_len] = 0;

        BabelTypesetter *typesetter = book->getTypesetter();
        int startY = typesetter->getCursorY();
        typesetter->setBold(true);
        typesetter->setItalic(false);
        typesetter->print(title);
        typesetter->print("\n");
        typesetter->setBold(false);
        typesetter->setItalic(true);
        typesetter->print(author);
        typesetter->setItalic(false);
        typesetter->print("\n");
        if (menu_item.is_current()) {
            selectionIndicatorOrigin = startY;
            selectionIndicatorHeight = typesetter->getCursorY() - startY + 2;
        }
        typesetter->print("\n");

        free(title);
        free(author);
        file.close();
    }

    void render_back_menu_item(BackMenuItem const& menu_item) const {
    }

    void render_numeric_menu_item(NumericMenuItem const& menu_item) const {
    }

    void render_menu(Menu const& menu) const {
        book->getDisplay()->clearBuffer();
        book->getDisplay()->setTextSize(1);
        book->getDisplay()->setCursor(16, 16);
        book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
        book->getDisplay()->setTextSize(3);
        book->getDisplay()->println(menu.get_name());
        book->getDisplay()->setTextSize(1);
        book->getDisplay()->setCursor(16, 48);
        book->getDisplay()->println("Listing files in /");
        BabelTypesetter *typesetter = book->getTypesetter();
        typesetter->setLayoutArea(24, 72, 272, 336);
    }
};

MyRenderer my_renderer;
MenuSystem ms(my_renderer);

void doMenu() {
    if (buttons & OPENBOOK_BUTTONMASK_UP) {
        ms.prev();
        ms.display();
    }
    if (buttons & OPENBOOK_BUTTONMASK_DOWN) {
        ms.next();
        ms.display();
    }
    if (buttons & OPENBOOK_BUTTONMASK_SELECT) {
        ms.select();
        ms.display();
    }
}

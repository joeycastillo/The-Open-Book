uint16_t selectionIndicatorOrigin = 0;
uint16_t selectionIndicatorHeight = 0;

class MyRenderer : public MenuComponentRenderer {
public:
    void render(Menu const& menu) const {
        book->getDisplay()->fillScreen(EPD_WHITE);
        menu.render(*this);

        // draw the items in the list
        int lastItem = min(menuOffset + 6, menu.get_num_components());
        for (int i = menuOffset; i < lastItem; ++i) {
            MenuComponent const* component = menu.get_menu_component(i);
            component->render(*this);
        }

        // draw the selection indicator
        book->getDisplay()->fillRect(12, selectionIndicatorOrigin, 8, selectionIndicatorHeight, EPD_BLACK);

        if (fullRefresh) {
            // render page indicator at bottom
            BabelTypesetter *typesetter = book->getTypesetter();
            char buf[20];
            sprintf(buf, "\n%d to %d of %d", menuOffset + 1, lastItem, menu.get_num_components());
            typesetter->setCursor(32, 360);
            typesetter->print(buf);

            // note: The quick waveform pushes a bit more ink than the partial one, so when scrolling from row 1
            // to row 2, a slight after-image may remain. You can address this by displaying the menu in quick mode,
            // without the selection indicator, and then refreshing only the selection area in partial mode. I used
            // to do this, but it takes more time and makes the interface feel less responsive, hence this tradeoff.
            book->getDisplay()->setDisplayMode(OPEN_BOOK_DISPLAY_MODE_QUICK);
            book->getDisplay()->display();
            fullRefresh = false;
        } else {
            // note that the partial update method is a mess right now; it should rotate this to the
            // screen orientation and expand to 8-byte boundaries, but right now i'm passing in raw coordinates
            // from the hardware's POV (and making sure they're multiples of 8 to avoid glitchiness)
            book->getDisplay()->displayPartial(72, 280, 320, 8);
        }
    }

    void render_menu_item(MenuItem const& menu_item) const {
        File file = SD.open((char *)menu_item.get_name());
        file.seek(8);
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

        file.seek(title_loc);
        file.read(title, title_len);
        title[title_len] = 0;
        file.seek(author_loc);
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

void selectFirstOnPage() {
    // I don't like this, but the MenuSystem library has no method to set the current index.
    // this just rewinds to the beginning and "clicks" next until we're at the right spot.
    ms.reset();
    while(ms.get_root_menu().get_current_component_num() != menuOffset) {
        ms.next();
    }
}

void doMenu() {
    if (buttons & OPENBOOK_BUTTONMASK_UP) {
        if (ms.get_root_menu().get_current_component_num() > menuOffset) {
            ms.prev();
            ms.display();
        }
    }
    if (buttons & OPENBOOK_BUTTONMASK_DOWN) {
        int lastItem = min(menuOffset + 6, ms.get_root_menu().get_num_components());
        if (ms.get_root_menu().get_current_component_num() < lastItem - 1) {
            ms.next();
            ms.display();
        }
    }
    if (buttons & OPENBOOK_BUTTONMASK_SELECT) {
        ms.select();
        ms.display();
    }
    if (buttons & OPENBOOK_BUTTONMASK_PREVIOUS) {
        if (menuOffset > 0) {
          menuOffset -= 6;
          selectFirstOnPage();
          fullRefresh = true;
          ms.display();
        }
    }
    if (buttons & OPENBOOK_BUTTONMASK_NEXT) {
        if ((menuOffset + 6) < ms.get_root_menu().get_num_components()) {
            menuOffset += 6;
            selectFirstOnPage();
            fullRefresh = true;
            ms.display();
        }
    }
}

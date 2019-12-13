class MyRenderer : public MenuComponentRenderer {
public:
    void render(Menu const& menu) const {
        book->getDisplay()->clearBuffer();
        menu.render(*this);
        for (int i = 0; i < menu.get_num_components(); ++i) {
            MenuComponent const* component = menu.get_menu_component(i);
            book->getDisplay()->setCursor(8 + (i * 200), 368);
            component->render(*this);
        }
    }

    void render_menu_item(MenuItem const& menu_item) const {
        if (menu_item.is_current()) book->getDisplay()->setTextColor(EPD_WHITE, EPD_BLACK);
        else book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
        book->getDisplay()->print(menu_item.get_name());
        book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
    }

    void render_back_menu_item(BackMenuItem const& menu_item) const {
        if (menu_item.is_current()) book->getDisplay()->setTextColor(EPD_WHITE, EPD_BLACK);
        else book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
        book->getDisplay()->print(menu_item.get_name());
        book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
    }

    void render_numeric_menu_item(NumericMenuItem const& menu_item) const {
        if (menu_item.is_current()) book->getDisplay()->setTextColor(EPD_WHITE, EPD_BLACK);
        else book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
        book->getDisplay()->print(menu_item.get_name());
        book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
    }

    void render_menu(Menu const& menu) const {
        book->getDisplay()->setTextColor(EPD_BLACK, EPD_WHITE);
        book->getDisplay()->setTextSize(2);
    }
};
MyRenderer my_renderer;

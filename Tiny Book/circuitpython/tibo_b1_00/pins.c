#include "shared-bindings/board/__init__.h"

#include "shared-module/displayio/__init__.h"

// This mapping only includes functional names because pins broken
// out on connectors are labeled with their MCU name available from
// microcontroller.pin.
STATIC const mp_rom_map_elem_t board_global_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_AUDIO), MP_ROM_PTR(&pin_PA02) },

    { MP_ROM_QSTR(MP_QSTR_ACCESSORY), MP_ROM_PTR(&pin_PA05) },

    { MP_ROM_QSTR(MP_QSTR_SECS), MP_ROM_PTR(&pin_PA18) },
    { MP_ROM_QSTR(MP_QSTR_SEDC), MP_ROM_PTR(&pin_PA17) },

    { MP_ROM_QSTR(MP_QSTR_TINT), MP_ROM_PTR(&pin_PA14) },
    { MP_ROM_QSTR(MP_QSTR_TRST), MP_ROM_PTR(&pin_PA20) },

    { MP_ROM_QSTR(MP_QSTR_SCK), MP_ROM_PTR(&pin_PA17) },
    { MP_ROM_QSTR(MP_QSTR_MOSI), MP_ROM_PTR(&pin_PB23) },
    { MP_ROM_QSTR(MP_QSTR_MISO), MP_ROM_PTR(&pin_PB22) },

    { MP_ROM_QSTR(MP_QSTR_TX), MP_ROM_PTR(&pin_PA00) },
    { MP_ROM_QSTR(MP_QSTR_RX), MP_ROM_PTR(&pin_PA01) },

    { MP_ROM_QSTR(MP_QSTR_SDA), MP_ROM_PTR(&pin_PB08) },
    { MP_ROM_QSTR(MP_QSTR_SCL), MP_ROM_PTR(&pin_PB09) },

    { MP_ROM_QSTR(MP_QSTR_SDCS), MP_ROM_PTR(&pin_PA15) },
    { MP_ROM_QSTR(MP_QSTR_SDCD), MP_ROM_PTR(&pin_PA13) },

    { MP_ROM_QSTR(MP_QSTR_NEOPIXEL), MP_ROM_PTR(&pin_PA30) },

    { MP_ROM_QSTR(MP_QSTR_LED), MP_ROM_PTR(&pin_PA27) },

    { MP_ROM_QSTR(MP_QSTR_BATTERY), MP_ROM_PTR(&pin_PA06) },
    { MP_ROM_QSTR(MP_QSTR_USB), MP_ROM_PTR(&pin_PA07) },

    { MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&board_i2c_obj) },
    { MP_ROM_QSTR(MP_QSTR_SPI), MP_ROM_PTR(&board_spi_obj) },
    { MP_ROM_QSTR(MP_QSTR_UART), MP_ROM_PTR(&board_uart_obj) },

    { MP_ROM_QSTR(MP_QSTR_DISPLAY), MP_ROM_PTR(&displays[0].display)}
};
MP_DEFINE_CONST_DICT(board_module_globals, board_global_dict_table);

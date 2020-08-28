# CircuitPython Board Definitions for Tiny Book prototypes

To build, follow the instructions in [Building CircuitPython](https://learn.adafruit.com/building-circuitpython?view=all), then copy one of these folders to the `ports/atmel-samd/boards` directory. Run `make BOARD=tibo_b1_00` (or whichever), put Tiny Book into bootloader mode, and then copy the UF2 to the BOOKBOOT partition. (you can use more or less any M4 bootloader)

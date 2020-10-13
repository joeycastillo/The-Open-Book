# Open Book CircuitPython Support

The Open Book works out of the oven with the [latest version of CircuitPython for the Open Book](https://circuitpython.org/board/openbook_m4/). This folder just has some additional libraries that may be of help. As I add more, I'll document them here. 

* `openbook_epd`
  * `il0398.py` - Custom driver for IL0398-based displays like the Open Book's 4.2" display. Currently only supports the "quick" mode, which refreshes the screen faster (at perhaps the cost of a little bit of contrast).
  * `il91874.py` - Custom driver for IL91874-based displays like the Tiny Book's 2.9" display. Also only supports the "quick" mode, though this is a bit more experimental; under some conditions it exhibits significant muddiness, though this is a temporary condition that is resolved with a refresh using the factory waveform.
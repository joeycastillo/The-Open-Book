#include <GxEPD.h>
#include <Wire.h>
#include "Adafruit_MCP23008.h"
#include "Adafruit_APDS9960.h"
#include <Adafruit_NeoPixel.h>

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#define MAX_DISPAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8))

#define EINK_CS        9
#define EINK_DC        10
#define EINK_BSY       A3
#define EINK_RST       A4

// Install the GxEPD2 library to use this demo.
GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=*/ 9, /*DC=*/ 10, /*RST=*/ A4, /*BUSY=*/ A3));

// This is the GPIO expander you should have on the board.
Adafruit_MCP23008 mcp;

// If you have a Neopixel strip, plug it into A1. If not, this sketch will just ignore it.
#define LED_PIN    A1
#define LED_COUNT 8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ400);

// For giggles, you could plug an APDS9960 into the I2C port for a gesture-driven interface.
// The sketch will still run if it's not there.
Adafruit_APDS9960 apds;

#include "001.h"
#include "002.h"
#include "003.h"
#include "004.h"
#include "005.h"
#include "006.h"

const char * titles [] = 
{
  "The Road",
  "Le Petit\nPrince",
  "Преступление\nи наказание",
  "道德經",
  "ﺃﻟﻒ ﻟﻴﻠﺔ ﻭ \nﻟﻴﻠﺔ‎",
  "Se una notte\nd'inverno un\nviaggiatore",
};
const char * authors [] = 
{
  "Cormac McCarthy",
  "Antoine de\nSaint-Exupéry",
  "Федор Достоевский",
  "老子",
  "ﻣﺆﻟﻒ ﻣﺠﻬﻮﻝ",
  "Italo Calvino",
};

void setup()
{
  Serial.begin(115200);

  // DEBUG: don't start sketch until Serial is up
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on at startup

  // Display
  display.init(115200); // enable diagnostic output on Serial
#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) // this works on other Express boards but for the moment I'm testing on an M4 Feather
  display.loadUnifontFile();
#endif
  display.setRotation(3);

  // Buttons
  mcp.begin();      // use default address 0

  for (int i = 0; i <= 7; i++)
  {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }

  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  if(!apds.begin()) Serial.println("failed to initialize APDS9960! Please check your wiring.");
  else Serial.println("APDS9960 initialized!");  
  apds.enableProximity(true);
  apds.enableGesture(true);

  display.firstPage();
}

void loop()
{
  // Backlight
  static bool lightEnabled = false;
  if (mcp.digitalRead(4) == LOW) lightEnabled = !lightEnabled;

  for(int c = 0; c < strip.numPixels(); c ++)
    if (lightEnabled)
      strip.setPixelColor(c, strip.Color(255, 255, 255));
    else
      strip.setPixelColor(c, strip.Color(0, 0, 0));
  strip.show(); // Update strip with new contents


  static int book = 0;
  static int page = 0;
  static int lastPage = -1;
  static int lastBook = -1;

  if (page < 0) page = 0;
  if (book < 0) book = 0;
  if (book > 5) book = 5;
  
  if (page != lastPage || book != lastBook)
  {
    lastPage = max(page, 0);
    lastBook = max(book, 0);
    display.setFullWindow();
    display.setCursor(0, 0);

    if (page == 0)
    {
      display.fillScreen(GxEPD_BLACK);
      display.setTextColor(GxEPD_WHITE);
      display.setTextSize(3);
      display.println();
      display.printlnUTF8((char *)titles[book]);
      display.println();
      display.setTextSize(2);
      display.printlnUTF8((char *)authors[book]);
    } else
    {
      display.fillScreen(GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.setTextSize(1);
      int start = (page - 1) * 25;
      for(int i = start; i < start + 25; i++)
      {
        switch (book)
        {
          case 0:
            display.printlnUTF8((char *)book001[i]);
            break;
          case 1:
            display.printlnUTF8((char *)book002[i]);
            break;
          case 2:
            display.printlnUTF8((char *)book003[i]);
            break;
          case 3:
            display.printlnUTF8((char *)book004[i]);
            break;
          case 4:
            display.printlnUTF8((char *)book005[i]);
            break;
          case 5:
            display.printlnUTF8((char *)book006[i]);
            break;
          default:
            break;
        }
      }
    }
    while (display.nextPage());
    Serial.print("Book ");
    Serial.print(book);
    Serial.print(", page ");
    Serial.print(page);
    Serial.println(".");
  }

  uint8_t gesture = apds.readGesture();
  if (gesture == APDS9960_DOWN) lightEnabled = true;
  if (gesture == APDS9960_UP) lightEnabled = false;
  if (mcp.digitalRead(5) == LOW || gesture == APDS9960_LEFT) page--;
  if (mcp.digitalRead(6) == LOW || gesture == APDS9960_RIGHT) page++;
  if (mcp.digitalRead(2) == LOW)
  {
     book--;
     page = 0;
  }
  if (mcp.digitalRead(1) == LOW)
  {
    book++;
    page = 0;
  }
  digitalWrite(LED_BUILTIN, page % 2 ? HIGH : LOW);   // turn the LED on (HIGH is the voltage level)
}

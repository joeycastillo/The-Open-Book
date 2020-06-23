/* 
 * This is the flash_from_header example sketch from Adafruit_DAP.
 *
 * NOTE: This sketch is intended to be run on a Feather M0.
 * Connect RESET on the Open Book to the Feather M0's pin 10.
 * Connect SWCLK on the Open Book to the Feather M0's pin 11.
 * Connect SWDIO on the Open Book to the Feather M0's pin 12.
 * Leave the Open Book's 3V and GND pins alone.
 * 
 * If you have another M0 board like a CircuitPlayground Express,
 * GEMMA M0, Trinket M0 or Arduino Zero, you can use those boards 
 * too; just change the pin #defines below to match your connections.
 */

#include "Adafruit_DAP.h"
#include "openbook_bootloader.h"

#define SWRST 10
#define SWCLK 11
#define SWDIO 12

#define BUFSIZE SAMx5_PAGE_SIZE       //don't change!

//create a DAP for programming Atmel SAM devices
Adafruit_DAP_SAMx5 dap;

// Function called when there's an SWD error
void error(const char *text) {
  Serial.println(text);
  while (1);
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while(!Serial);

  dap.begin(SWCLK, SWDIO, SWRST, &error);
  
  Serial.print("Connecting...");  
  if (! dap.dap_disconnect())                      error(dap.error_message);
  
  char debuggername[100];
  if (! dap.dap_get_debugger_info(debuggername))   error(dap.error_message);
  Serial.print(debuggername); Serial.print("\n\r");
  
  if (! dap.dap_connect())                         error(dap.error_message);
  
  if (! dap.dap_transfer_configure(0, 128, 128))   error(dap.error_message);
  if (! dap.dap_swd_configure(0))                  error(dap.error_message);
  if (! dap.dap_reset_link())                      error(dap.error_message);
  if (! dap.dap_swj_clock(50))               error(dap.error_message);
  dap.dap_target_prepare();

  uint32_t dsu_did;
  if (! dap.select(&dsu_did)) {
    Serial.print("Unknown device found 0x"); Serial.print(dsu_did, HEX);
    error("Unknown device found");
  }
  for (device_t *device = dap.devices; device->dsu_did > 0; device++) {
    if (device->dsu_did == dsu_did) {
      Serial.print("Found Target: ");
      Serial.print(device->name);
      Serial.print("\tFlash size: ");
      Serial.print(device->flash_size);
      Serial.print("\tFlash pages: ");
      Serial.println(device->n_pages);
    }
  }

  Serial.println(" done.");

  Serial.print("Erasing... ");
  dap.erase();
  Serial.println(" done.");
  
  Serial.print("Programming... ");
  unsigned long t = millis();
  uint32_t addr = dap.program_start();

  while(addr < sizeof(binfile)){
    dap.programBlock(addr, binfile + addr);
    addr += BUFSIZE;
  }
  
  Serial.print("took ");
  Serial.print(millis() - t);
  Serial.println(" milliseconds.\nDone!");
  dap.dap_set_clock(50);

  dap.deselect();
  dap.dap_disconnect();
  pinMode(12, INPUT);
}

void loop() {
  //blink led on the host to show we're done
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);  
}

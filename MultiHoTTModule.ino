#include "SoftwareSerial.h"

/**
 * MultiHoTT-Module is a stand alone Arduino based Application that acts as a bridge between
 * MutliWii FlightController and HoTTv4 capable devices to transmit telemetry information. 
 *
 * by Oliver Bayer, 07/2012
 */

#define DEBUG
#define LED 13

void setup() {
  pinMode(LED, OUTPUT);

  // Used for debuging and to communicate with MultiWii
  Serial.begin(115200);
  
  hottV4Setup();
}

static void blink() {
  static uint8_t blink = LOW;

  digitalWrite(LED, blink);
  blink = !blink;
}

void loop() {
  static uint32_t last = 0;
  static uint8_t state = 0;

  uint32_t now = millis();

  if ((now - last) > 250) {
    last = now;
    
    /** Be alive blink */
    blink();

    switch (state) {
      case 0:
        /** Read VBAT */
        sensorsReadVBAT();
        state++;
        break;

      case 1:
        /** Read temperatures */
        sensorsReadTemperatures();
        state++;
        break;

      default:
        state = 0;
    }
  }

  /** Request new data from MultiWii */
  //multiWiiRequestData();

  /** Send telemetry data via HoTTv4 */
  hottV4SendTelemetry();
}

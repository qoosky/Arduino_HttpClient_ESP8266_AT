#include <ArduinoHttpClient.h>
#include <ESP8266_AT.h>
#include "config.h"

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266
ESP8266_AT wifi(rxPin, txPin);

void setup() {
    Serial.begin(9600);

    while(true) {
        if(wifi.statusAT()) { Serial.println("AT status OK"); break; }
        else Serial.println("AT status NOT OK");
        delay(1000);
    }

    while(true) {
        if(wifi.connect(ssid, pass)) { Serial.println("Successfully connected to an AP"); break; }
        else Serial.println("Failed to connected to an AP. retrying...");
        delay(1000);
    }

    while(true) {
        if(wifi.statusWiFi()) { Serial.println("WiFi status OK"); break; }
        else Serial.println("WiFi status NOT OK");
        delay(1000);
    }
}

void loop() {
    // TODO
}

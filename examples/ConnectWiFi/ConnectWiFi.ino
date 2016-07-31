#include <ESP8266_AT.h>
#include "config.h"

// Three types of the constructors:

// 1. Create SoftwareSerial inside ESP8266_AT class
const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266
ESP8266_AT wifi(rxPin, txPin);

// 2. Provide SoftwareSerial
//SoftwareSerial softwareSerial(rxPin, txPin);
//ESP8266_AT wifi(softwareSerial);

// 3. Provide HardwareSerial
//ESP8266_AT wifi(Serial);


void setup() {
    // Set baud rate of the serial interfaces
     Serial.begin(9600); // in case of '1'
    //softwareSerial.begin(115200); // in case of '2'
    //Serial.begin(115200); // in case of '3'

    // Check the status of the ESP8266 AT command interface.
    while(true) {
        if(wifi.statusAT()) {
            Serial.println("AT status OK");
            break;
        }
        else {
            Serial.println("AT status NOT OK");
        }
        delay(1000);
    }

    // Connect to an AP
    while(true) {
        if(wifi.connect(ssid, pass)) {
            Serial.println("Successfully connected to an AP");
            break;
        }
        else {
            Serial.println("Failed to connected to an AP. retrying...");
        }
        delay(1000);
    }
}

void loop() {
    // Continue to check the status of the WiFi connectivity
    if(wifi.statusWiFi()) {
        Serial.println("WiFi status OK");
    }
    else {
        Serial.println("WiFi status NOT OK");
    }
    delay(1000);
}

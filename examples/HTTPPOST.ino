#include <HttpClient_ESP8266_AT.h>

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266
HttpClient_ESP8266_AT httpClient(rxPin, txPin);

void setup() {
    // HardwareSerial settings
    Serial.begin(9600);

    // Setup HttpClient_ESP8266_AT
    while(true) {
        if(httpClient.statusAT()) { Serial.println("AT status OK"); break; }
        else Serial.println("AT status NOT OK");
        delay(1000);
    }
    while(true) {
        if(httpClient.connectAP("SSID", "PASSWORD")) { Serial.println("Successfully connected to an AP"); break; }
        else Serial.println("Failed to connected to an AP. retrying...");
        delay(1000);
    }
    while(true) {
        if(httpClient.statusWiFi()) { Serial.println("WiFi status OK"); break; }
        else Serial.println("WiFi status NOT OK");
        delay(1000);
    }
}

void loop() {
    // TODO
}

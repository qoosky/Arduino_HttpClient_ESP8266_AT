#include <ArduinoHttpClient.h>
#include <ESP8266_AT.h>
#include "config.h"

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266
ESP8266_AT wifi(rxPin, txPin);

void setup() {
    // HardwareSerial settings
    Serial.begin(9600);

    // Setup ESP8266_AT
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
    // HTTP GET request
    HttpClient httpClient = HttpClient(wifi, "www.example.com", 80);
    httpClient.get("/");

    // Check response status code
    int status = httpClient.responseStatusCode();
    if(status != 200) {
        Serial.print("HTTP status code: ");
        Serial.println(status);
        delay(1000);
        return;
    }

    // Print response body
    String body = httpClient.responseBody();
    Serial.println(body);

    Serial.println("----------");
    delay(5000);
}

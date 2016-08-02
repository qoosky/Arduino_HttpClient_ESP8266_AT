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
    while(true) {
        // Let's start HTTP POST request
        httpClient.post("www.example.com", "/", "name=hoge%27hoge&val=123", "application/x-www-form-urlencoded");

        // Check the request data was sent successfully without timeout.
        //   -1: timeout error (request data was NOT sent)
        //    0: response parse error (request data sent SUCCESSFULLY, but response data was corrupted)
        //   else (>0): HTTP status code (200, 404, 500, 302,.. etc.)

        if(httpClient.responseStatusCode() >= 0) {
            // Serial data without parity check may be corrupted.
            // So, in this example, we do not care the actual value of it.
            // (The fact that request data was sent is important.)
            Serial.println("SUCCESS");
            break;
        }
        else {
            Serial.println("FAILURE, retrying...");
        }
    }
    delay(1000);
}

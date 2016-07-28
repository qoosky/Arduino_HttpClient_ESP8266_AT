#ifndef HTTPCLIENT_ESP8266_AT_H_
#define HTTPCLIENT_ESP8266_AT_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class HttpClient_ESP8266_AT
{
 public:
    // rxPin: Wire this to Tx Pin of ESP8266
    // txPin: Wire this to Rx Pin of ESP8266
    HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud = 115200);

    // Destructor
    ~HttpClient_ESP8266_AT();

 public:
    // Health check of m_serial
    bool statusAT();

 private:
    // Clear rx buffer
    void rxClear();

    // AT command serial interface for ESP8266
    SoftwareSerial *m_serial;
};

#endif // #ifndef HTTPCLIENT_ESP8266_AT_H_

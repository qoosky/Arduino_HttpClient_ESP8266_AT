#ifndef HTTPCLIENT_ESP8266_AT_H_
#define HTTPCLIENT_ESP8266_AT_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class HttpClient_ESP8266_AT
{
 public:
    HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud = 115200);
    ~HttpClient_ESP8266_AT();

 private:
    SoftwareSerial *m_serial;
};

#endif // #ifndef HTTPCLIENT_ESP8266_AT_H_

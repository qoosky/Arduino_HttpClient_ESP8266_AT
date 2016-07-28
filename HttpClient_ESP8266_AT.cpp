#include "HttpClient_ESP8266_AT.h"

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud)
{
    m_serial = new SoftwareSerial(rxPin, txPin);
    m_serial->begin(baud);
}

HttpClient_ESP8266_AT::~HttpClient_ESP8266_AT() {
    delete m_serial;
}

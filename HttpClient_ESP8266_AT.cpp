#include "HttpClient_ESP8266_AT.h"

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud)
{
    m_serial = new SoftwareSerial(rxPin, txPin);
    m_serial->begin(baud);
}

HttpClient_ESP8266_AT::~HttpClient_ESP8266_AT() {
    delete m_serial;
}

void HttpClient_ESP8266_AT::rxClear() {
    while(m_serial->available() > 0) m_serial->read();
}

bool HttpClient_ESP8266_AT::statusAT() {
    rxClear();
    m_serial->println("AT");

    // check "OK" is received
    String str;
    char c;
    unsigned long start = millis();
    const uint32_t timeout = 1000;
    const String target = "OK";
    while (millis() - start < timeout) {
        while(m_serial->available() > 0) {
            c = m_serial->read(); // read 1 byte
            if(c == '\0') continue;
            str += c;
        }
        if (str.indexOf(target) != -1) return true;
    }
    return false;
}

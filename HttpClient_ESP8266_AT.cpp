#include "HttpClient_ESP8266_AT.h"

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud) :
    m_rxPin(rxPin), m_txPin(txPin)
{
    SoftwareSerial *serial = new SoftwareSerial(rxPin, txPin);
    serial->begin(baud);
    m_serial = serial;
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(SoftwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial)
{
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(HardwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial)
{
}

HttpClient_ESP8266_AT::~HttpClient_ESP8266_AT() {
    if(m_rxPin != 0 && m_txPin !=0) delete m_serial;
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
            c = m_serial->read(); // 1 byte
            if(c == '\0') continue;
            str += c;
        }
        if (str.indexOf(target) != -1) return true;
    }
    return false;
}

bool HttpClient_ESP8266_AT::connect(String ssid, String password) {
    rxClear();
    return true; // TODO
}

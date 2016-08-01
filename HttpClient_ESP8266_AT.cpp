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
    disconnectAP();
    if(m_rxPin != 0 && m_txPin !=0) delete m_serial;
}

void HttpClient_ESP8266_AT::rxClear() {
    while(m_serial->available() > 0) m_serial->read();
}

bool HttpClient_ESP8266_AT::checkATResponse(String *buf, String target, uint32_t timeout) {
    *buf = "";
    char c;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(m_serial->available() > 0) {
            c = m_serial->read(); // 1 byte
            if(c == '\0') continue;
            *buf += c;
        }
        if (buf->indexOf(target) != -1) return true;
    }
    return false;
}

bool HttpClient_ESP8266_AT::checkATResponse(String target, uint32_t timeout) {
    String buf;
    return checkATResponse(&buf, target, timeout);
}

bool HttpClient_ESP8266_AT::statusAT() {
    rxClear();
    m_serial->println("AT");
    return checkATResponse();
}

bool HttpClient_ESP8266_AT::restart() {
    rxClear();
    m_serial->println("AT+RST");
    if(!checkATResponse()) return false;
    delay(2000);
    unsigned long start = millis();
    while(millis() - start < 3000) {
        if(statusAT()) {
            delay(1500);
            return true;
        }
        delay(100);
    }
    return false;
}

bool HttpClient_ESP8266_AT::connectAP(String ssid, String password) {
    rxClear();
    m_serial->println("AT+CWMODE_DEF=1"); // 1: station(client) mode, 2: softAP(server) mode, 3: 1&2
    if(!(checkATResponse() && restart())) return false; // change "DEF"ault cwMode and restart

    uint8_t retry = 5;
    do {
        // Connect to an AP
        rxClear();
        delay(500);
        m_serial->print("AT+CWJAP_DEF=\"");
        m_serial->print(ssid);
        m_serial->print("\",\"");
        m_serial->print(password);
        m_serial->println("\"");
        if(checkATResponse("OK", 10000)) return true;
    } while(--retry);
    return false;
}

bool HttpClient_ESP8266_AT::disconnectAP() {
    rxClear();
    m_serial->println("AT+CWQAP");
    return checkATResponse();
}

uint8_t HttpClient_ESP8266_AT::ipStatus() {
    String buf;
    rxClear();
    m_serial->println("AT+CIPSTATUS");
    checkATResponse(&buf, "S:");
    uint32_t index = buf.indexOf(":");
    return buf.substring(index + 1, index + 2).toInt();
}

bool HttpClient_ESP8266_AT::statusWiFi() {
    uint8_t checkCnt = 5;
    do {
        if(ipStatus() == 5) return false;
        delay(100);
    } while(--checkCnt);
    return true;
}

bool HttpClient_ESP8266_AT::connectedTcp() {
    uint8_t retry = 5;
    do {
        if(ipStatus() == 3) return true;
        delay(100);
    } while(--retry);
    return false;
}

bool HttpClient_ESP8266_AT::disconnectTcp() {
    rxClear();
    m_serial->println("AT+CIPCLOSE");
    return checkATResponse();
}

bool HttpClient_ESP8266_AT::connectTcp(String host, uint32_t port) {
    if(connectedTcp()) disconnectTcp();
    String buf;
    uint8_t retry = 10;
    do {
        rxClear();
        m_serial->print("AT+CIPSTART=\"TCP\",\"");
        m_serial->print(host);
        m_serial->print("\",");
        m_serial->println(port);
        checkATResponse(&buf);
        if(buf.indexOf("OK") != -1 || buf.indexOf("ALREADY") != -1) {
            return true;
        }
        delay(100);
    } while(retry--);
    return false;
}

bool HttpClient_ESP8266_AT::get(const String& host, const String& path, uint32_t port) {
    return true; // TODO
}

bool HttpClient_ESP8266_AT::post(const String& host, const String& path, const String& body,
                                 const String& contentType, uint32_t port) {
    return true; // TODO
}

int HttpClient_ESP8266_AT::responseStatusCode() {
    return 200; // TODO
}

String HttpClient_ESP8266_AT::responseBody() {
    return "BODY"; // TODO
}

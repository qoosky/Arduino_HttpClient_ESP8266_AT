#include "ESP8266_AT.h"

ESP8266_AT::ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud) :
    m_rxPin(rxPin), m_txPin(txPin)
{
    SoftwareSerial *serial = new SoftwareSerial(rxPin, txPin);
    serial->begin(baud);
    m_serial = serial;
}

ESP8266_AT::ESP8266_AT(SoftwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial)
{
}

ESP8266_AT::ESP8266_AT(HardwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial)
{
}

ESP8266_AT::~ESP8266_AT() {
    disconnect();
    if(m_rxPin != 0 && m_txPin !=0) delete m_serial;
}

void ESP8266_AT::rxClear() {
    while(m_serial->available() > 0) m_serial->read();
}

bool ESP8266_AT::checkATResponse(String *buf, String target, uint32_t timeout) {
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

bool ESP8266_AT::checkATResponse(String target, uint32_t timeout) {
    String buf;
    return checkATResponse(&buf, target, timeout);
}

bool ESP8266_AT::statusAT() {
    rxClear();
    m_serial->println("AT");
    return checkATResponse();
}

bool ESP8266_AT::restart() {
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

bool ESP8266_AT::connect(String ssid, String password) {
    rxClear();
    m_serial->println("AT+CWMODE_DEF=1"); // 1: station(client) mode, 2: softAP(server) mode, 3: 1&2
    if(!(checkATResponse() && restart())) return false; // change "DEF"ault cwMode and restart

    // Connect to an AP
    rxClear();
    m_serial->print("AT+CWJAP_DEF=\"");
    m_serial->print(ssid);
    m_serial->print("\",\"");
    m_serial->print(password);
    m_serial->println("\"");
    return checkATResponse("OK", 10000);
}

bool ESP8266_AT::disconnect() {
    rxClear();
    m_serial->println("AT+CWQAP");
    return checkATResponse();
}

bool ESP8266_AT::statusWiFi() {
    String buf;
    rxClear();
    m_serial->println("AT+CIPSTATUS");
    checkATResponse(&buf, "S:", 10000);
    uint32_t index = buf.indexOf(":");
    uint8_t stat = buf.substring(index + 1, index + 2).toInt();
    return (stat != 5); // 5: ESP8266 station is NOT connected to an AP
}

int ESP8266_AT::connect(const char *host, uint16_t port) {
    if(connected()) stop();
    String buf;
    uint8_t retry = 10;
    while(retry--) {
        rxClear();
        m_serial->print("AT+CIPSTART=\"TCP\",\"");
        m_serial->print(host);
        m_serial->print("\",");
        m_serial->println(port);
        checkATResponse(&buf, "OK", 2000);
        if(buf.indexOf("OK") != -1 || buf.indexOf("ALREADY") != -1) {
            return 1; // SUCCESS
        }
        delay(500);
    }
    return -1; // TIMED_OUT
}

int ESP8266_AT::connect(IPAddress ip, uint16_t port) {
    String host = "";
    for(uint8_t i = 0; i < 4;) {
        host += String(ip[i]);
        if(++i < 4) host += ".";
    }
    return connect(host.c_str(), port);
}

void ESP8266_AT::stop() {
    rxClear();
    m_serial->println("AT+CIPCLOSE");
    checkATResponse("OK", 5000);
}

uint8_t ESP8266_AT::connected() {
    return 1; // TODO
}

size_t ESP8266_AT::write(const uint8_t *buf, size_t size) {
    return 0; // TODO
}

size_t ESP8266_AT::write(uint8_t) {
    return 0; // TODO
}

int ESP8266_AT::available() {
    return 0; // TODO
}

int ESP8266_AT::read() {
    return 0; // TODO
}

int ESP8266_AT::read(uint8_t *buf, size_t size) {
    return 0; // TODO
}

int ESP8266_AT::peek() {
    return 0; // TODO
}

void ESP8266_AT::flush() {
    // TODO
}

ESP8266_AT::operator bool() {
    // TODO
}

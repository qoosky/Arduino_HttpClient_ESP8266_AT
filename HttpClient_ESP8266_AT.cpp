#include "HttpClient_ESP8266_AT.h"

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud) :
    m_rxPin(rxPin), m_txPin(txPin), m_httpClient(NULL)
{
    SoftwareSerial *serial = new SoftwareSerial(rxPin, txPin);
    serial->begin(baud);
    m_serial = serial;
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(SoftwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial), m_httpClient(NULL)
{
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(HardwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial), m_httpClient(NULL)
{
}

HttpClient_ESP8266_AT::~HttpClient_ESP8266_AT() {
    disconnect();
    if(m_httpClient != NULL) delete m_httpClient;
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

bool HttpClient_ESP8266_AT::connect(String ssid, String password) {
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

bool HttpClient_ESP8266_AT::disconnect() {
    rxClear();
    m_serial->println("AT+CWQAP");
    return checkATResponse();
}

bool HttpClient_ESP8266_AT::statusWiFi() {
    String buf;
    rxClear();
    m_serial->println("AT+CIPSTATUS");
    checkATResponse(&buf, "OK", 10000);
    uint32_t index = buf.indexOf(":");
    uint8_t stat = buf.substring(index + 1, index + 2).toInt();
    return (stat != 5);
}

void HttpClient_ESP8266_AT::createHttpClient(const String& host, uint32_t port) {
    if(m_httpClient != NULL) delete m_httpClient;
    m_httpClient = new HttpClient(*this, host, port); // HttpClient_ESP8266_AT (this) and HttpClient refers to each other
}

bool HttpClient_ESP8266_AT::get(const String& host, const String& path, uint32_t port) {
    createHttpClient(host, port);
    return (HTTP_SUCCESS == m_httpClient->get(path));
}

bool HttpClient_ESP8266_AT::post(const String& host, const String& path, const String& body,
                                 const String& contentType, uint32_t port) {
    createHttpClient(host, port);
    return (HTTP_SUCCESS == m_httpClient->post(path, contentType, body));
}

int HttpClient_ESP8266_AT::responseStatusCode() {
    return m_httpClient->responseStatusCode();
}

String HttpClient_ESP8266_AT::responseBody() {
    return m_httpClient->responseBody();
}

int HttpClient_ESP8266_AT::connect(IPAddress ip, uint16_t port) {
    return 0; // TODO
}

int HttpClient_ESP8266_AT::connect(const char *host, uint16_t port) {
    return 0; // TODO
}

size_t HttpClient_ESP8266_AT::write(uint8_t) {
    return 0; // TODO
}

size_t HttpClient_ESP8266_AT::write(const uint8_t *buf, size_t size) {
    return 0; // TODO
}

int HttpClient_ESP8266_AT::available() {
    return 0; // TODO
}

int HttpClient_ESP8266_AT::read() {
    return 0; // TODO
}

int HttpClient_ESP8266_AT::read(uint8_t *buf, size_t size) {
    return 0; // TODO
}

int HttpClient_ESP8266_AT::peek() {
    return 0; // TODO
}

void HttpClient_ESP8266_AT::flush() {
    // TODO
}

void HttpClient_ESP8266_AT::stop() {
    // TODO
}

uint8_t HttpClient_ESP8266_AT::connected() {
    return 0; // TODO
}

HttpClient_ESP8266_AT::operator bool() {
    // TODO
}

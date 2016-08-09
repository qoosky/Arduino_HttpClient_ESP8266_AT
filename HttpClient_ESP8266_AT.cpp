#include "HttpClient_ESP8266_AT.h"

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud) :
    m_rxPin(rxPin), m_txPin(txPin), m_responseStatusCode(0)
{
    SoftwareSerial *serial = new SoftwareSerial(rxPin, txPin);
    serial->begin(baud);
    m_serial = serial;
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(SoftwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial), m_responseStatusCode(0)
{
}

HttpClient_ESP8266_AT::HttpClient_ESP8266_AT(HardwareSerial &serial) :
    m_rxPin(0), m_txPin(0), m_serial(&serial), m_responseStatusCode(0)
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
    const unsigned long start = millis();
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
    m_serial->println(F("AT"));
    return checkATResponse();
}

bool HttpClient_ESP8266_AT::restart() {
    rxClear();
    m_serial->println(F("AT+RST"));
    if(!checkATResponse()) return false;
    delay(2000);
    const unsigned long start = millis();
    while(millis() - start < 3000) {
        if(statusAT()) {
            delay(1500);
            return true;
        }
        delay(100);
    }
    return false;
}

bool HttpClient_ESP8266_AT::connectAP(const String& ssid, const String& password) {
    rxClear();
    m_serial->println(F("AT+CWMODE_DEF=1")); // 1: station(client) mode, 2: softAP(server) mode, 3: 1&2
    if(!(checkATResponse() && restart())) return false; // change "DEF"ault cwMode and restart

    uint8_t retry = 5;
    do {
        // Connect to an AP
        rxClear();
        delay(500);
        m_serial->print(F("AT+CWJAP_DEF=\""));
        m_serial->print(ssid);
        m_serial->print(F("\",\""));
        m_serial->print(password);
        m_serial->println(F("\""));
        if(checkATResponse(F("OK"), 10000)) return true;
    } while(--retry);
    return false;
}

bool HttpClient_ESP8266_AT::disconnectAP() {
    rxClear();
    m_serial->println(F("AT+CWQAP"));
    return checkATResponse();
}

uint8_t HttpClient_ESP8266_AT::ipStatus() {
    String buf;
    rxClear();
    m_serial->println(F("AT+CIPSTATUS"));
    checkATResponse(&buf, F("S:"));
    uint32_t index = buf.indexOf(F(":"));
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
    m_serial->println(F("AT+CIPCLOSE"));
    return checkATResponse();
}

bool HttpClient_ESP8266_AT::connectTcp(const String& host, uint32_t port) {
    if(connectedTcp()) disconnectTcp();
    String buf;
    uint8_t retry = 10;
    do {
        rxClear();
        m_serial->print(F("AT+CIPSTART=\"TCP\",\""));
        m_serial->print(host);
        m_serial->print(F("\","));
        m_serial->println(port);
        checkATResponse(&buf);
        if(buf.indexOf(F("OK")) != -1 || buf.indexOf(F("ALREADY")) != -1) {
            return true;
        }
        delay(100);
    } while(retry--);
    return false;
}

int HttpClient_ESP8266_AT::responseStatusCode() {
    return m_responseStatusCode;
}

bool HttpClient_ESP8266_AT::get(const String& host, const String& path, uint32_t port) {
    return sendRequest(F("GET"), host, port, path);
}

bool HttpClient_ESP8266_AT::post(const String& host, const String& path, const String& body,
                                 const String& contentType, uint32_t port) {
    return sendRequest(F("POST"), host, port, path, contentType, body);
}

bool HttpClient_ESP8266_AT::sendRequest(const String& method,
                                        const String& host, uint32_t port, const String& path,
                                        const String& contentType, const String& body) {
    // Create TCP connection
    connectTcp(host, port);

    // HTTP Request parts
    const uint8_t nGetRequest = 3;
    String getRequest[] = {
        F("GET "),
        F(" HTTP/1.1\r\nHost: "),
        F("\r\nUser-Agent: Arduino ESP8266\r\nConnection: close\r\n\r\n"),
    };
    const uint8_t nPostRequest = 6;
    String postRequest[] = {
        F("POST "),
        F(" HTTP/1.1\r\nHost: "),
        F("\r\nContent-Type: "),
        "\r\nContent-Length: " + String(body.length()),
        F("\r\nUser-Agent: Arduino ESP8266\r\nConnection: close\r\n\r\n"),
        F("\r\n"),
    };
    uint32_t len = path.length() + host.length() + contentType.length() + body.length();
    if(method == F("GET")) for(uint8_t i = 0; i < nGetRequest; ++i) len += getRequest[i].length();
    else for(uint8_t i = 0; i < nPostRequest; ++i) len += postRequest[i].length();

    // prepare to send the request data
    uint8_t retry = 15;
    do {
        String buf;
        rxClear();
        m_serial->print(F("AT+CIPSEND="));
        m_serial->println(len);
        if(checkATResponse(&buf, F("> "))) break;
        if(!(--retry)) {
            m_responseStatusCode = -1; // timeout error (request data was NOT sent)
            return false;
        }
    } while(true);

    // send data
    uint32_t sentLen = 0;
    for(uint8_t i = 0; (method == F("GET") && i < nGetRequest) || (method == F("POST") && i < nPostRequest); ++i) {
        for(uint32_t j = 0; (method == F("GET") && j < getRequest[i].length()) || (method == F("POST") && j < postRequest[i].length()); ++j) {
            if(++sentLen % 64 == 0) delay(20); // Some Arduino's default serial buffer size is 64 bytes (Wait for it to be empty again.)
            m_serial->write(method == F("GET") ? getRequest[i][j] : postRequest[i][j]);
        }
        for(uint32_t j = 0; i == 0 && j < path.length(); ++j) {
            if(++sentLen % 64 == 0) delay(20);
            m_serial->write(path[j]);
        }
        for(uint32_t j = 0; i == 1 && j < host.length(); ++j) {
            if(++sentLen % 64 == 0) delay(20);
            m_serial->write(host[j]);
        }
        for(uint32_t j = 0; method == F("POST") && i == 2 && j < contentType.length(); ++j) {
            if(++sentLen % 64 == 0) delay(20);
            m_serial->write(contentType[j]);
        }
        for(uint32_t j = 0; method == F("POST") && i == 4 && j < body.length(); ++j) {
            if(++sentLen % 64 == 0) delay(20);
            m_serial->write(body[j]);
        }
    }

    // Start to buffer serial data fast!! to avoid serial buffer overflow.
    const unsigned long start = millis();
    String response = "";
    uint32_t lenLimit = 64;
    while (millis() - start < 1000) {
        if(m_serial->available() > 0) {
            response += (char)m_serial->read();
            if(--lenLimit == 0) break;
        }
    }

    // Parse response status code
    uint32_t index = response.indexOf(F(".1"));
    if(index == -1) m_responseStatusCode = 0; // response parse error (request data sent SUCCESSFULLY, but response data was corrupted)
    else m_responseStatusCode = response.substring(index + 3, index + 6).toInt();
    return true;
}

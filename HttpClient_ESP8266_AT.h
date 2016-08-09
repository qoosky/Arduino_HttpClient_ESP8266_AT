#ifndef HTTPCLIENT_ESP8266_AT_H_
#define HTTPCLIENT_ESP8266_AT_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class HttpClient_ESP8266_AT
{
 private:
    // SoftwareSerial rx and tx pins
    uint32_t m_rxPin;
    uint32_t m_txPin;

    // AT command serial interface for ESP8266
    Stream *m_serial; // base class of SoftwareSerial and HardwareSerial

 public:
    // Three types of the constructors:
    // - Create SoftwareSerial inside the constructor
    //   rxPin: Wire this to Tx Pin of ESP8266
    //   txPin: Wire this to Rx Pin of ESP8266
    HttpClient_ESP8266_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud = 115200);
    // - Construct from SoftwareSerial or HardwareSerial
    HttpClient_ESP8266_AT(SoftwareSerial &serial);
    HttpClient_ESP8266_AT(HardwareSerial &serial);

    // Destructor
    ~HttpClient_ESP8266_AT();

 public:
    // Health check of the serial interface
    bool statusAT();

    // Health check of the WiFi connection
    bool statusWiFi();

    // Connect/Disconnect ESP8266 to/from WiFi network
    bool connectAP(const String& ssid, const String& password);
    bool disconnectAP();

 private:
    // Clear rx buffer
    void rxClear();

    // Restart (Reset) ESP8266
    bool restart();

    // Check the response for the last AT command is "OK"
    bool checkATResponse(String target = "OK", uint32_t timeout = 1000);
    bool checkATResponse(String *buf, String target = "OK", uint32_t timeout = 1000); // store the data into buffer

    // Get IPSTATUS of ESP8266
    //   2: ESP8266 station connected to an AP and has obtained IP
    //   3: ESP8266 station created a TCP or UDP transmission
    //   4: the TCP or UDP transmission of ESP8266 station disconnected
    //   5: ESP8266 station did NOT connect to an AP
    uint8_t ipStatus();

 public:
    // HTTP GET and POST
    // - return true if successful, else error
    // - SSL/TLS is not supported
    bool get(const String& host, const String& path, uint32_t port = 80);
    bool post(const String& host, const String& path, const String& body,
              const String& contentType = "application/x-www-form-urlencoded", uint32_t port = 80);

    // HTTP response status code of the last request
    //   -1: timeout error (request data was NOT sent)
    //    0: response parse error (request data sent SUCCESSFULLY, but response data was corrupted)
    //   else (>0): HTTP status code (200, 404, 500, 302,.. etc.)
    int responseStatusCode();

 private:
    // Create or Destroy TCP connection
    bool connectTcp(const String& host, uint32_t port);
    bool disconnectTcp();
    bool connectedTcp(); // true if TCP connection exists

 private:
    // Common HTTP request interface for GET and POST
    bool sendRequest(const String& method,
                     const String& host, uint32_t port, const String& path,
                     const String& contentType = "", const String& body = "");
    int m_responseStatusCode;
};

#endif // #ifndef HTTPCLIENT_ESP8266_AT_H_

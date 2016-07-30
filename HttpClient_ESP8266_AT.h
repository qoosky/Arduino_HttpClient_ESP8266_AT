#ifndef HTTPCLIENT_ESP8266_AT_H_
#define HTTPCLIENT_ESP8266_AT_H_

#include "lib_HttpClient_ESP8266_AT.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Client.h>

class HttpClient_ESP8266_AT :
    public Client
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
    bool connect(String ssid, String password);
    bool disconnect();

 public:
    // HTTP GET and POST
    // - return true if successful, else error
    // - SSL/TLS is not supported
    bool get(const String& host, const String& path, uint32_t port = 80);
    bool post(const String& host, const String& path, const String& body,
              const String& contentType = "application/x-www-form-urlencoded", uint32_t port = 80);

    // HTTP response of the last request
    int responseStatusCode(); // MUST be called before responseBody()
    String responseBody(); // MUST be called after responseStatusCode()

 private:
    // (Re)create internal http client
    void createHttpClient(const String& host, uint32_t port);

 private:
    // Clear rx buffer
    void rxClear();

    // Restart (Reset) ESP8266
    bool restart();

    // Check the response for the last AT command is "OK"
    bool checkATResponse(String target = "OK", uint32_t timeout = 1000);
    bool checkATResponse(String *buf, String target = "OK", uint32_t timeout = 1000); // store the data into buffer

 private:
    // Arduino HTTP Client library
    // https://github.com/arduino-libraries/ArduinoHttpClient
    HttpClient *m_httpClient;

 private:
    // As a sub class of `Client`, these methods need to be implemented.
    // see. https://www.arduino.cc/en/Reference/ClientConstructor
    //      https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Client.h
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char *host, uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();
};

#endif // #ifndef HTTPCLIENT_ESP8266_AT_H_

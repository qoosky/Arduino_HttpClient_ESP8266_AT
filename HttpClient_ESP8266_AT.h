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
    Stream *m_serial; // super class of SoftwareSerial and HardwareSerial

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
    // Health check of m_serial
    bool statusAT();

    // connect ESP8266 to WiFi network
    bool connect(String ssid, String password);

 private:
    // Clear rx buffer
    void rxClear();
};

#endif // #ifndef HTTPCLIENT_ESP8266_AT_H_

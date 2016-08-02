# HttpClient for Arduino with ESP8266_AT

`Arduino_HttpClient_ESP8266_AT` is an Arduino HTTP client library to be used with [ESP-WROOM-02](https://espressif.com/en/products/hardware/esp-wroom-02/overview), which is sometimes referred to as [ESP8266](http://espressif.com/en/products/hardware/esp8266ex/overview), through the AT command interface.

# Target Arduino Boards

This library is made to be usable even with Arduino UNO.  It has very limited memory resource, works on different voltage level (5.0V) from ESP8266's 3.3V, and has only one hardware serial.  Which means that you can use any kind of Arduino boards with this library without modifying default serial buffer size.  It works fine with just 64 byte buffer size, using SoftwareSerial or HardwareSerial, whichever you like.

# Limitation

To be usable for Arduino UNO without serial communication's parity check, we abandoned to receive data from servers.  HTTP POST and even HTTP GET can only be used to SEND data.

# How to get started

Please see the following simple and yet sufficient examples.

- [ConnectWiFi.ino](https://github.com/qoosky/Arduino_HttpClient_ESP8266_AT/blob/master/examples/ConnectWiFi.ino)
- [HTTPGET.ino](https://github.com/qoosky/Arduino_HttpClient_ESP8266_AT/blob/master/examples/HTTPGET.ino)
- [HTTPPOST.ino](https://github.com/qoosky/Arduino_HttpClient_ESP8266_AT/blob/master/examples/HTTPPOST.ino)

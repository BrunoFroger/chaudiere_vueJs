//----------------------------------------------
//
//      wifiTools.hpp
//
//----------------------------------------------
#include <Arduino.h>


#ifndef __WIFITOOLS__
#define __WIFITOOLS__

    #define modeServerAsync 

    #ifdef lolin_s2_mini
        #include <ESP8266WiFi.h>
        #ifdef modeServerAsync
            #include <ESPAsyncTCP.h>
            #include "ESPAsyncWebServer.h"
        #elif
            #include <ESP8266WebServer.h>
        #endif
        //#define BOARD "lolin s2 mini ESP8266"
        #pragma message("lolin_s2_mini")
        extern AsyncWebServer server;
        //extern ESP8266WebServer server;
    #elif wemos_d1_mini32 
        #include <SPI.h>
        #include <WiFi.h>
        #include <WebServer.h>
        #define BOARD "Wemos D1 mini esp32"
        #pragma message("wemos_d1_mini32")
        extern WebServer server(80);
    #elif adafruit_feather_m0 
        #include <WiFi.h>
        #include <WebServer.h>
        #define BOARD "feather esp32"
        #pragma message("adafruit_feather_m0")
        extern WebServer server(80);
    #else 
        #pragma message("Unsupported board selection")
        #error Unsupported board selection.
    #endif

    extern void setWifiParameters(char *ssid, char *pwd, char *mode);
    extern bool initWifi(void);
    extern int getRSSI();
    extern bool isWifiConnected(void);
    //extern void handleWebRequete(void);

#endif
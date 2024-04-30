//----------------------------------------------
//
//      main.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "serverWeb.hpp"
#include "wifiTools.hpp"
#include "temperature.hpp"
#include <ESP8266mDNS.h>

#ifdef lolin_s2_mini
    #pragma message("lolin_s2_mini")
    #define TITRE "WeMos ESP8266"
#elif wemos_d1_mini32 
    #pragma message("wemos_d1_mini32")
    #define TITRE "Wemos D1 mini ESP 32"
#elif adafruit_feather_m0 
    #pragma message("adafruit_feather_m0")
    #define TITRE "Feather ESP32"
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif

int loopDelay = 2000;

long refreshNtpDelay = 0;

//=================================================
//
//      stop
//
//=================================================
void stop(void){
    Serial.print("une des initialisations est KO ...... ");
    Serial.println ("STOP");
    while(1) {
        delay(10000);
        Serial.print(".");
    } 
}

//=================================================
//
//      setup
//
//=================================================
void setup(void){
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
    delay(1000);
    Serial.println("Serial initialized");

    char titre[50];
    int lngDecalageTexteDebut = (31 - strlen(TITRE)) / 2;
    int lngDecalageTexteFin = 31 - lngDecalageTexteDebut - strlen(TITRE);
    sprintf(titre, "+%*s%s%*s+", lngDecalageTexteDebut, " ", TITRE, lngDecalageTexteFin, " ");
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println(titre);
    Serial.println("+       Capteur temperature     +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println("+        debut setup            +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");

    initTemperature();

    setWifiParameters((char *)"", (char *)"", (char *)"");
    initWifi();

    // demarrage du serveur Web
    Serial.println("serveur Web démarré");
    initServerWeb();

    server.begin();

    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println("+        fin setup              +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
}

//=================================================
//
//      loop
//
//=================================================
void loop() {
    refreshTemperature();
    MDNS.update();
    delay(loopDelay);
}
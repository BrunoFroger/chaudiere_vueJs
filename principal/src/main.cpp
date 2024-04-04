//----------------------------------------------
//
//      main.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "wifiTools.hpp"
#include "flash.hpp"
#include "sdCard.hpp"
#include "ntp.hpp"
#include "serverWeb.hpp"
#include "temperatures.hpp"
#include "pilotageChaudiere.hpp"
#include "calendrier.hpp"

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

int loopDelay = 1;

long refreshNtpDelay = 0;
long refreshTemperaturespDelay = 0;

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
    //Serial.begin(115200);
    Serial.begin(460800);
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
    Serial.println("+        Horloge chaudiere      +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println("+        debut setup            +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
    initFlash();
    sdcardInit();
    initCalendrier();
    setWifiParameters((char *)"", (char *)"", (char *)"");
    initWifi();
    if (isWifiConnected()){
      initNtp();
    }
    // demarrage du serveur Web
    Serial.println("serveur Web démarré");
    initServerWeb();

    server.begin();

    initTemperatures();

    Serial.println("allumage Led integrée");
    // LED_BUILTIN BUILTIN_LED
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);


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
    refreshNtpDelay -= loopDelay;
    if (refreshNtpDelay <= 0){
        refreshNtpDelay = 1000;  // refresh toutes les secondes
        refreshNtp();
    }
    refreshTemperaturespDelay -= loopDelay;
    if (refreshTemperaturespDelay <= 0){
        refreshTemperaturespDelay = 5000;  // refresh toutes les 5 secondes
        refreshTemperatures();
    }
    //handleWebRequete();
    refreshChaudiere();
    delay(loopDelay);
}

//----------------------------------------------
//
//      serverWeb.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "wifiTools.hpp"
#include "temperature.hpp"

//----------------------------------------------
//
//      initServerWeb
//
//----------------------------------------------
void initServerWeb(void){
    Serial.println("======================");
    Serial.println("|   Init Server Web  |");
    Serial.println("----------------------");
    Serial.println("initServerWeb => debut");

    //----------------------
    //
    // pages annexes
    //    
    //----------------------
    server.onNotFound([](AsyncWebServerRequest *request){ 
        Serial.println("requete /pageNotFound.html");
        request->send(404, "text/html", "Page Not Found ...."); });
        
    //----------------------
    //
    // requetes de recuperation de données
    //    
    //----------------------

    server.on("/getTemperature", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(200, "text/plain", String(typeIntExt) + "=" + String(getTemperature()));});

    server.on("/getHumidite", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(200, "text/plain", String(getHumidite()));});

    server.on("/getDatas", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String reponse = "";
        reponse += "Temperature = " + String (getTemperature());
        reponse += "\nHumidite    = " + String (getHumidite());
        request->send(200, "text/plain", reponse);});



    Serial.println("initServerWeb => fin");
}
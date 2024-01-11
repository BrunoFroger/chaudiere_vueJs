//----------------------------------------------
//
//      serverWeb.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include <SD.h>

#include "wifiTools.hpp"
#include "ntp.hpp"
#include "sdCard.hpp"
#include "flash.hpp"
#include "temperatures.hpp"
#include "pilotageChaudiere.hpp"
#include "calendrier.hpp"

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
    
    //Begin LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    Serial.println("LittleFS OK");

    delay(100);

    //----------------------
    //
    // pages annexes
    //    
    //----------------------
    server.onNotFound([](AsyncWebServerRequest *request){ 
        Serial.println("requete /pageNotFound.html");
        request->send(SDFS, "/chaudiere/html/pageNotFound.html", "text/html"); });
        //request->send(LittleFS, "/pageNotFound.html", "text/html"); });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /favicon.ico");
        request->send(SDFS, "/chaudiere/html/favicon.png", "image/png");});
        //request->send(LittleFS, "/favicon.png", "image/png");});

    server.on("/entete.html", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /entete.html");
        request->send(SDFS, "/chaudiere/html/entete.html", "text/html");});
        //request->send(LittleFS, "/entete.html", "text/html");});

    server.on("/piedPage.html", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /piedPage.html");
        request->send(SDFS, "/chaudiere/html/piedPage.html", "text/html");});
        //request->send(LittleFS, "/piedPage.html", "text/html");});

    //----------------------
    //
    // feuilles de style
    //    
    //----------------------
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /style.css");
        request->send(SDFS, "/chaudiere/html/style.css", "text/css");});
        //request->send(LittleFS, "/style.css", "text/css");});

    //----------------------
    //
    // fichiers de scripts JS
    //    
    //----------------------

    server.on("/w3data.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /w3data.js");
        request->send(SDFS, "/chaudiere/html/w3data.js", "text/javascript");});

    server.on("/vue.global.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /vue.global.js");
        request->send(SDFS,"/chaudiere/html/vue.global.js", "text/javascript");});

    server.on("/scripts.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /scripts.js");
        request->send(SDFS,"/chaudiere/html/scripts.js", "text/javascript");});

    //----------------------
    //
    // pages fonctionnelles
    //    
    //----------------------

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /");
        request->send(SDFS, "/chaudiere/html/index.html", "text/html"); });

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /index.html");
        request->send(SDFS, "/chaudiere/html/index.html", "text/html"); });

    //----------------------
    //
    // requetes de modification de données
    //    
    //----------------------

    server.on("/switchChauffageMode", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String parametre = request->getParam(0)->value();
        Serial.println("requete /switchChauffageMode => " + parametre);
        if (strcmp(parametre.c_str(), "FORCE") == 0){
            setChauffageMode(MODE_CHAUFFAGE_FORCE);
        } else if (strcmp(parametre.c_str(), "PROG") == 0){
            setChauffageMode(MODE_CHAUFFAGE_PROG);
        } else if (strcmp(parametre.c_str(), "MANUEL") == 0){
            setChauffageMode(MODE_CHAUFFAGE_MANU);
        } else {
            setChauffageMode(MODE_CHAUFFAGE_OFF);
        }
        refreshChaudiere();
        request->send(200, "text/plain", "");});

    server.on("/updateConsigne", HTTP_GET, [](AsyncWebServerRequest *request){ 
        String parametre = request->getParam(0)->value();
        Serial.println("requete /updateConsigne => " + parametre);
        setConsigne(parametre.toInt());
        request->send(200, "text/plain", "");});

    server.on("/updateIpTempInt", HTTP_GET, [](AsyncWebServerRequest *request){ 
        const char *parametre = request->getParam(0)->value().c_str() ;
        Serial.println("requete /updateIpTempInt => " + (String)parametre);
        setIPCapteurTemperatureInterieure((char *)parametre);
        request->send(200, "text/plain", "");});

    server.on("/updateIpTempExt", HTTP_GET, [](AsyncWebServerRequest *request){ 
        const char *parametre = request->getParam(0)->value().c_str() ;
        Serial.println("requete /updateIpTempExt => " + (String)parametre);
        setIPCapteurTemperatureExterieure((char *)parametre);
        request->send(200, "text/plain", "");});

    //----------------------
    //
    // requetes de recuperation de données
    //    
    //----------------------

    server.on("/environnements.json", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /environnements.json");
        String reponse = getEnvironnements();
        request->send(200, "text/json", reponse);});
        //request->send(SDFS, "/chaudiere/html/environnements.json", "text/json"); });

    server.on("/calendrier.json", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /calendrier.json");
        //String calendrier = getCalendrierJson();
        //Serial.printf("taille du calendrier : %d\n", calendrier.length());
        request->send(SDFS, "/chaudiere/html/calendrier.json", "text/json"); });
        //request->send(200, "text/json", calendrier); });

    server.on("/getConsigne", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getConsigne");
        request->send(200, "text/plain", String(getConsigne()));});

    server.on("/getDatasPiedPage", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getDatasPiedPage");
        String reponse = "{";
        reponse += "\"board\": \"" + String(BOARD) + "\",";
        reponse += "\"copyright\": \"" + String(copyright) + "\"";
        reponse += "}";
        request->send(200, "text/json", reponse);});

    server.on("/getDatasConfig", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getDatasConfig");
        String reponse = "{";
        reponse += "\"consigne\": \"" + String(getConsigne()) + "\",\n";
        reponse += "\"envSelectionne\": \"" + String(environnementReference) + "\",\n";
        reponse += "\"pinRelai\": \"" + String(pinoutRelai) + "\",\n";
        reponse += "\"heureEteHivers\": \"a completer\",\n";
        reponse += "\"chauffageOnOff\": \"a completer\",\n";
        reponse += "\"adresseIpTempInt\": \"a completer\",\n";
        reponse += "\"consigneJour\": \"" + String(consigneReferenceJour) + "\",\n";
        reponse += "\"consigneNuit\": \"" + String(consigneReferenceNuit) + "\",\n";
        reponse += "\"consigneAbsent\": \"" + String(consigneReferenceAbsent) + "\"\n";
        reponse += "}";
        request->send(200, "text/json", reponse);});

    server.on("/getDatasCommande", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getDatasCommande");
        String reponse = "{";
        reponse += "\"consigne\": \"" + String(getConsigne()) + "\",\n";
        reponse += "\"chauffageMode\": \"" + String(getChauffageModeString()) + "\",\n";
        reponse += "\"chauffageStatus\": \"a completer\",\n";
        reponse += "\"circulateurOnOff\": \"" + String(getCirculateurOnOff() ? "ON" : "OFF") + "\",\n";
        reponse += "\"commandeVanneChauffage\": \"" + String(getCommandeVanneChauffage()) + "\",\n";
        reponse += "\"temperatureInterieure\": \"" + String(getTemperatureInterieure()) + "\",\n";
        reponse += "\"temperatureExterieure\": \"" + String(getTemperatureExterieure()) + "\",\n";
        reponse += "\"afficheurOnOff\": \"a completer\"\n";
        reponse += "}";
        request->send(200, "text/json", reponse);});

    server.on("/getDatasTemperatures", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getDatasTemperatures");
        String reponse = "{";
        reponse += "\"consigne\": \"" + String(getConsigne()) + "\",\n";
        reponse += "\"ipTempInt\": \"" + String(getIPCapteurTemperatureInterieure()) + "\",\n";
        reponse += "\"ipTempExt\": \"" + String(getIPCapteurTemperatureExterieure()) + "\",\n";
        reponse += "\"temperatureInterieure\": \"" + String(getTemperatureInterieure()) + "\",\n";
        reponse += "\"temperatureExterieure\": \"" + String(getTemperatureExterieure()) + "\"\n";
        reponse += "}";
        request->send(200, "text/json", reponse);});

    server.on("/getBoard", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getBoard");
        request->send(200, "text/plain", String(BOARD));});

    server.on("/getCopyright", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getCopyright");
        request->send(200, "text/plain", String(copyright));});

    server.on("/getFormatedDate", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getFormatedDate");
        request->send(200, "text/plain", String("28/12/2023"));});

    server.on("/getFormatedTime", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getFormatedTime");
        request->send(200, "text/plain", String("11:25"));});

    server.on("/getTemperatureInterieure", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getTemperatureInterieure");
        request->send(200, "text/plain", String("TODO 20.0"));});

    server.on("/getTemperatureExterieure", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /getTemperatureExterieure");
        request->send(200, "text/plain", String("TODO 15.0"));});

    Serial.println("initServerWeb => fin");
}
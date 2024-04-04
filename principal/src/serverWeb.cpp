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

int delaySendDatas=100;

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
        //delay(delaySendDatas);
        request->send(SDFS, "/pageNotFound.html", "text/html"); });
        //request->send(LittleFS, "/pageNotFound.html", "text/html"); });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /favicon.ico");
        request->send(SDFS, "/favicon.png", "image/png");});
        //request->send(LittleFS, "/favicon.png", "image/png");});

    server.on("/entete.html", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /entete.html");
        //delay(delaySendDatas);
        request->send(SDFS, "/entete.html", "text/html");});
        //request->send(LittleFS, "/entete.html", "text/html");});

    server.on("/piedPage.html", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /piedPage.html");
        //delay(delaySendDatas);
        request->send(SDFS, "/piedPage.html", "text/html");});
        //request->send(LittleFS, "/piedPage.html", "text/html");});

    //----------------------
    //
    // feuilles de style
    //    
    //----------------------
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request){
        // Serial.println("requete /style.css");
        request->send(SDFS, "/style.css", "text/css");});
        // String datas = "";
        // datas = lireFichier("style.css");
        // //delay(delaySendDatas);
        // request->send(200, "text/css", datas);});
        // request->send(LittleFS, "/style.css", "text/css");});

    //----------------------
    //
    // fichiers de scripts JS
    //    
    //----------------------

    server.on("/w3data.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /w3data.js");
        request->send(SDFS, "/w3data.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("w3data.js");
        // request->send(200, "text/javascript", datas);});

    server.on("/vue.global.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /vue.global.js");
        request->send(SDFS,"/vue.global.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("vue.global.js");
        // request->send(200, "text/javascript", datas);});

    server.on("/scripts.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /scripts.js");
        request->send(SDFS,"/scripts.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("scripts.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /main.js");
        request->send(SDFS,"/main.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("main.js");
        // delay(delaySendDatas);
        // //Serial.println(datas);
        // request->send(200, "text/javascript", datas);});

    server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /bundle.js");
        request->send(SDFS,"/bundle.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("bundle.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/home.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /home.js");
        request->send(SDFS,"/home.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("home.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/commande.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /commande.js");
        request->send(SDFS,"/commande.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("commande.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/calendrier.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /calendrier.js");
        request->send(SDFS,"/calendrier.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("calendrier.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/temperature.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /temperature.js");
        request->send(SDFS,"/temperature.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("temperature.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    server.on("/configuration.js", HTTP_GET, [](AsyncWebServerRequest * request){
        Serial.println("requete /configuration.js");
        request->send(SDFS,"/configuration.js", "text/javascript");});
        // String datas = "";
        // datas = lireFichier("configuration.js");
        // delay(delaySendDatas);
        // request->send(200, "text/javascript", datas);});

    //----------------------
    //
    // pages fonctionnelles
    //    
    //----------------------

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /");
        request->send(SDFS, "/index.html", "text/html"); });
        // String datas = "";
        // datas = lireFichier("index.html");
        // delay(50);
        // request->send(200, "text/html", datas);});

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println("requete /index.html");
        request->send(SDFS, "/index.html", "text/html"); });
        // String datas = "";
        // datas = lireFichier("index.html");
        // delay(50);
        // request->send(200, "text/html", datas);});

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
        request->send(SDFS, "/calendrier.json", "text/json"); });
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
        reponse += "\"pinRelai\": \"" + String(getPinRelai()) + "\",\n";
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
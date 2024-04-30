//----------------------------------------------
//
//      wifiTools.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "sdCard.hpp"

#ifdef lolin_s2_mini
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
    #include "ESPAsyncWebServer.h"
    #include <ESP8266mDNS.h>
    //#include <ESP8266WebServer.h>
    //#define BOARD "lolin s2 mini ESP8266"
    #pragma message("lolin_s2_mini")
    AsyncWebServer server(80);
    //ESP8266WebServer server(80);
#elif wemos_d1_mini32 
    #include <SPI.h>
    #include <WiFi.h>
    #include <WebServer.h>
    #define BOARD "Wemos D1 mini esp32"
    #pragma message("wemos_d1_mini32")
    WebServer server(80);
#elif adafruit_feather_m0 
    #include <WiFi.h>
    #include <WebServer.h>
    #define BOARD "feather esp32"
    #pragma message("adafruit_feather_m0")
    WebServer server(80);
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif


char wifiSsid[50];
char wifiPwd[50];
char wifiMode[10];

int cptTryWifi = 0;
char ipLocale[50] = "";
char ipGateway[50];
char netMask[50];
String piedDePage = "";
String enteteDePage = "";
String webPageStyle = "";
bool wifiConnected = false;

//----------------------------------------------
//
//      isWifiConnected
//
//----------------------------------------------
bool isWifiConnected(void){
    return wifiConnected;
}

//----------------------------------------------
//
//      getWifiSsid
//
//----------------------------------------------
char *getWifiSsid(void){
    return wifiSsid;
}

//----------------------------------------------
//
//      getIpAddress
//
//----------------------------------------------
char *getIpAddress(){
    if (strlen(ipLocale) == 0) return (char *)"non connecte";
    return ipLocale;
}

//----------------------------------------------
//
//      isApMode
//
//----------------------------------------------
bool isApMode(){
    if (strcmp(wifiMode, "AP") == 0)
        return true;
    else
        return false;
}

//----------------------------------------------
//
//      getRSSI
//
//----------------------------------------------
int getRSSI(){
    return WiFi.RSSI();
}

//=========================================
//
//          deconnecteWifi
//
//=========================================
void deconnecteWifi(){
    //Serial.println("deconnecteWifi => debut");
    if (WiFi.status() == WL_CONNECTED){
        //Serial.println("deconnecteWifi => try");
        delay(100);
        WiFi.disconnect();
        //Serial.println("deconnecteWifi => OK");
    }
    //Serial.println("deconnecteWifi => fin");
    wifiConnected = false;
}

//----------------------------------------------
//
//      connectWifi
//
//----------------------------------------------
bool connectWifi(void){
    Serial.println("connectWifi => debut");
    //WiFi.onEvent(onWiFiEvent);
    if (strcmp(wifiMode, "AP") == 0){
        Serial.println("Initialisation du mode Acces Point");
        WiFi.mode(WIFI_AP);
    } else {
        Serial.println("Initialisation du mode Station");
        WiFi.mode(WIFI_STA);
    }

    WiFi.setHostname(localHostname);
    if (strcmp(wifiMode, "AP") == 0){
        delay(500);
        IPAddress apLocalIp(192,168,10,1);
        IPAddress apGateway(192,168,10,0);
        IPAddress apSubnetMask(255,255,255,0);
        WiFi.softAPConfig(apLocalIp, apGateway, apSubnetMask);
        WiFi.softAP(wifiSsid, wifiPwd);
        delay(1000);
        sprintf(ipGateway,"gateway = %d.%d.%d.%d",apGateway[0],apGateway[1],apGateway[2],apGateway[3]); Serial.println(ipGateway);
        sprintf(ipLocale,"LocalIp = %d.%d.%d.%d",apLocalIp[0],apLocalIp[1],apLocalIp[2],apLocalIp[3]); Serial.println(ipLocale);
        sprintf(netMask,"netmask = %d.%d.%d.%d",apSubnetMask[0],apSubnetMask[1],apSubnetMask[2],apSubnetMask[3]); Serial.println(netMask);
    } else { // mode STATION
        WiFi.disconnect();
        delay(100);
        //Serial.println("initWifi => check wifi status");
        if (WiFi.status() == WL_NO_SHIELD){
            Serial.println("initWifi => ERROR : No shield detected !!");
            Serial.print("error code = ");
            Serial.println(WiFi.status());
            wifiConnected = false;
            return false;
        }
        Serial.println("initWifi => a shield is detected");
        delay(1000);
        deconnecteWifi();
        delay(1000);
        //WiFi.setHostname(localHostname);
        cptTryWifi = 0;
        // Connect to WiFi network
        Serial.print("Connecting to ");
        Serial.println(wifiSsid);
        delay(100);
        WiFi.begin(wifiSsid, wifiPwd);
        int cpt=0;
        int cpt2=0;
        int connected = WiFi.status() != WL_CONNECTED;
        while (connected) {  //Attente de la connexion
            delay(500);
            //sprintf(buffer,"ssid = %s, pwd = %s", mesDonneesCapteurs.liveboxSsid, mesDonneesCapteurs.liveboxPwd); Serial.println(buffer);
            //Serial.print("wifiStatus = "); Serial.println(connected);
            //WiFi.begin(mesDonneesCapteurs.liveboxSsid, mesDonneesCapteurs.liveboxPwd);
            Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
            if (cpt++ >= 10){
                Serial.println();
                cpt=0;
            }
            if (cpt2++ > 20){
                break;
            }
            connected = WiFi.status() != WL_CONNECTED;
        }
        if (cpt2 > 20){
            // on a  fait 20 tentatives
            // imposible de se connecter au wifi !
            Serial.println("\nWifi non connecte");
            wifiConnected = false;
            // TODO setWifiMode(wifiSsid, SSID_UNAVAILABLE);
            return false;
        } else {
            // on a reussit a se connecter au wifi
            Serial.println("");
            Serial.println("WiFi connecte");
            // TODO setWifiMode(wifiSsid, SSID_AVAILABLE);
        }
        if (MDNS.begin(String(localHostname))) {  // on declare le nom au DNS du routeur
            Serial.println("mDNS responder started");
        } else {
            Serial.println("Error setting up MDNS responder!");
        }
        Serial.println("hostname = <" + String(localHostname) + ".local>");
        // Print the IP address
        Serial.print("ip locale = ");
        IPAddress tmpIp = WiFi.localIP();
        sprintf(ipLocale,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]); Serial.println(ipLocale);
        //IPAddress gatewayIp = WiFi.gatewayIP();
        tmpIp = WiFi.gatewayIP();
        sprintf(ipGateway,"gateway = %d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]); Serial.println(ipGateway);
        Serial.println((String)"RSSI = " + WiFi.RSSI() + " db");
        wifiConnected = true;
    }
    Serial.println("wifi connecte ok");

    
    Serial.println("connectWifi => fin");
    return true;
}

//----------------------------------------------
//
//      initWifi
//
//----------------------------------------------
bool initWifi(void){

    Serial.println("======================");
    Serial.println("|     Init Wifi      |");
    Serial.println("----------------------");
    Serial.println("initWifi => debut");
    //Serial.println("Connexion en mode : " + String(wifiMode));
    if (strcmp(wifiSsid, "") == 0){
        strcpy(wifiSsid, localWifiSsid);
        strcpy(wifiPwd, localWifiPwd);
        strcpy(wifiMode, "STATION");
    }
    //return connectWifi();
    while (connectWifi() == false){
        if (!getNewSsid()){
            return false;
        } 
        setWifiMode(wifiSsid,SSID_AVAILABLE);
    }
    return true;
}

//----------------------------------------------
//
//      setWifiParameters
//
//----------------------------------------------
void setWifiParameters(char *ssid, char *pwd, char *mode){
    char ligne[100];
    strcpy(wifiSsid, ssid);
    strcpy(wifiPwd, pwd);
    strcpy(wifiMode, mode);
    sprintf(ligne, "setWifiParameters : ssid = <%s> / pwd = <%s> / mode = <%s>\n", ssid, pwd, mode); Serial.print(ligne);
}

//----------------------------------------------
//
//      handleWebRequete
//
//----------------------------------------------
// void handleWebRequete(void){
//     // test si requete http reçue 
//     //Serial.print("debut handleWebRequete\n");
//     server.handleClient();
//     //Serial.print("fin handleWebRequete\n");
// }
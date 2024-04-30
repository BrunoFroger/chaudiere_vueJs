//----------------------------------------------
//
//      temperatures.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "ntp.hpp"
#include "wifiTools.hpp"
#include "temperatures.hpp"
//#include "pilotageChaudiere.hpp"

#ifdef lolin_s2_mini
    //#include <ESP8266WiFi.h>
    //#include <ESP8266WebServer.h>
    #include <ESP8266HTTPClient.h>
#elif wemos_d1_mini32 
    //#include <WiFi.h>
    //#include <WebServer.h>
    #include <HTTPClient.h>
#elif adafruit_feather_m0 
    //#include <WiFi.h>
    //#include <WebServer.h>
    #include <HTTPClient.h>
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif


// les temperatures sont exprimées en 1/10 de degres
int temperatureInterieure;
int temperatureExterieure;
int consigneTemperature = -1;
char localIPCapteurTemperatureInterieure[50] = "";
char localIPCapteurTemperatureExterieure[50] = "";

char baseUrl[50] = "/getTemperature";
int delayRefreshTemperatures = 5000;
int lastRefreshTemperatures= 0;
HTTPClient http;
WiFiClient client;
bool temperatureDepassee=false;

//----------------------------------------------
//
//      setTemperatureInterieure
//
//----------------------------------------------
void setTemperatureInterieure(int temp){
    // if (simulation){
    //     int valeurVanne = getCommandeVanneChauffage();
    //     temperatureInterieure -= 1;
    //     if (valeurVanne > 0){
    //         temperatureInterieure += valeurVanne / 30;
    //     }
    // } else {
        temperatureInterieure=temp;
    // }
    //Serial.print("setTemperatureInterieure => "); Serial.println(temperatureInterieure);
}

//----------------------------------------------
//
//      setTemperatureInterieure
//
//----------------------------------------------
void setTemperatureExterieure(int temp){
    temperatureExterieure=temp;
    //Serial.print("setTemperatureExterieure => "); Serial.println(temperatureExterieure);
}

//----------------------------------------------
//
//      temperatureAtteinte
//
//----------------------------------------------
bool temperatureAtteinte(void){
    char ligne[100];
    if (temperatureInterieure > (consigneTemperature + 15)){
        if (!temperatureDepassee){
            sprintf(ligne, "consigne = %d, tempInt = %d => temperature atteinte\n", consigneTemperature, temperatureInterieure); Serial.print(ligne);
            temperatureDepassee = true;
        }
        return true;
    }
    temperatureDepassee=false;
    return false;
}

//----------------------------------------------
//
//      refreshTemperatures
//
//----------------------------------------------
void refreshTemperatures(){
    int tmp;
    tmp = millis() - lastRefreshTemperatures;
    //Serial.println(tmp);
    if (tmp > delayRefreshTemperatures){
        //Serial.println("refresh temperature");
        lastRefreshTemperatures = millis();
        if (strcmp(getIPCapteurTemperatureInterieure(), "Non connecté") != 0){
            //Serial.println("Adresse IP temperature interieure = " + String(getIPCapteurTemperatureInterieure()));
            if (isWifiConnected()){
                //Serial.println("get temperature interieure depuis " + String(getIPCapteurTemperatureInterieure()));
                char ligne[300];
                char url[200];
                sprintf(url, "http://%s%s", localIPCapteurTemperatureInterieure, baseUrl);
                http.begin(client, url);
                int httpCode = http.GET();
                // httpCode will be negative on error
                if (httpCode >= 0) {
                    // HTTP header has been send and Server response header has been handled
                    sprintf(ligne, "[HTTP] GET %s => code:%d", url, httpCode); //Serial.println(ligne);
                    String payload, variable, valeur;
                    int idx;
                    payload = http.getString();
                    idx = payload.indexOf('=');
                    variable = payload.substring(0, idx);
                    valeur = payload.substring(idx+1);
                    if (variable.equals("temperatureInt")){
                        setTemperatureInterieure(valeur.toInt());
                    }
                    if (variable.equals("temperatureExt")){
                        setTemperatureExterieure(valeur.toInt());
                    }
                    // Serial.println("refresh " + variable + " : " + valeur);
                } else {
                    sprintf(ligne, "[HTTP] GET %s => failed:%s", url, http.errorToString(httpCode).c_str()); Serial.println(ligne);
                }
                http.end();
            } else {
                Serial.println("Impossible recuperer temperature interieure : wifi non connecte");
            }
        //} else {
            //Serial.println("Impossible recuperer temperature interieure : IP vide");
        }
        if (strcmp(getIPCapteurTemperatureExterieure(), "Non connecté") != 0){
            Serial.println("Adresse IP temperature exterieure = " + String(getIPCapteurTemperatureExterieure()));
            if (isWifiConnected()){
                Serial.println("get temperature exterieure depuis " + String(getIPCapteurTemperatureExterieure()));
                char ligne[300];
                char url[200];
                sprintf(url, "http://%s%s", localIPCapteurTemperatureExterieure, baseUrl);
                http.begin(client, url);
                int httpCode = http.GET();
                // httpCode will be negative on error
                if (httpCode >= 0) {
                    // HTTP header has been send and Server response header has been handled
                    sprintf(ligne, "[HTTP] GET %s => code:%d", url, httpCode); //Serial.println(ligne);
                    String payload, variable, valeur;
                    int idx;
                    payload = http.getString();
                    idx = payload.indexOf('=');
                    variable = payload.substring(0, idx);
                    valeur = payload.substring(idx+1);
                    if (variable.equals("temperatureInt")){
                        setTemperatureInterieure(valeur.toInt());
                    }
                    if (variable.equals("temperatureExt")){
                        setTemperatureExterieure(valeur.toInt());
                    }
                } else {
                    sprintf(ligne, "[HTTP] GET %s => failed:%s", url, http.errorToString(httpCode).c_str()); //Serial.println(ligne);
                }
                http.end();
            } else {
                Serial.println("Impossible recuperer temperature exterieure : wifi non connecte");
            }
        //} else {
            //Serial.println("Impossible recuperer temperature exterieure : IP vide");
        }
    }
}

//----------------------------------------------
//
//      getTemperatureSring
//
//----------------------------------------------
char tmp[20];
char *getTemperatureSring(int temperature){
    sprintf(tmp,"%d.%d", (int)(temperature / 10), temperature % 10 );
    return tmp;
}

//----------------------------------------------
//
//      updateTemperature
//
//----------------------------------------------
int updateTemperature(int *ptrTemperature, char *newValue){
    char ligne[100];
    sprintf(ligne, "update temperature debut => ancien = %d, nouveau = %s\n", *ptrTemperature, newValue);Serial.print(ligne);
    float tmp;
    tmp = atof(newValue);
    sprintf(ligne, "update temperature => tmp = %f\n", tmp);Serial.print(ligne);
    tmp *= 10;
    *ptrTemperature = (int)tmp;
    sprintf(ligne, "update temperature nouvelle valeur = %d\n", *ptrTemperature);Serial.print(ligne);
    return *ptrTemperature;
}

//----------------------------------------------
//
//      getTemperatureInterieure
//
//----------------------------------------------
int getTemperatureInterieure(void){
    return temperatureInterieure;
}

//----------------------------------------------
//
//      getTemperatureExterieure
//
//----------------------------------------------
int getTemperatureExterieure(void){
    return temperatureExterieure;
}

//----------------------------------------------
//
//      getConsigne
//
//----------------------------------------------
int getConsigne(void){
    return consigneTemperature;
}

//----------------------------------------------
//
//      getIPCapteurTemperatureInterieure
//
//----------------------------------------------
char *getIPCapteurTemperatureInterieure(void){
    if (strcmp(localIPCapteurTemperatureInterieure, "") == 0){
        return "Non connecté";
    }
    return localIPCapteurTemperatureInterieure;
}

//----------------------------------------------
//
//      setIPCapteurTemperatureInterieure
//
//----------------------------------------------
void setIPCapteurTemperatureInterieure(char *adresse){
    strcpy(localIPCapteurTemperatureInterieure, adresse);
}

//----------------------------------------------
//
//      setIPCapteurTemperatureExterieure
//
//----------------------------------------------
void setIPCapteurTemperatureExterieure(char *adresse){
    strcpy(localIPCapteurTemperatureExterieure, adresse);
}

//----------------------------------------------
//
//      getIPCapteurTemperatureExterieure
//
//----------------------------------------------
char *getIPCapteurTemperatureExterieure(void){
    if (strcmp(localIPCapteurTemperatureExterieure, "") == 0){
        return "Non connecté";
    }
    return localIPCapteurTemperatureExterieure;
}

//----------------------------------------------
//
//      setConsigne
//
//----------------------------------------------
void setConsigne(int consigne){
    consigneTemperature=consigne;
    Serial.print("mise a jour de la consigne : ");
    Serial.println(consigneTemperature);
}

//----------------------------------------------
//
//      initTemperatures
//
//----------------------------------------------
  void initTemperatures(void){
    Serial.println("=======================");
    Serial.println("     Init Temperatures ");
    Serial.println("-----------------------");
    if (consigneTemperature == -1){
        consigneTemperature = consigneReferenceJour;
    }
    strcpy(localIPCapteurTemperatureExterieure, ipTemperatureExterieure);
    strcpy(localIPCapteurTemperatureInterieure, ipTemperatureInterieure);
}
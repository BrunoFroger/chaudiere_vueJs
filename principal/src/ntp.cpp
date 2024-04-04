//----------------------------------------------
//
//      ntp.cpp
//
//----------------------------------------------

#include <Arduino.h>

#ifdef lolin_s2_mini
    #pragma message("lolin_s2_mini")
    #include <ESP8266WiFi.h>
#elif adafruit_feather_m0 
    #pragma message("adafruit_feather_m0")
    #include <WiFi.h>
#elif wemos_d1_mini32 
    #pragma message("wemos_d1_mini32")
    #include <WiFi.h>
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "wifiTools.hpp"

WiFiUDP ntpUDP;
/*
* Choix du serveur NTP pour récupérer l'heure, 3600 =1h est le fuseau horaire et 60000=60s est le * taux de rafraichissement
*/
const char *ntpServer = "fr.pool.ntp.org";
//NTPClient temps(ntpUDP, ntpServer, 3600, 60000); // heure d'hivers
NTPClient temps(ntpUDP, ntpServer, 7200, 60000); // heure d'été
char formatedTimeFull[30];
char formatedTime[30];
char formatedDate[30];
char fullDate[50];
int dayOfWeek, heure, minute, seconde, jour, mois, annee;
char dayString[15];
bool HeureEteHivers;
time_t epochTime;

//----------------------------------------------
//
//      getDayOfWeek
//
//----------------------------------------------
char *getDayString(int idx){
    switch(idx){
        case 0 : return ((char *)"dimanche"); break;
        case 1 : return ((char *)"lundi"); break;
        case 2 : return ((char *)"mardi"); break;
        case 3 : return ((char *)"mercredi"); break;
        case 4 : return ((char *)"jeudi"); break;
        case 5 : return ((char *)"vendredi"); break;
        case 6 : return ((char *)"samedi"); break;
    }
    return (char *)"inconnu";
}

//----------------------------------------------
//
//      getFrmatedTime
//
//----------------------------------------------
char *getFormatedTimeFull(void){
    //sprintf(formatedTime,"%s", temps.getFormattedTime().c_str() );
    return formatedTimeFull;
}

//----------------------------------------------
//
//      getFrmatedTime
//
//----------------------------------------------
char *getFormatedTime(void){
    //sprintf(formatedTime,"%s", temps.getFormattedTime().c_str() );
    return formatedTime;
}

//----------------------------------------------
//
//      getDayOfWeek
//
//----------------------------------------------
int getDayOfWeek(void){
    //refreshNtp();
    return temps.getDay();
}

//----------------------------------------------
//
//      getHour
//
//----------------------------------------------
int getHour(void){
    //refreshNtp();
    return temps.getHours();
}

//----------------------------------------------
//
//      getMinute
//
//----------------------------------------------
int getMinute(void){
    //refreshNtp();
    return temps.getMinutes();
}

//----------------------------------------------
//
//      getJour
//
//----------------------------------------------
int getJour(void){
    //refreshNtp();
    return jour;
}

//----------------------------------------------
//
//      getMois
//
//----------------------------------------------
int getMois(void){
    //refreshNtp();
    return mois;
}

//----------------------------------------------
//
//      getHeureEteHivers
//
//----------------------------------------------
bool getHeureEteHivers(void){
    return HeureEteHivers;
}

//----------------------------------------------
//
//      calcHeureEteHivers
//
//----------------------------------------------
void calcHeureEteHivers(void){
    if((mois >= 11) || (mois <= 4)){
        // heure d'hiver
        //Serial.println("Heure d'hiver");
        HeureEteHivers = false;
    } else {
        //Serial.println("Heure d'ete");
        HeureEteHivers = true;
    }
    if (!HeureEteHivers){
        // heure d'hiver
        temps.setTimeOffset(3600);
    } else {
        // heure d'été
        temps.setTimeOffset(0);
    }
}

//----------------------------------------------
//
//      getFormatedDate
//
//----------------------------------------------
char *getFormatedDate(void){
    //refreshNtp();
    return formatedDate;
}

//----------------------------------------------
//
//      getFullDate
//
//----------------------------------------------
char *getFullDate(void){
    //refreshNtp();
    return fullDate;
}

//----------------------------------------------
//
//      refreshNtp
//
//----------------------------------------------
void refreshNtp(void){
    calcHeureEteHivers();
    if ((heure == 0) && (minute == 0)) temps.update();
    if (temps.getDay() != dayOfWeek){
        dayOfWeek = temps.getDay();
        strcpy(dayString, getDayString(dayOfWeek));
    }
    heure = temps.getHours();
    minute = temps.getMinutes();
    seconde = temps.getSeconds();
    epochTime = temps.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    jour = ptm->tm_mday;
    mois = ptm->tm_mon+1;
    annee = ptm->tm_year + 1900;
    //Serial.print("refresh Ntp : annee = "); Serial.println(annee);
    sprintf(formatedTimeFull, temps.getFormattedTime().c_str());
    sprintf(formatedTime, "%02d:%02d", heure, minute);
    sprintf(formatedDate, "%02d/%02d/%d", jour, mois, annee);
    sprintf(fullDate, "%s %02d/%02d/%d", getDayString(getDayOfWeek()), jour, mois, annee);
    //Serial.print("refresh Ntp : formatedTime = "); Serial.println(formatedTime);
    //Serial.print("refresh Ntp : formatedDate = "); Serial.println(formatedDate);
    //Serial.print("refresh Ntp : fullDate = "); Serial.println(fullDate);
}

//----------------------------------------------
//
//      initNtp
//
//----------------------------------------------
void initNtp(){
    Serial.println("======================");
    Serial.println("     Init NTP         ");
    Serial.println("----------------------");
    temps.begin(); 
    refreshNtp();
    Serial.println("initialisation NTP => OK");
    delay(10);
    Serial.println(formatedTime);
}


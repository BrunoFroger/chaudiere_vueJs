//----------------------------------------------
//
//      pilotagePompe.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include "wifiTools.hpp"
//#include "calendrier.hpp"
#include "pilotageChaudiere.hpp"
#include "ntp.hpp"
//#include "afficheur.hpp"
//#include "regulation.hpp"
#include "temperatures.hpp"
#include "regulation.hpp"
#include "datas.hpp"

// pinoutRelai defini dans platformIO.ini
int pinRelai = pinoutRelai;
bool circulateurOnOff, lastCirculateurOnOff;
int modeChauffage;
int commandeVanneChauffage = 0;

//----------------------------------------------
//
//      setRelai
//
//----------------------------------------------
void setRelai(bool etat){
    if (etat){
        digitalWrite(pinRelai, LOW);
        Serial.println("set circulateur ON");
        setLedChaudiere(true);
    } else {
        digitalWrite(pinRelai, HIGH);
        Serial.println("set circulateur OFF");
        setLedChaudiere(false);
    }
}

//----------------------------------------------
//
//      setPinRelai
//
//----------------------------------------------
void setPinRelai(int pin){
    // TODO supprimer return => voir pourquoi bug si on utilise cette fonction
    return;
    Serial.print("nouvelle valeur de pin relai : "); Serial.println(pin);
    pinRelai = pin;
    pinMode(pinRelai, OUTPUT);
    setRelai(false);
    Serial.println("fin setPinRelai");
}

//----------------------------------------------
//
//      getCommandeVanneChauffage
//
//----------------------------------------------
int getCommandeVanneChauffage(){
    return commandeVanneChauffage;
}

//----------------------------------------------
//
//      setCommandeVanneChauffage
//
//----------------------------------------------
void setCommandeVanneChauffage(int valeur){
    if (valeur <= 0) valeur = 0;
    if (valeur >= 100) valeur = 100;
    commandeVanneChauffage = valeur;
}

//----------------------------------------------
//
//      getPinRelai
//
//----------------------------------------------
int getPinRelai(){
    return pinRelai;
}

//----------------------------------------------
//
//      getCirculateurOnOff
//
//----------------------------------------------
bool getCirculateurOnOff(){
    return circulateurOnOff;
}

//----------------------------------------------
//
//      setCirculateurOnOff
//
//----------------------------------------------
void setCirculateurOnOff(bool etat){
    circulateurOnOff = etat;
    if (circulateurOnOff != lastCirculateurOnOff){
        lastCirculateurOnOff = circulateurOnOff;
        if (circulateurOnOff){
            setRelai(true);
        } else {
            setRelai(false);
        }
    }
}

//----------------------------------------------
//
//      refreshChaudiere
//
//----------------------------------------------
void refreshChaudiere(void){
    switch(getChauffageMode()){
        case MODE_CHAUFFAGE_FORCE:
            setCirculateurOnOff(true);
            break;
        case MODE_CHAUFFAGE_PROG:
            // TODO 
            // if (getChauffageStatus()){
                regulation();
            //}
            break;
        case MODE_CHAUFFAGE_OFF:
            setCirculateurOnOff(false);
            break;
        case MODE_CHAUFFAGE_AUTO:
            regulation();
            break;
    }
}

//----------------------------------------------
//
//      setChauffageMode
//
//----------------------------------------------
void setChauffageMode(int mode){
    modeChauffage = mode;
}

//----------------------------------------------
//
//      getChauffageMode
//
//----------------------------------------------
int getChauffageMode(){
    return modeChauffage;
}

//----------------------------------------------
//
//      getChauffageModeString
//
//----------------------------------------------
char *getChauffageModeString(void){
    switch(modeChauffage){
        case MODE_CHAUFFAGE_OFF:
            return (char*)"OFF";
        case MODE_CHAUFFAGE_PROG:
            return (char*)"PROG";
        case MODE_CHAUFFAGE_FORCE:
            return (char*)"FORCE";
        case MODE_CHAUFFAGE_AUTO:
            return (char*)"AUTO";
    }
    return (char*)"";
}

//----------------------------------------------
//
//      initChaudiere
//
//----------------------------------------------
void initChaudiere(void){
    Serial.println("=====================================");
    Serial.println("     Init pilotage chaudière (relai) ");
    Serial.println("------------------------------------");
    commandeVanneChauffage=0;
    modeChauffage=MODE_CHAUFFAGE_OFF;
    setPinRelai(pinRelai);
}
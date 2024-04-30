//----------------------------------------------
//
//      datas.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <ArduinoJson.h>

#include "datas.hpp"

String buffer;
JsonDocument calendar;


//----------------------------------------------
//
//      initBuffer
//
//----------------------------------------------
void initBuffer(){
    Serial.println("datas => initBuffer debut");
    buffer.reserve(10000);

    Serial.println("datas => initBuffer init led chaudiere");
    pinMode(LED_CHAUDIERE, OUTPUT);
    Serial.println("datas => initBuffer fin");
}

//----------------------------------------------
//
//      setLedChaudiere
//
//----------------------------------------------
void setLedChaudiere(bool etat){
    if (etat){
        digitalWrite(LED_CHAUDIERE, HIGH);
    } else {
        digitalWrite(LED_CHAUDIERE, LOW);
    }
}

//----------------------------------------------
//
//      getPlage
//
//----------------------------------------------
void getPlage(plageHoraire *ptrPlage, int idJour, int idPlage){
    //Serial.println("datas.cpp : getPlage => debut ()" + String(idJour) + "," + String(idPlage) + ")");
    String strJour = "jour" + (String)idJour;
    String strPlage = "plage" + (String)idPlage;
    //Serial.println("datas.cpp : getPlage => debut (" + strJour + "," + strPlage + ")");
    //JsonObject tmpJour = calendar[strJour];
    //serializeJson(tmpJour, Serial);
    //Serial.println();
    JsonObject tmpPlage = calendar[strJour][strPlage];
    //serializeJson(tmpPlage, Serial);
    //Serial.println();
    //Serial.println("datas.cpp : getPlage => tmpPlage OK");

    ptrPlage->modele = 0;
    strcpy(ptrPlage->nomPlage, "");
    //ptrPlage->modele = tmpPlage["modele"];
    //strcpy(ptrPlage->nomPlage, tmpPlage["nomPlage"]);
    ptrPlage->heureDebut = tmpPlage["heureDebut"];
    //Serial.println("datas.cpp : getPlage => heureDebut = " + ptrPlage->heureDebut);
    ptrPlage->minuteDebut = tmpPlage["minuteDebut"];
    ptrPlage->heureFin = tmpPlage["heurefin"];
    ptrPlage->minuteFin = tmpPlage["minuteFin"];
    ptrPlage->chauffageOnOff = tmpPlage["chauffageOnOff"];
    ptrPlage->consigne = tmpPlage["consigne"];
    ptrPlage->plageActive = tmpPlage["plageActive"];
    //Serial.println("datas.cpp : getPlage => debut");
}

//----------------------------------------------
//
//      initBuffer
//
//----------------------------------------------
void initDatas(void){
    initBuffer();
}

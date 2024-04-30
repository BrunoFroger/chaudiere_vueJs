//----------------------------------------------
//
//      datas.hpp
//
//----------------------------------------------


#include <Arduino.h>
#include <ArduinoJson.h>

#include "calendrier.hpp"

#ifndef __DATAS__
#define __DATAS__

    #define LED_CHAUDIERE   D1

    extern String buffer;
    extern JsonDocument calendar;

    extern void initDatas(void);
    //extern void initBuffer();
    extern void setLedChaudiere(bool etat);
    void getPlage(plageHoraire *ptrPlage, int idJour, int idPlage);

#endif
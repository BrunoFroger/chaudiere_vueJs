//----------------------------------------------
//
//      pilotageChaudiere.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __PILOTAGE_CHAUDIERE__
#define __PILOTAGE_CHAUDIERE__

    #define MODE_CHAUFFAGE_OFF      0
    #define MODE_CHAUFFAGE_PROG     1
    #define MODE_CHAUFFAGE_FORCE    2
    #define MODE_CHAUFFAGE_MANU     3

    extern void initChaudiere(void);
    extern void refreshChaudiere(void);

    extern int getChauffageMode();
    extern void setChauffageMode(int mode);
    extern char *getChauffageModeString();

    extern int getCommandeVanneChauffage();
    extern void setCommandeVanneChauffage(int valeur);

    extern int getPinRelai(void);
    extern void setPinRelai(int pin);
    
    extern bool getCirculateurOnOff(void);
    extern void setCirculateurOnOff(bool etat);

#endif
//----------------------------------------------
//
//      regulation.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "temperatures.hpp"
#include "pilotageChaudiere.hpp"

bool regulationMode=false;
double deltaVanneChauffage=0;

//----------------------------------------------
//
//      regulation
//
//----------------------------------------------
void regulation(void){
    int diffTemp, vanneChauffage;
    //TODO
    // faire calcul d'activation ou desactivation du circulateur en fonction de la difference de temperature 
    // programmer la valeur de la vanne de cahffage en consequence.
    diffTemp = getConsigne() - getTemperatureInterieure();
    if(abs(diffTemp) > 5){
        // la temperature interieure est au dessous de la consigne
        // on calcul la variation de vanne chauffage
        setCirculateurOnOff(true);
        //Serial.printf("diffTemp = %d\n", diffTemp);
        deltaVanneChauffage += ((double)diffTemp / 100);
        //Serial.printf("deltaVanneChauffage = %lf\n", deltaVanneChauffage);
        if (abs(deltaVanneChauffage) > 1){
            vanneChauffage = getCommandeVanneChauffage() + deltaVanneChauffage;
            setCommandeVanneChauffage(vanneChauffage);
            // Serial.printf("+--------------------+\n");
            // Serial.printf("|     regulation     |\n");
            // Serial.printf("+-----------+--------+\n");
            // Serial.printf("| temp int  |  %4d  |\n", getTemperatureInterieure());
            // Serial.printf("| consigne  |  %4d  |\n", getConsigne());
            // Serial.printf("| circul    |  %4s  |\n", getCirculateurOnOff() ? "ON" : "OFF");
            // Serial.printf("| diff temp |  %4d  |\n", diffTemp);
            // Serial.printf("| delta vann| %0.3lf |\n", deltaVanneChauffage);
            // Serial.printf("| vann chau |  %4d  |\n", getCommandeVanneChauffage());
            // Serial.printf("+-----------+--------+\n");
            deltaVanneChauffage = 0;
        }
    }else{
        // la temperature interieure egale la consigne => arret
        setCirculateurOnOff(false);
    }
}
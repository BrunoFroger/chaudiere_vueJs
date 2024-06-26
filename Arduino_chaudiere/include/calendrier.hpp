//----------------------------------------------
//
//      calendrier.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __CALENDRIER__
#define __CALENDRIER__

    #define NB_JOURS            7
    #define NB_PLAGES_PAR_JOUR  4
    #define NB_PLAGES_MODELE    6
    #define NB_PLAGES_USER      5

    #define PLAGE_MODELE_NUIT       0
    #define PLAGE_MODELE_LEVER      1
    #define PLAGE_MODELE_COUCHER    2
    #define PLAGE_MODELE_ALL        3
    #define PLAGE_MODELE_JOUNEE     4
    #define PLAGE_MODELE_ABSENT     5

    struct plageHoraire{
        int8_t modele;
        char nomPlage[20];
        int8_t heureDebut;
        int8_t minuteDebut;
        int8_t heureFin;
        int8_t minuteFin;
        bool chauffageOnOff;
        int consigne;
        bool plageActive = false;
    };

    struct structCalendrier{
        plageHoraire plagesHoraires[NB_JOURS][NB_PLAGES_PAR_JOUR];
        //plageHoraire plagesModeles[NB_PLAGES_MODELE];
        //plageHoraire plagesUtilisateur[NB_PLAGES_USER];
    };

    //extern structCalendrier calendrier;
    
    extern void initCalendrier(void);
    extern void pageCalendrier(void);
    extern bool getChauffageStatus(void);
    extern structCalendrier *getCalendrier(void);
    extern String getCalendrierJson(void);
    extern int getSizeofCalendrier(void);
    extern String listeDonneesCalendrier(void);
    extern void listeDonneesCalendrier2(void);
    extern String chargeCalendrier();
    
#endif
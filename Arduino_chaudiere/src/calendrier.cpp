//----------------------------------------------
//
//      calendrier.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <stdlib.h>
#include <ArduinoJson.h>

#include "ntp.hpp"
#include "wifiTools.hpp"
#include "calendrier.hpp"
#include "temperatures.hpp"
#include "sdCard.hpp"
#include "flash.hpp"
#include "datas.hpp"

//structCalendrier calendrier;
//JsonDocument calendar;
bool chauffageCalendrier = false;
int jourEnEdition, plageEnEdition;
char filenameCalendrier[100] = "/calendrier.json";
//char filenameConfig[100] = "/config.txt";
//String calendrierJson;

//----------------------------------------------
//
//      setPlage
//
//----------------------------------------------
void setPlage(int idJour, int idPlage, int8_t modele, char *nom, 
    int8_t hDeb, int8_t mDeb, int8_t hFin, int8_t mFin, 
    bool chauffage, int consigne, bool active){
    //char ligne[200];
    plageHoraire *ptrPlage;
    getPlage(ptrPlage, idJour, idPlage);
    ptrPlage->modele = modele;
    strcpy(ptrPlage->nomPlage, nom);
    ptrPlage->heureDebut = hDeb;
    ptrPlage->minuteDebut = mDeb;
    ptrPlage->heureFin = hFin;
    ptrPlage->minuteFin = mFin;
    ptrPlage->chauffageOnOff = chauffage;
    ptrPlage->consigne = consigne;
    ptrPlage->plageActive = active;
    //sprintf(ligne, "setPlage => modele=%d, nom=%s, hdeb=%d, mdeb=%d, hfin=%d, mfin=%d, chaufOnOff=%d, consigne=%d, active=%d\n",
    //        ptrPlage->modele, ptrPlage->nomPlage, ptrPlage->heureDebut, ptrPlage->minuteDebut, ptrPlage->heureFin, ptrPlage->minuteFin,
    //        ptrPlage->chauffageOnOff, ptrPlage->consigne, ptrPlage->plageActive); Serial.print(ligne);
}

// //----------------------------------------------
// //
// //      setPlageNuit
// //
// //----------------------------------------------
// void setPlageNuit(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_NUIT, (char *)"Nuit", 22, 30, 6, 30, true, consigneReferenceNuit, true);
//     //Serial.println("plage Nuit selectionnee 22:30 => 6:30");
// }

// //----------------------------------------------
// //
// //      setPlageLever
// //
// //----------------------------------------------
// void setPlageLever(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_LEVER, (char *)"Lever", 6, 30, 8, 30, true, consigneReferenceJour, true);
//     //Serial.println("plage Lever selectionnee 6:30 => 8:30");
// }

// //----------------------------------------------
// //
// //      setPlageCoucher
// //
// //----------------------------------------------
// void setPlageCoucher(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_COUCHER, (char *)"Coucher", 21, 0, 23, 0, true, consigneReferenceJour, true);
// }

// //----------------------------------------------
// //
// //      setPlageAll
// //
// //----------------------------------------------
// void setPlageAll(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_ALL, (char *)"Entiere", 0, 0, 23, 59, true, consigneReferenceJour, true);
// }

// //----------------------------------------------
// //
// //      setPlageJournee
// //
// //----------------------------------------------
// void setPlageJournee(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_JOUNEE, (char *)"Journee", 8, 0, 19, 59, true, consigneReferenceJour, true);
// }

// //----------------------------------------------
// //
// //      setPlageAbsent
// //
// //----------------------------------------------
// void setPlageAbsent(plageHoraire *ptrPlage){
//     setPlage(ptrPlage, PLAGE_MODELE_ABSENT, (char *)"Abscent", 0, 0, 23, 59, true, consigneReferenceAbsent, true);
// }



//----------------------------------------------
//
//      listePlageHoraire
//
//----------------------------------------------
void listePlageHoraire(plageHoraire *plage){
    char tmpBuffer[50];
    Serial.println("+----------------+-----------+");
    Serial.println("|   cle          |   valeur  |");
    Serial.println("+----------------+-----------+");
    sprintf(tmpBuffer, "|  modele        |   %6d  |", plage->modele); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  nom plage     |   %6s  |", plage->nomPlage); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  heure debut   |   %6d  |", plage->heureDebut); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  min debut     |   %6d  |", plage->minuteDebut); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  heure fin     |   %6d  |", plage->heureFin); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  min fin       |   %6d  |", plage->minuteFin); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  chauf ON/OFF  |   %6s  |", plage->chauffageOnOff? "ON" : "OFF"); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  consigne      |   %6d  |", plage->consigne); Serial.println(tmpBuffer);
    sprintf(tmpBuffer, "|  active        |   %6s  |", plage->plageActive? "ON" : "OFF"); Serial.println(tmpBuffer);
    Serial.println("+----------------+-----------+");
}

//----------------------------------------------
//
//      chargeCalendrier
//
//----------------------------------------------
String chargeCalendrier(){
    buffer = "";
    Serial.println("---------------------------------------");
    Serial.print("calendrier : chargeCalendrier => recuperation du calendrier sauvegarde sur carte SD : ");
    Serial.println(filenameCalendrier);
    buffer = lireFichier(filenameCalendrier);
    //Serial.println(buffer);
    //Serial.println("buffer.length() = " + buffer.length());
    //Serial.println("calendrier : chargeCalendrier => debut deserialisation de buffer vers calendar");
    DeserializationError erreur = deserializeJson(calendar, buffer);
    if (erreur) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(erreur.f_str());
        return buffer;
    }
    //Serial.println("calendrier : chargeCalendrier => fin deserialisation de buffer vers calendar");
    // serializeJson(calendar, Serial);
    // Serial.println("\nfin serialisation de calendar");
    // for (int idJour=0 ; idJour < NB_JOURS ; idJour ++){
    //     for (int idPlage=0 ; idPlage < NB_PLAGES_PAR_JOUR ; idPlage++){
    //         // Serial.print("remplissage jour ");
    //         // Serial.print(idJour);
    //         // Serial.print(", plage ");
    //         // Serial.println(idPlage);
    //         String stringJour = "jour" + String(idJour);
    //         String stringPlage = "plage" + String(idPlage);
    //         JsonObject plage = calendar[stringJour][stringPlage];
    //         //Serial.println("debut serialisation plage " + stringJour + "/" + stringPlage);
    //         //serializeJson(plage, Serial);
    //         //Serial.println("\nfin serialisation plage");
    //         plageHoraire *ptrPlageHoraire = &(calendrier.plagesHoraires[idJour][idPlage]);
    //         //calendar[idJour][idPlage].printTo(buffer);
    //         ptrPlageHoraire->modele=plage["modele"].as<int8_t>();
    //         strcpy(ptrPlageHoraire->nomPlage,plage["nomPlage"].as<String>().c_str());
    //         ptrPlageHoraire->heureDebut=plage["heureDebut"].as<int8_t>();
    //         ptrPlageHoraire->heureFin=plage["heureFin"].as<int8_t>();
    //         ptrPlageHoraire->minuteDebut=plage["minuteDebut"].as<int8_t>();
    //         ptrPlageHoraire->minuteFin=plage["minuteFin"].as<int8_t>();
    //         ptrPlageHoraire->minuteFin=plage["minuteFin"].as<int8_t>();
    //         ptrPlageHoraire->chauffageOnOff=plage["minuteFin"].as<bool>();
    //         ptrPlageHoraire->consigne=plage["onsigne"].as<int8_t>();
    //         ptrPlageHoraire->plageActive=plage["plageActive"].as<bool>();
    //         //listePlageHoraire(ptrPlageHoraire);
    //         // TODO supprimer le return
    //         //return buffer;
    //     }
    // }

    plageHoraire tmpPlage;
    getPlage(&tmpPlage, 0, 1);
    listePlageHoraire(&tmpPlage);
    listeDonneesCalendrier2();

    Serial.println("calendrier : chargeCalendrier => fin");
    return buffer;


    String ligne = "";
    int numLigne = 0;
    unsigned int idx = 0;
    int itemJour = -1, itemPlage = -1, itemModele = -1;
    char itemNomPlage[20] = "";
    int itemHeureDeb = -1, itemMinuteDeb = -1;
    int itemHeureFin = -1, itemMinuteFin = -1, itemConsigne = -1;
    bool itemChauffageOnOff = false, itemPlageActive = false;
    while(idx < buffer.length()){
        while (buffer[idx] != '\n'){
            ligne += buffer[idx];
            idx++;
        }
        idx++;
        //Serial.print(numLigne); Serial.print(" ligne = <"); Serial.print(ligne); Serial.println(">");
        plageHoraire plage;
        unsigned int k = 0;
        int numItem = 0;
        String item = "";
        //Serial.println(ligne.substring(0,4));
        if (ligne.substring(0,4) != "jour"){
            while (k < ligne.length()){
                while ((ligne[k] != ';') && (k < ligne.length())){
                    //Serial.print(ligne[k]);
                    item += ligne[k++];
                }
                k++;
                //Serial.print(numItem); Serial.print(" item = <"); Serial.print(item); Serial.print("> : ");
                switch(numItem){
                    case 0: // jour
                        itemJour = item.toInt();
                        //Serial.print("itemJour = "); Serial.println(itemJour);
                        break;
                    case 1: // num plage
                        itemPlage = item.toInt();
                        //Serial.print("itemPlage = "); Serial.println(itemPlage);
                        break;
                    case 2: // modele
                        itemModele = item.toInt();
                        //Serial.print("itemModele = "); Serial.println(itemModele);
                        break;
                    case 3: // nom
                        strcpy(itemNomPlage, item.c_str());
                        //Serial.print("itemNomPlage = "); Serial.println(itemNomPlage);
                        break;
                    case 4: // heure debut
                        itemHeureDeb = item.toInt();
                        //Serial.print("itemHeureDeb = "); Serial.println(itemHeureDeb);
                        break;
                    case 5: // minute debut
                        itemMinuteDeb = item.toInt();
                        //Serial.print("itemMinuteDeb = "); Serial.println(itemMinuteDeb);
                        break;
                    case 6: // heure fin
                        itemHeureFin = item.toInt();
                        //Serial.print("itemHeureFin = "); Serial.println(itemHeureFin);
                        break;
                    case 7: // minute fin
                        itemMinuteFin = item.toInt();
                        //Serial.print("itemMinuteFin = "); Serial.println(itemMinuteFin);
                        break;
                    case 8: // chauffage on off
                        if (item.toInt() == 0){
                            itemChauffageOnOff = false;
                        } else {
                            itemChauffageOnOff = true;
                        }
                        //Serial.print("itemChauffageOnOff = "); Serial.println(itemChauffageOnOff);
                        break;
                    case 9: // consigne
                        itemConsigne = item.toInt();
                        //Serial.print("itemConsigne = "); Serial.println(itemConsigne);
                        break;
                    case 10: // plage active
                        if (item.toInt() == 0){
                            itemPlageActive = false;
                        } else {
                            itemPlageActive = true;
                        }
                        //Serial.print("itemPlageActive = "); Serial.println(itemPlageActive);
                        break;
                    default:
                        Serial.print("numItem "); Serial.print(numItem); Serial.println(" inconnu");
                }
                item = "";
                numItem++;
            }
            setPlage(itemJour, itemPlage, 
                itemModele, itemNomPlage, itemHeureDeb, itemMinuteDeb, 
                itemHeureFin, itemMinuteFin, itemChauffageOnOff, 
                itemConsigne, itemPlageActive);

            //if (numLigne >= 5) break; // pour debug
        }
        ligne = "";
        numLigne++;
    }
    //listeDonneesCalendrier();
    sauvegardeFichier("/calendrier.init.json", getCalendrierJson());
    Serial.print("fin de la recuperation du calendrier sauvegarde sur carte SD\n");
    Serial.println("---------------------------------------");
}

//----------------------------------------------
//
//      getSizeofCalendrier
//
//----------------------------------------------
int getSizeofCalendrier(void){
    serializeJson(calendar, buffer);
    return buffer.length();
}

// //----------------------------------------------
// //
// //      getCalendrier
// //
// //----------------------------------------------
// structCalendrier *getCalendrier(void){
//     return &calendrier;
// }

// //----------------------------------------------
// //
// //      getCalendrierJson
// //
// //----------------------------------------------
// String getCalendrierJson(void){
//     Serial.println("---------------------------------------");
//     Serial.println("getCalendrierJson => debut");
//     String calendrierJson="";
//     calendrierJson += "{";
//     for (int jour = 0 ; jour < NB_JOURS ; jour++){
//         if (jour != 0) calendrierJson += ",";
//         calendrierJson += "\n\t\"jour" + String(jour) + "\": {";
//         for (int plage = 0 ; plage < NB_PLAGES_PAR_JOUR ; plage++){
//             plageHoraire *ptrPlage = &(calendrier.plagesHoraires[jour][plage]);
//             if (plage != 0) calendrierJson += ",";
//             calendrierJson += "\n\t\t\"plage" + String(plage) + "\": {";
//             calendrierJson += "\n\t\t\t\"modele\": \"" + String(ptrPlage->modele) + String("\"");
//             calendrierJson += ",\n\t\t\t\"nomPlage\": \"" + String(ptrPlage->nomPlage) + String("\"");
//             calendrierJson += ",\n\t\t\t\"heureDebut\": \"" + String(ptrPlage->heureDebut) + String("\"");
//             calendrierJson += ",\n\t\t\t\"minuteDebut\": \"" + String(ptrPlage->minuteDebut) + String("\"");
//             calendrierJson += ",\n\t\t\t\"heureFin\": \"" + String(ptrPlage->heureFin) + String("\"");
//             calendrierJson += ",\n\t\t\t\"minuteFin\": \"" + String(ptrPlage->minuteFin) + String("\"");
//             calendrierJson += ",\n\t\t\t\"chauffageOnOff\": \"" + String(ptrPlage->chauffageOnOff) + String("\"");
//             calendrierJson += ",\n\t\t\t\"consigne\": \"" + String(ptrPlage->consigne) + String("\"");
//             calendrierJson += ",\n\t\t\t\"plageActive\": \"" + String(ptrPlage->plageActive) + String("\"");
//             calendrierJson += "\n\t\t}";
//         }
//         calendrierJson += "\n\t}";
//     }
//     calendrierJson += "\n}\n";
//     //Serial.print(calendrierJson);
//     Serial.println("getCalendrierJson => fin");
//     Serial.println("---------------------------------------");
//     return calendrierJson;
// }

//----------------------------------------------
//
//      getChauffageStatus
//
//----------------------------------------------
bool getChauffageStatus(void){
    int heureActuelle = getHour();
    int minuteActuelle = getMinute();
    int jourActuel = getDayOfWeek();
    chauffageCalendrier = false;
    int heureD, heureF, minuteD, minuteF;
    bool onOff[NB_PLAGES_PAR_JOUR], actif;


    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        plageHoraire *tmpPlage;
        getPlage(tmpPlage, jourActuel, i);
        heureD = tmpPlage->heureDebut;
        heureF = tmpPlage->heureFin;
        minuteD = tmpPlage->minuteDebut;
        minuteF = tmpPlage->minuteFin;
        onOff[i] = false;
        actif = tmpPlage->plageActive;

        if (actif){
            if ((heureActuelle >= heureD) && (minuteActuelle >= minuteD)){
                // on est au dela du début de la plage
                onOff[i] = true;
                if (heureActuelle > heureF + 1){
                    // fin de plage en debut d'heure
                    onOff[i] = false;
                } else if (heureF == heureActuelle){
                    if (minuteActuelle >= minuteF){
                        onOff[i] = false;
                    }
                }
            }
        }
    }

    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        chauffageCalendrier |= onOff[i]; 
    }
    return chauffageCalendrier;
}

// //----------------------------------------------
// //
// //      listeDonneesCalendrier
// //
// //----------------------------------------------
// String listeDonneesCalendrier(void){
//     char ligne[200];
//     Serial.println("listeDonneesCalendrier => debut");
//     String page ="";
//     page += "Données calendrier :\n";
//     //page += "|012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789|\n";
//     page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
//     page += "|           |            plage 0           |            plage 1           |            plage 2           |            plage 3           |\n";
//     page += "|           +------------------------------+------------------------------+------------------------------+------------------------------+\n";
//     page += "|           | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons |\n";
//     page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
//     for (int j = 0 ; j < NB_JOURS ; j++){
//         sprintf(ligne, "| %9s |", getDayString(j) );
//         page += ligne;
//         for (int p = 0 ; p < NB_PLAGES_PAR_JOUR ; p++){
//             if (calendrier.plagesHoraires[j][p].heureDebut != -1){
//                 sprintf(ligne, " %02d:%02d |", calendrier.plagesHoraires[j][p].heureDebut, calendrier.plagesHoraires[j][p].minuteDebut);
//                 page += ligne;
//                 sprintf(ligne, " %02d:%02d |", calendrier.plagesHoraires[j][p].heureFin, calendrier.plagesHoraires[j][p].minuteFin);
//                 page += ligne;
//                 if (calendrier.plagesHoraires[j][p].plageActive){
//                     page += "  OUI  |";
//                 } else {
//                     page += "  NON  |";
//                 }
//                 sprintf(ligne, "  %02.1f |", (float)calendrier.plagesHoraires[j][p].consigne / 10.0);
//                 page += ligne;
//             } else {
//                 page += "           inactive           |";
//             }

//         }
//         page += "\n";
//     page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
//     }
//     Serial.print(page);
//     Serial.println("listeDonneesCalendrier => fin");
//     return page;
// }

//----------------------------------------------
//
//      listeDonneesCalendrier2
//
//----------------------------------------------
void listeDonneesCalendrier2(void){
    char ligne[200];
    Serial.println("listeDonneesCalendrier2=> debut");
    //String page ="";
    //page += "Données calendrier :\n";
    //page += "|0         1         2         3         4         5         6         7         8         9         10        11        12        13        |\n";
    //page += "|01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789|\n";
    sprintf(ligne, "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n");Serial.print(ligne);
    sprintf(ligne, "|           |            plage 0           |            plage 1           |            plage 2           |            plage 3           |\n");Serial.print(ligne);
    sprintf(ligne, "|           +------------------------------+------------------------------+------------------------------+------------------------------+\n");Serial.print(ligne);
    sprintf(ligne, "|           | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons |\n");Serial.print(ligne);
    sprintf(ligne, "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n");Serial.print(ligne);
    for (int j = 0 ; j < NB_JOURS ; j++){
        sprintf(ligne, "| %9s |", getDayString(j) );Serial.print(ligne);
        for (int p = 0 ; p < NB_PLAGES_PAR_JOUR ; p++){
            plageHoraire tmpPlage;
            getPlage(&tmpPlage, j, p);
            //listePlageHoraire(&tmpPlage);
            if (tmpPlage.heureDebut != -1){
                sprintf(ligne, " %02d:%02d |", tmpPlage.heureDebut, tmpPlage.minuteDebut);Serial.print(ligne);
                sprintf(ligne, " %02d:%02d |", tmpPlage.heureFin, tmpPlage.minuteFin);Serial.print(ligne);
                if (tmpPlage.plageActive){
                    Serial.print("  OUI  |");
                } else {
                    Serial.print("  NON  |");
                }
                sprintf(ligne, " %04.1f |", (float)(tmpPlage.consigne/10));Serial.print(ligne);
            } else {
                Serial.print("           inactive           |");
            }
        }
        Serial.println();
    sprintf(ligne, "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+\n");Serial.print(ligne);
    }
    Serial.println("listeDonneesCalendrier2=> fin");
    return;
}


//----------------------------------------------
//
//      initCalendrier
//
//----------------------------------------------
void initCalendrier(void){
    Serial.println("======================");
    Serial.println("     Init Calendrier ");
    Serial.println("----------------------");
    Serial.println("initialisation des donnees calendrier");
    Serial.print ("la taille du calendrier est : ");
    Serial.println(getSizeofCalendrier());

    // // plage modele NUIT
    // setPlageNuit(&calendrier.plagesModeles[PLAGE_MODELE_NUIT]);
    // // plage modele LEVER
    // setPlageLever(&calendrier.plagesModeles[PLAGE_MODELE_LEVER]);
    // // plage modele COUCHER
    // setPlageCoucher(&calendrier.plagesModeles[PLAGE_MODELE_COUCHER]);
    // // plage modele ALL
    // setPlageAll(&calendrier.plagesModeles[PLAGE_MODELE_ALL]);
    // // plage modele JOURNEE
    // setPlageJournee(&calendrier.plagesModeles[PLAGE_MODELE_JOUNEE]);
    // //Serial.print(listeDonneesCalendrier());
    // // plage modele ABSCENT
    // setPlageAbsent(&calendrier.plagesModeles[PLAGE_MODELE_ABSENT]);
    // //Serial.print(listeDonneesCalendrier());

    // for (int i = 0 ; i < NB_PLAGES_USER ; i++){
    //     char nomPlage[20];
    //     sprintf(nomPlage, "user_%d", i);
    //     setPlage(&calendrier.plagesUtilisateur[i],-1,nomPlage,-1,-1,-1,-1,0,-1,0);
    // }

    if (1){
        chargeCalendrier();
    // } else {
    //     for (int j = 0 ; j < NB_JOURS ; j++){
    //         for (int i = 0 ; i < NB_PLAGES_PAR_JOUR - 1; i++){
    //             calendrier.plagesHoraires[j][i].modele = -1;
    //             calendrier.plagesHoraires[j][i].heureDebut = -1;
    //             calendrier.plagesHoraires[j][i].minuteDebut = -1;
    //             calendrier.plagesHoraires[j][i].heureFin = -1;
    //             calendrier.plagesHoraires[j][i].minuteFin = -1;
    //             calendrier.plagesHoraires[j][i].chauffageOnOff = false;
    //             calendrier.plagesHoraires[j][i].consigne = -1;
    //             calendrier.plagesHoraires[j][i].plageActive = false;
    //         }
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].modele = 0;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].heureDebut = 0;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].minuteDebut = 0;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].heureFin = 23;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].minuteFin = 59;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].chauffageOnOff = false;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].consigne = consigneReferenceJour;
    //         calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].plageActive = true;
    //     }
    }
    Serial.println("initCalendrier => Fin ");
    Serial.println("---------------------------------------");
}
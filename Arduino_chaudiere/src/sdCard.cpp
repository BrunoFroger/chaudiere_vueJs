//----------------------------------------------
//
//      sdCard.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

//#include "temperatures.hpp"
#include "wifiTools.hpp"
#include "pilotageChaudiere.hpp"
#include "sdCard.hpp"
//#include "ntp.hpp"
#include "datas.hpp"

// change this to match your SD shield or module;
// WeMos Micro SD Shield V1.0.0: D8
// LOLIN Micro SD Shield V1.2.0: D4 (Default)
// Feather ESP32 HUZZAH : ?? 
#ifdef lolin_s2_mini
    const int chipSelect = D4;
#elif wemos_d1_mini32 
    const int chipSelect = D8;
#elif adafruit_feather_m0 
    const int chipSelect = 5;
#else 
    #error Unsupported board selection.
#endif

File myFile;
String environnement;
bool insideEnvironnement;
int pin_relai;
int SdChauffageOnOff;
int SdConsigne;
boolean SDCardInitOK=false;

#define NB_ENVIRONNEMENTS   10

struct structEnvironnement{
    int index;
    char wifiMode[10];   // AP : Accespoint ; STATION : connection a un reseau Wifi
    char nom[20];
    char wifiSsid[20];
    char wifiPwd[25];
    char ipTempInt[17];
    char ipTempExt[17];
    int available = SSID_NOTCHECKED;
};

structEnvironnement listeEnvironnement[NB_ENVIRONNEMENTS];
//structEnvironnement *newEnvironnement;
int indexEnvironnementCourant;

String tmpValeur;

//----------------------------------------------
//
//      getValeur
//
//----------------------------------------------
String getValeur(String ligne){
    tmpValeur = "";
    //Serial.print("environnement : ");
    //Serial.println(indexEnvironnementCourant);
    int i = ligne.indexOf('=') + 1; // i = debut chaine apres le =
    while (tmpValeur[i++] == ' '); // on supprime les blancs apres le =
    tmpValeur = ligne.substring(i); // i correspond au debut de la chaine definissant la valeur
    //Serial.println("getValeur : <" + tmpValeur + ">");
    return tmpValeur;
}

//----------------------------------------------
//
//      listeEnvironnements
//
//----------------------------------------------
void listeEnvironnements(void){
    char ligne[100];
    Serial.print("+-----+-----------+-----------+----------------------+----------------------+-------------------+\n");
    Serial.print("| idx |   nom     | Wifi Mode |      ssid            |     pwd              |   ip capt temp    |\n");
    Serial.print("+-----+-----------+-----------+----------------------+----------------------+-------------------+\n");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") != 0){
            structEnvironnement *env = &listeEnvironnement[i];
            sprintf(ligne, "|  %d  |%10s |%10s | %20s | %20s | %17s |\n", i, env->nom, env->wifiMode, env->wifiSsid, env->wifiPwd, env->ipTempInt);
            Serial.print(ligne);
        }
    }
    Serial.print("+-----+-----------+-----------+----------------------+----------------------+-------------------+\n");
}

//----------------------------------------------
//
//      getEnvironnements
//
//----------------------------------------------
String getEnvironnements(void){
    String environnements ="[\n";
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        String environnement="";
        if (strcmp(listeEnvironnement[i].nom, "") != 0){
            //Serial.printf("i = %d\n", i);
            if (i > 0) {
                Serial.println("ajout de la virgule");
                environnement += ",";
                environnement += "\n";
            }
            structEnvironnement *env = &listeEnvironnement[i];
            //Serial.println("traitement de l'environnment : " +  String(env->nom));
            environnement += "\t{";
            environnement += "\n\t\t\"nom\": \"" + String(env->nom) + "\"";
            environnement += ",\n\t\t\"ssid\": \"" + String(env->wifiSsid) + "\"";
            environnement += ",\n\t\t\"pwd\": \"" + String(env->wifiPwd) + "\"";
            environnement += ",\n\t\t\"ip temp interieure\": \"" + String(env->ipTempInt) + "\"";
            environnement += ",\n\t\t\"ip temp exterieure\": \"" + String(env->ipTempExt) + "\"";
            environnement += "\n\t}";
        } else break;
        environnements += environnement;
    }
    environnements += "\n]\n";
    //Serial.print(environnements);
    return environnements;
}

//----------------------------------------------
//
//      initEnvironnement
//
//----------------------------------------------
void initEnvironnement(String ligne){
    char nomEnvironnement[20];
    strcpy(nomEnvironnement,ligne.substring(1, ligne.indexOf(']')).c_str());
    //Serial.print("initEnvironnement : nom de l'environemment => <");
    //Serial.print(nomEnvironnement);
    //Serial.println(">");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") == 0){
            structEnvironnement *env = &listeEnvironnement[i];
            env->index = i;
            strcpy(env->nom, nomEnvironnement);
            if (strcmp(nomEnvironnement, "AP") == 0){
                strcpy(env->wifiMode, nomEnvironnement);
            } else {
                strcpy(env->wifiMode, "STATION");
            }
            strcpy(env->wifiSsid, "");
            strcpy(env->wifiPwd, "");
            strcpy(env->ipTempInt, "");
            strcpy(env->ipTempExt, "");
            env->available = SSID_NOTCHECKED;
            indexEnvironnementCourant = i;
            break;
        }
    }
    insideEnvironnement = true;
    environnement = String(nomEnvironnement);
}
/*
//----------------------------------------------
//
//      setEnvironnement
//
//----------------------------------------------
void setEnvironnement(String envName){
    Serial.println("setEnvironnement : nom de l'environemment => <" + envName + ">");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        structEnvironnement *env = &listeEnvironnement[i];
        if (envName = env->nom){
            setIPCapteurTemperatureInterieure(env->ipTempInt);
            return;
        }
    }
    Serial.print("ERREUR : environnement ");
    Serial.print(envName);
    Serial.println(" inconnu");

}*/

//----------------------------------------------
//
//      analyseLigne
//
//----------------------------------------------
void analyseLigne(String ligne){
    String tmpData;
    //Serial.println("sdCard config => analyse ligne : " + ligne);
    //Serial.println(ligne);
    if (ligne.length() == 0){
        if (insideEnvironnement){
            //listeEnvironnements();
            insideEnvironnement = false;
            // TODO
            //setEnvironnement(environnement);
        }
        return;
    } 
    if (ligne[0] == '#') return;
    //Serial.println("----------------------------");
    tmpData = getValeur(ligne);
    if (ligne.startsWith(String("["))){
        if (!insideEnvironnement){
            // traitement de debut d'environnement
            //Serial.println("traitement de debut d'environnement");
            initEnvironnement(ligne);
        } else {
            Serial.print("ERREUR ......\n");
            Serial.print("Impossible de commencer un nouvel environnement\n");
            Serial.print("un environnement est deja ouvert\n");
        }
    } else if (ligne.startsWith(String("CONSIGNE"))){
        // traitement de la valeur de consigne
        //Serial.println("traitement de la consigne");
        SdConsigne = tmpData.toInt();
        // TODO
        //setConsigne(SdConsigne);
        //Serial.print("consigne = "); Serial.println(getConsigne());
    } else if (ligne.startsWith("ENV")){
        // fixe l'environnement a traiter
        //Serial.println("fixe l'environnement a traiter");
        environnement = tmpData;
        //Serial.print("Utilisation de l'environnement : <"); Serial.print(environnement); Serial.println(">");
        for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
            structEnvironnement *env = &listeEnvironnement[i];
            if (environnement == String(env->nom)){
                setWifiParameters(env->wifiSsid, env->wifiPwd, env->wifiMode);
                break;
            }
        }
    } else if (ligne.startsWith("WIFI_SSID")){
        // fixe le nom du ssid
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->wifiSsid, tmpData.c_str());
        //Serial.println("fixe nom du ssid");
        //Serial.print("ssid = <"); Serial.print(env->wifiSsid); Serial.println(">");
    } else if (ligne.startsWith("WIFI_PWD")){
        // fixe le nom du wifi Pwd
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->wifiPwd, tmpData.c_str());
        //Serial.println("fixe nom du wifi Pwd");
        //Serial.print("pwd = <"); Serial.print(env->wifiPwd); Serial.println(">");
    } else if (ligne.startsWith("IP_TEMP_INT")){
        // fixe l'adresse du capteur de temperature interieure
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->ipTempInt, tmpData.c_str());
        //Serial.println("fixe l'adresse du capteur de temperature");
        //Serial.print("ip temp int = <"); Serial.print(env->ipTempInt); Serial.println(">");
    } else if (ligne.startsWith("IP_TEMP_EXT")){
        // fixe l'adresse du capteur de temperature exterieure
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->ipTempExt, tmpData.c_str());
        //Serial.println("fixe l'adresse du capteur de temperature");
        //Serial.print("ip temp int = <"); Serial.print(env->ipTempInt); Serial.println(">");
    } else if (ligne.startsWith("CHAUFFAGE")){
        // fixe l'activation ou non du chauffage
        // TODO
        if (tmpData == "FORCE"){
            SdChauffageOnOff = MODE_CHAUFFAGE_FORCE;
        } else if (tmpData == "AUTO"){
            SdChauffageOnOff = MODE_CHAUFFAGE_AUTO;
        } else if (tmpData == "PROG"){
            SdChauffageOnOff = MODE_CHAUFFAGE_PROG;
        } else {
            SdChauffageOnOff = MODE_CHAUFFAGE_OFF;
        }
        // TODO
        //setChauffageMode(SdChauffageOnOff);
    } else if (ligne.startsWith("PIN_RELAI")){
        // fixe la broche sur laquelle est connectee le relai de pilotage
        //Serial.print("AnalyseLigne : pinRelai = "); Serial.println(tmpData.toInt());
        setPinRelai(tmpData.toInt());
    } else if (ligne.startsWith("WIFI_MODE")){
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        if ((tmpData == "STATION") || (tmpData == "AP") ){
            strcpy(env->wifiMode, tmpData.c_str());
        } else {
            Serial.println("ERREUR : mode wifi " + tmpData + " indefini (STATION ou AP)");
        }
    } else {
        Serial.print("ligne invalide : ");
        Serial.println(ligne);
    }
}

//----------------------------------------------
//
//      sdCardOpenFile
//
//----------------------------------------------
File sdCardOpenFile(char *filename){
    String ligne;
    return SD.open(filename);
}

//----------------------------------------------
//
//      readConfig
//
//----------------------------------------------
void readConfig(void){
    String ligne;
    String filename = "/config.txt";
    myFile = SD.open(filename);

    if (myFile){
        Serial.println("readConfig => Analyse du fichier de config sur carte SD");
        while (myFile.available()){
            ligne = "";
            //Serial.println("readConfig => analyse ligne : ");
            ligne = myFile.readStringUntil('\n');
            //Serial.println(ligne);
            analyseLigne(ligne);
        }
        //Serial.println("readConfig => fin analyse du fichier de config");
        myFile.close();
    } else {
        Serial.print("Le fichier ");
        Serial.print(filename);
        Serial.println(" n'existe pas");
    }
}

//----------------------------------------------
//
//      sauvegardeFichier
//
//----------------------------------------------
void sauvegardeFichier(String filename, String datas){
    if (SD.exists(filename)){
        SD.remove(filename);
    }
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile){
        //Serial.println("sdCard => sauvegarde de " + filename);
        myFile.print(datas);
        myFile.close();
        //Serial.print("sdCard => taille du fichier : ");
        //Serial.println(sizeof(datas));
    } else {
        Serial.print("impossible d'ouvrir le fichier ");
        Serial.print(filename);
        Serial.println();
    }
}

//----------------------------------------------
//
//      lireFichier
//
//----------------------------------------------
String lireFichier(String filename){
    buffer = "";
    char car;
    myFile = SD.open(filename);
    if (myFile){
        //Serial.print("SDCard : lireFichier => lecture de " + filename);
        while(myFile.available()){
            car = myFile.read();
            buffer += car;
        }
        Serial.print(" (");
        Serial.print(buffer.length());
        Serial.println(" octets)");
        myFile.close();
    } else {
        Serial.print("impossible d'ouvrir le fichier ");
        Serial.print(filename);
        Serial.println();
    }
    //delay(10);
    // Serial.println("SDCARD => datas : " + buffer.length());
    // Serial.println(buffer);
    //Serial.println("SDCard : lireFichier => fin ");
    return buffer;
}

//----------------------------------------------
//
//      writeConfig
//
//----------------------------------------------
void writeConfig(void){
    String page="";
    page += "###########################################\n";
    page += "#\n";
    page += "#           Chauffage (config)\n";
    page += "#\n";
    page += "###########################################\n";
    page += "\n";
    page += "# Ce fichier contient les parametres\n";
    page += "# de pilotage d'une chaudiere\n";
    page += "\n";
    page += "# les regles de syntaxe de ce fichier sont les suivantes :\n";
    page += "#	- les lignes qui commences par # sont des commentaires\n";
    page += "#	- le fichier doit se terminer par une ligne vide\n";
    page += "#	- le nom d'une variable doit etre en debut de ligne\n";
    page += "#	- une seule variable par ligne\n";
    page += "#	- les temperatures sont exprimees en 1/10eme de degres (200 pour une temperature de 20°)\n";
    page += "#  - les environnements doivent etre definis avant les parametres generaux\n";
    page += "\n";
    page += "# Les paramètres disponibles sont listés ci dessous \n";
    page += "# avec la valeur par defaut s'ils ne sont pas renseignés\n";
    page += "\n";
    page += "# Parametres generaux :\n";
    page += "# CONSIGNE = 200 : consigne de temperature a maintenir 20,0° (exprime en 1/10 de degre)\n";
    page += "# CHAUFFAGE = OFF : determine si le chauffage est active ou non lors du demarrage du systeme\n";
    page += "# ENV = maison : definition de l'environnement a utiliser\n";
    page += "# PIN_RELAI = pinNumber : definition de la broche de l'arduino sur laquelle est connectee le relai\n";
    page += "\n";
    page += "# Parametres d'environnement :\n";
    page += "# [NOM_ENV] => liste de definitions de données specifiques a un environnement jusqu'a une ligne vide\n";
    page += "# WIFI_SSID = ssid : nom du point d'accces wifi\n";
    page += "# WIFI_PWD = passwd : passwd du point d'acces wifi\n";
    page += "# IP_TEMP_INT : adresse IP du capteur de temperature interieure (reference pour la regulation)\n";
    page += "# IP_TEMP_EXT : adresse IP du capteur de temperature exterieure \n";
    page += "# PIN_RELAI = pinNumber : definition de la broche de l'arduino sur laquelle est connectee le relai (26 correspond a D0 sur wemos esp 32, 16 = D0 sur wemos D1 mini)\n";
    page += "# REGULATION = OFF : prise en compte ou non de la temperature pour piloter la chaudiere (si OFF seul le calendrier pilote la chaudiere)\n";
    page += "\n";
    
    for (int i = 0 ; i < 10 ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") != 0){
            page += "[";
            page += listeEnvironnement[i].nom ;
            page += "]\n";
            page += "WIFI_SSID = ";
            page += listeEnvironnement[i].wifiSsid;
            page += "\n";
            page += "WIFI_PWD = ";
            page += listeEnvironnement[i].wifiPwd;
            page += "\n";
            page += "IP_TEMP_INT = ";
            page += listeEnvironnement[i].ipTempInt;
            page += "\n";
            page += "IP_TEMP_EXT = ";
            page += listeEnvironnement[i].ipTempExt;
            page += "\n";
            page += "\n";
        } else {
            break;
        }
    }

    page += "CONSIGNE = ";
    // TODO
    //page += getConsigne();
    page += "\n" ;

    if (SdChauffageOnOff){
        page += "CHAUFFAGE = ON\n";
    } else {
        page += "CHAUFFAGE = OFF\n";
    }
    page += "ENV = " + String(environnement) + "\n";
    page += "PIN_RELAI = ";
    page += getPinRelai();
    page += "\n";
    Serial.print(page);


    String filename = "/config.txt";
    if (SD.exists(filename)){
        SD.remove(filename);
    }
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile){
        myFile.print(page);
        myFile.close();
    }

}

//----------------------------------------------
//
//      getNewSsid
//
//----------------------------------------------
int lastSsidChecked=0;
bool getNewSsid(void){
    for (int i = lastSsidChecked ; i < NB_ENVIRONNEMENTS ; i++){
        structEnvironnement *env = &listeEnvironnement[i];
        if (env->available == SSID_NOTCHECKED){
            setWifiParameters(env->wifiSsid, env->wifiPwd, env->wifiMode);
            return true;
        }
    }
    return false;
}

//----------------------------------------------
//
//      setWifiMode
//
//----------------------------------------------
void setWifiMode(char *ssid,int available){
    for (int i = 0 ;i < NB_ENVIRONNEMENTS ; i++){
        structEnvironnement *env = &listeEnvironnement[i];
        if (strcmp(env->wifiSsid,ssid) == 0){
            env->available = available;
            return;
        }
    }
}

//----------------------------------------------
//
//      sdcardInit
//
//----------------------------------------------
bool sdcardInit(void){

    Serial.print("Initializing SD card  ...   ");
    insideEnvironnement=false;
    for (int i = 0 ; i < 10 ; i++){
        strcpy(listeEnvironnement[i].nom, "");
    }
    //listeEnvironnements();

#ifdef lolin_s2_mini
    if (!SD.begin(chipSelect)) {
# else 
    if (!SD.begin()) {
# endif
        Serial.println("initialization failed!");
        SDCardInitOK=false;
        return false;
    }
    Serial.println("initialization done.");
    readConfig();
    Serial.print("consigne        = ");
    // TODO
        //Serial.println(getConsigne());
    Serial.print("chauffage       = ");
    // TODO
        //Serial.println(getChauffageModeString());
    Serial.print("environnement   = ");
    Serial.println(environnement);
    Serial.print("pin relai       = ");
    Serial.println(getPinRelai());
    listeEnvironnements();
    SDCardInitOK=true;
    return true;
}
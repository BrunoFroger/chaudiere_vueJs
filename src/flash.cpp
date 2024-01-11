//----------------------------------------------
//
//      flash.cpp
//
//----------------------------------------------

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

String homeDir = "/";

//----------------------------------------------
//
//      initFlash
//
//----------------------------------------------
void initFlash(void){
    // Afficher la taille de la mémoire flash
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println("+        init  flash            +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
    Serial.print("Taille de la mémoire flash : ");
    Serial.print(ESP.getFlashChipSize());
    Serial.println(" octets");

    // Begin LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    FSInfo fs_info;
    char tmpFsInfo[40];
    LittleFS.info(fs_info);
    Serial.println("LittleFS OK");
    Serial.println("+-------------------------------+");
    Serial.println("|     info LittleFS             |");
    Serial.println("+-------------------------------+");
    sprintf(tmpFsInfo, "| totalBytes     |   %10d |", fs_info.totalBytes);Serial.println(tmpFsInfo);
    sprintf(tmpFsInfo, "| usedBytes      |   %10d |", fs_info.usedBytes);Serial.println(tmpFsInfo);
    sprintf(tmpFsInfo, "| blockSize      |   %10d |", fs_info.blockSize);Serial.println(tmpFsInfo);
    sprintf(tmpFsInfo, "| pageSize       |   %10d |", fs_info.pageSize);Serial.println(tmpFsInfo);
    sprintf(tmpFsInfo, "| maxOpenFiles   |   %10d |", fs_info.maxOpenFiles);Serial.println(tmpFsInfo);
    sprintf(tmpFsInfo, "| maxPathLength  |   %10d |", fs_info.maxPathLength);Serial.println(tmpFsInfo);
    Serial.println("+-------------------------------+");

    Serial.println(F("Liste des fichiers en flash :"));
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
        Serial.println(dir.fileName());
    }
    Serial.println("+-------------------------------+");
    Serial.println("+        Fin de init flash      +");
    Serial.println("+-------------------------------+");
}

//----------------------------------------------
//
//      flashLireFichier
//
//----------------------------------------------
String flashLireFichier(String filename){
    String contenu = "";
    filename = homeDir + filename;
    File file = LittleFS.open(filename, "r");
    Serial.println("lecture du fichier " + filename);
    if (!file || file.isDirectory()){
        Serial.print("ERREUR : Impossible de lire le fichier ");
        Serial.print(filename);
        Serial.println(" dans la memoire flash");
    } else {
        while (file.available()){
            contenu += file.readString();
        }
    }
    Serial.printf("lecture Fichier flash :\n%s\n", contenu.c_str());
    return contenu;
}
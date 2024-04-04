//----------------------------------------------
//
//      temperature.cpp
//
//----------------------------------------------

#include <Arduino.h>
#include <DHT.h>

DHT dht(dhtPin, dhtType);
float temperature;
float humidite;

#define potentiometrePin   A0
bool potentiometre = true;

//----------------------------------------------
//
//      getTemperature
//
//----------------------------------------------
int getTemperature(void){
    Serial.println("getTemperature => " + String(temperature));
    return temperature;
}

//----------------------------------------------
//
//      getHumidite
//
//----------------------------------------------
int getHumidite(void){
    Serial.println("getHumidite => " + String(humidite));
    return humidite;
}

//----------------------------------------------
//
//      refreshTemperature
//
//----------------------------------------------
void refreshTemperature(void){
    if (potentiometre){
        int tmp = analogRead(potentiometrePin);
        temperature = map(tmp, 0, 1024, 120, 250);
    } else {
        temperature = dht.readTemperature();
        humidite = dht.readHumidity();
    }
    Serial.print("refreshTemerature => temp = ");
    Serial.print(temperature);
    Serial.print(", hum = ");
    Serial.println(humidite);
}

//----------------------------------------------
//
//      initTemperature
//
//----------------------------------------------
void initTemperature(void){
    Serial.println("initTemperature => debut");
    if (potentiometre){
        pinMode(potentiometrePin, INPUT);
    } else {
        dht.begin();
    }
    delay(2000);
    refreshTemperature();
    Serial.println("initTemperature => fin");
}

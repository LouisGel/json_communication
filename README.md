# json_communication
Permet de communiquer en Json via le port Serial entre un Arduino et un PC
Code écrit lors du projet final en Génie Informatique S2


## Exemple du code sur Arduino
```cpp
#include <Arduino.h>
#include <ArduinoJson.h>
#include "communication.hpp"

void onMessage(StaticJsonDocument<500>& doc);

Commuication<500> com;                          //On crée une communication

void setup() {
  Serial.begin(9600);                           //On set normalement notre Serial
  com.setSerial(&Serial);                       //Ici on passe par quel Serial la communication sera faite
  com.onMessageFunc(&onMessage);                //On indique quoi faire quand on reçois un message
}

void loop() {
    com.update();                               //Dans la loop principale, on update la communication
}

void onMessage(StaticJsonDocument<500>& doc)    //! La variable "doc" contient le message reçus
{
    com.sendMessage(doc);                       //? Ici on renvois le message reçus tel quel
}
```

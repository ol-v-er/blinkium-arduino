#include "Blinking.h"

//Pour le port série
#define BAUDS 9600

//false = Lis les transmissions envoyées
//true = Affiche la valeur de luminosité lue en temps réel. Ne lis pas les transmissions envoyées
bool testBrightness = false;

Blinking blink(A0, 60, 40);

void setup() {
  Serial.begin(BAUDS);//A besoin d'être fait pour que le message de la transmission puisse être envoyée sur le port série et pour le debugging
}

void loop() {
  if (testBrightness){
     Serial.println(analogRead(A0));
  } 
  if (!testBrightness){
    blink.listen();
    blink.serialPrintDatasAsBits();
    blink.serialPrintDatasAsStrings(); 
    blink.clear();
  }
}



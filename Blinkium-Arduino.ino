#include "Blinking.h"

//Un peu brouillon ici
//La seule chose qu'il faut regarder est l'objet Blinking
//Et les 2 fonctions setup et loop.

bool testBrightness = true;


Blinking blink(A0);

void setup() {
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);//For debugging

  if (!testBrightness)
    blink.listen();
}

void loop() {
  if (testBrightness){
     Serial.println(analogRead(A0));
  } 
}



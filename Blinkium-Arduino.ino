#include "Blinking.h"

//Un peu brouillon ici
//La seule chose qu'il faut regarder est l'objet Blinking
//Et les 2 fonctions setup et loop.

int lightsensorPin = A0;
//int ledPin = 8;
int sensorValue = 0;

int lightLevel = 0;
bool start = false;
bool end = false;

long time = 0;
long time2 = 0;
long t;
//long totalTime = 0;
//double totalTime2 = 0;
long begining = 0;
double lambdaAcc = 0;

//Contains the start code to start getting datas
int startCode[3] = { 1, 0, 1 };
//Will contain the datas received in binary form
//int* datas;
int datas[800] = {0};
int bitRead;

//dataLen will contain how much "groups" of data we will transfer. dataSize is the size of one group (in Bytes).
//For exemple : "CODE" in ASCII =  01000011 01001111 01000100 01000101
//Here dataLen = 4 and dataSize = 1
int dataLen, dataSize;

//Frequence of the transfer
double  frequency = 59.559261;
double lambda = 1000000.0 / frequency;

//Will run until it detects the start code
void detectStartCode();
//Will initialize dataLen and dataSize
void getHeader();
//Will store the datas received
void getDatas();

int detectBit();
void readValues(int* buf, int nb, int dec);

int binToDec(int data[8]);

Blinking blink(A0);

void setup() {
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);//For debugging

  lightLevel = 50;
  blink.listen();

}

void loop() {

  /*detectStartCode();
  getHeader();
  getDatas();*/

  delay(100000); 
}

/*
 * Actual Start Code = 1 0 1
 * Most significant bit needs to be 1
 */
void detectStartCode(){
  bool start = false;
  int lightValue = lightLevel; //Will be changed to adapt to differents brightnesses 

  while(!start){
    if(micros() - time2 >  1000000.0/480.0){
      time2 = micros();     
      sensorValue = analogRead(lightsensorPin);
      //Serial.println(sensorValue);
      if (sensorValue > lightValue){
        begining = micros();        
        time = begining;
        lambdaAcc += lambda;
        int count = 1;
        while (count < 3){
          t = micros();
          if( t - begining >  lambdaAcc){
            time = t;
            bitRead = detectBit();  
            lambdaAcc += lambda;
            //Serial.println(count); //For debugging
            //Serial.println(bitRead); //For debugging
            //If we detect a "1" and there should be a "0", we break out of the loop and restart reading start code from the begenning.
            //Needs more work because we can "miss" the start code
            //Maybe store the "startCodeLenght" last values and compare thoses with the start code, to avoid missing it.
            if (startCode[count] == 0 && bitRead == 1){
              begining = 0;
              lambdaAcc = 0;
              break;
            }
            else if (startCode[count] == 1 && bitRead == 0){
              begining = 0;
              lambdaAcc = 0;
              break;
            }
            //If what we detect is what we should detect, we continue
            else{     
              count++;
            }
          }
        }
    
        if (count == 3) // Will be changed. We need to compare with the size of the Start Code.
          start = true;        
      }
    }
  }
 Serial.println("Start code detected");
}

/*
 * Header Composition
 * Data Length = 8 bits; 0000 0000 = 0; 1111 1111 = 255 //Number of groups of data
 * Data Size = 4 bits; 0000 = 0 Byte; 1111 = 15 Bytes // We can change 0000 to be 1 to improve the range
 */
void getHeader(){
    int buf[8] = { 0 };  
    int lightValue = lightLevel;
    
    readValues(buf, 8, 0);
    dataLen = binToDec(buf);

    readValues(buf, 4, 4);
    dataSize = binToDec2(buf, 4);
    
    Serial.println("Header Read");//For debugging
    Serial.println(dataLen);
    Serial.println(dataSize);
}


/*
 * Get the data transmitted
 */
void getDatas(){
  int i = 0;
  int lightValue = lightLevel;

  readValues(datas, dataLen * dataSize * 8, 0);
  Serial.println("Datas Read");

  for (i = 0; i < dataLen*dataSize*8; i++){//For debbugging
    Serial.print(datas[i]);
  }
}

int binToDec(int data[8]){
  int number = 0;
  int count = 0;
  for(int i = 7; i >= 0; i--, count++){
    number += data[i]*power(2, count);
  }
  return number;
}

int binToDec2(int data[8], int s){
  int number = 0;
  int count = 0;
  for(int i = 7; i >= s; i--, count++){
    number += data[i]*power(2, count);
  }
  return number;
}

int power(int a, int b){
  int result = 1;
  for(int i = 0; i < b; i++){
    result *= a;
  }
  return result;  
}

int detectBit(){
  int count = 0;
  int b1, b2, b3, b4, b5, b6, b7;
  long timer = micros();
  long t;
  float div = 5.0;
  
  while (count < 1){
    t = micros();
    if(t - timer > 2*lambda/div){
      timer = t;
      b1 = analogRead(lightsensorPin);
      count++;     
    } 
  }

  while (count < 2){
    t = micros();
    if(t - timer > lambda/div){
      timer = t;
      b2 = analogRead(lightsensorPin);
      count++;     
    } 
  }

  while (count < 3){
    t = micros();
    if(t - timer > lambda/div){
      timer = t;
      b3 = analogRead(lightsensorPin);
      count++;     
    } 
  }

  b1 = b1 > lightLevel ? 1 : 0;
  b2 = b2 > lightLevel ? 1 : 0;
  b3 = b3 > lightLevel ? 1 : 0;

  if (b1 + b2 + b3  > 1)
    return 1;
  else 
    return 0;
}

void readValues(int* buf, int nb, int dec){
  int count = 0;
  long t;
  while (count < nb){
      t = micros();
      if (t - begining >  lambdaAcc){
        if (t - begining - lambdaAcc > 20)
          Serial.println(t - begining - lambdaAcc);
        buf[count+dec] = detectBit();
        time = t;
        lambdaAcc += lambda;     
        count++; 
     }
    }  
}


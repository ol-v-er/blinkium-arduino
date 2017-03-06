#include <elapsedMillis.h>
#include <math.h>

int lightsensorPin = A0;
//int ledPin = 8;
int sensorValue = 0;

int lightLevel = 0;
bool start = false;
bool end = false;

long time = 0;

//Contains the start code to start getting datas
int startCode[3] = { 1, 0, 1 };
//Will contain the datas received in binary form
int* datas;

//dataLen will contain how much "groups" of data we will transfer. dataSize is the size of one group (in Bytes).
//For exemple : "CODE" in ASCII =  01000011 01001111 01000100 01000101
//Here dataLen = 4 and dataSize = 1
int dataLen, dataSize;

//Frequence of the transfer
int frequency = 25;

//Will run until it detects the start code
void detectStartCode();
//Will initialize dataLen and dataSize
void getHeader();
//Will store the datas received
void getDatas();

int binToDec(int data[8]);

void setup() {
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);//For debugging

  //lightLevel = analogRead(lightsensorPin);

}

void loop() {

  detectStartCode();
  getHeader();
  getDatas();

  delay(100000); 
}

/*
 * Actual Start Code = 1 0 1
 * Most significant bit needs to be 1
 */
void detectStartCode(){
  bool start = false;
  //int lightValue = lightLevel + 50;
  int lightValue = 700; //Will be changed to adapt to differents brightnesses 

  while(!start){
    sensorValue = analogRead(lightsensorPin);
    if (sensorValue > lightValue){
      time = millis();
      lightLevel  = sensorValue - 20;
      lightValue = lightLevel;
      int count = 1;
      while (count < 3){
        if(millis() - time > 1000.0/frequency){
          time = millis();
          sensorValue = analogRead(lightsensorPin);
          Serial.println(count); //For debugging
          //If we detect a "1" and there should be a "0", we break out of the loop and restart reading start code from the begenning.
          //Needs more work because we can "miss" the start code
          //Maybe store the "startCodeLenght" last values and compare thoses with the start code, to avoid missing it.
          if (startCode[count] == 0 && sensorValue >= lightValue){
            break;
          }
          else if (startCode[count] == 1 && sensorValue <= lightValue){
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
  Serial.println("Start code detected");
}

/*
 * Header Composition
 * Data Length = 8 bits; 0000 0000 = 0; 1111 1111 = 255 //Number of groups of data
 * Data Size = 4 bits; 0000 = 0 Byte; 1111 = 15 Bytes // We can change 0000 to be 1 to improve the range
 */
void getHeader(){
    int i;
    int count = 0;
    //Will contain the dataLen
    int buf[8] = { 0 };
    //Will contain the dataSize
    int buf2[8] = { 0 };
    
    //int lightValue = lightLevel + 50;
    int lightValue = 700;

    //Read data lenght 
    while (count < 8){
      if (millis() - time > 1000.0/frequency){
        time = millis();
        sensorValue = analogRead(lightsensorPin);
        if (sensorValue > lightValue){
          buf[count] = 1;
        }
        else{
          buf[count] = 0;
        }
        count++; 
     }
    }

    dataLen = binToDec(buf);
    count = 0;

    //Read Data Size
    while (count < 4){
      if (millis() - time > 1000.0/frequency){
        time = millis();
        sensorValue = analogRead(lightsensorPin);
        if (sensorValue > lightValue){
          buf2[count+4] = 1;
        }
        else{
          buf2[count+4] = 0;
        }
        count++; 
     }
    }

    
    dataSize = binToDec(buf2);
    
    Serial.println("Header Read");//For debugging
    Serial.println(dataLen);
    Serial.println(dataSize);
}


/*
 * Get the data transmitted
 */
void getDatas(){
  int i = 0, j = 0;
  //int lightValue = lightLevel + 50;
  int lightValue = 700;
  datas = (int*)malloc((dataLen * dataSize * 8));
  
  //For each "group" of data
  while (i < dataLen){
    //For the length of the data
    while (j < dataSize * 8){
      if (millis() - time > 1000.0/frequency){
        time = millis();
        sensorValue = analogRead(lightsensorPin);
        if (sensorValue > lightValue){
          datas[i * dataSize * 8 + j] = 1;          
        }
        else{
          datas[i * dataSize * 8 + j] = 0;
        }
        Serial.println(sensorValue);//For debugging
        j++; 
      }
    }
    i++;
    j = 0;
  }
  
  Serial.println("Datas Read");

  for (i = 0; i < dataLen*dataSize*8; i++){//For debbugging
    Serial.println(datas[i]);
  }
}

int binToDec(int data[8]){
  int number = 0;
  for(int i = 7, count = 0; i >= 0; i--, count++){
    number += data[i]*pow(2, count);
  }
  return number;
}




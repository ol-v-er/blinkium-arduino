#ifndef BLINKING_H
#define BLINKING_H
#include <arduino.h>
#include "Timer.h"
#include "Utils.h"

#define DEBUG 1
#define BAUDS 9600

#define STARTCODE_LENGTH 3


class Blinking{
	
	public :
		Blinking(int sensorPin, double freq, int lightThreshold);
		void listen();
		
	private :
		void detectStartCode();
    void synchronise(); 
		void receiveHeader();
		void receiveDatas();
		int readBit();
		void readValues(Bit* buf, int number);

    Bit* getDatas();
    void printDatasAsStrings();
		
		int lightsensorPin;

		int threshold;
    int maxBrightness = 0;
    int minBrightness = 1000;

    Timer timer;

		//Contains the start code to start getting datas
		int startCode[3] = { 1, 0, 1 };
		//Will contain the datas received in binary form
		Bit datas[1200];
    
		Bit bitRead;

		//dataLen will contain how much "groups" of data will be transfered
		//For exemple : "CODE" in ASCII =  01000011 01001111 01000100 01000101
		//Here dataLen = 4 and dataSize = 1
		int dataLen;

		//Frequence of the transfer
    double frequency;
		double lambda;

			
};

#endif

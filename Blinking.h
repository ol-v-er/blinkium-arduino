#ifndef BLINKING_H
#define BLINKING_H
#include <arduino.h>

typedef struct {
	unsigned int value : 1;
} Bit;

class Blinking{
	
	public :
		Blinking(int sensorPin);
		void listen();
		
	private :
		void detectStartCode();
    void synchronise(); 
		void receiveHeader();
		void receiveDatas();
		int readBit();
		void readValues(Bit* buf, int nb, int dec);

    Bit* getDatas();
    char** getDatasAsStrings();

    void printDatasAsStrings();
    
		int binToDec(Bit* data, int size);
		int binToDec2(Bit* data, int size, int shift);
		int power(int a, int b);
    char charOfBin(Bit* data, int shift);
		
		int lightsensorPin;
		int sensorValue = 0;

		int lightLevel = 50;

		long time = 0;
		long time2 = 0;
		long begining = 0;
		double lambdaAcc = 0;

		//Contains the start code to start getting datas
		int startCode[3] = { 1, 0, 1 };
		//Will contain the datas received in binary form
		Bit datas[800];
		Bit bitRead;

		//dataLen will contain how much "groups" of data we will transfer. dataSize is the size of one group (in Bytes).
		//For exemple : "CODE" in ASCII =  01000011 01001111 01000100 01000101
		//Here dataLen = 4 and dataSize = 1
		int dataLen;

		//Frequence of the transfer
		//double  frequency = 59.559261;
    double  frequency = 60.0;
		double lambda = 1000000.0 / frequency;

			
};

#endif

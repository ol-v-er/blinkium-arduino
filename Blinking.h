#ifndef BLINKING_H
#define BLINKING_H
#include <arduino.h>
#include "Timer.h"
#include "Utils.h"

/* Active ou non le mode DEBUG (0 = désactivé, 1 = activé)
 * Si le mode DEBUG est actif des informations concernant l'entrée dans le code de départ, la fréquence mesurée et la luminosité seront envoyées sur le port série
 */
#define DEBUG 0

//Taille du code de départ
#define STARTCODE_LENGTH 3

//Nombre maximal de bits transférés
#define MAX_DATA_LENGTH 1200

class Blinking{
	
	public :
		Blinking(int sensorPin, double freq, int lightThreshold);
		void listen(); //Launch the procedure to read the transmission
    void clear(); //Reset the array with the message of the previous transmission
    void serialPrintDatasAsStrings(); //Envoi les données sous forme binaire sur le prot série
    void serialPrintDatasAsBits(); //Envois les données sous forme de chaînes de caractères séparées par des tabulations
		
	private :
		void detectStartCode(); //Block the reading of datas until the start code is found
    void synchronize(); //Synchronize the frequency the program with the android device
		void receiveHeader(); //Read the header of the transmission
		void receiveDatas(); //Read the message of the transmission
		int readBit(); //Read one bit of data
		void readValues(Bit* buf, int number); //Read a nb number of bits and copy then in a buffer buf

    Bit* getDatas();
    	
		int lightsensorPin;

		int threshold;
    //Valeurs arbitraires
    int maxBrightness = 0;
    int minBrightness = 1000;

    Timer timer;

		//Contains the start code to start getting datas
		int startCode[3] = { 1, 0, 1 };
		//Will contain the datas received in binary form
		Bit datas[MAX_DATA_LENGTH];
    
		Bit bitRead;

		//dataLen will contain how much "groups" of data will be transfered
		//For exemple : "CODE" in ASCII =  01000011 01001111 01000100 01000101
		//Here dataLen = 4
		int dataLen;

		//Frequence of the transfer
    double frequency;
		double lambda;

			
};

#endif

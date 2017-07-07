#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <unistd.h>
#include <fstream>
#include <stdio.h>

using namespace std;

RF24 radio(0,17);

// The addresses used in RF24Audio
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

int main(int argc, char** argv){

	cout << "RF24Audio On Rpi\n";

	radio.begin(); 

	radio.setChannel(1);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_1MBPS);
	radio.setAutoAck(0);
	radio.setCRCLength(RF24_CRC_8);
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);
	radio.printDetails();
	radio.startListening();

	unsigned char audioData[32];
	unsigned char samplesToDisplay = 3;

	while(1) { 
		if(radio.available()) {
			radio.read(&audioData, 32);

			cout << "Data Available" << endl;

			for (int i=0; i<samplesToDisplay; i++) {
				//cout << audioData[i] << endl;
				printf("Got response ::: %c \n",audioData[i]);
	
			}
		} else {
			cout << "Audio Not Available" << endl;
		}
	}
} 
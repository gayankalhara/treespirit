#include <iostream>
#include <RF24/RF24.h>
#include <string>
#include <sstream>
#include <ctime>

#include "wavfile.h"

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);

// The address used in RF24Audio
const uint64_t address = { 0x544d52687CLL };

int main(int argc, char** argv){
	radio.begin(); 

	radio.setChannel(3);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(0);
	radio.setCRCLength(RF24_CRC_8);
	radio.openReadingPipe(1, address);
	radio.printDetails();
	radio.startListening();
	
	int count = 0;
	const int samples = 44000 * 10; // 10 Seconds
	uint8_t soundClip[samples];

	clock_t start;
    double duration;

    start = clock();

	while(count <= samples) { 
		if(radio.available()) {
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
			int payloadSize = 32;

			cout << "Time: " << duration << " s" << endl;
			cout << "Sample ID: " << count << "/" << samples << endl;
			cout << "Payload Size: " << payloadSize << endl;
			uint8_t audioData[payloadSize];

			radio.read(&audioData, payloadSize);

			for (int i=0; i < payloadSize; i++) {
				cout<<audioData[i];
				soundClip[count++] = audioData[i];
			}

			cout << endl << endl;		
		}
	}

	time_t unixTimestamp = time(0);
	stringstream waveFileName;
	waveFileName << "audio_n3_" << unixTimestamp << ".wav";

	FILE * audioFile = wavfile_open(waveFileName.str().c_str());

	if(audioFile) {
		wavfile_write(audioFile, soundClip, samples);
		wavfile_close(audioFile);
		cout << "Audio File Generated: " << waveFileName.str() << endl;
		return 0;
	} else {
		cout << "Couldn't open sound.wav for writing!" << endl;
		return 1;
	}
}
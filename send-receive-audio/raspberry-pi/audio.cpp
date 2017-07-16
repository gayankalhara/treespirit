#include <iostream>
#include <RF24/RF24.h>
#include <string>
#include <sstream>
#include <ctime>
#include "wavfile.h"

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);

const uint64_t pipes[14] = { 0xABCDABCD71LL,
							 0x544d52687CLL,
							 0x544d526832LL,
							 0x544d52683CLL,
							 0x544d526846LL,
							 0x544d526850LL,
							 0x544d52685ALL,
							 0x544d526820LL,
							 0x544d52686ELL,
							 0x544d52684BLL,
							 0x544d526841LL,
							 0x544d526855LL,
							 0x544d52685FLL,
							 0x544d526869LL};

int main(int argc, char** argv){
	radio.begin(); 

	radio.setChannel(0);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(0);
	radio.setCRCLength(RF24_CRC_8);
	radio.openReadingPipe(2, pipes[2+1]);
	radio.openReadingPipe(3, pipes[3+1]);
	radio.printDetails();
	radio.startListening();

	uint8_t pipe_num = 0;
	int count2 = 0;
	int count3 = 0;
	const int samples = 11040 * 60; // ~ 10 Seconds
	int payloadSize = 32;
	
	uint8_t soundClip2[samples];
	uint8_t soundClip3[samples];

	clock_t start;
    double duration;

    start = clock();

    if(radio.isChipConnected()){
    	cout << "NRF24L01+ is connected!" << endl;
    } else {
    	cout << "NRF24L01+ is not connected! Terminating..." << endl;
    	return 1;
    }

	while((count2 < samples) || (count3 < samples)) {
		if(radio.available(&pipe_num)) {
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			switch((int) pipe_num){
				case 2:
					if((count2 <= samples)){
						cout << "Payload Received from Pipe: " << (int) pipe_num << endl;
						cout << "Time: " << duration << " s" << endl;
						cout << "Sample ID: " << count2 << "/" << samples << endl;
						cout << "Payload Size: " << payloadSize << endl;
						uint8_t audioData2[payloadSize];

						radio.read(&audioData2, payloadSize);

						for (int i=0; (i < payloadSize && count2 < samples); i++) {
							cout << audioData2[i];
							soundClip2[count2++] = audioData2[i];
						}

						cout << endl << endl;
					}

					break;

				case 3:
					if((count3 <= samples)){	
						cout << "Payload Received from Pipe: " << (int) pipe_num << endl;		
						cout << "Time: " << duration << " s" << endl;
						cout << "Sample ID: " << count3 << "/" << samples << endl;
						cout << "Payload Size: " << payloadSize << endl;
						uint8_t audioData3[payloadSize];

						radio.read(&audioData3, payloadSize);

						for (int i=0; (i < payloadSize && count3 < samples); i++) {
							cout << audioData3[i];
							soundClip3[count3++] = audioData3[i];
						}

						cout << endl << endl;
					}
					
					break;

				default:
					cout << "Payload Received from Pipe: " << (int) pipe_num << endl;
					break;
			}
	
		}
	}

	time_t unixTimestamp = time(0);

	stringstream waveFileName2;
	waveFileName2 << "audio_n2_" << unixTimestamp << ".wav";

	FILE * audioFile2 = wavfile_open(waveFileName2.str().c_str());

	if(audioFile2) {
		wavfile_write(audioFile2, soundClip2, samples);
		wavfile_close(audioFile2);
		cout << "Audio File Generated: " << waveFileName2.str() << endl;
	} else {
		cout << "Couldn't open sound.wav for writing!" << endl;
	}

	stringstream waveFileName3;
	waveFileName3 << "audio_n3_" << unixTimestamp << ".wav";

	FILE * audioFile3 = wavfile_open(waveFileName3.str().c_str());

	if(audioFile3) {
		wavfile_write(audioFile3, soundClip3, samples);
		wavfile_close(audioFile3);
		cout << "Audio File Generated: " << waveFileName3.str() << endl;
	} else {
		cout << "Couldn't open sound.wav for writing!" << endl;
	}
}
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);

// The address used in RF24Audio
const uint64_t address = { 0x544d52687CLL };

int main(int argc, char** argv){
	radio.begin(); 

	radio.setChannel(1);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_1MBPS);
	radio.setAutoAck(0);
	radio.setCRCLength(RF24_CRC_8);
	radio.openReadingPipe(1, address);
	radio.printDetails();
	radio.startListening();

	while(1) { 
		if(radio.available()) {
			int payloadSize = radio.getDynamicPayloadSize(); // Get Dynamic Payload Size
			cout << endl << "Payload Size: " << payloadSize << endl;
			signed char audioData[payloadSize];

			radio.read(&audioData, payloadSize);

			for (int i=0; i < payloadSize; i++) {
				cout << audioData[i];
			}

			cout << endl;
		}
	}
}
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
unsigned char address[] = { 0xCC, 0xCE, 0xCC, 0xCE, 0xCC };

int main(int argc, char** argv){

	radio.begin();

	if(radio.failureDetected){ 
		radio.begin();                       	// Attempt to re-configure the radio with defaults
		radio.failureDetected = 0;           	// Reset the detection value
		radio.openReadingPipe(1, address);		// Re-configure pipe addresses
		cout << "Failure Detected!" << endl;
	}

	if(radio.isChipConnected()){ // Checks if the chip is connected to the SPI bus
		cout << "Chip is connected to the SPI bus!" << endl;

		radio.setChannel(95); // Set RF communication channel
		radio.setPALevel(RF24_PA_MAX); // Set Power Amplifier (PA) level
		radio.setDataRate(RF24_2MBPS); // Set the transmission data rate
		radio.setAutoAck(1); // Enable auto-acknowlede packets on a per pipeline basis
		radio.setCRCLength(RF24_CRC_16); // Set the CRC length
		radio.openReadingPipe(1, address); // Open a pipe for reading
		radio.printDetails(); // Print a giant block of debugging information to stdout
		radio.enableDynamicPayloads(); // Write an ack payload for the specified pipe
		radio.startListening();

		while(1) { 
			/**
			 * Check whether there are bytes available to be read
			 * Returns True if there is a payload available, false if none is
			 */
			if(radio.available()) {
				int payloadSize = radio.getDynamicPayloadSize(); // Get Dynamic Payload Size
				cout << endl << "Payload Size: " << payloadSize << endl;
				signed char stringData[payloadSize];

				if(payloadSize < 1){
					cout << "Corrupt payload has been flushed!" << endl;
		        	break;
		        }

				radio.read(&stringData, payloadSize);
				for (int i=0; i < payloadSize; i++) {
					cout << stringData[i];
				}

				cout << endl;
			}
			delay(100);
		}
	} else {
		cout << "Chip is not connected!" << endl;
	}
}
#include <RF24.h>
#include <SPI.h>
#include "printf.h"

RF24 radio(7,8);        // Set radio up using pins 7 (CE) 8 (CS)


// These are the default multicast addresses used by the library. By simply using the
// RF24 radio library, we can interact with the RF24 Audio library directly
const uint64_t addresses[2] = { 0xABCDABCD71LL, 0x544d52687CLL};

// Note: This will only receive data broadcast to the entire radio group. Capturing private
// channel communication as well would require listening to more addresses on more pipes.


void setup() {      
  
  Serial.begin(115200);             // Set up the serial port
  printf_begin();
  
  radio.begin();        // Initialize the radio  
  /* Set our radio options to the audio library defaults */
  radio.setChannel(1);              // Set RF channel to 1
  radio.setAutoAck(0);              // Disable ACKnowledgement packets to allow multicast reception
  radio.setCRCLength(RF24_CRC_8);   // Only use 8bit CRC for audio
  //radio.setDataRate(RF24_1MBPS);  // Library default is RF24_1MBPS for RF24 and RF24Audio
  radio.openWritingPipe(addresses[0]);  // Set up reading and writing pipes. 
  radio.openReadingPipe(1,addresses[1]);// All of the radios listen by default to the same multicast pipe  
  radio.printDetails();
  radio.startListening();           // Need to start the radio listening
  
  Serial.println("Setup OK. Begin transmission of audio");
}


byte audioData[32];                 // Set up a buffer for the received data

byte samplesToDisplay = 3;          // Change this to 32 to send the entire payload over USB/Serial

void loop() {
  
  if(radio.available()){       
      radio.read(&audioData,32);
      // Now do whatever you want with the audio data.
      // Maybe send it over USB to a PC to analyze further?
      
      for(int i=0; i<samplesToDisplay; i++){
        Serial.write(audioData[i]); 
      }
      Serial.println("");
      
     // Note: The Audio library default is 24kHZ sample rate which results in 750 payloads/second with 8bit audio
     // Decrease the audio sample rate via userConfig.h if the data stream is too fast, or increase the Serial baud rate 
  }
  
}
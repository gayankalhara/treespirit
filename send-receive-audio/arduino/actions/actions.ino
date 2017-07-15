#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>
#include "printf.h"    // General includes for radio and audio lib

RF24 radio(7,8);    // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio, 1, 3); // Set up the audio using the radio, and set to radio number 1 and set channel to 3

void setup() {      
  Serial.begin(115200);   // Enable Arduino serial library
  radio.begin();                // Must start the radio here, only if we want to print debug info
  printf_begin();               // Radio library uses printf to output debug info  
  radio.setPayloadSize(32);
  radio.setChannel(3);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);
  radio.setCRCLength(RF24_CRC_8);
  radio.setPALevel(RF24_PA_MAX);
  radio.powerUp();
  radio.printDetails();         // Print the info
  rfAudio.begin();    // Start up the radio and audio libararies
  rfAudio.setVolume(7); // max vol
  rfAudio.transmit();
}

void loop() {

}

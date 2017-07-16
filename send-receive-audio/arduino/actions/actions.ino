#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>
#include "printf.h"                             // General includes for radio and audio lib
#define CE_PIN      7
#define CS_PIN      8
#define RADIO_NUM   3
#define CHANNEL     0

RF24 radio(CE_PIN, CS_PIN);                     // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio, RADIO_NUM, CHANNEL);   // Set up the audio using the radio, and set to radio number 1 and set channel to 3

void setup() {      
  Serial.begin(115200);                         // Enable Arduino serial library
  radio.begin();                                // Must start the radio here, only if we want to print debug info
  printf_begin();                               // Radio library uses printf to output debug info  
  radio.setPayloadSize(32);
  radio.setPALevel(RF24_PA_MAX);
  radio.powerUp();
  rfAudio.begin();                              // Start up the radio and audio libararies
  rfAudio.setVolume(7);                         // max vol
  rfAudio.transmit();
  radio.printDetails();                         // Print the info
  Serial.print("Radio Number     = ");
  Serial.println(RADIO_NUM);
  Serial.print("Channel          = ");
  Serial.println(CHANNEL);
}

void loop() {

}

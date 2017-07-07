#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>
#include "printf.h"    // General includes for radio and audio lib

RF24 radio(7,8);    // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio,1); // Set up the audio using the radio, and set to radio number 0

void setup() {      
  Serial.begin(115200);   // Enable Arduino serial library
  
  printf_begin();               // Radio library uses printf to output debug info  
  radio.begin();                // Must start the radio here, only if we want to print debug info
  radio.printDetails();         // Print the info
  rfAudio.begin();    // Start up the radio and audio libararies
}

void loop() {
    if(Serial.available()) { // Receive and analyze incoming serial data
        switch(Serial.read()) {
            case 'r': rfAudio.transmit();
              Serial.print("done");
              break; // Send an r or an s over serial to control playback
            case 's': rfAudio.receive();
              break;
            case '=': rfAudio.volume(1);
              break; // Control volume by sending = or - over serial
            case '-': rfAudio.volume(0);
              break;
        }
    }
 
    delay(1000);
}
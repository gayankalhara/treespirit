#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);
int count = 0;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0x7365727631LL };

void setup() {
 Serial.begin(9600);
  //CONFIGURE RADIO
  radio.begin();
  // Enable this seems to work better
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  
  radio.openWritingPipe(pipes[0]);
  radio.powerUp();
}

void loop() {
  count++;
  
  // Define 
  String str = "Sending Packet: " + String(count); 

  Serial.println(str);
  
  // Length (with one extra character for the null terminator)
  int str_len = str.length() + 1; 
  
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  
  // Copy it over 
  str.toCharArray(char_array, str_len);

  radio.write(char_array, sizeof(char_array));
  delay(100);
}
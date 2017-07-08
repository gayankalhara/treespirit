#include <SPI.h>
#include "RF24.h"
#include "printf.h"

RF24 radio(7,8);
int count = 0;

byte address[] = { 0xCC, 0xCE, 0xCC, 0xCE, 0xCC };

void setup() {
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(95);
  radio.openWritingPipe(address);
  radio.powerUp();
  radio.printDetails();
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

import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM)

pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)

radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MAX)
radio.enableDynamicPayloads()
radio.openReadingPipe(1, pipes[1])
radio.printDetails()

while(1):
    radio.startListening()
    while not radio.available(0):
            time.sleep(1000/1000000.0)
            # print("Timed out.")
            break

    receivedMessage = []
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    #print("Received: {}".format(receivedMessage))
    string = ""
    for n in receivedMessage:
        # Decode into standard unicode set
        if (n >= 32 and n <= 126):
            string += chr(n)
    if (string != ""):
        print(format(string))

    #radio.stopListening()
    time.sleep(0.05)

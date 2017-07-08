import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM)

pipe = [0xCC, 0xCE, 0xCC, 0xCE, 0xCC]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 8)
radio.setChannel(95)
radio.setPALevel(NRF24.PA_MAX)
radio.setDataRate(NRF24.BR_2MBPS)
radio.setAutoAck(True)
radio.setCRCLength(NRF24.CRC_16)
radio.openReadingPipe(1, pipe)
radio.printDetails()
radio.enableDynamicPayloads()
radio.startListening()

while(1):
    while not radio.available(0):
        time.sleep(1000/1000000.0)
        print("Timed out.")
        break

    receivedMessage = []
    radio.setPayloadSize(radio.getDynamicPayloadSize())
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    string = ""
    for n in receivedMessage:
        # Decode into standard unicode set
        if (n >= 32 and n <= 126):
            string += chr(n)
    if (string != ""):
        print(format(string))

    time.sleep(0.05)
import time

import serial

import utilx

#serial1 = serial.Serial(port='/dev/tty.ArduinoMasterBluetooh', baudrate=9600, timeout=1, write_timeout=1)
serial1 = serial.Serial(port='/dev/tty.ArduinoMasterBluetooh', baudrate=9600, timeout=1, write_timeout=1)


texto= str(1)


while True:
    serial1.write(texto.encode('utf-8'))
    serial1.readline()
    print("Mensaje enviado")


import utilx
import serial
import time
import platform
import threading


def greeting():
    print('''
Bienvenido al App Communication, recuerda que debes conectarte via bluetooh a los dispositivos, 
si estas en modo debug,solo se usaran los puertos seleccionados, 
caso contrario se usaran todos los puertos com disponibles, 
asi que recuerda no tener ningun otro dispositivo conectado a la maquina.
O ingresar correctamente los COM que vas a utilizar si estas en modo debug.

    ''')
    input("Una vez preparado, Presiona cualquier tecla para continuar")
    return True


debug=True





if debug and platform.system() == 'Darwin' or debug and platform.system() == 'Linux':
    puertos = ['/dev/cu.ArduinoMasterBluetooh']
elif debug and platform.system() == 'Windows':
    puertos = ['COM4']
else:
    puertos=utilx.serial_ports() #toma todos los puertos com conectados sin distincion

continuar = greeting()
if continuar:
    comunica = []
    for p in puertos:
        print("abriendo puerto", p)
        try:
            comunica.append(serial.Serial(port=str(p), baudrate=9600, timeout=0.5, write_timeout=0))
        except Exception as e:
            print("Error al abrir puerto", e)

def recibido(puerto,numero_puerto):
    "los puertos van desde {0,1,2,3}"
    while True:
        byte = puerto.read()
        if len(byte)>0:
            strings = "".join(map(chr, byte))
            print(strings)
            if strings=='1':
                for p in comunica:
                    if numero_puerto==0:
                        p.write("{1,0,0,0}".encode('utf-8'))
                    elif numero_puerto==1:
                        p.write("{0,1,0,0}".encode('utf-8'))
                    elif numero_puerto==2:
                        p.write("{0,0,1,0}".encode('utf-8'))
                    elif numero_puerto==3:
                        p.write("{0,0,0,1}".encode('utf-8'))




def escritura(puerto):
    while True:
        time.sleep(3)
        puerto.write("1".encode('utf-8'))
        print("1 enviado")



if __name__=='__main__':

    for p in comunica:
        t1 = threading.Thread(target=recibido, args=(p,comunica.index(p)))
        #t2 = threading.Thread(target=escritura, args=(p,))
        t1.start()
        #t2.start()





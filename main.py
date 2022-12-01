import utilx
import serial
import time
import platform
print(utilx.serial_ports())

debug=True

if debug and platform.system() == 'Darwin' or debug and platform.system() == 'Linux':
    puertos = ['/dev/tty.usbserial-1410']
elif debug and platform.system() == 'Windows':
    puertos = ['COM4','COM5']
else:
    puertos=utilx.serial_ports()


comunica = []
for p in puertos:
    print("open ", p)
    comunica.append(serial.Serial(port=str(p), baudrate=9600, timeout=0.5, write_timeout=0))

while True:
    buffer = ['A', 'B', 'C', 'D']
    c=0
    for idx in range(len(comunica)):
        byte = comunica[idx].read()
        if len(byte)>0:
            c+=1
            buffer[idx]=byte.decode('utf-8')
            comunica[idx].write("X".encode('utf-8'))

    txt="{"
    for b in buffer:
        txt += b
    txt+="}"

    for se in comunica:
        #se.write(txt.encode('utf-8'))
        se.write("1".encode('utf-8'))

    #print(txt)
    print("A")
    #time.sleep(10/1000.0)
import pygame
import serial

import utilx

pygame.init()
pygame.display.set_caption('Simon')
tamano = width, height = 540, 360
pantalla = pygame.display.set_mode(tamano)

serial1 = serial.Serial(port='/dev/tty.ArduinoMasterBluetooh', baudrate=9600, timeout=1, write_timeout=1)
#serial1=utilx.serial_ports()[1]
color_negro = 0, 0, 0
color_blanco = 255,255,255

flag_salir = False
flag_send = False
x=0
y=0
y_=0
pantalla.fill(color_negro)
while not flag_salir:
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT: flag_salir=True

    mouse_x, mouse_y = pygame.mouse.get_pos()
    mouse_botonpress =  (evento.type == pygame.MOUSEBUTTONDOWN)
    mouse_botonup =  (evento.type == pygame.MOUSEBUTTONUP)

    byte = serial1.readline()
    if len(byte)>0:
        txt = byte.decode('utf-8')
        txt = "".join(e for e in txt if e.isalnum())
        print("recv ["+ txt + "]")
        if mouse_botonpress==True:
            flag_send=not flag_send

        if flag_send:
            sendstr = int((mouse_x/width)*10.0)
            print("send",sendstr)
            sendstr = str(sendstr)+str(int((mouse_y/height)*10.0))
            serial1.write(sendstr.encode('utf-8'))
        else:
            print("send 0")
            serial1.write("0".encode('utf-8'))
        y = int(txt)
        #pantalla.set_at((x, y), color_blanco)
        pygame.draw.line(pantalla, color_blanco, (x-1, y_), (x, y))
        y_=y
        x+=1
        if x>width:
            pantalla.fill(color_negro)
            y_=0
            x=0
        pygame.display.update()

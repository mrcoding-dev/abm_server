#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth
 
void setup()
{
  BT.begin(9600);       // Inicializamos el puerto serie BT (Para Modo AT 2)
  Serial.begin(9600);   // Inicializamos  el puerto serie  
}
 

void hacerAlgo(){
  //Esta funcion es lo que debe hacer el arduino, de resto es comunicacion
}


void loop()
{
  if(BT.available())    // Aca si llega un dato se imprime
  {
    //Serial.write(BT.read());

     Serial.print("Recibi: ");
     Serial.print(BT.readString());
    Serial.print("\n");
    

  }
 
  if(Serial.available())  // Aca podemos controlar lo que se manda al bluetooh
  {
     BT.write(Serial.read());
  }
}

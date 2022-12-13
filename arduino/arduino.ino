#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>   

SoftwareSerial BT(10,11);    //6 7  10 11 (rx, tx)

unsigned char *buffer;
String mensaje = "";

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

#define LEYENDO 1
#define LEIDO 2
#define ESPERANDO 0

#define CARGA 1
#define ADELANTE 2
#define DESCARGA 3
#define ATRAS 4

#define DESACTIVADO 0
#define ACTIVADO 1
// valor entre 1 y 5 para el mensaje que recibo

// Mensaje para matar al robot
#define Desactivar 'f'
// Mensaje grua 1
#define inGrua1 'a'
#define outGrua1 'c'
//mensaje grua2
#define inGrua2 'r'
#define outGrua2 'd'
// Sensores
//Adelante
#define echoPin 12 
#define trigPin 13 

long duration1;
int distance1;
//Atras
#define echoPin2 2 
#define trigPin2 3 

long duration2;
int distance2;

int estado;
int desviado;
void setup() {
  
  BT.begin(9600);  //AT1:9600 AT2:38400    
  buffer = malloc(1000);
  Serial.begin(9600);           // set up Serial library at 9600 bps

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  estado = CARGA;
  desviado = DESACTIVADO;
  myMotor1->setSpeed(70);
  myMotor1->run(FORWARD);
  myMotor1->run(RELEASE);
  myMotor2->setSpeed(70);
  myMotor2->run(FORWARD);
  myMotor2->run(RELEASE);
  //BT.write(outGrua1);
}

int st_read = ESPERANDO;
int size = 0;


void recibir(){
  unsigned char c;
  if(BT.available())   
  { 

    //Serial.write(BT.read());
    c = BT.read();
    Serial.println(char(c));
    if (st_read == LEYENDO && c == '}')
    {
      size = size + 1;
      buffer[size] = c;      
      for (int i=0; i<size+1;i++){
          mensaje =  mensaje + String(char(buffer[i]));
          
      }
      st_read = LEIDO;
    }
    if (st_read == ESPERANDO && c == '{'){
      size = 0;
      st_read = LEYENDO;
      buffer[size] = c;
    }
    if (st_read == LEYENDO && c != '{' && c != '}'){
      size = size + 1;
      buffer[size] = c;
    }
  }  
}

int mensaje_disp(){
    if (st_read == LEIDO){
      return 1;
    }
    return 0;
}

String leerMensaje(){
  String aux = mensaje;
    mensaje = "";
    st_read = ESPERANDO;
    return aux;
}

int distAtras(){
  // Trig 12 y Echo 11
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration1 = pulseIn(echoPin, HIGH);

  return duration1 * 0.034 / 2;
}
int distFrente(){
  // Trig 3 y echo 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);

  return duration2 * 0.034 / 2;
}

void loop() {
  recibir();
  distance1 = distFrente();
  distance2 = distAtras();
  Serial.println(estado);
      
  if (mensaje_disp()){
    String aux = leerMensaje();
    
    if (aux[1] == inGrua1 && estado == CARGA){estado = ADELANTE;} 
        
    if (aux[2] == inGrua2 && estado == DESCARGA){estado = ATRAS;}

    if (aux[3] == Desactivar){estado = DESACTIVADO;}
      
    aux = "";}

  if (distance1 < 5 && estado == ADELANTE){
    estado = DESCARGA;
    //llegue enviar mensaje Grua2
    BT.write(outGrua2);
    }

  if (distance2 < 5 && estado == ATRAS){
    estado = CARGA;
    //llegue enviar mensaje Grua1
    BT.write(outGrua1);
  }
  if (distance1 == 0 || distance1 > 250 || distance2 == 0 || distance2 > 250 ){
    desviado = ACTIVADO;
  }
  if (desviado == ACTIVADO && distance1 != 0 && distance2 != 0 && distance1<250 && distance2<250)  { desviado = DESACTIVADO;}


  if (estado == ADELANTE){
      myMotor1->run(FORWARD);
      myMotor2->run(FORWARD);}

  if (estado == ATRAS){
      myMotor1->run(BACKWARD);
      myMotor2->run(BACKWARD);}
  
  if (estado == CARGA || estado == DESCARGA){
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE); }    
    
  if (desviado == ACTIVADO){
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
  }
  if (estado == DESACTIVADO){
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
  }

  if (desviado == DESACTIVADO && estado == ADELANTE){
      myMotor1->run(FORWARD);
      myMotor2->run(FORWARD);}
      
  if (desviado == DESACTIVADO && estado == ATRAS){
    myMotor1->run(BACKWARD);
    myMotor2->run(BACKWARD);}

}
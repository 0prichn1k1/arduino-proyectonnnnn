
#include <Servo.h>
Servo servogy; //nombre del servo

int sigPin =8;
int servoPin =6;
int buzzerPin =10;
int ledPinRojo =11;
int ledPinVerde =12;
int duracion;
float distancia;
int dt = 10;            //cada vez que hay un "delay dt" es para q el prcoesador del arduino descanse
int serial = 9600;

void setup() {

  servogy.attach(servoPin);
  Serial.begin(serial);
  pinMode(buzzerPin, 1);
  pinMode(ledPinVerde, 1);      //ponemos los pines de los leds y el buzzer en salida
  pinMode(ledPinRojo, 1);
  servogy.write(0);         //seteamos el servo en 0 grados

}

void loop() {
  
  digitalWrite(ledPinVerde, 1);     //prendemos el led verde
  pinMode(sigPin, OUTPUT);          //se pone en salida para que emita señal ultrasonica 
  delay(dt);
  digitalWrite(sigPin, 1);          // se activa la señal
  delay(dt);
  digitalWrite(sigPin, 0);          //se apaga
  pinMode(sigPin, INPUT);           //se pone en emtrada para poder registrar el pulso 

  duracion = pulseIn(sigPin, 1);    //se registra el pulso y se coloca en la variable "duracion"
  distancia = duracion/58.2;        //se divide en 58.2 para dejarlo en centimetros y se guarda en "distancia"

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia < 50) {             //si está a menos de 50cm se activan el led rojo y el buzzer
    digitalWrite(ledPinRojo, 1);    // tambien el servomotor se pone a 90° 
    digitalWrite(buzzerPin, 1);
    servogy.write(90);
  } else {
    digitalWrite(ledPinRojo, 0);    //si no se setea todo a 0
    digitalWrite(buzzerPin, 0);
    servogy.write(0);
  }

}

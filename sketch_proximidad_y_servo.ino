
#include <Servo.h>
Servo servogy; //nombre del servo

int sigPin =8;  //asigna el pin 8 para el signal  del sensor de proximidad
int servoPin =6;  //asigna el pin 6 para la señal del servo
int buzzerPin =10;  //asigna el pin 10 para el buzzer
int ledPinRojo =11;  //asigna el pin 11 para el led rojo 
int ledPinVerde =12;  //asigna el pin 12 para el led verde
int duracion;  //variable que se declara para  despues darle el valor del ultrasonido del sensor
float distancia;  //variable que se declara para  despues guardar el resultado del calculo de distancia
int dt = 10;            //cada vez que hay un "delay dt" es para q el prcoesador del arduino descanse
int serial = 9600;  //estandar de frecuencia del monitor serial para el  muestreo

void setup() {

  servogy.attach(servoPin);  //le doy el pin del servo al arduino
  Serial.begin(serial);  //le doy la frecuencial al monitor serial
  pinMode(buzzerPin, 1);
  pinMode(ledPinVerde, 1);      //ponemos los pines de los leds (rojo y verde) y el buzzer en salida
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
  Serial.print(distancia);          // escribimos la distancia en el monitor serial
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

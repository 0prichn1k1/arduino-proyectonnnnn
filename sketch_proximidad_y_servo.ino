
#include <Servo.h>
Servo servogy;

int sigPin =8;
int servoPin =6;
int buzzerPin =10;
int ledPinRojo =11;
int ledPinVerde =12;
int duracion;
float distancia;
int dt = 10;
int serial = 9600;

void setup() {

  servogy.attach(servoPin);
  Serial.begin(serial);
  pinMode(buzzerPin, 1);
  pinMode(ledPinVerde, 1);
  pinMode(ledPinRojo, 1);
  servogy.write(0);

}

void loop() {
  
  digitalWrite(ledPinVerde, 1);
  pinMode(sigPin, OUTPUT);
  delay(dt);
  digitalWrite(sigPin, 1);
  delay(dt);
  digitalWrite(sigPin, 0);
  pinMode(sigPin, INPUT);

  duracion = pulseIn(sigPin, 1);
  distancia = duracion/58.2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia < 50) {
    digitalWrite(ledPinRojo, 1);
    digitalWrite(buzzerPin, 1);
    servogy.write(90);
  } else {
    digitalWrite(ledPinRojo, 0);
    digitalWrite(buzzerPin, 0);
    servogy.write(0);
  }

}

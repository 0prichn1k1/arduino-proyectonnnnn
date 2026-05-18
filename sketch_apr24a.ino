
#include <Servo.h>

Servo servogy;
int trig = 10;
int echo = 8;
int dt = 10;
int servopin = 9;
int serial = 9600;
int duracion = pulseIn(echo, 1 );
int distancia;

void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servogy.attach(servopin);
  Serial.begin(serial);

}

void loop() {

  digitalWrite(trig, 1);
  delay(dt);
  digitalWrite(trig, 0);
  duracion;
  Serial.println(distancia);

}
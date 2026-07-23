#include <Servo.h>

Servo servogy;
#define servopin 11
#define LDR1 A0
#define LDR2 A1
#define dt 10
#define tolerancia 15
int lecLDR1;
int lecLDR2;
int diferencia;
int posServo;

void setup() {
  servogy.attach(servopin);
  servogy.write(90);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  Serial.begin(9600);
  delay(dt);
}

void loop() {
 lecLDR1 = analogRead(LDR1);
 lecLDR2 = analogRead(LDR2);
 diferencia = abs(lecLDR1-lecLDR2);
 delay(dt);
  if(diferencia>tolerancia){
    if(lecLDR1>lecLDR2){
      posServo= posServo -10;
    }else{
      posServo= posServo +10;
    }
  }
  posServo = constrain(posServo, 0, 180);
  servogy.write(posServo);
  delay(dt);
  Serial.println(posServo);
  Serial.println(lecLDR1);
  Serial.println(lecLDR2);
}

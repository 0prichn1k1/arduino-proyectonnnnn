const int pinleds[] = {13, 12, 14, 27, 26, 25};
#define trigPin 4
#define echoPin 16
#define dt 10

int duracion;
int distancia;
int nivel;

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(pinleds[i], OUTPUT);
  }
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(dt);
  digitalWrite(trigPin, 0);
  
  duracion = pulseIn(echoPin, 1);
  distancia = duracion / 58.2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  distancia = constrain(distancia, 15, 100); 
  nivel = map(distancia, 15, 100, 6, 0);

  Serial.print("Nivel de LEDs: ");
  Serial.println(nivel);

  for (int i = 0; i < 6; i++) {
    if (i < nivel) {
      digitalWrite(pinleds[i], 1);
    } else {
      digitalWrite(pinleds[i], 0);
    }
  }

  delay(200);
}
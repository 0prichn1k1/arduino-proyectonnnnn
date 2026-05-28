
int sigPin = 2;
int dt = 10;
int duracion;
int distancia;
int nivel;
int serial = 9600;

void setup() {
  
  for (int i=0; i<6 ;i++){ pinMode (pinleds[i], OUTPUT); }
  Serial.begin(serial);

}

void loop() {

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

}

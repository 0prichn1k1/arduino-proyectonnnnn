int triggerPin = 2;
int echoPin = 3;
int dt = 10;
int duracion;
int distancia;

void setup() {

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {


  digitalWrite(triggerPin, 1);
  delay(dt);
  digitalWrite(triggerPin, 0);

  duracion = pulseIn(echoPin, 1);
  distancia = duracion/58.2;

}

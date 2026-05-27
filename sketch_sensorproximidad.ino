int pinleds[] = {8, 9, 10, 11, 12, 13};
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

  // constrain limita el valor para que no se salga del rango
  distancia = constrain(distancia, 15, 300);
  // Mapeamos: a menor distancia (5), más LEDs (6). A mayor (30), menos (0).
  int nivel = map(distancia, 15, 300, 6, 0);
  for (int i = nivel; nivel>0; nivel--){ digitalWrite (pinleds[nivel], 1); }
  Serial.println(nivel);

}

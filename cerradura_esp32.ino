// =============================================
//   CERRADURA ELECTROMECÁNICA CON DIAL
//   ESP32 - Arduino IDE
// =============================================

#include <LiquidCrystal.h>
#include <ESP32Servo.h>
#include <Stepper.h>

// ---- PINES ----
#define TRIG_PIN   15
#define ECHO_PIN    4
#define POT_PIN    34
#define SERVO_PIN  26
#define LED_PIN    22   // Conectar LED verde aquí (con resistencia 220Ω)

// LCD (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(16, 17, 5, 18, 19, 21);

// Servo
Servo servo;

// Motor paso a paso 28BYJ-48
// Orden correcto para ULN2003: IN1, IN3, IN2, IN4
#define PASOS_POR_VUELTA 2048
Stepper motor(PASOS_POR_VUELTA, 13, 14, 12, 27);

// ---- CONFIGURACIÓN ----
#define VALOR_CORRECTO  2048  // Valor exacto (rango 0-4095), cambiá si querés otro
#define TOLERANCIA        50  // Margen aceptable alrededor del valor correcto
#define DIST_ACTIVACION    5  // Distancia en cm para activar con la mano
#define SERVO_CERRADO      0  // Ángulo servo posición cerrada
#define SERVO_ABIERTO     90  // Ángulo servo posición abierta

// ---- ESTADO GLOBAL ----
bool potCorrecto = false;

// =============================================
//   FUNCIONES AUXILIARES
// =============================================

long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duracion == 0) return 999; // sin eco = lejos
  return duracion * 0.034 / 2;
}

void flashLEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    digitalWrite(LED_PIN, LOW);
    delay(150);
  }
  digitalWrite(LED_PIN, HIGH); // queda encendido al final
}

void abrirCerradura() {
  // 1. Servo destraba el pestillo
  servo.write(SERVO_ABIERTO);
  delay(600);

  // 2. Motor retira el perno principal
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retirando ->");
  motor.step(PASOS_POR_VUELTA / 2); // media vuelta = retirar perno

  // 3. LEDs de bienvenida
  flashLEDs();

  // 4. Mensaje final
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("** Bienvenido **");
  lcd.setCursor(0, 1);
  lcd.print("  Puerta abierta");
  delay(4000);

  // 5. Volver a cerrar
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cerrando...");
  motor.step(-(PASOS_POR_VUELTA / 2)); // vuelve el perno
  servo.write(SERVO_CERRADO);          // cierra el pestillo
  digitalWrite(LED_PIN, LOW);

  // 6. Resetear para nueva apertura
  potCorrecto = false;
  delay(500);
}

// =============================================
//   SETUP
// =============================================

void setup() {
  Serial.begin(115200);

  // LCD
  lcd.begin(16, 2);
  lcd.print("  Iniciando...");
  delay(1500);
  lcd.clear();

  // Pines
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Servo
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CERRADO);

  // Motor velocidad
  motor.setSpeed(10); // RPM
}

// =============================================
//   LOOP PRINCIPAL
// =============================================

void loop() {
  int valorPot = analogRead(POT_PIN);
  int diferencia = valorPot - VALOR_CORRECTO;

  // ---- PASO 1: Ajustar potenciómetro ----
  if (!potCorrecto) {
    lcd.setCursor(0, 0);
    lcd.print("Gira la perilla:");

    lcd.setCursor(0, 1);

    if (abs(diferencia) <= TOLERANCIA) {
      // Valor correcto alcanzado
      lcd.print("  >> CENTRO <<  ");
      potCorrecto = true;
      delay(600);
      lcd.clear();

    } else if (diferencia < 0) {
      // Hay que girar hacia la derecha (+)
      lcd.print(" Girar +  >>>>  ");

    } else {
      // Hay que girar hacia la izquierda (-)
      lcd.print(" <<<<  - Girar  ");
    }
  }

  // ---- PASO 2: Detectar mano ----
  if (potCorrecto) {
    lcd.setCursor(0, 0);
    lcd.print("Pasa la mano!   ");

    long distancia = medirDistancia();

    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    if (distancia < 100) lcd.print(" "); // espaciado
    lcd.print(distancia);
    lcd.print(" cm     ");

    Serial.print("Distancia: ");
    Serial.println(distancia);

    if (distancia > 0 && distancia <= DIST_ACTIVACION) {
      abrirCerradura();
    }
  }

  delay(100);
}

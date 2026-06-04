// =============================================
//   CERRADURA ELECTROMECÁNICA CON DIAL
//   ESP32 - Arduino IDE - Driver A4988
// =============================================

#include <LiquidCrystal.h>
#include <ESP32Servo.h>

// ---- PINES ----
#define TRIG_PIN   15
#define ECHO_PIN    4
#define POT_PIN    34
#define SERVO_PIN  26
#define LED_PIN    22

// Motor A4988
#define STEP_PIN   13
#define DIR_PIN    12

// LCD (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(16, 17, 5, 18, 19, 21);

// Servo
Servo servo;

// ---- CONFIGURACIÓN ----
#define VALOR_CORRECTO  2048  // Valor exacto potenciómetro (0-4095)
#define TOLERANCIA        50  // Margen aceptable
#define DIST_ACTIVACION    5  // cm para activar con la mano
#define SERVO_CERRADO      0  // Ángulo servo cerrado
#define SERVO_ABIERTO     90  // Ángulo servo abierto

// Motor: NEMA17 = 200 pasos/vuelta en full-step
#define PASOS_APERTURA   200  // 1 vuelta completa para retirar el perno
#define VELOCIDAD_PASO   800  // Microsegundos entre pulsos (más bajo = más rápido)

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
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracion == 0) return 999;
  return duracion * 0.034 / 2;
}

// Mover motor con A4988
// pasos positivos = adelante, negativos = atrás
void moverMotor(int pasos) {
  if (pasos > 0) {
    digitalWrite(DIR_PIN, HIGH);
  } else {
    digitalWrite(DIR_PIN, LOW);
    pasos = -pasos;
  }
  for (int i = 0; i < pasos; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(VELOCIDAD_PASO);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(VELOCIDAD_PASO);
  }
}

void flashLEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    digitalWrite(LED_PIN, LOW);
    delay(150);
  }
  digitalWrite(LED_PIN, HIGH);
}

void abrirCerradura() {
  // 1. Servo destraba el pestillo
  servo.write(SERVO_ABIERTO);
  delay(600);

  // 2. Motor retira el perno
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retirando ->");
  moverMotor(PASOS_APERTURA);

  // 3. LEDs de bienvenida
  flashLEDs();

  // 4. Mensaje
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
  moverMotor(-PASOS_APERTURA); // vuelve el perno
  servo.write(SERVO_CERRADO);
  digitalWrite(LED_PIN, LOW);

  // 6. Reset
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
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);

  // Servo
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CERRADO);
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
      lcd.print("  >> CENTRO <<  ");
      potCorrecto = true;
      delay(600);
      lcd.clear();
    } else if (diferencia < 0) {
      lcd.print(" Girar +  >>>>  ");
    } else {
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
    if (distancia < 100) lcd.print(" ");
    lcd.print(distancia);
    lcd.print(" cm     ");

    if (distancia > 0 && distancia <= DIST_ACTIVACION) {
      abrirCerradura();
    }
  }

  delay(100);
}

// Librerías necesarias
#include <LiquidCrystal.h>   // Librería estándar para controlar el LCD en modo paralelo
#include <Stepper.h>          // Librería estándar para controlar motores paso a paso

// pines del LCD 1602A (conexión en modo 4 bits)
const int pinRS = 8;         // pin RS del LCD: en LOW recibe comando, en HIGH recibe dato
const int pinEN = 9;         // pin EN del LCD: pulso de habilitación para escribir al display
const int pinD4 = 4;         // pin de dato D4 del LCD (modo 4 bits usa sólo D4–D7)
const int pinD5 = 5;         // pin de dato D5 del LCD
const int pinD6 = 6;         // pin de dato D6 del LCD
const int pinD7 = 7;         // pin de dato D7 del LCD

// pines del driver ULN2003 (motor 28BYJ-48)
// IMPORTANTE: el orden correcto para el 28BYJ-48 es IN1,IN3,IN2,IN4
// Si se pasan en otro orden el motor vibra pero no gira correctamente
const int pinIN1 = 10;       // Conectado a la entrada IN1 del driver
const int pinIN2 = 11;       // Conectado a la entrada IN2 del driver
const int pinIN3 = 12;       // Conectado a la entrada IN3 del driver
const int pinIN4 = 13;       // Conectado a la entrada IN4 del driver

// pin del buzzer pasivo
const int pinBuzzer = 3;     // pin PWM del Arduino conectado al pin de señal del buzzer

// Parámetros del motor 28BYJ-48
const int pasosPorVuelta = 2048;  // Pasos para una vuelta completa del eje de salida
                                     // (motor interno: 32 pasos * reducción 1:64 = 2048)
const int totalVueltas    = 5;     // Cantidad de vueltas a realizar en cada sentido
const int velocidad          = 10;    // Velocidad del motor

// Parámetros del LCD
const int columnasLCD = 16;      // Cantidad de columnas del display lcd
const int filasLCD = 2;       // Cantidad de filas del display lcd

// Parámetros del buzzer
const int frecDer   = 1000;    // Frecuencia en Hz del beep al girar hacia la derecha
const int frecIzq = 500;     // Frecuencia en Hz del beep al girar hacia la izquierda
const int duracionBeep = 150;     // Duración de cada beep en milisegundos

// Parámetros del ciclo general
const int dt = 1000;       // Tiempo de espera (ms) entre un ciclo completo y el siguiente

// Instanciación de objetos de las librerías
LiquidCrystal lcd(pinRS, pinEN, pinD4, pinD5, pinD6, pinD7);
// Crea el objeto lcd indicando qué pines del Arduino están conectados al LCD

Stepper motor(pasosPorVuelta, pinIN1, pinIN3, pinIN2, pinIN4);
// Crea el objeto motor con el total de pasos por vuelta y los pines en orden especial

// SETUP: se ejecuta una sola vez al encender o resetear el Arduino
void setup() {
  lcd.begin(columnasLCD, filasLCD);  // Inicializa el LCD indicando su tamaño físico (16x2)
  motor.setSpeed(velocidad);        // Establece la velocidad de giro del motor en RPM
  pinMode(pinBuzzer, OUTPUT);     // Configura el pin del buzzer como salida digital
}

// LOOP: se ejecuta en forma continua y repetida
void loop() {

  // BLOQUE 1: Girar 5 vueltas hacia la DERECHA
  for (int v = 1; v <= totalVueltas; v++) {   // Itera desde la vuelta 1 hasta totalVueltas

    lcd.clear();                                 // Borra todo el contenido visible del LCD
    lcd.setCursor(0, 0);                         // Mueve el cursor a columna 0, fila 0 (arriba izquierda)
    lcd.print("Vueltas a la");                   // Imprime la primera parte del mensaje en fila 0
    lcd.setCursor(0, 1);                         // Mueve el cursor a columna 0, fila 1 (fila inferior)
    lcd.print("derecha: ");                       // Imprime la dirección y los dos puntos en fila 1
    lcd.print(v);                                // Imprime el número de vuelta actual (1, 2, 3...)

    tone(pinBuzzer, frecDer, duracionBeep);   // Emite un tono de 1000 Hz durante DUR_BEEP ms

    motor.step(pasosPorVuelta);               // Gira el motor una vuelta completa en sentido horario (positivo)
  }                                              // Fin del for: repite hasta completar totalVueltas

  // BLOQUE 2: Girar 5 vueltas hacia la IZQUIERDA
  for (int v = 1; v <= totalVueltas; v++) {   // Itera desde la vuelta 1 hasta totalVueltas

    lcd.clear();                                 // Borra todo el contenido visible del LCD
    lcd.setCursor(0, 0);                         // Mueve el cursor a columna 0, fila 0
    lcd.print("Vueltas a la");                   // Imprime la primera parte del mensaje en fila 0
    lcd.setCursor(0, 1);                         // Mueve el cursor a columna 0, fila 1
    lcd.print("izquierda: ");                     // Imprime la dirección y los dos puntos en fila 1
    lcd.print(v);                                // Imprime el número de vuelta actual

    tone(pinBuzzer, frecIzq, duracionBeep); // Emite un tono de 500 Hz durante DUR_BEEP ms

    motor.step(-pasosPorVuelta);              // Gira el motor una vuelta completa en sentido antihorario (negativo)
  }                                              // Fin del for: repite hasta completar totalVueltas

  // BLOQUE 3: Pausa de 1 segundo antes de repetir el ciclo
  lcd.clear();                                   // Borra el LCD para mostrar el mensaje de pausa
  lcd.setCursor(0, 0);                           // Posiciona el cursor en columna 0, fila 0
  lcd.print("Esperando...");                      // Muestra al usuario que el sistema está en pausa
  noTone(pinBuzzer);                            // Detiene cualquier tono residual en el buzzer
  delay(dt);                            // Detiene la ejecución 1000 ms (1 segundo)

}

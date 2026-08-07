/*
TAREA #6 - SISTEMAS EMBEBIDOS
Ejercicio 1 - Gestion de Energia en el ESP32
Nombre: Saúl Alejandro Quiroz Vargas

Descripción del ejercicio:

La tarea activa corre un tiempo, luego el ESP32 entra en DEEP SLEEP
y despierta por TEMPORIZADOR o por un BOTON (pin externo).
Los estados se indican con LED RGB y mensajes por Serial.
*/

#include <Arduino.h>
#include "esp_sleep.h"

// Pines del LED RGB
#define PIN_ROJO   25
#define PIN_VERDE  26
#define PIN_AZUL   27

// Boton para despertar (uso el 33 porque sirve para ext0)
#define PIN_BOTON  GPIO_NUM_33

// Tiempos
#define SEG_A_US        1000000ULL   // paso de segundos a microsegundos
#define SEGUNDOS_DORMIR 10           // cuanto duerme antes de despertar solo
#define MS_TRABAJANDO   8000         // cuanto dura la parte "activa"

// Este contador no se borra al dormir porque vive en la memoria RTC
RTC_DATA_ATTR int vecesEncendido = 0;

// Prende o apaga cada color del LED
void ponerColor(bool rojo, bool verde, bool azul) {
  digitalWrite(PIN_ROJO, rojo);
  digitalWrite(PIN_VERDE, verde);
  digitalWrite(PIN_AZUL, azul);
}

// Revisa por que se desperto y lo muestra con un color
void mostrarMotivoDespertar() {
  esp_sleep_wakeup_cause_t motivo = esp_sleep_get_wakeup_cause();

  switch (motivo) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println(">> Me desperto el temporizador");
      ponerColor(false, false, true);   // azul
      break;
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println(">> Me desperto el boton");
      ponerColor(true, false, true);    // magenta
      break;
    default:
      Serial.println(">> Arranque normal (recien encendido)");
      ponerColor(false, false, false);  // apagado
      break;
  }
  delay(1500);   // dejo el color un ratito para alcanzar a verlo
}

// La parte donde el ESP32 hace su "trabajo": parpadea y manda datos
void hacerTrabajo() {
  Serial.println("== Trabajando... ==");
  unsigned long arranque = millis();
  int i = 0;

  // Repito hasta que pasen los segundos de trabajo
  while (millis() - arranque < MS_TRABAJANDO) {
    ponerColor(false, true, false);   // verde encendido
    Serial.printf("   Dato #%d = %d\n", i++, analogRead(34));
    delay(500);
    ponerColor(false, false, false);  // lo apago para que parpadee
    delay(500);
  }
  Serial.println("== Termine el trabajo ==");
}

// Deja listas las formas de despertar y manda a dormir al ESP32
void irADormir() {
  // Despertar solo despues de X segundos
  esp_sleep_enable_timer_wakeup(SEGUNDOS_DORMIR * SEG_A_US);
  Serial.printf("Voy a dormir %d segundos\n", SEGUNDOS_DORMIR);

  // Despertar tambien si aprieto el boton (cuando el pin se pone en ALTO)
  esp_sleep_enable_ext0_wakeup(PIN_BOTON, 1);
  Serial.println("Tambien despierto si aprieto el boton (GPIO33)");

  Serial.println(">> Durmiendo...");
  ponerColor(true, false, false);   // rojo = ya me voy a dormir
  delay(1000);
  ponerColor(false, false, false);  // apago antes de dormir
  Serial.flush();

  esp_deep_sleep_start();   // aqui se apaga; al despertar arranca de nuevo en setup()
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);

  vecesEncendido++;   // llevo la cuenta de cuantas veces desperto
  Serial.println("\n=============================================");
  Serial.printf("Veces que ha arrancado: %d\n", vecesEncendido);

  mostrarMotivoDespertar();
  hacerTrabajo();
  irADormir();
}

void loop() {
  // Queda vacio a proposito: como el Deep Sleep reinicia el chip,
  // todo el programa vuelve a correr desde setup().
}

# Tarea #6 - Ahorro de Energia en el ESP32 (Deep Sleep)

Este es mi Ejercicio 1 de Sistemas Embebidos. La idea es que el ESP32 trabaje
un rato, se duerma para gastar menos bateria y despierte cuando pasa un tiempo
o cuando aprieto un boton. En cada momento muestro en que estado esta usando un
LED RGB y mensajes por el monitor serie.

## Que hace

El ESP32 repite este ciclo una y otra vez:

1. Arranca y revisa por que se desperto (temporizador, boton o encendido normal).
2. Trabaja durante 8 segundos: parpadea en verde y va mandando datos por Serial.
3. Se prepara para dormir y entra en Deep Sleep.
4. Despierta a los 10 segundos por si solo, o antes si aprieto el boton, y vuelve
   a empezar.

Guardo un contador (`vecesEncendido`) en la memoria RTC, que es la unica que no
se borra al dormir, para llevar la cuenta de cuantas veces ha despertado.

## Colores del LED

| Color   | Que significa                      |
|---------|------------------------------------|
| Verde   | Esta trabajando                    |
| Rojo    | Ya se va a dormir                  |
| Azul    | Desperto por el temporizador       |
| Magenta | Desperto porque aprete el boton    |
| Apagado | Esta durmiendo o recien encendido  |

## Conexiones

| Componente              | Pin del ESP32   |
|-------------------------|-----------------|
| LED RGB - Rojo          | GPIO25          |
| LED RGB - Verde         | GPIO26          |
| LED RGB - Azul          | GPIO27          |
| LED RGB - Comun         | GND             |
| Boton                   | GPIO33 y 3V3    |
| Resistencia 10k (pull-down) | GPIO33 a GND |

La resistencia mantiene el GPIO33 en bajo cuando no aprieto nada, para que el
boton solo lo suba a alto al presionarlo y asi despierte al ESP32.

## Como probarlo

Necesitas PlatformIO en Visual Studio Code.

```bash
# Compilar
pio run

# Cargar al ESP32
pio run --target upload

# Ver los mensajes por Serial
pio device monitor
```

### Simularlo en Wokwi de VSC

1. De no tener la extención, instalar la extension Wokwi Simulator en VS Code.
2. Compila el proyecto primero, para que se genere el firmware.
3. Abre `diagram.json` y dale a Play para arrancar la simulacion.
4. Para probar el despertar por boton, aprieta el boton mientras esta dormido.

## Archivos del proyecto
esp32_deep_sleep/
├── platformio.ini # configuracion de PlatformIO
├── wokwi.toml # enlaza Wokwi con el firmware compilado
├── diagram.json # el circuito para la simulacion
└── src/
└── main.cpp 

## Enlace de consulta
Video de Youtube:





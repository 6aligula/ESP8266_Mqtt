# Controlador de Dispensador MQTT

Este proyecto permite controlar un dispensador usando un microcontrolador ESP8266, que recibe comandos a través de MQTT para activar o desactivar las funciones de soplar y succionar de un motor. Es ideal para aplicaciones donde se requiere control remoto y automatización de dispensadores.

## Características

- **Control Remoto**: Activa o desactiva las funciones del motor remotamente.
- **Integración MQTT**: Usa MQTT para recibir comandos de soplar y succionar.
- **Respuesta Inmediata**: Actúa sobre los comandos recibidos sin demora.

## Requisitos de Hardware

- ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
- Motor del dispensador
- Transistores o relés para controlar el motor
- Fuente de alimentación adecuada

## Dependencias de Software

- Arduino IDE o PlatformIO
- Biblioteca PubSubClient para MQTT
- Biblioteca ESP8266WiFi para manejar la conexión WiFi

## Configuración

1. **Conexión WiFi**: Configura las credenciales de tu red WiFi en el archivo `secrets.h`:
   ```cpp
   const char* ssid = "tu_ssid";
   const char* password = "tu_password";
   ```
2. **Servidor MQTT**: Define la dirección y las credenciales del servidor MQTT en `secrets.h`:
   ```cpp
   const char* mqtt_server = "direccion_del_servidor";
   const char* mqtt_user = "usuario_mqtt";
   const char* mqtt_password = "contraseña_mqtt";
   ```

## Instalación

1. Clona o descarga este repositorio.
2. Abre el proyecto en Arduino IDE o PlatformIO.
3. Instala las dependencias mencionadas.
4. Sube el código al microcontrolador ESP8266.

## Uso

El dispositivo se suscribe a los topics `dispensar` y `soplar` para recibir los comandos:

- **`dispensar`**: Envía '1' para activar la succión y '0' para desactivar.
- **`soplar`**: Envía '1' para activar la función de soplar y '0' para desactivar.

## Código Ejemplo

El siguiente código demuestra cómo se maneja el callback de MQTT:

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "dispensar") == 0) {
    if (payload[0] == '1') {
      digitalWrite(motor1, HIGH);  // Activar motor
    } else {
      digitalWrite(motor1, LOW);   // Desactivar motor
    }
  }
  if (strcmp(topic, "soplar") == 0) {
    if (payload[0] == '1') {
      digitalWrite(motor2, HIGH);  // Activar motor
    } else {
      digitalWrite(motor2, LOW);   // Desactivar motor
    }
  }
}
```

## Contribuir

Si deseas contribuir a este proyecto, por favor considera hacer un fork y enviar un pull request.

## Licencia

Este proyecto está licenciado bajo licencia MIT 

---
Suscribirse
```bash
mosquitto_sub -h localhost -t recibido -u user -P password
```
Testear:
```bash
mosquitto_pub -h 192.168.1.165 -t soplar -u user -P password -m "0"
```
```bash
mosquitto_pub -h 192.168.1.165 -t dispensar -u user -P password -m "0"
```






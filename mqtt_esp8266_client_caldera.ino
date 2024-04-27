#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

WiFiClient espClient;//Creamos un objeto de la clase cliente wifi
PubSubClient client(espClient);//
//definimos los pines
int motor1 = 5; // D1 motor 1 activa succecion
int motor2 = 4; // D2 motor 2 activa soplar
char dispensar[] = "dispensar";
char soplar[] = "soplar";

//Funcion para conectarnos a la red wifi
void setup_wifi() {

  delay(10);
  // Empezamos a conectar con el router de casa
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  //Mientras se esta conectando estamos imprimiendo puntos en la salida serial
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //iniciar semilla para la funcion ramdom
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//Funcion que se ejecuta cuando recibimos el mensaje
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, dispensar) == 0) {
    if ((char)payload[0] == '1') {
      digitalWrite(motor1, HIGH);
      //digitalWrite(motor2, LOW);
      client.publish("recibido", "1");
    } else {
      digitalWrite(motor1, LOW);
      //digitalWrite(motor2, HIGH);
      client.publish("recibido", "0");
    }
  }

  // Agregar una comparación para "soplar" si es necesario
  if (strcmp(topic, soplar) == 0) {
    if ((char)payload[0] == '1') {
      //digitalWrite(motor1, LOW);
      digitalWrite(motor2, HIGH);
      client.publish("recibido", "1");
    } else {
      //digitalWrite(motor1, HIGH);
      digitalWrite(motor2, LOW);
      client.publish("recibido", "0");
    }     
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publicar mensajes
      client.publish("recibido", "Bienvenido al control del dispensador de antibioticos to wapo hermano");
      // ... and resubscribe en el topic casa/caldera(la raspberry enviara las ordenes)
      client.subscribe("dispensar");
      client.subscribe("soplar");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//Primera funcion a ejecutarse en el programa
void setup() {
  pinMode(motor1, OUTPUT);  //Programar pin como salida
  pinMode(motor2, OUTPUT);
  digitalWrite(motor1, LOW);  //Ponemos la salida del pin en alto para que la cladera este en OFF al principio
  digitalWrite(motor2, LOW);
  Serial.begin(115200);// Configurar velocidad de comunicacion en bites por segundo 
  setup_wifi(); //Llama a la funcion para poner en marcha el cliente wifi
  client.setServer(mqtt_server, 1883);//Configurar el servidor mqtt_server al cual se conectara el cliente por el puerto 1883
  client.setCallback(callback);//Llamar a la funcion callback para procesar las ordenes del broker
  client.subscribe("dispensar");  // Suscripción al topic
  client.subscribe("soplar");
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

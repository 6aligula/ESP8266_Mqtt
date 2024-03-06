#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Información de nuestro WIFI
const char* ssid = "La wifi k lo k";
const char* password = "la password de la wifi k lo k";
const char* mqtt_server = "xu.pa.la.com";
const char* mqtt_user = "loko";
const char* mqtt_password = "eldiablo";

WiFiClient espClient;//Creamos un objeto de la clase cliente wifi
PubSubClient client(espClient);//
//definimos el pin 0 como controlador del rele
int ledPin = 0;

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
  //Mediante el bucle for mostramos por pantalla los mensajes contenidos en la variable payload
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Encender caldera si recibimos un 1 o apagar caldera si recibimos un 0
  if ((char)payload[0] == '1') {
    digitalWrite(ledPin, LOW);   // Activamos la caldera con low porque el hardware es de logica negada
    client.publish("test", "1");//publico un 1 para indicar que se ha encencido la caldera
  } else {
    digitalWrite(ledPin, HIGH);  // Apagamos la caldera con High porque el hardware es de logica negada
    client.publish("test", "0");//publico un 0 para indicar que se ha apagado la caldera
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
      client.publish("test", "Bienvenido al control de la caldera to wapo hermano");
      // ... and resubscribe en el topic casa/caldera(la raspberry enviara las ordenes)
      client.subscribe("test/return");
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
  pinMode(ledPin, OUTPUT);  //Programar pin como salida
  digitalWrite(ledPin, HIGH);  //Ponemos la salida del pin en alto para que la cladera este en OFF al principio
  Serial.begin(115200);// Configurar velocidad de comunicacion en bites por segundo 
  setup_wifi(); //Llama a la funcion para poner en marcha el cliente wifi
  client.setServer(mqtt_server, 1883);//Configurar el servidor mqtt_server al cual se conectara el cliente por el puerto 1883
  client.setCallback(callback);//Llamar a la funcion callback para procesar las ordenes del broker
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

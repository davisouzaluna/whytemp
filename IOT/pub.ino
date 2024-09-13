#define DHTPIN 2 //pino de dados do sensor dht11


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqttServer = "127.0.0.1";  // Ou um domínio, ex: "mqtt.eclipse.org". Ou simplesmente o IP do broker.
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";


// Variáveis para armazenar valores máximos e mínimos
float maxTemperature = -100;
float minTemperature = 100;
float maxHumidity = 0;
float minHumidity = 100;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao Broker MQTT");
    } else {
      Serial.print("Falha na conexão com o Broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Lida com as mensagens recebidas (se necessário)
}//irei deixar essa função aqui, mas não será utilizada. Uma possível utilização seria uma mudança de ação ou uma outra subscrição, a depender do contexto da aplicação.


void loop() {
  // Inicia a comunicação com o sensor
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(40);
  
  pinMode(DHTPIN, INPUT);
  
  // Espera até o sensor responder
  while (digitalRead(DHTPIN) == HIGH);
  while (digitalRead(DHTPIN) == LOW);
  while (digitalRead(DHTPIN) == HIGH);
  
  // Lê os dados do sensor
  int data[5];
  for (int i = 0; i < 5; i++) {
    data[i] = 0;
    for (int j = 0; j < 8; j++) {
      while (digitalRead(DHTPIN) == LOW);
      unsigned long startTime = micros();
      while (digitalRead(DHTPIN) == HIGH);
      unsigned long duration = micros() - startTime;
      data[i] |= (duration > 40) << (7 - j);
    }


  }
  
  // Verifica a validade dos dados(o 5° byte é o byte de verificação)
  int checksum = data[0] + data[1] + data[2] + data[3];
  if (checksum == data[4]) {
    float humidity = data[0] + data[1] / 10.0;
    float temperature = data[2] + data[3] / 10.0;
    
    // Atualiza os valores máximos e mínimos
    if (temperature > maxTemperature) {
      maxTemperature = temperature;
    }
    if (temperature < minTemperature) {
      minTemperature = temperature;
    }
    if (humidity > maxHumidity) {
      maxHumidity = humidity;
    }
    if (humidity < minHumidity) {
      minHumidity = humidity;
    }
    // Mostra os valores
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print("°C - Umidade: ");
    Serial.print(humidity);
    Serial.println("%");
    
    // Exibe valores máximos e mínimos
    Serial.print("Máx Temp: ");
    Serial.print(maxTemperature);
    Serial.print("°C - Min Temp: ");
    Serial.print(minTemperature);
    Serial.print("°C - Máx Umidade: ");
    Serial.print(maxHumidity);
    Serial.print("% - Min Umidade: ");
    Serial.print(minHumidity);
    Serial.println("%");
  } else {
    Serial.println("Erro na leitura dos dados.");
  }

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  // Publica uma mensagem no tópico
  String payloadStr = "Dados a serem enviados";
  client.publish("dataSet", payloadStr.c_str());
  
  delay(2000); // Aguarda 2 segundos antes de fazer a próxima leitura e intervalo de envio
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Tentando reconectar ao Broker MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao Broker MQTT");
    } else {
      Serial.print("Falha na conexão com o Broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

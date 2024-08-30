#include <ESP8266WiFi.h>  // Ou <WiFi.h> se estiver usando um ESP32
#include <PubSubClient.h> // Biblioteca para comunicação MQTT (lembre-se de baixar antes de compilar na IDE)

// A biblioteca tem o autor O'Leary, Nick. "PubSubClient". Disponível em: https://pubsubclient.knolleary.net/ Acesso em: 30 de agosto de 2024.

/*
A escolha pelo MQTT é pelo motivo de que eu não quero me preocupar com a comunicação entre os dispositivos, que é o que o MQTT faz por mim.
Caso eu fizesse com TCP ou UDP eu teria que criar uma API ou abstração para o contexto do projeto, o que não é o foco.
O MQTT é um protocolo de mensagens leve, que é ideal para comunicação entre dispositivos IoT.
Ele é baseado em um modelo de publicação e assinatura, onde os dispositivos se conectam a um broker MQTT e enviam mensagens para tópicos específicos.
Outros dispositivos podem se inscrever nesses tópicos para receber as mensagens.
*/

// Configurações do Wi-Fi
const char* ssid = ""; // Nome da sua rede Wi-Fi
const char* password = ""; // Senha da sua rede Wi-Fi

// Configurações do MQTT
const char* mqttServer = "192.168.3.4";// Veja qual o Ip da sua máquina real(endereço da interface real, seja ethernet ou wifi), que esteja conectada a mesma rede que o ESP
const int mqttPort = 1883;
const char* mqttUser = "YOUR_MQTT_USER";    // Se necessário. Nesse caso não usei
const char* mqttPassword = "YOUR_MQTT_PASS"; // Se necessário. Nesse caso não usei
const char* temperatureTopic = "sensor/temperature";
const char* humidityTopic = "sensor/humidity";

#define DHTPIN D2  // D2 é GPIO 4 no ESP8266 ou GPIO 2 no ESP32
#define DHTTYPE 11 // Defina o tipo de sensor (DHT11)

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis para armazenar valores máximos e mínimos
float maxTemperature = -100;
float minTemperature = 100;
float maxHumidity = 0;
float minHumidity = 100;

void setup() {
  Serial.begin(9600);
  
  // Configura o Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi");

  // Configura o MQTT
  client.setServer(mqttServer, mqttPort);
  
  // Verifica a conexão com o MQTT
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESPClient", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao MQTT");
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tente novamente em 5 segundos");
      delay(5000);
    }
  }
}

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
  int data[5] = {0}; // Inicializa o array com zeros
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      while (digitalRead(DHTPIN) == LOW); // Espera para o sinal subir
      unsigned long startTime = micros();
      while (digitalRead(DHTPIN) == HIGH); // Espera para o sinal cair
      unsigned long duration = micros() - startTime;
      data[i] |= (duration > 40) << (7 - j);
    }
  }
  
  // Verifica a validade dos dados (o 5° byte é o byte de verificação)
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
    
    // Envia os dados para o broker MQTT
    if (client.connected()) {
      String tempStr = String(temperature);
      String humStr = String(humidity);
      
      // Publica a temperatura
      if (client.publish(temperatureTopic, tempStr.c_str())) {
        Serial.println("Temperatura enviada com sucesso.");
      } else {
        Serial.println("Falha ao enviar temperatura.");
      }
      
      // Publica a umidade
      if (client.publish(humidityTopic, humStr.c_str())) {
        Serial.println("Umidade enviada com sucesso.");
      } else {
        Serial.println("Falha ao enviar umidade.");
      }
    } else {
      Serial.println("Não conectado ao MQTT, tentando reconectar...");
      if (client.connect("ESPClient", mqttUser, mqttPassword)) {
        Serial.println("Reconectado ao MQTT");
      }
    }
  } else {
    Serial.println("Erro na leitura dos dados.");
  }
  
  delay(1000); // Aguarda 1 segundo antes de fazer a próxima leitura
}

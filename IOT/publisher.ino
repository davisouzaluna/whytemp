#define DHTPIN D2        // D2 é GPIO 4 no ESP8266 ou GPIO 2 no ESP32)
#define DHTTYPE 11     

// Variáveis para armazenar valores máximos e mínimos
float maxTemperature = -100;
float minTemperature = 100;
float maxHumidity = 0;
float minHumidity = 100;

void setup() {
  Serial.begin(9600);
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
  } else {
    Serial.println("Erro na leitura dos dados.");
  }
  
  delay(1000); // Aguarda 2 segundos antes de fazer a próxima leitura
}

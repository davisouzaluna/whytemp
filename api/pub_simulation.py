import paho.mqtt.client as mqtt
import random
import time

# Configurações do MQTT
mqtt_server = "127.0.0.1"  
mqtt_port = 1883
#mqtt_user = "YOUR_MQTT_USER"  # Se necessário
#mqtt_password = "YOUR_MQTT_PASS"  # Se necessário
temperature_topic = "sensor/temperature"
humidity_topic = "sensor/humidity"

# Funções de callback
def on_connect(client, userdata, flags, rc):
    print(f"Conectado ao MQTT com código de retorno {rc}")
    # Inscrever-se em tópicos, se necessário
    # client.subscribe("outro/topico")

def on_publish(client, userdata, mid):
    print("Mensagem publicada com ID:", mid)

# Configuração do cliente MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_publish = on_publish

# Configuração das credenciais MQTT
#client.username_pw_set(mqtt_user, mqtt_password)

# Conectar ao broker MQTT
client.connect(mqtt_server, mqtt_port, 60)
client.loop_start()

try:
    while True:
        # Gerar dados simulados de temperatura e umidade
        temperature = round(random.uniform(15.0, 30.0), 2)
        humidity = round(random.uniform(30.0, 60.0), 2)
        
        # Publicar dados no broker MQTT
        client.publish(temperature_topic, str(temperature))
        client.publish(humidity_topic, str(humidity))
        
        print(f"Temperatura: {temperature}°C - Umidade: {humidity}%")
        
        # Aguarda 1 segundo antes da próxima publicação
        time.sleep(1)
except KeyboardInterrupt:
    print("\nInterrompido pelo usuário")
finally:
    client.loop_stop()
    client.disconnect()

import paho.mqtt.client as mqtt

# Configurações do broker MQTT
broker = '127.0.0.1'
port = 1883
topic = [("sensor/temperature",0),("sensor/humidity",0)] #a variável topic é uma lista de tuplas, onde cada tupla é um tópico e um QoS associado
client_id = f'device-humidity_whytemp'# O ID deve ser único. Futuramente o MAC do dispositivo pode ser o ID dele

# Callback de conexão
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT com sucesso!")
        client.subscribe(topic)
    else:
        print(f"Falha na conexão, código {rc}")

# Callback da mensagem recebida
def on_message(client, userdata, msg):
    print(f"Mensagem recebida no tópico '{msg.topic}': {msg.payload.decode()}")

client = mqtt.Client(client_id)
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port)
#Loop principal
client.loop_forever()

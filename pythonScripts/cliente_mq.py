import time
import paho.mqtt.client as mqtt

broker_address = "34.230.38.53"  # reemplazar  actual

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)  # crea nueva instancia

client.connect(broker_address)  # conecta al servidor
while True:
    client.publish("austral","hola")
    print("hola")
    time.sleep(0.1)


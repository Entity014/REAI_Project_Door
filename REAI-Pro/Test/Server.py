import paho.mqtt.client as mqtt

host = "broker.emqx.io"
port = 1883

def on_connect(self, client, userdata, rc):
    print("MQTT Connected.")
    self.subscribe("reai/door/b1")
    
def on_message(client, userdata, msg):
    payload = msg.payload.decode("utf-8", "strict")
    if (payload == "On"):
        client.publish("reai/pc/hand","HELLO MQTT")
    print(payload)

client = mqtt.Client()
# client.on_connect = on_connect
# client.on_message = on_message
client.connect(host)
client.publish("reai/pc/hand","HELLO MQTT2")
# client.loop_forever()
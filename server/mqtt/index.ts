import mqtt, { MqttClient } from "mqtt";
import { handleMessage } from "./messageHandlers";
import dotenv from "dotenv";

dotenv.config();
const urlBrokerMqtt = process.env.URL_BROKER_MQTT;
if (!urlBrokerMqtt) {
  console.error("URL_BROKER_MQTT environment variable is not defined.");
  process.exit(1);
}
const client: MqttClient = mqtt.connect(urlBrokerMqtt);

client.on("connect", () => {
  console.log("Connected to MQTT Broker");
});

client.on("message", async (topic: string, message: Buffer) => {
  console.log("Message received: ", message.toString());
  handleMessage(topic, message);
});

export default client;

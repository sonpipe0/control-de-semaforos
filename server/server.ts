import express, {Express, Request, Response} from "express";
import mongoose, {Connection} from "mongoose";
import userRouter from "./routes/user";
import semaphoreRouter from "./routes/semaphoreRouter";
import SemaphoreSchema from "./models/semaphore";
import mqtt from "mqtt";

require("dotenv").config();
const app = express();

const urlBrokerMqtt = process.env.URL_BROKER_MQTT;
if (!urlBrokerMqtt) {
    console.error("URL_BROKER_MQTT environment variable is not defined.");
    process.exit(1);
}
const client = mqtt.connect(urlBrokerMqtt);

//config
const mongoDB = process.env.MONGO_DB;
if (!mongoDB) {
    console.error("MongoDB connection string is not provided.");
    process.exit(1);
}
app.use(express.json()); // To parse JSON bodies
mongoose
    .connect(mongoDB)
    .then(() => console.log("Connected to MongoDB successfully."))
    .catch((err) => {
        console.error("Error while connecting to MongoDB:", err);
        process.exit(1);
    });
const db: Connection = mongoose.connection;

const topic: mqtt.MqttClient = client.subscribe("semaphore/create");

client.on('message', async (topic, message) => {
        if (topic === "semaphore/create") {
            console.log("message", message.toString());
        }
        message = JSON.parse(message.toString());
        const semaphore = new SemaphoreSchema(message);
        try {
            await semaphore.save();
            console.log("Semaphore saved successfully");
        } catch (err: any) {
            console.error("Error while saving semaphore", err);
        }
    }
);

app.use("/user", userRouter);
app.use("/semaphore", semaphoreRouter)

app.listen(3000, () => {
    console.log("Server is running on port 3000");
});

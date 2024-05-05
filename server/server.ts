import express, { Express, Request, Response } from "express";
import semaphoreRouter from "./controllers/semaphoreController";
import mqtt from "mqtt";
import auth from "./controllers/authController";
import { connectToDatabase } from "./config/database";
import { filterBySessionId } from "./middleware/sessionFilter";
import client from "./mqtt";

require("dotenv").config();
const app = express();

app.use(express.json()); // To parse JSON bodies

connectToDatabase();

const topic: mqtt.MqttClient = client.subscribe("semaphore/create");

app.all("*", filterBySessionId);

app.use("/auth", auth);
app.use("/semaphore", semaphoreRouter);

app.listen(3000, () => {
  console.log("Server is running on port 3000");
});

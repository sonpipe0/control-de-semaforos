import express, { Express, Request, Response } from "express";
import mongoose, { Connection } from "mongoose";
import semaphoreRouter from "./routes/semaphoreRouter";
import mqtt from "mqtt";
import { SessionSchema } from "./models/SessionSchema";
import SemaphoreSchema from "./models/semaphore";
import auth from "./routes/auth";


require("dotenv").config();
const app = express();

function filterBySessionId(request: Request): Promise<boolean> {
	let sessionId = request.headers["Authorization"];
	if (!sessionId) {
		return Promise.resolve(false);
	}
	if (sessionId.includes("Bearer ")) {
		sessionId = sessionId.slice(7);
	}
	let user;
	try {
		let Session = mongoose.model("Session", SessionSchema);
		let user = Session.findOne({ sessionId });
	} catch (error) {
		console.error("Error while filtering by session id:", error);
		return Promise.resolve(false);
	}
	return Promise.resolve(!!user);
}

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

app.use("/auth", auth);

app.use(async (req, res, next) => {
	const isValidSession = await filterBySessionId(req);
	if (!isValidSession) {
		res.status(401).send("Invalid session or unauthorized");
	} else {
		next();
	}
});

app.use("/semaphore", semaphoreRouter);

client.on("message", async (topic: string, message: Buffer) => {
	if (topic === "semaphore/create") {
		console.log("message", message.toString());

		let json = JSON.parse(message.toString());

		//create a default semaphore
		let semaphore = {
			name: json.name,
			red_time: 30,
			green_time: 30,
			operating_time: [
				{
					day: "Monday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Tuesday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Wednesday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Thursday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Friday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Saturday",
					start_time: "00",
					end_time: "23",
				},
				{
					day: "Sunday",
					start_time: "00",
					end_time: "23",
				},
			],
		};
		try {
			let semaphoreObject = new SemaphoreSchema(semaphore);
			await semaphoreObject.save();
		} catch (error) {
			console.error("Error while creating semaphore", error);
		}
	}
});

app.listen(3000, () => {
	console.log("Server is running on port 3000");
});

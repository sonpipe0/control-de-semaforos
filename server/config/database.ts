//config
import mongoose, { Connection } from "mongoose";

export function connectToDatabase(): void {
	const mongoDB = process.env.MONGO_DB;
	if (!mongoDB) {
		console.error("MongoDB connection string is not provided.");
		process.exit(1);
	}
	mongoose
		.connect(mongoDB)
		.then(() => console.log("Connected to MongoDB successfully."))
		.catch((err) => {
			console.error("Error while connecting to MongoDB:", err);
			process.exit(1);
		});
	const db: Connection = mongoose.connection;
}

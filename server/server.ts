import express, { Express, Request, Response } from "express";
import mongoose, {Connection} from "mongoose";
import userRouter from "./routes/user";

require("dotenv").config();
const app = express();

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
const db:Connection = mongoose.connection;


// Routes

app.use("/user", userRouter);

app.listen(3000, () => {
  console.log("Server is running on port 3000");
});

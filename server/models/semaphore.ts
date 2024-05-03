import mongoose from "mongoose";
import { StatusType } from "../types/statusTypes";
import { DayObjectSchema } from "./DayObjectSchema";

const semaphore = new mongoose.Schema({
	name: {
		required: true,
		unique: true,
		type: String,
	},
	status: {
		required: true,
		type: StatusType,
		default: StatusType.NORMAL,
	},
	green_time: {
		required: true,
		type: Number,
	},
	red_time: {
		required: true,
		type: Number,
	},

	operating_time: {
		required: true,
		type: [DayObjectSchema],
	},
});

export default mongoose.model("Semaphore", semaphore);

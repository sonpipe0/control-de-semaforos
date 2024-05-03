import mongoose from "mongoose";
import { randomUUID } from "node:crypto";

export const SessionSchema = new mongoose.Schema({
	user_id: {
		type: String,
		required: true,
	},
	session_id: {
		type: String,
		required: true,
	},
	expires: {
		type: Date,
		required: true,
	},
});

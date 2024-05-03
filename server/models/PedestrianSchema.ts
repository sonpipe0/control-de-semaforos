import mongoose from "mongoose";

const PedestrianSchema = new mongoose.Schema({
	id: {
		type: String,
		unique: true,
	},
	name: {
		type: String,
		required: true,
	},
	time: {
		type: Date,
		required: true,
	},
});

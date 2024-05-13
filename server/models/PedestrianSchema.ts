import mongoose from "mongoose";

const PedestrianSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  time: {
    type: Date,
    required: true,
  },
});

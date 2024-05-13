import mongoose from "mongoose";

export interface Obstruction {
  semaphore_id: string;
  time: Date;
  handled: boolean;
  description: string;
}

export const ObstructionSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  time: {
    type: Date,
    required: true,
  },
  handled: {
    type: Boolean,
    required: true,
    default: false,
  },
});

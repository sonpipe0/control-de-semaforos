import mongoose from "mongoose";

export interface Obstruction {
  semaphore_id: string;
  time: Date;
  handled: boolean;
  description: string;
}

export const ObstructionSchema = new mongoose.Schema({
  id: {
    type: String,
    unique: true,
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

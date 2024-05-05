import mongoose, { Schema } from "mongoose";
import { ObstructionSchema } from "./ObstructionSchema";

const log = new mongoose.Schema({
  semaphore_id: {
    type: String,
    required: true,
  },
  log: {
    type: Schema.Types.Mixed,
    required: false,
  },
});

import mongoose, { Schema } from "mongoose";

interface DayObject {
  day: string;
  open: string;
  close: string;
}

const DayObjectSchema: Schema = new mongoose.Schema({
  day: {
    type: String,
    required: true,
  },
  open: {
    type: String,
    required: true,
  },
  close: {
    type: String,
    required: true,
  },
});

export { DayObjectSchema, DayObject };

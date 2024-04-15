import mongoose from "mongoose";
import {v4 as uuidv4} from "uuid";

const semaphore = new mongoose.Schema({
    id: {
        type: String,
        default: uuidv4,
        unique: true,
    },
    name: {
        required: true,
        unique: true,
        type: String,
    },
    isObstructed: {
        required: true,
        type: Boolean,
    },
    green_time: {
        required: true,
        type: Number,
    },
    red_time: {
        required: true,
        type: Number,
    },
    yellow_time: {
        required: true,
        type: Number,
    }

});

export default mongoose.model("Semaphore", semaphore);
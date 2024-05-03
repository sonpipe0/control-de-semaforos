import express, { Router, Request, Response } from "express";
import SemaphoreSchema from "../models/semaphore";
import { StatusType } from "../types/statusTypes";
import { DayObject } from "../models/DayObjectSchema";

const router = Router();

// Define the User type
type Semaphore = {
	id: string;
	name: string;
	isObstructed: boolean;
	green_time: number;
	red_time: number;
	yellow_time: number;
};

// Create a new user
router.post("/create", async (req: Request, res: Response) => {
	const {
		name,
		green_time,
		red_time,
		active_time,
	}: {
		name: string;
		green_time: number;
		red_time: number;
		active_time: [DayObject];
	} = req.body;

	const semaphore = new SemaphoreSchema({
		name,
		green_time,
		red_time,
		operating_time: active_time,
	});
	let isValid: boolean = true;
	for (let i = 0; i < active_time.length; i++) {
		if (
			active_time[i].open > active_time[i].close ||
			!validateHour(active_time[i].open)
		) {
			isValid = false;
			break;
		}
	}
	if (!isValid) {
		return res.status(400).json({ message: "Invalid operating time" });
	}
	try {
		await semaphore.save();
		res.status(201).json({ message: "Semaphore created successfully" });
	} catch (err: any) {
		res.status(400).json({ message: err.message });
	}
});

router.post("/update-status", async (req: Request, res: Response) => {
	const { id, status }: { id: string; status: string } = req.body;
	const semaphore = await SemaphoreSchema.findOne({ id });
	if (!semaphore) {
		return res.status(404).json({ message: "Semaphore not found" });
	}
	let actualStatus: StatusType = StatusType[status as keyof typeof StatusType];
	try {
		await SemaphoreSchema.updateOne({ id }, { status: actualStatus });
		res.status(200).json({ message: "Semaphore updated successfully" });
	} catch (err: any) {
		res.status(400).json({ message: err.message });
	}
});

function validateHour(time: string) {
	const hourRegex: RegExp = /^(0[0-9]|1[0-9]|2[0-3])$/;
	return hourRegex.test(time);
}

router.post("/update-active-time", async (req: Request, res: Response) => {
	const {
		id,
		day,
		open,
		close,
	}: { id: string; day: string; open: string; close: string } = req.body;
	const semaphore = await SemaphoreSchema.findOne({ id });
	if (!semaphore) {
		return res.status(404).json({ message: "Semaphore not found" });
	}
	if (validateHour(open) && validateHour(close)) {
		try {
			await SemaphoreSchema.updateOne(
				{ id, "operating_time.day": day },
				{
					$set: {
						"operating_time.$.open": open,
						"operating_time.$.close": close,
					},
				},
			);
			res.status(200).json({ message: "Semaphore updated successfully" });
		} catch (err: any) {
			res.status(400).json({ message: err.message });
		}
	}
});

export default router;

import express, { Router, Request, Response } from "express";
import SemaphoreSchema from "../models/semaphore";
import { StatusType } from "../types/statusTypes";
import { DayObject } from "../models/DayObjectSchema";
import {
  createSemaphore, getSemaphoreInformation,
  updateSemaphoreActiveTime,
  updateSemaphoreStatus, UpdateSemaphoreTiming,
} from "../services/semaphoreService";

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
  const { status, body }: { status: number; body: any } =
    await createSemaphore(req);
  res.status(status).json(body);
});

router.post("/update-status", async (req: Request, res: Response) => {
  const { status, body }: { status: number; body: any } =
    await updateSemaphoreStatus(req);
  res.status(status).json(body);
});

router.post("/update-active-time", async (req: Request, res: Response) => {
  const { status, body }: { status: number; body: any } =
    await updateSemaphoreActiveTime(req);
  res.status(status).json(body);
});

router.get("/get-semaphore", async (req: Request, res: Response) => {
  const {status, body}: {status: number; body: any} = await getSemaphoreInformation(req);
    res.status(status).json(body);
});

router.post("/update-timing", async (req: Request, res: Response) => {
  const {status, body} : {status: number; body: any} = await UpdateSemaphoreTiming(req);
    res.status(status).json(body);
});

export default router;

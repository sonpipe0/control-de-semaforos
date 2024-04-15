import express,{Router,Request,Response} from "express";
import SemaphoreSchema from "../models/semaphore";

const router = Router();

// Define the User type
type Semaphore = {
    id: string,
    name: string,
    isObstructed: boolean,
    green_time: number,
    red_time: number,
    yellow_time: number,
}

// Create a new user
router.post("/create", async (req: Request, res:Response) => {
    const data: Semaphore = req.body;
    const semaphore = new SemaphoreSchema(data);

    try {
        await semaphore.save();
        res.status(201).json(semaphore);
    } catch (err: any) {
        res.status(400).json({message: err.message});
    }
});


export default router;
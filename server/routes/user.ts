import express,{Router,Request,Response} from "express";
import UserSchema from "../models/userMongoSchema";

const router = Router();

// Define the User type
type User = {
    username: string;
    name: string;
    age: number;
    email: string;
};

// Create a new user
router.post("/create", async (req: Request, res:Response) => {
    const user: User = req.body;
    const newUser = new UserSchema(user);

    try {
        await newUser.save();
        res.status(201).json(newUser);
    } catch (err: any) {
        res.status(400).json({message: err.message});
    }
});


export default router;
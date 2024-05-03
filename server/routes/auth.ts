import express, { Router, Request, Response } from "express";
import UserSchema from "../models/userSchema";
import { SessionSchema } from "../models/SessionSchema";
import { randomUUID } from "node:crypto";
import mongoose from "mongoose";
import { StatusCodes } from "http-status-codes";
const router = Router();

// Define the User type
type User = {
    username: string;
    name: string;
    password: string;
    age: number;
    email: string;
    isAdminToken: string;
};

type UserLogin = {
    username: string;
    email: string;
    password: string;
};

// Create a new user
router.post("/create", async (req: Request, res: Response) => {
    const user: User = req.body;
    let newUser;
    if (user.isAdminToken == process.env.ADMIN_TOKEN) {
        newUser = new UserSchema({
            username: user.username,
            name: user.name,
            password: user.password,
            age: user.age,
            email: user.email,
            isAdmin: true,
        });
    } else {
        newUser = new UserSchema({
            username: user.username,
            name: user.name,
            password: user.password,
            age: user.age,
            email: user.email,
            isAdmin: false,
        });
    }

    try {
        await newUser.save();
        res.status(StatusCodes.OK).json(newUser);
    } catch (err: any) {
        res.status(400).json({ message: err.message });
    }
});

router.post("/login", async (req: Request, res: Response) => {
    const user: UserLogin = req.body;
    const userFound = await UserSchema.findOne({
        $or: [{ username: user.username }, { email: user.email }],
    });
    if (!userFound) {
        return res
            .status(StatusCodes.NOT_FOUND)
            .json({ message: "User not found" });
    }
    if (userFound.password !== user.password) {
        return res
            .status(StatusCodes.UNAUTHORIZED)
            .json({ message: "Invalid credentials" });
    }
    const SessionModel = mongoose.model("Session", SessionSchema);
    const session = new SessionModel({
        user_id: userFound.id,
        session_id: randomUUID(),
        expires: new Date(Date.now() + 1000 * 60 * 60 * 24),
    });

    try {
        await session.save();
        res.status(200).json(session);
    } catch (err: any) {
        res.status(400).json({ message: err.message });
    }
    res.status(200).json(userFound);
});

export default router;

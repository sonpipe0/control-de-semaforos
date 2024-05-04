import UserSchema from "../models/userSchema";
import { Request, Response } from "express";
import { StatusCodes } from "http-status-codes";
import mongoose from "mongoose";
import { SessionSchema } from "../models/SessionSchema";
import { randomUUID } from "node:crypto";

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

export async function createUser(
	req: Request,
): Promise<{ status: number; body: any }> {
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
		return { status: StatusCodes.OK, body: newUser };
	} catch (err: any) {
		return { status: 400, body: { message: err.message } };
	}
}

export async function loginUser(
	req: Request,
): Promise<{ status: number; body: any }> {
	const user: UserLogin = req.body;
	const userFound = await UserSchema.findOne({
		$or: [{ username: user.username }, { email: user.email }],
	});
	if (!userFound) {
		return {
			status: StatusCodes.NOT_FOUND,
			body: { message: "User not found" },
		};
	}
	if (userFound.password !== user.password) {
		return {
			status: StatusCodes.UNAUTHORIZED,
			body: { message: "Invalid password" },
		};
	}
	const SessionModel = mongoose.model("Session", SessionSchema);
	const session = new SessionModel({
		user_id: userFound.id,
		session_id: randomUUID(),
		expires: new Date(Date.now() + 1000 * 60 * 60 * 24),
	});

	try {
		await session.save();
		return { status: StatusCodes.OK, body: session };
	} catch (err: any) {
		return { status: 400, body: { message: err.message } };
	}
}

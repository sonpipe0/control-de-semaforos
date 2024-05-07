import UserSchema from "../models/userSchema";
import { Request, Response } from "express";
import { StatusCodes } from "http-status-codes";
import mongoose, { Model } from "mongoose";
import { SessionSchema } from "../models/SessionSchema";
import { randomUUID } from "node:crypto";
import dotenv from "dotenv";

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
  dotenv.config();
  const user: User = req.body;
  let newUser;
  if (await UserSchema.findOne({ username: user.username })) {
    return {
      status: StatusCodes.BAD_REQUEST,
      body: { message: "Username already exists" },
    };
  }
  if (await UserSchema.findOne({ email: user.email })) {
    return {
      status: StatusCodes.BAD_REQUEST,
      body: { message: "Email already exists" },
    };
  }
  console.log(user.isAdminToken)
  newUser = new UserSchema({
    username: user.username,
    name: user.name,
    password: user.password,
    age: user.age,
    email: user.email,
    isAdmin: user.isAdminToken === process.env.IS_ADMIN_PASSWORD,
  });

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
  const sessionFound = await SessionModel.findOne(
    { user_id: userFound.id },
    { _id: 1 },
  );
  if (sessionFound) {
    await sessionFound.deleteOne();
  }
  const session = new SessionModel({
    user_id: userFound.id,
    expires: new Date(Date.now() + 1000 * 60 * 60 * 24),
  });

  await session.save();
  const sessionSaved = await SessionModel.findOne(
    { user_id: userFound.id },
    { _id: 1 },
  );

  if (sessionSaved != null) {
    const session_id = sessionSaved._id;
    return { status: StatusCodes.OK, body: { session_id } };
  } else {
    return {
      status: StatusCodes.INTERNAL_SERVER_ERROR,
      body: { message: "Internal server error" },
    };
  }
}

export const refresh = async (req: Request) => {
  const SessionModel = mongoose.model("Session", SessionSchema);
  const session_id: string = req.body.session_id;
  const session = await SessionModel.findOne({ _id: session_id });
  if (!session) {
    return {
      status: StatusCodes.UNAUTHORIZED,
      body: { message: "Session not found" },
    };
  }
  if (session.expires < new Date()) {
    await session.deleteOne();
    return {
      status: StatusCodes.UNAUTHORIZED,
      body: { message: "Session expired, you should Login again" },
    };
  } else {
    const user = await UserSchema.findOne({ _id: session.user_id });
    if (!user) {
      return {
        status: StatusCodes.NOT_FOUND,
        body: { message: "User not found" },
      };
    }
    await session.updateOne({
      expires: new Date(Date.now() + 1000 * 60 * 60 * 24),
    });

    const body = {
      user_id: user._id,
      username: user.username,
      name: user.name,
      age: user.age,
      email: user.email,
      isAdmin: user.isAdmin,
    };
    return { status: StatusCodes.OK, body };
  }
};

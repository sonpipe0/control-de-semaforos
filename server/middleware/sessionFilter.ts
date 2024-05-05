import { NextFunction, Request, Response } from "express";
import mongoose from "mongoose";
import { SessionSchema } from "../models/SessionSchema";

export async function filterBySessionId(
  req: Request,
  res: Response,
  next: NextFunction,
): Promise<void> {
  if (req.path.includes("/auth")) {
    next();
    return;
  } else {
    let sessionId = req.headers["Authorization"];
    if (!sessionId) {
      res.status(401).json({ message: "Unauthorized" });
      return;
    }
    if (sessionId.includes("Bearer ")) {
      sessionId = sessionId.slice(7);
    }
    let user;
    const Session = mongoose.model("Session", SessionSchema);
    try {
      let user = await Session.findOne({
        _id: sessionId,
        expires: { $gte: new Date() },
      });
    } catch (error) {
      console.error("Error while filtering by session id:", error);
      res.status(500).json({ message: "Internal server error" });
      return;
    }

    if (user != null) {
      next();
      return;
    } else {
      const session = await Session.findOne({ _id: sessionId });
      if (session) {
        //this means the session has expired
        await session.deleteOne();
        res.status(401).json({ message: "Unauthorized, session expired" });
        return;
      }
      res.status(401).json({ message: "Unauthorized" });
      return;
    }
  }
}

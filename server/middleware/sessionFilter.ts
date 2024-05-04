import { NextFunction, Request, Response } from "express";
import mongoose from "mongoose";
import { SessionSchema } from "../models/SessionSchema";

export function filterBySessionId(
	req: Request,
	res: Response,
	next: NextFunction,
): void {
	if (req.path === "/auth/*") {
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
		try {
			let Session = mongoose.model("Session", SessionSchema);
			let user = Session.findOne({ sessionId });
		} catch (error) {
			console.error("Error while filtering by session id:", error);
			res.status(500).json({ message: "Internal server error" });
			return;
		}
		if (user != null) {
			next();
		} else {
			res.status(401).json({ message: "Unauthorized" });
		}
	}
}

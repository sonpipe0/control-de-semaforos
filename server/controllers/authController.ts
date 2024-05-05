import express, { Router, Request, Response } from "express";
import { createUser, loginUser } from "../services/authService";

const router = Router();

// Create a new user
router.post("/create", async (req: Request, res: Response) => {
  const result = await createUser(req);
  res.status(result.status).json(result.body);
});

router.post("/login", async (req: Request, res: Response) => {
  const result = await loginUser(req);
  res.status(result.status).json(result.body);
});

export default router;

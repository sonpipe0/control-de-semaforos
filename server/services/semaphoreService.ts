import {DayObject} from "../models/DayObjectSchema";
import SemaphoreSchema from "../models/semaphore";
import {Request} from "express";
import mongoose from "mongoose";
import {StatusType} from "../types/statusTypes";

function validateHour(time: string) {
    const hourRegex: RegExp = /^(0[0-9]|1[0-9]|2[0-3])$/;
    return hourRegex.test(time);
}

function validateDay(day: string) {
    const dayRegex: RegExp =
        /^(Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)$/;
    return dayRegex.test(day);
}

export async function createSemaphore(
    req: Request,
): Promise<{ status: number; body: any }> {
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
        return {
            status: 400,
            body: {message: "Invalid operating time"},
        };
    }
    try {
        await semaphore.save();
        return {status: 201, body: {message: "Semaphore created successfully"}};
    } catch (err: any) {
        return {status: 400, body: {message: err.message}};
    }
}

export async function updateSemaphoreStatus(
    req: Request,
): Promise<{ status: number; body: any }> {
    const {id, status}: { id: string; status: string } = req.body;
    const semaphore = await SemaphoreSchema.findOne({id});
    if (!semaphore) {
        return {status: 404, body: {message: "Semaphore not found"}};
    }
    try {
        await SemaphoreSchema.updateOne({id}, {status: status});
        return {
            status: 200,
            body: {message: "Semaphore status updated successfully"},
        };
    } catch (err: any) {
        return {status: 400, body: {message: err.message}};
    }
}


export async function updateSemaphoreActiveTime(
    req: Request,
): Promise<{ status: number; body: any }> {
    const {
        id,
        dayList
    }: { id: string; dayList: Array<DayObject> } = req.body;
    const semaphore = await SemaphoreSchema.findOne({id});
    if (!semaphore) {
        return {status: 404, body: {message: "Semaphore not found"}};
    }
    dayList.forEach((day) => {
        if (!validateDay(day.day) || !validateHour(day.open) || !validateHour(day.close)) {
            return {status: 400, body: {message: "Invalid operating time"}};
        }
    });

    try {
        await SemaphoreSchema.updateOne({id}, {operating_time: dayList});
        return {
            status: 200,
            body: {message: "Semaphore operating time updated successfully"},
        };
    } catch (err: any) {
        return {status: 400, body: {message: err.message}};
    }
}


export async function UpdateSemaphoreTiming(
    req: Request,
): Promise<{ status: number; body: any }> {
    const {
        id,
        green_time,
        red_time
    }: { id: string; green_time: number; red_time: number } = req.body;
    const semaphore = await SemaphoreSchema.findOne({id});
    if (!semaphore) {
        return {status: 404, body: {message: "Semaphore not found"}};
    }
    try {

        await SemaphoreSchema.updateOne({id}, {green_time: green_time, red_time: red_time});
    } catch (err: any) {
        return {status: 400, body: {message: err.message}};
    }
    return {status: 200, body: {message: "Semaphore timing updated successfully"}};
}

export async function getSemaphoreInformation(req: Request): Promise<{ status: number; body: any }> {
    const semaphores = await SemaphoreSchema.find({},{_id:1, name:1, status:1, green_time:1, red_time:1, operating_time:1});
    const result: Semaphore[] = [];
    semaphores.forEach((semaphore) => {
        const active_time: DayObject[] = [];
        semaphore.operating_time.forEach((day) => {
            const dayObject: DayObject = { day: day.day as string, open: day.open as string, close: day.close as string};
            active_time.push(dayObject);
        });
        result.push({
            id: semaphore._id.toString(),
            name: semaphore.name,
            status: semaphore.status,
            green_time: semaphore.green_time,
            red_time: semaphore.red_time,
            operating_time: active_time
        });
    });
    return {status: 200, body: result};
}


interface Semaphore{
    id: string,
    name: string,
    status: StatusType,
    green_time: number,
    red_time: number,
    operating_time: DayObject[]
}
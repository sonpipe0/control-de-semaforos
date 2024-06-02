import SemaphoreSchema from "../models/semaphore";

const messageHandlers = new Map<string, Function>();

messageHandlers.set("semaphore/create", createSemaphore);

export function handleMessage(topic: string, message: Buffer) {
  console.log("Message received: ", message.toString());
  const handler = messageHandlers.get(topic);
  if (handler) {
    handler(message);
  } else {
    console.error("No handler for topic: ", topic);
  }
}

async function createSemaphore(message: Buffer) {
  let json = JSON.parse(message.toString());

  //create a default semaphore
  let semaphore = {
    name: json.name,
    red_time: 30,
    green_time: 30,
    operating_time: [
      {
        day: "Monday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Tuesday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Wednesday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Thursday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Friday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Saturday",
        open: "00:00:00",
        close: "23:59:59",
      },
      {
        day: "Sunday",
        open: "00:00:00",
        close: "23:59:59",
      },
    ],
  };
  try {
    let semaphoreObject = new SemaphoreSchema(semaphore);
    await semaphoreObject.save();
  } catch (error) {
    //semaphore already exists
    console.log("Semaphore already exists");
    }
}

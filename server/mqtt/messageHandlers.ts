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
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Tuesday",
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Wednesday",
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Thursday",
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Friday",
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Saturday",
        start_time: "00",
        end_time: "23",
      },
      {
        day: "Sunday",
        start_time: "00",
        end_time: "23",
      },
    ],
  };
  try {
    let semaphoreObject = new SemaphoreSchema(semaphore);
    await semaphoreObject.save();
  } catch (error) {
    console.error("Error while creating semaphore", error);
  }
}

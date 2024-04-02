import express,{ Express, Request, Response} from 'express';
const app = express();
app.use(express.json()); // To parse JSON bodies

// Firebase setup
import {initializeApp, FirebaseApp} from 'firebase/app';
import {getDatabase, ref, set, Database} from 'firebase/database';



// Your web app's Firebase configuration

const firebaseConfig = fireBaseConfigHandle();

// Initialize Firebase and get a reference to the service
const appIni: FirebaseApp = initializeApp(firebaseConfig);
const db: Database = getDatabase(appIni);

type User = {
    username: string;
    name: string;
    age: number;
    email: string;

};

// Example route to set data
app.post('/setUser', (req: Request , res: Response) => {
    const userData: User = req.body; // Assuming the body contains the user data

    set(ref(db, 'users/' + userData.username), {
        name: userData.name,
        age: userData.age,
        email: userData.email
    })
        .then(() => res.status(200).send("User data set successfully."))
        .catch(error => res.status(400).send(error.message));
});

// Start listening on port 3000
app.listen(3000, () => {
    console.log('Server is running on port 3000');
});

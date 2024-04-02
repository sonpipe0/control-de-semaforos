"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const app = (0, express_1.default)();
app.use(express_1.default.json()); // To parse JSON bodies
// Firebase setup
const app_1 = require("firebase/app");
const database_1 = require("firebase/database");
// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyAA0DhV8T7fT-QJQfoYITRexPyPKtRk-u0",
    authDomain: "electronicainformatica-1ced8.firebaseapp.com",
    databaseURL: "https://electronicainformatica-1ced8-default-rtdb.firebaseio.com",
    projectId: "electronicainformatica-1ced8",
    storageBucket: "electronicainformatica-1ced8.appspot.com",
    messagingSenderId: "406314016628",
    appId: "1:406314016628:web:dbae1d7c7a12ec8d909621",
    measurementId: "G-DPTSWHEEFJ"
};
// Initialize Firebase and get a reference to the service
const appIni = (0, app_1.initializeApp)(firebaseConfig);
const db = (0, database_1.getDatabase)(appIni);
// Example route to set data
app.post('/setUser', (req, res) => {
    const userData = req.body; // Assuming the body contains the user data
    (0, database_1.set)((0, database_1.ref)(db, 'users/' + userData.username), {
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

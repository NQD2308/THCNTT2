const firebase = require("firebase")
const firebaseConfig = {
    apiKey: "AIzaSyCJZgT1kbFksl4QXxWvHdmWPPgAdriMfdg",
    authDomain: "thcntt2-assignment02.firebaseapp.com",
    projectId: "thcntt2-assignment02",
    storageBucket: "thcntt2-assignment02.appspot.com",
    messagingSenderId: "570016373257",
    appId: "1:570016373257:web:ec0c3f70a4147523b076d8",
    measurementId: "G-WJTTYSEZ8W"
  };
firebase.initializeApp(firebaseConfig)
const db = firebase.firestore();
const User = db.collection("Farm");
module.exports = User;
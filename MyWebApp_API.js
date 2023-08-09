const express = require('express')
const cors = require('cors')
const User = require('./config')
const { async } = require('@firebase/util')

const app =  express()
app.use(express.json())
app.use(cors())

app.get('/', async(req,res) => {
    const snapshot = await User.get();
    const list = snapshot.docs.map((doc) => ({ id: doc.id, ...doc.data()}));
    res.send(list)
});

app.post('/create', async(req, res)=>{
    const data = req.body;
    console.log(data);
    // console.log("Data of Users ", data)
    await User.doc("123 NY").set(data);
    res.send({msg: "User added"});
})

app.put('/update', async(req, res) => {
    const id = req.body.id;
    delete req.body.id;
    const data = req.body;
    await User.doc(id).update(data);
    res.send({msg: "Updated"});
});

app.delete('/delete', async(req, res) => {
    const id = req.body.id;
    await User.doc(id).delete();
    res.send({msg: "Deleted"});
});

app.listen(1234, () => console.log("Port 1234"))
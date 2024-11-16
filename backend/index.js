const express = require('express');
const app = express();

const PORT = 3000;

app.get('/', (req, res) => {
    res.send('Geofence IoT Blind Stick is running! Making real time changes to the code using Nodemon');
});

app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const bodyParser = require('body-parser');

const app = express();
const server = http.createServer(app);
const io = socketIO(server);

let latestCoords = { lat: 0, lng: 0 }; // Default coordinates

app.use(bodyParser.json()); // Middleware to parse JSON payloads
app.use(express.static(__dirname + '/public')); // Serve static files from "public"

// Endpoint to update GPS coordinates
app.post('/update-coords', (req, res) => {
  latestCoords = req.body; // Receive latitude and longitude
  io.emit('update', latestCoords); // Send updated coordinates to clients
  res.sendStatus(200); // Respond with status OK
});

// Serve the main page
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/public/index.html'); // Load the map page
});

// Start the server
server.listen(3000, () => console.log('Server running on http://192.168.7.132:3000'));
// CHANGE THE ADDRESS ABOVE BASED ON YOUR LAPTOP ADDRESS (SEE THE ARDUINO CODE FOR MORE INSTRUCTIONS)

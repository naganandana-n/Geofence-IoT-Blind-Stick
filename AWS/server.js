const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const cors = require('cors');
const path = require('path'); // For serving static files

const app = express();
const server = http.createServer(app);
const io = socketIO(server);

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public'))); // Serve frontend files from "public"

// MongoDB Connection
const MONGO_URI = 'mongodb://localhost:27017/geofence-db';
mongoose.connect(MONGO_URI, { useNewUrlParser: true, useUnifiedTopology: true });

const db = mongoose.connection;
db.on('error', (err) => console.error('MongoDB connection error:', err));
db.once('open', () => console.log('Connected to MongoDB'));

// Define Schema and Model for Geofence
const geofenceSchema = new mongoose.Schema({
  name: { type: String, required: true },
  geojson: { type: Object, required: true }, // GeoJSON format for geofence
  createdAt: { type: Date, default: Date.now },
});

const Geofence = mongoose.model('Geofence', geofenceSchema);

// Store the latest coordinates received
let latestCoords = { lat: 0, lng: 0 };

// API: Save geofence to MongoDB
app.post('/api/geofence', async (req, res) => {
  try {
    const { name, geojson } = req.body;

    if (!geojson) {
      return res.status(400).json({ message: 'Invalid geofence data.' });
    }

    const geofence = new Geofence({ name, geojson });
    await geofence.save();

    io.emit('geofenceUpdated', geojson); // Notify clients of new geofence
    res.status(200).json({ message: 'Geofence saved successfully.' });
  } catch (err) {
    console.error('Error saving geofence:', err);
    res.status(500).json({ message: 'Server error while saving geofence.' });
  }
});

// API: Retrieve all geofences from MongoDB
app.get('/api/geofence', async (req, res) => {
  try {
    const geofences = await Geofence.find();
    res.status(200).json(geofences);
  } catch (err) {
    console.error('Error fetching geofences:', err);
    res.status(500).json({ message: 'Server error while retrieving geofences.' });
  }
});

// API: Receive GPS coordinates and broadcast to clients
app.post('/update-coords', (req, res) => {
  latestCoords = req.body;
  console.log('Received updated coordinates:', latestCoords);

  io.emit('locationUpdate', latestCoords); // Notify clients of the new location
  res.sendStatus(200);
});

// API: Retrieve latest GPS coordinates
app.get('/api/gps', (req, res) => {
  res.status(200).json(latestCoords);
});

// Serve Frontend File (index.html)
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start the server
const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server running on http://3.28.218.118:${PORT}`);
});
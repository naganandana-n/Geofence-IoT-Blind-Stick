const express = require('express');
const cors = require('cors');

// Import the database connection
require('./models/database'); // Adjust the path if necessary

// Import models for GPS and Geofence data
const Geofence = require('./models/Geofence');

const app = express();

// Enable CORS
app.use(cors());

// Body parser middleware
app.use(express.json());

// Example API endpoint for GPS data
app.get('/api/gps', async (req, res) => {
  try {
    const gpsData = await Geofence.find(); // Assuming the Geofence model includes GPS-related data
    res.json(gpsData);
  } catch (error) {
    console.error('Error fetching GPS data:', error);
    res.status(500).json({ message: 'Failed to fetch GPS data' });
  }
});

// API endpoint to handle geofence data
app.post('/api/geofence', async (req, res) => {
  const { northEast, southWest } = req.body;

  try {
    const geofence = new Geofence({ northEast, southWest, timestamp: Date.now() });
    await geofence.save();
    console.log('Saved geofence:', geofence);
    res.status(200).send({ message: 'Geofence saved successfully!', geofence });
  } catch (error) {
    console.error('Error saving geofence:', error);
    res.status(500).json({ message: 'Failed to save geofence' });
  }
});

// Start the server
const PORT = 3000;
app.listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`));
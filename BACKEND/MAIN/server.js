const express = require('express');
const cors = require('cors');

const app = express();

// Enable CORS
app.use(cors());

// Body parser middleware
app.use(express.json());

// Example API endpoint
app.get('/api/gps', (req, res) => {
  res.json([
    { latitude: 25.1315, longitude: 55.4201, timestamp: Date.now() },
    { latitude: 25.1320, longitude: 55.4210, timestamp: Date.now() }
  ]);
});

app.post('/api/geofence', (req, res) => {
  console.log(req.body);
  res.status(200).send({ message: 'Geofence saved successfully!' });
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`));
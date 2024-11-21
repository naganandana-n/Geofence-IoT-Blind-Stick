const express = require('express');
const { setGeoFence, getGeoFence } = require('../models/geofence');

const router = express.Router();

// POST Geo-Fence
router.post('/', async (req, res) => {
  const { latitude, longitude, radius } = req.body;
  if (!latitude || !longitude || !radius) {
    return res.status(400).json({ error: 'Invalid geo-fence data' });
  }
  await setGeoFence(latitude, longitude, radius);
  res.json({ status: 'success', message: 'Geo-fence set' });
});

// GET Geo-Fence
router.get('/', async (req, res) => {
  const geoFence = await getGeoFence();
  res.json(geoFence);
});

module.exports = router;
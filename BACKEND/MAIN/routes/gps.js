const express = require('express');
const { saveGPSData, getGPSData } = require('../models/gps');

const router = express.Router();

// POST GPS Data
router.post('/', async (req, res) => {
  const { latitude, longitude } = req.body;
  if (!latitude || !longitude) {
    return res.status(400).json({ error: 'Invalid GPS data' });
  }
  await saveGPSData(latitude, longitude);
  res.json({ status: 'success' });
});

// GET Recent GPS Data
router.get('/', async (req, res) => {
  const data = await getGPSData();
  res.json(data);
});

module.exports = router;
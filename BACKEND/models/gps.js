const mongoose = require('./database');

// Define GPS Schema
const gpsSchema = new mongoose.Schema({
  latitude: { type: Number, required: true },
  longitude: { type: Number, required: true },
  timestamp: { type: Date, default: Date.now },
});

// Create Model
const GPS = mongoose.model('GPS', gpsSchema);

// Save GPS Data
const saveGPSData = async (latitude, longitude) => {
  const gpsData = new GPS({ latitude, longitude });
  await gpsData.save();
};

// Get Recent GPS Data
const getGPSData = async () => {
  return await GPS.find().sort({ timestamp: -1 }).limit(10);
};

module.exports = { saveGPSData, getGPSData };
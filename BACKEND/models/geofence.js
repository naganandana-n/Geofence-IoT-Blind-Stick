const mongoose = require('./database');

// Define Geo-Fence Schema
const geoFenceSchema = new mongoose.Schema({
  latitude: { type: Number, required: true },
  longitude: { type: Number, required: true },
  radius: { type: Number, required: true },
});

// Create Model
const GeoFence = mongoose.model('GeoFence', geoFenceSchema);

// Set Geo-Fence
const setGeoFence = async (latitude, longitude, radius) => {
  await GeoFence.deleteMany(); // Clear existing geo-fence
  const geoFence = new GeoFence({ latitude, longitude, radius });
  await geoFence.save();
};

// Get Geo-Fence
const getGeoFence = async () => {
  return await GeoFence.findOne();
};

module.exports = { setGeoFence, getGeoFence };
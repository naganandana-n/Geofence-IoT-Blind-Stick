const haversine = require('haversine-distance');
const { getGeoFence } = require('../models/geofence');

const checkGeoFence = async (latitude, longitude) => {
  const geoFence = await getGeoFence();
  if (!geoFence) return false;

  const point = { lat: latitude, lon: longitude };
  const center = { lat: geoFence.latitude, lon: geoFence.longitude };
  const distance = haversine(point, center);

  return distance <= geoFence.radius;
};

module.exports = { checkGeoFence };
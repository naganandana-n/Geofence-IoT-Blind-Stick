const map = L.map('map').setView([25.1315, 55.4201], 16);
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  attribution: '© OpenStreetMap contributors',
}).addTo(map);

const drawnItems = new L.FeatureGroup();
map.addLayer(drawnItems);

const drawControl = new L.Control.Draw({
  draw: {
    rectangle: true,
    polygon: true,
    circle: false,
    marker: false,
  },
  edit: {
    featureGroup: drawnItems,
  },
});
map.addControl(drawControl);

// Save Drawn Geofence
map.on(L.Draw.Event.CREATED, async (event) => {
  const layer = event.layer;
  drawnItems.addLayer(layer);

  const geojson = layer.toGeoJSON();
  const payload = {
    name: `Geofence ${Date.now()}`,
    geojson: geojson,
  };

  try {
    const response = await fetch('http://3.29.102.4:3000/api/geofence', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload),
    });

    if (response.ok) {
      alert('Geofence saved successfully!');
    } else {
      alert('Failed to save geofence.');
    }
  } catch (error) {
    console.error('Error saving geofence:', error);
  }
});

// Fetch and Render Saved Geofences
async function fetchGeofences() {
  try {
    const response = await fetch('http://3.29.102.4:3000/api/geofence');
    const geofences = await response.json();

    geofences.forEach((geofence) => {
      const geojsonLayer = L.geoJSON(geofence.geojson);
      drawnItems.addLayer(geojsonLayer); // Add to drawnItems for checking later
    });

    console.log('Geofences loaded:', drawnItems.toGeoJSON());
  } catch (error) {
    console.error('Error fetching geofences:', error);
  }
}

// Load saved geofences on page load
fetchGeofences();

const socket = io('http://3.29.102.4:3000');
let locationMarker = null;
let isOutsideGeofence = false; // Flag to track if the user is outside the geofence

// Helper Function: Check if a point is inside any polygon
function isPointInGeofence(lat, lng) {
  const point = turf.point([lng, lat]); // Use Turf.js for robust checks
  const geofences = drawnItems.toGeoJSON();

  console.log('Checking point:', { lat, lng });
  console.log('Loaded geofences:', geofences);

  for (let i = 0; i < geofences.features.length; i++) {
    const polygon = turf.polygon(geofences.features[i].geometry.coordinates);
    if (turf.booleanPointInPolygon(point, polygon)) {
      console.log(`Point is inside geofence ${i}`);
      return true;
    }
  }

  console.log('Point is outside all geofences');
  return false;
}

// Update Table
function updateTable(lat, lng) {
  const tableBody = document.querySelector('#gps-table tbody');
  const row = document.createElement('tr');

  const timestamp = new Date().toLocaleString(); // Current timestamp
  row.innerHTML = `
    <td>${lat.toFixed(6)}</td>
    <td>${lng.toFixed(6)}</td>
    <td>${timestamp}</td>
  `;

  tableBody.prepend(row); // Add the new row to the top
}

// Show Notification
function showNotification(message) {
  const notificationDiv = document.getElementById('notification');
  notificationDiv.textContent = message;
  notificationDiv.style.display = 'block';
}

// Hide Notification
function hideNotification() {
  const notificationDiv = document.getElementById('notification');
  notificationDiv.style.display = 'none';
}

// Listen for location updates
socket.on('locationUpdate', (coords) => {
  const { lat, lng } = coords;
  console.log('Updated coordinates:', lat, lng);

  // Update or add marker on the map
  if (!locationMarker) {
    locationMarker = L.marker([lat, lng]).addTo(map);
    locationMarker.bindPopup('Current Location').openPopup();
  } else {
    locationMarker.setLatLng([lat, lng]);
    locationMarker.getPopup().setContent(`Current Location: ${lat.toFixed(4)}, ${lng.toFixed(4)}`);
    locationMarker.openPopup();
  }

  // Update the table with the new coordinates
  updateTable(lat, lng);

  // Check if the marker is inside a geofence
  const insideGeofence = isPointInGeofence(lat, lng);

  if (!insideGeofence && !isOutsideGeofence) {
    isOutsideGeofence = true;
    showNotification('The marker has left the geofence!');
  }

  if (insideGeofence && isOutsideGeofence) {
    isOutsideGeofence = false;
    hideNotification();
  }
});
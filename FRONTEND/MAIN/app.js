// Initialize the map and set the view to BITS Pilani, Dubai campus
const map = L.map('map').setView([25.1315, 55.4201], 16); // Zoom level set to 16 for campus-level detail

// Add OpenStreetMap tiles to the map
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  attribution: '© OpenStreetMap contributors',
}).addTo(map);

// Initialize a feature group to store drawn shapes
const drawnItems = new L.FeatureGroup();
map.addLayer(drawnItems);

// Add Leaflet.draw controls for drawing and editing
const drawControl = new L.Control.Draw({
  draw: {
    rectangle: true, // Enable rectangle drawing
    polyline: false,
    polygon: false,
    circle: false,
    marker: false,
  },
  edit: {
    featureGroup: drawnItems, // Allow editing of existing shapes
  },
});
map.addControl(drawControl);

// Event listener for when a new shape is created
map.on(L.Draw.Event.CREATED, (event) => {
  const layer = event.layer;

  // Add the newly created shape to the feature group
  drawnItems.addLayer(layer);

  // Enable the Save button
  document.getElementById('save-draw').disabled = false;
});

// Event listener for when shapes are edited
map.on(L.Draw.Event.EDITED, (event) => {
  event.layers.eachLayer((layer) => {
    console.log('Updated shape bounds:', layer.getBounds());
  });
});

// Event listener for the Save button
document.getElementById('save-draw').addEventListener('click', async () => {
  const layers = drawnItems.getLayers();
  if (layers.length === 0) {
    alert('No shapes to save!');
    return;
  }

  // Save the bounds of the first shape
  const layer = layers[0];
  const bounds = layer.getBounds();
  const coordinates = {
    northEast: { lat: bounds.getNorthEast().lat, lng: bounds.getNorthEast().lng },
    southWest: { lat: bounds.getSouthWest().lat, lng: bounds.getSouthWest().lng },
  };

  try {
    const response = await fetch('http://localhost:3000/api/geofence', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(coordinates),
    });

    if (response.ok) {
      alert('Geofence saved successfully!');
      displaySavedGeofence(coordinates);
    } else {
      alert('Failed to save geofence.');
    }
  } catch (error) {
    console.error('Error while saving geofence:', error);
  }
});

// Function to display saved geofence data on the webpage
const displaySavedGeofence = (coordinates) => {
  const savedGeofences = document.getElementById('saved-geofences');
  const geofenceInfo = `
    <li>
      <strong>Geofence:</strong> 
      NorthEast: [${coordinates.northEast.lat}, ${coordinates.northEast.lng}], 
      SouthWest: [${coordinates.southWest.lat}, ${coordinates.southWest.lng}]
    </li>
  `;
  savedGeofences.innerHTML += geofenceInfo; // Append the new geofence to the list
};

// Real-time Table Updates
const locationTable = document.getElementById('location-table');

// Function to fetch real-time data
async function fetchLocationData() {
  try {
    const response = await fetch('http://localhost:3000/api/gps');
    const data = await response.json();

    // Clear the table
    locationTable.innerHTML = '';

    // Populate the table with new data
    data.forEach((location) => {
      const row = `
        <tr>
          <td>${location.latitude}</td>
          <td>${location.longitude}</td>
          <td>${new Date(location.timestamp).toLocaleString()}</td>
        </tr>
      `;
      locationTable.innerHTML += row;
    });
  } catch (error) {
    console.error('Error fetching location data:', error);
  }
}

// Fetch data every 5 seconds
setInterval(fetchLocationData, 5000);
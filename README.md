# Smart Walking Stick for the Visually Impaired

## Project Overview
The **Smart Walking Stick** is designed to assist visually impaired individuals during mobility training in schools for the blind. It features a geo-fencing capability accessible through a web app, obstacle detection with an IR sensor and buzzer, and real-time GPS tracking for monitoring and safety. The system aims to enhance independence and safety during training.

## Objectives
- **Real-Time Location Tracking:** Monitor the stick's location using GPS and IoT.
- **Geo-Fencing:** Allow schools to define safe zones and trigger alerts if the stick moves outside.
- **Obstacle Detection:** Warn users of obstacles using an IR sensor and buzzer.
- **Web App Interface:** Manage geo-fences and monitor the stick’s location via an easy-to-use interface.
- **Local Alarm:** Provide immediate audio alerts for safety.

## System Components

### Hardware
1. **NodeMCU (ESP8266):** Microcontroller for communication and control.
2. **GPS Module (NEO-6M/NEO-7M/NEO-8M):** Tracks the stick’s location.
3. **IR Sensor:** Detects nearby obstacles.
4. **Buzzer:** Alerts the user to obstacles or geo-fence breaches.
5. **Battery:** Rechargeable power source for portability.
6. **Stick Structure:** Lightweight walking stick housing the components.

### Software
1. **Firmware:** Runs on NodeMCU to integrate sensors, GPS, and buzzer.
2. **Web Application:** Allows geo-fence setup and real-time monitoring.

## System Workflow

### Obstacle Detection
- The IR sensor detects objects within range.
- If an obstacle is detected, the buzzer activates to alert the user.

### Geo-Fencing
- Schools define a safe zone using the web app.
- The stick sends GPS data to the backend server via WiFi.
- If the stick moves outside the geo-fence, alerts are triggered both on the web app and the stick (via the buzzer).

### Real-Time Monitoring
- The web app displays the current location of the stick on an interactive map.
- Administrators can track the movement and progress of users.

## Technical Details

### Hardware Integration
| Component       | Purpose                                  | Connection to NodeMCU  |
|------------------|------------------------------------------|-------------------------|
| GPS Module       | Real-time location tracking             | UART (TX/RX pins)       |
| IR Sensor        | Obstacle detection                      | Digital input pin       |
| Buzzer           | Audio alert                             | Digital output pin      |
| Battery          | Power supply                            | 3.7V or 5V input        |

### Web Application Features
1. **Geo-Fence Configuration:** Define and save circular or polygonal boundaries.
2. **Real-Time Location Tracking:** Display the stick’s current location on a map.
3. **Alerts:** Notify administrators and sound alarms for geo-fence breaches.
4. **Device Management:** Support for managing multiple sticks.

### Communication Protocol
- **WiFi (NodeMCU):** Sends GPS data and receives geo-fence configurations.
- **HTTP/MQTT:** Protocols for data transmission.

## Technology Stack

### Frontend
- **Framework:** React.js or Vanilla JavaScript
- **Mapping Library:** Leaflet.js or Google Maps API
- **Styling:** Bootstrap or Tailwind CSS

### Backend
- **Framework:** Flask (Python) or Express.js (Node.js)
- **Database:** PostgreSQL with PostGIS or MongoDB
- **Real-Time Communication:** Flask-SocketIO or Socket.IO

### Hosting
- **AWS EC2 Instance:** Hosts both the frontend and backend.

## Power Management
- Rechargeable **Li-ion battery** with a charging module (e.g., TP4056).
- Optimized power consumption for prolonged use.

## Challenges and Solutions
| **Challenge**                     | **Solution**                                               |
|------------------------------------|-----------------------------------------------------------|
| Reliable obstacle detection        | Calibrate IR sensor for different environments.           |
| GPS accuracy                       | Use GPS + assisted techniques if possible.                |
| Real-time updates in low WiFi areas| Cache GPS data locally and send it when WiFi is available.|
| Compact design                     | Optimize the layout of hardware components.               |

## Potential Extensions
1. **Mobile App:** Develop a companion app for notifications and real-time tracking.
2. **Advanced Sensors:** Include ultrasonic sensors for enhanced obstacle detection.
3. **Voice Feedback:** Add text-to-speech modules for directional or alert messages.
4. **Haptic Feedback:** 
   - Integrate a vibration motor for tactile alerts.  
   - Alternatively, use a servo motor to communicate tactile messages via Morse code, enabling users to receive nuanced feedback about their surroundings.

## Project Timeline
| **Phase**             | **Duration**                |
|-----------------------|-----------------------------|
| Backend Development   | 15 Nov - 20 Nov, 2024       |
| Frontend Development  | 15 Nov - 20 Nov, 2024       |
| Hardware Assembly     | 20 Nov - 25 Nov, 2024       |
| Integration & Testing | 20 Nov - 25 Nov, 2024       |

## Expected Outcomes
- A functional smart walking stick for visually impaired training schools.
- A web app to configure geo-fences and monitor real-time movements.
- Improved safety and mobility for visually impaired individuals during training.

## Contributing
Marvin Showkat, Sneha Sunil, Naganandana Nagendra, Suleiman Bin Daud under the guidance of Prof Jagadish Nayak, Prof Ashwani Saini, Prof Hidayathulla.

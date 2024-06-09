# Smart-Agriculture-IoT

## Overview
Smart-Agriculture-IoT is an innovative project aimed at improving agricultural practices using IoT technology. The system utilizes various sensors to monitor environmental conditions and security, sending all data to Firebase for real-time monitoring and control via a web interface.

## Hardware Components
- **DHT11 Sensor:** Measures temperature and humidity.
- **Rain Sensor:** Detects rainfall.
- **Soil Moisture Sensor:** Measures soil moisture levels.
- **Laser and LDR Module:** Ensures field security.
- **Water Pump:** Controls water supply.
- **ESP32:** Connects all sensors and sends data to Firebase.

## Software Components
- **Frontend:** HTML, CSS, JavaScript for the web interface.
- **Backend:** Firebase for real-time database and data storage.

## Features
- **Temperature and Humidity Monitoring:** Using the DHT11 sensor.
- **Rain Detection:** Through the rain sensor.
- **Soil Moisture Monitoring:** Using the soil moisture sensor.
- **Field Security:** Managed with the laser and LDR module.
- **Automated Irrigation:** Controlled by the water pump.
- **Real-time Data Monitoring:** All sensor data and device statuses are stored in Firebase and accessible via a web interface.

## Project Setup
### Hardware Setup
1. Connect the DHT11, rain sensor, soil moisture sensor, laser and LDR module, and water pump to the ESP32.
2. Ensure all connections are secure and properly configured.

### Software Setup
1. Clone the repository:
    ```bash
    git clone https://github.com/griffin-k/Smart-Agriculture-IoT.git
    ```
2. Navigate to the project directory:
    ```bash
    cd Smart-Agriculture-IoT
    ```
3. Set up Firebase:
    - Create a Firebase project at [Firebase Console](https://console.firebase.google.com/).
    - Configure Firebase Realtime Database and get the project credentials.
    - Update the Firebase configuration in the web app.

4. Deploy the web interface:
    - Open `index.html` in your web browser.
    - Ensure your ESP32 is connected and sending data to Firebase.

### Code Explanation
- **HTML/CSS/JavaScript:** The frontend files located in the root directory.
- **ESP32 Code:** The Arduino sketches for sensor data collection and Firebase communication.

## Usage
1. Power the ESP32 and connected sensors.
2. Open the web interface to monitor real-time data.
3. Check temperature, humidity, soil moisture, rain status, and field security.
4. Control the water pump remotely via the web interface.

## Contributing
Contributions are welcome! Please fork the repository and create a pull request with your changes.

## License
This project is licensed under the MIT License.

## Contact
For questions or suggestions, please contact [karimullah.khan3637@gmail.com].

# Smart Water Meter

## Project Description
The **Smart Water Meter** is an IoT-based system designed to monitor water flow, detect leaks, measure water quality parameters, and send alerts through Telegram and Blynk platforms. It incorporates various sensors to measure pH, turbidity, soil moisture, and temperature/humidity, making it ideal for smart water management applications.

## Features
- **Water Flow Monitoring:** Measures flow rate and total volume of water using a flow sensor.
- **Leak Detection:** Detects water leaks and sends alerts via Telegram.
- **Water Quality Monitoring:**
  - Measures pH and turbidity levels.
  - Provides real-time notifications if values go out of range.
- **Soil Moisture Detection:** Monitors soil moisture levels using digital and capacitive soil moisture sensors.
- **Temperature and Humidity Monitoring:** Utilizes a DHT22 sensor for environmental data.
- **Automation:** Controls a water pump based on water quality and soil conditions.
- **Data Logging:** Logs sensor data to ThingSpeak for visualization and analysis.
- **WiFi Connectivity:** Integrates with Blynk for remote monitoring and control.


<img src="https://github.com/user-attachments/assets/24fca919-8044-4902-a2fd-91886afc9a93" alt="Smart Water Meter" width="500">

### Clickable Link
[Watch the video demonstration here](https://youtu.be/2EYEUr2KouQ?si=mXCB3wliCxFeNHoB)


## Components Used
1. **Microcontroller:** ESP32
2. **Sensors:**
   - Flow Sensor
   - DHT22 Temperature and Humidity Sensor
   - pH Sensor
   - Turbidity Sensor
   - Soil Moisture Sensors (Digital and Capacitive)
3. **Other Hardware:**
   - Water Pump
   - LEDs (for status indication)
   - Relay Module (for pump control)
4. **Platforms:**
   - Blynk
   - ThingSpeak
   - Telegram Bot


## Code Structure
- **`setup()`**:
  - Initializes sensors, WiFi, Blynk, and ThingSpeak.
  - Sets up Telegram bot and interrupts for the flow sensor.
- **`loop()`**:
  - Reads data from sensors.
  - Sends data to ThingSpeak and Blynk.
  - Sends alerts via Telegram based on conditions.
  - Controls the water pump based on sensor readings.

## Example Outputs
### Serial Monitor
```
flow rate : 12 L/Hour
volume : 0.2 L
pH value: 6.5
Turbidity value: 2.1
NTU levels: 1200
Humidity: 45.2 %, Temp: 27.3 °Celsius
Digital Sensor Value1: 0
Digital Sensor Value2: 1
Digital Sensor Value3: 0
Analog Sensor Value: 56
```
### Telegram Alerts
- "Leak detected"
- "pH is out of range"
- "High turbid water"

## Future Enhancements
1. Add more advanced data visualization dashboards.
2. Integrate AI/ML models for predictive maintenance.
3. Expand the system to manage larger-scale irrigation networks.

## Acknowledgments
This project utilizes resources and APIs from:
- [Blynk](https://blynk.io/)
- [ThingSpeak](https://thingspeak.com/)
- [Telegram Bot API](https://core.telegram.org/bots)


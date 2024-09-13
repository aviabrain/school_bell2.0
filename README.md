# school_bell2.0
Automating school's bell system with ESP8266


---

# **School Bell Automation Project**

### Developer: [Shahriyor]

## **Overview**

This project automates the ringing of school bells using an ESP8266 microcontroller. The system retrieves real-time from an NTP server, and users can update the bell ring times through a web interface. It's designed to be user-friendly, allowing teachers or administrators to change schedules as needed without technical knowledge.

---

## **Features**
- **Real-time Clock Synchronization**: Automatically syncs time using NTP to ensure accurate bell ringing.
- **Web Interface**: Accessible from any device connected to the same network, allowing users to update ring times easily.
- **Multiple Bell Times**: Supports up to 14 configurable ring times for different periods during the day.
- **Relay Control**: Operates a relay connected to the school bell, turning it on and off at the specified times.
- **Fallback on Wi-Fi Failure**: The system automatically restarts and reconnects in case of Wi-Fi connection failure.

---

## **Components Used**
- **ESP8266 (NodeMCU)**
- **Relay Module**
- **NTP Server (pool.ntp.org)** for real-time clock synchronization
- **Web Server** (via ESP8266WebServer library)

---

## **How It Works**
1. **Setup**: The ESP8266 connects to the Wi-Fi network using the provided credentials. It synchronizes with the NTP server to get the current time.
2. **User Interface**: A web interface is served at the ESP8266’s IP address. Users can access this via their phone or computer to view and update the ring times.
3. **Bell Ringing**: At the specified times, the relay connected to the school bell is activated, ringing the bell for a set duration (default: 10 seconds).
4. **Custom Ring Times**: Users can customize up to 14 ring times using the web interface.
5. **Current Time Display**: The web interface dynamically updates and shows the current time.

---

## **Setup Instructions**
1. **Connect the ESP8266**: Ensure the ESP8266 is wired to a relay module controlling the school bell.
2. **Upload the Code**: Flash the provided Arduino code onto the ESP8266 using the Arduino IDE.
3. **Configure Wi-Fi**: Update the Wi-Fi credentials in the code (`ssid` and `password`) before uploading.
4. **Access the Web Interface**:
   - Once the ESP8266 is connected to Wi-Fi, open your browser and navigate to the ESP8266’s IP address (shown in the serial monitor).
   - Use the web form to set the desired ring times.
5. **Time Synchronization**: The system will automatically synchronize with the NTP server to get the current time.

---

## **Pin Configuration**
- **Relay Pin**: `D1` (Connected to the relay controlling the school bell)
- **Seismometer Pin (Optional)**: `D2` (Reserved for future earthquake detection feature)

---

## **Libraries Required**
1. **ESP8266WiFi.h**: For Wi-Fi connectivity.
2. **NTPClient.h**: For time synchronization using the NTP protocol.
3. **WiFiUdp.h**: To send and receive data over UDP.
4. **ESP8266WebServer.h**: For setting up a web server on the ESP8266.

---

## **Future Enhancements**
- **Seismometer Integration**: In future versions, a seismometer can be added for earthquake detection, triggering an emergency alarm.
- **Mobile Optimization**: Improve the web interface to be more mobile-responsive for easier control on smartphones.
- **Holiday/Custom Schedule Support**: Add an option to input custom schedules or holiday breaks.

---

## **Troubleshooting**
- **Wi-Fi Connection Issues**:
  - Ensure the Wi-Fi credentials are correct and that the ESP8266 is within range of the router.
  - Check the serial monitor for connection status.
- **Bell Not Ringing**:
  - Double-check the relay wiring.
  - Make sure the relay pin (`D1`) is correctly connected and receiving signals from the ESP8266.

---

## **Credits**
This project was developed as a school project to demonstrate practical skills in IoT, automation, and web-based control using the ESP8266.

---

## **License**
This project is free to use and modify for educational purposes.

---

You can update any sections based on your actual project. This README gives a professional touch and will explain your project well to anyone evaluating it!

# ESP32-CAM Surveillance Robot

A Wi-Fi controlled surveillance robot built using the AI-Thinker ESP32-CAM, OV2640 camera and L298N motor driver.

## Overview

This project is a compact robotic surveillance platform that combines wireless communication, live video streaming and motor control.

The ESP32-CAM provides the camera and Wi-Fi functionality. A web interface allows the user to view the live camera stream and remotely control the robot's movement.

## Features

- 📷 Live camera streaming
- 📡 Wi-Fi communication
- 🌐 Browser-based control interface
- 🚗 Forward movement
- 🚗 Backward movement
- ↩️ Left turning
- ↪️ Right turning
- 🛑 Stop control
- ⚙️ L298N motor driver
- 📱 Control from a phone or computer

## Hardware Components

| Component | Purpose |
|---|---|
| AI-Thinker ESP32-CAM | Main controller and Wi-Fi |
| OV2640 Camera | Live video capture |
| L298N Motor Driver | Motor control |
| DC Geared Motors ×2 | Robot movement |
| Robot Chassis | Mechanical structure |
| Wheels | Robot movement |
| Caster Wheel | Mechanical support |
| Battery Pack | Power supply |
| 5V Regulated Supply | ESP32-CAM power |

For the complete components, instruments and wiring information, see:

`hardware/Components_and_Instruments.txt`

## Pin Configuration

### L298N → ESP32-CAM

| ESP32-CAM | L298N | Function |
|---|---|---|
| GPIO 12 | IN1 | Left motor control |
| GPIO 13 | IN2 | Left motor control |
| GPIO 14 | IN3 | Right motor control |
| GPIO 15 | IN4 | Right motor control |

### Motors
L298N OUT1 and OUT2 → Left Motors
L298N OUT3 and OUT4 → Right Motors




##Power
Motor Battery + → L298N Motor Supply
Motor Battery - → L298N GND
ESP32-CAM GND → L298N GND
Regulated 5V → ESP32-CAM 5V

##System Architecture
             Wi-Fi
               │
       ┌───────▼───────┐
       │ Phone / Laptop│
       │ Web Interface │
       └───────┬───────┘
               │
               │ Wi-Fi
               │
       ┌───────▼────────┐
       │   ESP32-CAM    │
       │                │
       │ Camera + Wi-Fi │
       │ Web Server     │
       └───────┬────────┘
               │
        Motor Control
               │
       ┌───────▼───────┐
       │     L298N     │
       │ Motor Driver  │
       └───────┬───────┘
              / \
             /   \
   Left Motors   Right Motors



##How It Works
The ESP32-CAM starts and initializes the OV2640 camera.
The ESP32-CAM connects to the configured Wi-Fi network.
A web server is started on the ESP32-CAM.
The ESP32-CAM displays its IP address through the Serial Monitor.
The user opens the IP address in a web browser.
The browser displays the live MJPEG camera stream.
Movement buttons send commands to the ESP32-CAM.
The ESP32-CAM controls the L298N motor driver.
The L298N drives the two DC motors.

##Software
Arduino IDE
ESP32 Arduino Core
C/C++
HTML
CSS
JavaScript

##Installation
1. Install Arduino IDE

Install Arduino IDE on your computer.

2. Install ESP32 Board Support

Install the ESP32 board package in Arduino IDE.

3. Select Board

Select:

AI Thinker ESP32-CAM
4. Configure Wi-Fi

Open:

src/ESP32-CAM_Surveillance_Robot_Code.ino

Change:

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

to your own Wi-Fi credentials.

Do not upload real Wi-Fi passwords to a public GitHub repository.

5. Upload

Use a USB-to-TTL/FTDI programmer.

FTDI 5V → ESP32-CAM 5V
FTDI GND → ESP32-CAM GND
FTDI TX → ESP32-CAM U0R
FTDI RX → ESP32-CAM U0T
GPIO0 → GND during programming

After uploading:

Disconnect GPIO0 from GND.
Press RESET.
Open Serial Monitor.
Set baud rate to 115200.
Find the ESP32-CAM IP address.
6. Open the Control Interface

Connect your phone or computer to the same Wi-Fi network.

Open the IP address shown in the Serial Monitor.

The web interface will provide:

Live camera stream
Forward
Backward
Left
Right
Stop


##Project Structure
ESP32-CAM-Surveillance-Robot/
│
├── README.md
│
├── src/
│   └── ESP32-CAM_Surveillance_Robot_Code.ino
│
└── hardware/
    └── Components_and_Instruments.txt



##Important Notes
Use a suitable regulated supply for the ESP32-CAM.
Do not connect an unsuitable motor battery directly to the ESP32-CAM.
Connect ESP32-CAM and L298N grounds together.
GPIO12 and GPIO15 are ESP32 boot-strapping pins and may cause boot/programming issues depending on the motor-driver wiring.
Disconnect motor-driver inputs during programming if the ESP32-CAM does not enter flashing mode.
This project does not use an SD card.
Future Improvements

##Possible future upgrades include:

Pan-tilt camera
Obstacle detection
Ultrasonic sensor
GPS tracking
Gas detection
Environmental sensors
Mobile application
Object detection
Face detection
AI-based surveillance
Author

Om Patil

Electronics & Telecommunication Engineering


##License

This project is intended for educational and development purposes.




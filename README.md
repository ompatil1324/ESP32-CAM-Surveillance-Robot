# ESP32-CAM Surveillance Robot

A Wi-Fi controlled surveillance robot built using the AI-Thinker ESP32-CAM, OV2640 camera and L298N motor driver.

## Overview

This project is a compact robotic surveillance platform combining wireless communication, live video streaming and motor control.

The ESP32-CAM provides the camera and Wi-Fi functionality. A web interface allows the user to view the live camera stream and remotely control the robot.

## Features

- Live camera streaming
- Wi-Fi communication
- Browser-based control
- Forward movement
- Backward movement
- Left and right turning
- Stop control
- L298N motor driver
- Remote control from phone or computer

## Hardware Components

| Component | Purpose |
|---|---|
| AI-Thinker ESP32-CAM | Main controller and Wi-Fi |
| OV2640 Camera | Live video capture |
| L298N Motor Driver | Motor control |
| DC Geared Motors | Robot movement |
| Robot Chassis | Mechanical structure |
| Wheels | Robot movement |
| Caster Wheel | Mechanical support |
| Battery Pack | Power supply |

See `hardware/Components_and_Instruments.txt` for the complete components and instruments list.

## Pin Configuration

### ESP32-CAM → L298N

| ESP32-CAM | L298N | Function |
|---|---|---|
| GPIO 12 | IN1 | Left motor control |
| GPIO 13 | IN2 | Left motor control |
| GPIO 14 | IN3 | Right motor control |
| GPIO 15 | IN4 | Right motor control |

### Motor Connections
L298N OUT1 and OUT2 → Left Motor
L298N OUT3 and OUT4 → Right Motor

Power
Motor Battery + → L298N Motor Supply
Motor Battery - → L298N GND
ESP32-CAM GND → L298N GND
Regulated 5V → ESP32-CAM 5V

All grounds must be connected together.

System Architecture
             Wi-Fi
               │
       ┌───────▼────────┐
       │ Phone / Laptop │
       │  Web Interface │
       └───────┬────────┘
               │
               │ Wi-Fi
               │
       ┌───────▼────────┐
       │   ESP32-CAM    │
       │ Camera + Wi-Fi │
       │   Web Server   │
       └───────┬────────┘
               │
        Motor Control
               │
       ┌───────▼───────┐
       │     L298N     │
       │  Motor Driver │
       └───────┬───────┘
              / \
             /   \
        Left Motor  Right Motor
How It Works
The ESP32-CAM initializes the OV2640 camera.
It connects to the configured Wi-Fi network.
A web server starts on the ESP32-CAM.
The ESP32-CAM IP address is displayed through the Serial Monitor.
The user opens the IP address in a browser.
The browser displays the live camera stream.
Movement commands are sent to the ESP32-CAM.
The ESP32-CAM controls the L298N.
The L298N drives the motors.
Software
Arduino IDE
ESP32 Arduino Core
C/C++
HTML
CSS
JavaScript
Installation
1. Install Arduino IDE

Install Arduino IDE on your computer.

2. Install ESP32 Board Support

Install the ESP32 board package through the Arduino IDE Boards Manager.

3. Select the Board

Select:

AI Thinker ESP32-CAM
4. Configure Wi-Fi

Open:

src/ESP32-CAM_Surveillance_Robot_Code.ino

Set your Wi-Fi credentials:

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

Do not upload real Wi-Fi passwords to a public GitHub repository.

5. Upload the Code

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
Set the baud rate to 115200.
Find the ESP32-CAM IP address.
6. Open the Control Interface

Connect your phone or computer to the same Wi-Fi network.

Open the IP address displayed in the Serial Monitor.

Project Structure
ESP32-CAM-Surveillance-Robot/
│
├── hardware/
│   └── Components_and_Instruments.txt
│
├── src/
│   └── ESP32-CAM_Surveillance_Robot_Code.ino
│
├── .gitignore
├── LICENSE
└── README.md
Important Notes
Use a suitable regulated power supply for the ESP32-CAM.
Do not connect the motor battery directly to the ESP32-CAM.
Connect the ESP32-CAM and L298N grounds together.
GPIO12 and GPIO15 are boot-strapping pins and may cause boot/programming issues depending on the motor-driver wiring.
Disconnect motor-driver inputs during programming if necessary.
This project does not use an SD card.
Future Improvements
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

License

This project is licensed under the MIT License.

/*
 * ============================================================
 * ESP32-CAM SURVEILLANCE ROBOT
 * ============================================================
 *
 * Board:
 *   AI-Thinker ESP32-CAM
 *
 * Features:
 *   - Wi-Fi connection
 *   - Live MJPEG camera stream
 *   - Browser-based robot control
 *   - Forward / Backward / Left / Right / Stop
 *   - L298N motor driver control
 *
 * Hardware:
 *   AI-Thinker ESP32-CAM
 *   OV2640 camera
 *   L298N motor driver
 *   2 x DC geared motors
 *
 * IMPORTANT:
 *   Replace the Wi-Fi credentials below before uploading.
 *   Never publish your real Wi-Fi password on GitHub.
 *
 * ============================================================
 */

#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// WI-FI SETTINGS
// ============================================================

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ============================================================
// AI-THINKER ESP32-CAM CAMERA PINS
// ============================================================

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ============================================================
// L298N MOTOR PINS
// ============================================================

#define MOTOR_LEFT_IN1    12
#define MOTOR_LEFT_IN2    13

#define MOTOR_RIGHT_IN1   14
#define MOTOR_RIGHT_IN2   15

// ============================================================
// WEB SERVER
// ============================================================

WebServer server(80);

// ============================================================
// MOTOR FUNCTIONS
// ============================================================

void stopMotors() {

  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);

  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void moveForward() {

  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);

  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void moveBackward() {

  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);

  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
}

void turnLeft() {

  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);

  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void turnRight() {

  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);

  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
}

// ============================================================
// WEB PAGE
// ============================================================

const char MAIN_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width,initial-scale=1">

<title>ESP32-CAM Surveillance Robot</title>

<style>

body {
  background: #111;
  color: white;
  font-family: Arial;
  text-align: center;
  margin: 0;
  padding: 15px;
}

h1 {
  font-size: 24px;
}

img {
  width: 100%;
  max-width: 640px;
  border: 2px solid #444;
  border-radius: 10px;
}

button {
  width: 120px;
  height: 55px;
  margin: 6px;
  font-size: 17px;
  font-weight: bold;
  border: 0;
  border-radius: 10px;
  background: #333;
  color: white;
}

button:active {
  background: #777;
}

.controls {
  max-width: 450px;
  margin: auto;
}

</style>

</head>

<body>

<h1>ESP32-CAM Surveillance Robot</h1>

<img src="/stream">

<div class="controls">

  <div>
    <button onclick="cmd('forward')">
      FORWARD
    </button>
  </div>

  <div>

    <button onclick="cmd('left')">
      LEFT
    </button>

    <button onclick="cmd('stop')">
      STOP
    </button>

    <button onclick="cmd('right')">
      RIGHT
    </button>

  </div>

  <div>

    <button onclick="cmd('backward')">
      BACKWARD
    </button>

  </div>

</div>

<script>

function cmd(direction) {

  fetch('/control?cmd=' + direction);

}

</script>

</body>

</html>

)rawliteral";

// ============================================================
// ROOT WEB PAGE
// ============================================================

void handleRoot() {

  server.send(
    200,
    "text/html",
    MAIN_PAGE
  );
}

// ============================================================
// MOTOR CONTROL WEB ENDPOINT
// ============================================================

void handleControl() {

  if (!server.hasArg("cmd")) {

    server.send(
      400,
      "text/plain",
      "Missing command"
    );

    return;
  }

  String command = server.arg("cmd");

  if (command == "forward") {

    moveForward();

  }

  else if (command == "backward") {

    moveBackward();

  }

  else if (command == "left") {

    turnLeft();

  }

  else if (command == "right") {

    turnRight();

  }

  else if (command == "stop") {

    stopMotors();

  }

  else {

    server.send(
      400,
      "text/plain",
      "Unknown command"
    );

    return;
  }

  server.send(
    200,
    "text/plain",
    "OK"
  );
}

// ============================================================
// MJPEG CAMERA STREAM
// ============================================================

void handleStream() {

  WiFiClient client = server.client();

  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; "
    "boundary=frame\r\n"
    "Cache-Control: no-cache\r\n"
    "Access-Control-Allow-Origin: *\r\n\r\n"
  );

  while (client.connected()) {

    camera_fb_t* fb =
      esp_camera_fb_get();

    if (!fb) {

      Serial.println(
        "Camera capture failed"
      );

      break;
    }

    client.printf(
      "--frame\r\n"
      "Content-Type: image/jpeg\r\n"
      "Content-Length: %u\r\n\r\n",
      fb->len
    );

    client.write(
      fb->buf,
      fb->len
    );

    client.print("\r\n");

    esp_camera_fb_return(fb);

    delay(30);
  }
}

// ============================================================
// CAMERA INITIALIZATION
// ============================================================

bool setupCamera() {

  camera_config_t config;

  config.ledc_channel =
    LEDC_CHANNEL_0;

  config.ledc_timer =
    LEDC_TIMER_0;

  config.pin_d0 =
    Y2_GPIO_NUM;

  config.pin_d1 =
    Y3_GPIO_NUM;

  config.pin_d2 =
    Y4_GPIO_NUM;

  config.pin_d3 =
    Y5_GPIO_NUM;

  config.pin_d4 =
    Y6_GPIO_NUM;

  config.pin_d5 =
    Y7_GPIO_NUM;

  config.pin_d6 =
    Y8_GPIO_NUM;

  config.pin_d7 =
    Y9_GPIO_NUM;

  config.pin_xclk =
    XCLK_GPIO_NUM;

  config.pin_pclk =
    PCLK_GPIO_NUM;

  config.pin_vsync =
    VSYNC_GPIO_NUM;

  config.pin_href =
    HREF_GPIO_NUM;

  config.pin_sccb_sda =
    SIOD_GPIO_NUM;

  config.pin_sccb_scl =
    SIOC_GPIO_NUM;

  config.pin_pwdn =
    PWDN_GPIO_NUM;

  config.pin_reset =
    RESET_GPIO_NUM;

  config.xclk_freq_hz =
    20000000;

  config.pixel_format =
    PIXFORMAT_JPEG;

  if (psramFound()) {

    config.frame_size =
      FRAMESIZE_VGA;

    config.jpeg_quality =
      10;

    config.fb_count =
      2;

  }

  else {

    config.frame_size =
      FRAMESIZE_QVGA;

    config.jpeg_quality =
      12;

    config.fb_count =
      1;
  }

  esp_err_t err =
    esp_camera_init(&config);

  if (err != ESP_OK) {

    Serial.printf(
      "Camera init failed: 0x%x\n",
      err
    );

    return false;
  }

  sensor_t* sensor =
    esp_camera_sensor_get();

  if (sensor) {

    sensor->set_framesize(
      sensor,
      FRAMESIZE_VGA
    );
  }

  return true;
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  // Motor pins

  pinMode(
    MOTOR_LEFT_IN1,
    OUTPUT
  );

  pinMode(
    MOTOR_LEFT_IN2,
    OUTPUT
  );

  pinMode(
    MOTOR_RIGHT_IN1,
    OUTPUT
  );

  pinMode(
    MOTOR_RIGHT_IN2,
    OUTPUT
  );

  stopMotors();

  Serial.println();
  Serial.println(
    "ESP32-CAM Surveillance Robot"
  );

  Serial.println(
    "Starting camera..."
  );

  if (!setupCamera()) {

    Serial.println(
      "Camera initialization failed."
    );

    while (true) {

      delay(1000);

    }
  }

  // Wi-Fi

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  Serial.print(
    "Connecting to Wi-Fi"
  );

  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println(
    "Wi-Fi connected!"
  );

  Serial.print(
    "Robot IP address: "
  );

  Serial.println(
    WiFi.localIP()
  );

  // Web server routes

  server.on(
    "/",
    HTTP_GET,
    handleRoot
  );

  server.on(
    "/control",
    HTTP_GET,
    handleControl
  );

  server.on(
    "/stream",
    HTTP_GET,
    handleStream
  );

  server.begin();

  Serial.println(
    "Web server started."
  );

  Serial.println(
    "Open the IP address in your browser."
  );
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  server.handleClient();

}

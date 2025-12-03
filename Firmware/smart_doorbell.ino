#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include <Wire.h>
#include <Adafruit_AHTX0.h>

const char *WIFI_SSID     = "BLANK_FOR_SECURITY_REASONS";
const char *WIFI_PASSWORD = "BLANK_DITTO";

const char *DISCORD_HOST = "discord.com";
const char *DISCORD_PATH =
  "/api/webhooks/1428854392655708231/"
  "ndoVAxldr4pqDMZOqxiD_aN-xoe7i-uEq-vfJGrtfNGJsD6mkCE5v6PfY2Ki7gLd4CIq";

const float MOTION_DISTANCE_CM = 200.0;
const unsigned long ULTRASONIC_INTERVAL_MS = 5000;
const unsigned long AHT20_REPORT_INTERVAL_MS = 3600000UL;

#define PIN_BUTTON   D0
#define PIN_BUZZER   D1
#define PIN_LED      D2
#define PIN_US_TRIG  D3
#define PIN_US_ECHO  D8
#define PIN_I2C_SDA  D4
#define PIN_I2C_SCL  D5

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

Adafruit_AHTX0 aht20;

unsigned long lastUltrasonicCheck = 0;
unsigned long lastAHTReport = 0;
int lastButtonState = HIGH;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size   = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count     = 2;
  } else {
    config.frame_size   = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count     = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }
  Serial.println("Camera initialized.");
  return true;
}

bool captureAndSendImage(const char *caption) {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return false;
  }

  bool ok = false;
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect(DISCORD_HOST, 443)) {
    Serial.println("Connection to Discord failed");
  } else {
    String boundary = "------------------------XIAODOORBELL12345";
    String bodyStart = "--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"content\"\r\n\r\n";
    bodyStart += caption;
    bodyStart += "\r\n--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"file\"; filename=\"doorbell.jpg\"\r\n";
    bodyStart += "Content-Type: image/jpeg\r\n\r\n";

    String bodyEnd = "\r\n--" + boundary + "--\r\n";
    size_t totalLen = bodyStart.length() + fb->len + bodyEnd.length();

    client.print(String("POST ") + DISCORD_PATH + " HTTP/1.1\r\n");
    client.print(String("Host: ") + DISCORD_HOST + "\r\n");
    client.print("User-Agent: XIAO-ESP32S3-doorbell\r\n");
    client.print("Content-Type: multipart/form-data; boundary=" + boundary + "\r\n");
    client.print("Content-Length: ");
    client.print(totalLen);
    client.print("\r\nConnection: close\r\n\r\n");

    client.print(bodyStart);
    client.write(fb->buf, fb->len);
    client.print(bodyEnd);

    unsigned long timeout = millis() + 10000;
    while (client.connected() && millis() < timeout) {
      while (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    ok = true;
  }

  esp_camera_fb_return(fb);
  client.stop();
  return ok;
}

bool sendDiscordText(const String &content) {
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect(DISCORD_HOST, 443)) {
    Serial.println("Connection to Discord failed (text)");
    return false;
  }

  String payload = String("{\"content\":\"") + content + "\"}";

  String request =
      String("POST ") + DISCORD_PATH + " HTTP/1.1\r\n" +
      "Host: " + DISCORD_HOST + "\r\n" +
      "User-Agent: XIAO-ESP32S3-doorbell\r\n" +
      "Content-Type: application/json\r\n" +
      "Content-Length: " + String(payload.length()) + "\r\n" +
      "Connection: close\r\n\r\n" +
      payload;

  client.print(request);

  unsigned long timeout = millis() + 10000;
  while (client.connected() && millis() < timeout) {
    while (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  client.stop();
  return true;
}

float readUltrasonicDistanceCM() {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_US_ECHO, HIGH, 30000UL);
  if (duration == 0) {
    return -1.0;
  }

  float distance = (duration * 0.0343) / 2.0;
  return distance;
}

bool initAHT20() {
  if (!aht20.begin()) {
    Serial.println("Could not find AHT20 sensor, check wiring.");
    return false;
  }
  Serial.println("AHT20 initialized.");
  return true;
}

bool readAHT20(float &tempC, float &humidity) {
  sensors_event_t humidity_event, temp_event;
  if (!aht20.getEvent(&humidity_event, &temp_event)) {
    return false;
  }
  tempC = temp_event.temperature;
  humidity = humidity_event.relative_humidity;
  return true;
}

void doorbellBuzzSequence() {
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(1000);

  captureAndSendImage("Doorbell pressed - snapshot");

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(1000);

  digitalWrite(PIN_BUZZER, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);

  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_US_TRIG, LOW);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  connectWiFi();
  initCamera();
  initAHT20();

  lastUltrasonicCheck = millis();
  lastAHTReport = millis();
}

void loop() {
  unsigned long now = millis();

  int buttonState = digitalRead(PIN_BUTTON);
  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("Doorbell pressed!");
    doorbellBuzzSequence();
  }
  lastButtonState = buttonState;

  if (now - lastUltrasonicCheck >= ULTRASONIC_INTERVAL_MS) {
    lastUltrasonicCheck = now;
    float dist = readUltrasonicDistanceCM();
    Serial.print("Ultrasonic distance: ");
    Serial.println(dist);

    if (dist > 0 && dist < MOTION_DISTANCE_CM) {
      Serial.println("Motion detected by ultrasonic sensor!");
      captureAndSendImage("Motion detected at door");
    }
  }

  if (now - lastAHTReport >= AHT20_REPORT_INTERVAL_MS) {
    lastAHTReport = now;
    float t, h;
    if (readAHT20(t, h)) {
      String msg = "Hourly environment report: ";
      msg += String(t, 1) + " C, ";
      msg += String(h, 1) + " % RH";
      sendDiscordText(msg);
      Serial.println(msg);
    } else {
      Serial.println("Failed to read AHT20");
    }
  }

  delay(10);
}

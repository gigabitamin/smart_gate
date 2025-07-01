// #include <Wire.h>
// #include <MPU6050.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_SHT31.h>
// #include <NewPing.h>
// #include <Adafruit_NeoPixel.h>
// #include <Servo.h>

// // 핀 및 객체 선언
// #define RED_LED 44
// #define YELLOW_LED 45
// #define GREEN_LED 46

// #define TRIG_PIN 25
// #define ECHO_PIN 26
// #define MAX_DISTANCE 200

// #define BUZZER_PIN 7

// #define R_PIN 11
// #define G_PIN 10
// #define B_PIN 9

// #define LASER_PIN 12

// #define BUTTON1_PIN 22
// #define BUTTON2_PIN 23

// #define NEOPIXEL_PIN 16
// #define NEOPIXEL_COUNT 16

// #define SERVO1_PIN 2

// #define SAMPLE_COUNT 10 // 가속도 센서 샘플링 횟수

// int16_t axSum = 0, aySum = 0, azSum = 0;
// int16_t ax, ay, az;

// MPU6050 mpu;
// Adafruit_SHT31 sht31 = Adafruit_SHT31();
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Adafruit_SSD1306 display(128, 64, &Wire, -1);
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Servo servo1;

// enum TrafficLightState { RED, GREEN, YELLOW };
// TrafficLightState trafficState = RED;

// unsigned long previousMillis = 0;
// const long interval = 10000;

// bool gateOpen = true;
// bool lastButtonState = HIGH;
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;

// void setup() {
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   servo1.attach(SERVO1_PIN);
//   servo1.write(90);

//   Serial.begin(115200);
//   Wire.begin();

//   mpu.initialize();
//   mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
//   mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);

//   sht31.begin(0x44);

//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);

//   pinMode(BUZZER_PIN, OUTPUT);
//   pinMode(R_PIN, OUTPUT);
//   pinMode(G_PIN, OUTPUT);
//   pinMode(B_PIN, OUTPUT);

//   pinMode(LASER_PIN, OUTPUT);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);

//   pixels.begin();
//   pixels.show();

//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("OLED 초기화 실패");
//   }
//   display.clearDisplay();
//   display.display();
// }

// void loop() {
//   unsigned long currentMillis = millis();

//   unsigned int distance = sonar.ping_cm();
//   bool carDetected = (distance > 0 && distance < 20);

//   int reading = digitalRead(BUTTON1_PIN);
//   if (reading != lastButtonState) {
//     lastDebounceTime = millis();
//   }
//   if ((millis() - lastDebounceTime) > debounceDelay) {
//     static bool buttonPressed = false;
//     if (reading == LOW && !buttonPressed) {
//       buttonPressed = true;
//       if (gateOpen) {
//         servo1.write(0);
//         Serial.println("Gate closed");
//       } else {
//         servo1.write(90);
//         Serial.println("Gate opened");
//       }
//       gateOpen = !gateOpen;
//     } else if (reading == HIGH) {
//       buttonPressed = false;
//     }
//   }
//   lastButtonState = reading;

//   axSum = aySum = azSum = 0;
//   for (int i = 0; i < SAMPLE_COUNT; i++) {
//     mpu.getAcceleration(&ax, &ay, &az);
//     axSum += ax;
//     aySum += ay;
//     azSum += az;
//     delay(10);
//   }
//   ax = axSum / SAMPLE_COUNT;
//   ay = aySum / SAMPLE_COUNT;
//   az = azSum / SAMPLE_COUNT;

//   Serial.print("Avg Accel: ");
//   Serial.print(ax); Serial.print(", ");
//   Serial.print(ay); Serial.print(", ");
//   Serial.println(az);

//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;
//     changeTrafficLight();
//   }

//   if (digitalRead(BUTTON1_PIN) == LOW) {
//     trafficState = RED;
//     previousMillis = currentMillis;
//   }
//   if (digitalRead(BUTTON2_PIN) == LOW) {
//     trafficState = GREEN;
//     previousMillis = currentMillis;
//   }

//   int16_t gx, gy, gz;
//   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//   float temp = sht31.readTemperature();
//   float humidity = sht31.readHumidity();

//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0, 0);

//   display.print("Traffic: ");
//   switch (trafficState) {
//     case RED: display.println("RED"); break;
//     case GREEN: display.println("GREEN"); break;
//     case YELLOW: display.println("YELLOW"); break;
//   }
//   display.print("Distance: "); display.print(distance); display.println(" cm");
//   display.print("Temp: "); display.print(temp, 1); display.println(" C");
//   display.print("Humidity: "); display.print(humidity, 1); display.println(" %");
//   display.print("Accel: "); display.print(ax); display.print(", "); display.print(ay); display.print(", "); display.println(az);

//   display.display();

//   updateTrafficLights(carDetected);
//   updateNeoPixel(carDetected);
//   updateRGBLed(carDetected);

//   Serial.println(mpu.testConnection() ? "MPU 연결 OK" : "MPU 연결 실패");
//   delay(100);
// }

// void changeTrafficLight() {
//   if (trafficState == RED) trafficState = GREEN;
//   else if (trafficState == GREEN) trafficState = YELLOW;
//   else if (trafficState == YELLOW) trafficState = RED;
// }

// void updateTrafficLights(bool overrideRed) {
//   if (overrideRed) {
//     digitalWrite(RED_LED, HIGH);
//     digitalWrite(YELLOW_LED, LOW);
//     digitalWrite(GREEN_LED, LOW);
//   } else {
//     digitalWrite(RED_LED, trafficState == RED ? HIGH : LOW);
//     digitalWrite(YELLOW_LED, trafficState == YELLOW ? HIGH : LOW);
//     digitalWrite(GREEN_LED, trafficState == GREEN ? HIGH : LOW);
//   }
// }

// void updateNeoPixel(bool overrideRed) {
//   uint32_t color;
//   if (overrideRed || trafficState == RED) {
//     color = pixels.Color(255, 0, 0);
//   } else if (trafficState == YELLOW) {
//     color = pixels.Color(255, 150, 0);
//   } else {
//     color = pixels.Color(0, 255, 0);
//   }
//   for (int i = 0; i < NEOPIXEL_COUNT; i++) {
//     pixels.setPixelColor(i, color);
//   }
//   pixels.show();
// }

// void updateRGBLed(bool overrideRed) {
//   if (overrideRed || trafficState == RED) {
//     analogWrite(R_PIN, 255);
//     analogWrite(G_PIN, 0);
//     analogWrite(B_PIN, 0);
//   } else if (trafficState == YELLOW) {
//     analogWrite(R_PIN, 255);
//     analogWrite(G_PIN, 150);
//     analogWrite(B_PIN, 0);
//   } else {
//     analogWrite(R_PIN, 0);
//     analogWrite(G_PIN, 255);
//     analogWrite(B_PIN, 0);
//   }
// }

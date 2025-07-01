// #include <Wire.h>
// #include <MPU6050.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_SHT31.h>
// #include <NewPing.h>
// #include <Adafruit_NeoPixel.h>
// #include <Servo.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_LEDBackpack.h>
// #include <TM1637Display.h>

// // 핀 및 상수 선언
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

// #ifndef LED_ON
// #define LED_ON 1
// #endif

// #define SAMPLE_COUNT 10

// // FND 핀 정의
// #define CLK_PIN 29
// #define DIO_PIN 30

// // 객체들
// TM1637Display fnd(CLK_PIN, DIO_PIN);  // FND
// Adafruit_SSD1306 oled(128, 64, &Wire, -1);  // OLED
// MPU6050 mpu;
// Adafruit_SHT31 sht31 = Adafruit_SHT31();
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Servo servo1;
// Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// enum TrafficLightState { RED, GREEN, YELLOW };
// TrafficLightState trafficState = RED;

// unsigned long previousMillis = 0;
// const long interval = 10000;

// bool gateOpen = true;
// bool lastButtonState = HIGH;
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;

// int16_t last_ax = 0, last_ay = 0, last_az = 0;
// int16_t axSum = 0, aySum = 0, azSum = 0;
// int16_t ax, ay, az;

// // 함수 프로토타입
// void drawConcentricCircle(int level);
// void changeTrafficLight();
// void updateTrafficLights(bool overrideRed);
// void updateNeoPixel(bool overrideRed);
// void updateRGBLed(bool overrideRed);

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();

//   fnd.setBrightness(0x0f); // FND 밝기 설정

//   matrix.begin(0x70);
//   matrix.clear();
//   matrix.writeDisplay();

//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);
//   servo1.attach(SERVO1_PIN);
//   servo1.write(90);

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

//   pixels.begin();
//   pixels.show();

//   if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("OLED 초기화 실패");
//   }
//   oled.clearDisplay();
//   oled.display();
// }

// void loop() {
//   unsigned long currentMillis = millis();

//   // 거리 측정
//   unsigned int distance = sonar.ping_cm();
//   bool carDetected = (distance > 0 && distance < 20);

//   // FND에 거리 표시
//   fnd.showNumberDec(distance, false);

//   // 버튼 처리
//   int reading = digitalRead(BUTTON1_PIN);
//   if (reading != lastButtonState) {
//     lastDebounceTime = currentMillis;
//   }
//   if ((currentMillis - lastDebounceTime) > debounceDelay) {
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

//   // MPU6050 평균 가속도
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

//   oled.clearDisplay();
//   oled.setTextSize(1);
//   oled.setTextColor(SSD1306_WHITE);
//   oled.setCursor(0, 0);

//   oled.print("Traffic: ");
//   switch (trafficState) {
//     case RED: oled.println("RED"); break;
//     case GREEN: oled.println("GREEN"); break;
//     case YELLOW: oled.println("YELLOW"); break;
//   }
//   oled.print("Distance: "); oled.print(distance); oled.println(" cm");
//   oled.print("Temp: "); oled.print(temp, 1); oled.println(" C");
//   oled.print("Humidity: "); oled.print(humidity, 1); oled.println(" %");
//   oled.print("Accel: ");
//   oled.print(ax); oled.print(", ");
//   oled.print(ay); oled.print(", ");
//   oled.println(az);
//   oled.display();

//   updateTrafficLights(carDetected);
//   updateNeoPixel(carDetected);
//   updateRGBLed(carDetected);

//   Serial.println(mpu.testConnection() ? "MPU 연결 OK" : "MPU 연결 실패");

//   int delta = abs(ax - last_ax) + abs(ay - last_ay) + abs(az - last_az);
//   last_ax = ax; last_ay = ay; last_az = az;

//   int level = 1;
//   if (delta > 3000) level = 4;
//   else if (delta > 2000) level = 3;
//   else if (delta > 1000) level = 2;

//   drawConcentricCircle(level);
//   delay(100);
// }

// void changeTrafficLight() {
//   if (trafficState == RED) trafficState = GREEN;
//   else if (trafficState == GREEN) trafficState = YELLOW;
//   else if (trafficState == YELLOW) trafficState = RED;
// }

// void updateTrafficLights(bool overrideRed) {
//   digitalWrite(RED_LED, overrideRed || trafficState == RED);
//   digitalWrite(YELLOW_LED, trafficState == YELLOW);
//   digitalWrite(GREEN_LED, !overrideRed && trafficState == GREEN);
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
//     analogWrite(R_PIN, 255); analogWrite(G_PIN, 0); analogWrite(B_PIN, 0);
//   } else if (trafficState == YELLOW) {
//     analogWrite(R_PIN, 255); analogWrite(G_PIN, 150); analogWrite(B_PIN, 0);
//   } else {
//     analogWrite(R_PIN, 0); analogWrite(G_PIN, 255); analogWrite(B_PIN, 0);
//   }
// }

// void drawConcentricCircle(int level) {
//   matrix.clear();
//   if (level >= 1) {
//     matrix.drawPixel(3, 3, LED_ON);
//     matrix.drawPixel(3, 4, LED_ON);
//     matrix.drawPixel(4, 3, LED_ON);
//     matrix.drawPixel(4, 4, LED_ON);
//   }
//   if (level >= 2) matrix.drawRect(2, 2, 4, 4, LED_ON);
//   if (level >= 3) matrix.drawRect(1, 1, 6, 6, LED_ON);
//   if (level >= 4) matrix.drawRect(0, 0, 8, 8, LED_ON);
//   matrix.writeDisplay();
// }

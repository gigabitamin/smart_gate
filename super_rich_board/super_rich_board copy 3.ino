// #include <Wire.h>
// #include <MPU6050.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_SHT31.h>
// #include <NewPing.h>
// #include <Adafruit_NeoPixel.h>
// #include <Servo.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_LEDBackpack.h>

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

// #define SAMPLE_COUNT 10 // 가속도 센서 샘플링 횟수

// // 전역 변수 선언
// int16_t last_ax = 0, last_ay = 0, last_az = 0;
// int16_t axSum = 0, aySum = 0, azSum = 0;
// int16_t ax, ay, az;

// MPU6050 mpu;
// Adafruit_SHT31 sht31 = Adafruit_SHT31();
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Adafruit_SSD1306 display(128, 64, &Wire, -1);
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Servo servo1;

// Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
// Adafruit_7segment fnd = Adafruit_7segment();

// enum TrafficLightState { RED, GREEN, YELLOW };
// TrafficLightState trafficState = RED;

// unsigned long previousMillis = 0;
// const long interval = 10000;

// bool gateOpen = true;
// bool lastButtonState = HIGH;
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;

// // 함수 프로토타입 (필요 시)
// void drawConcentricCircle(int level);
// void changeTrafficLight();
// void updateTrafficLights(bool overrideRed);
// void updateNeoPixel(bool overrideRed);
// void updateRGBLed(bool overrideRed);

// void setup() {  
//   Serial.begin(9600);
//   Wire.begin();

//   // FND 초기화
//   fnd.begin(0x70);  // I2C 주소 확인 필요 (0x70이 일반적임)
//   fnd.clear();
//   fnd.writeDisplay();

//   // I2C LED 매트릭스 초기화
//   matrix.begin(0x70);
//   matrix.clear();
//   matrix.writeDisplay();

//   // 버튼 및 서보 설정
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);

//   servo1.attach(SERVO1_PIN);
//   servo1.write(90); // 기본 열림 위치

//   // MPU6050 초기화
//   mpu.initialize();
//   mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
//   mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);

//   // SHT31 초기화
//   sht31.begin(0x44);

//   // LED 핀 설정
//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);

//   pinMode(BUZZER_PIN, OUTPUT);

//   pinMode(R_PIN, OUTPUT);
//   pinMode(G_PIN, OUTPUT);
//   pinMode(B_PIN, OUTPUT);

//   pinMode(LASER_PIN, OUTPUT);

//   // NeoPixel 초기화
//   pixels.begin();
//   pixels.show();

//   // OLED 초기화
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("OLED 초기화 실패");
//   }
//   display.clearDisplay();
//   display.display();
// }

// void loop() {
//   unsigned long currentMillis = millis();

//   // 초음파 거리 측정
//   unsigned int distance = sonar.ping_cm();
//   bool carDetected = (distance > 0 && distance < 20);

//   // FND에 거리 표시 (최대 4자리 표시 가능, 거리 최대 200cm이므로 문제 없음)
//   fnd.print(distance);
//   fnd.writeDisplay();

//   // 버튼 디바운스 처리 및 차단기 토글
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

//   // MPU6050 가속도 샘플링 평균
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

//   // 교통 신호등 상태 변경 타이머
//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;
//     changeTrafficLight();
//   }

//   // 버튼2에 따른 교통 신호 강제 제어
//   if (digitalRead(BUTTON1_PIN) == LOW) {
//     trafficState = RED;
//     previousMillis = currentMillis;
//   }
//   if (digitalRead(BUTTON2_PIN) == LOW) {
//     trafficState = GREEN;
//     previousMillis = currentMillis;
//   }

//   // MPU6050 자이로 데이터 읽기 (필요 시)
//   int16_t gx, gy, gz;
//   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//   // 온도 및 습도 읽기
//   float temp = sht31.readTemperature();
//   float humidity = sht31.readHumidity();

//   // OLED 정보 출력
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
//   display.print("Accel: ");
//   display.print(ax); display.print(", ");
//   display.print(ay); display.print(", ");
//   display.println(az);

//   display.display();

//   // 신호등, 네오픽셀, RGB LED 상태 업데이트
//   updateTrafficLights(carDetected);
//   updateNeoPixel(carDetected);
//   updateRGBLed(carDetected);

//   Serial.println(mpu.testConnection() ? "MPU 연결 OK" : "MPU 연결 실패");

//   // 가속도 변화량 계산 및 레벨 산출
//   int delta = abs(ax - last_ax) + abs(ay - last_ay) + abs(az - last_az);
//   last_ax = ax;
//   last_ay = ay;
//   last_az = az;

//   int level = 1;
//   if (delta > 3000) level = 4;
//   else if (delta > 2000) level = 3;
//   else if (delta > 1000) level = 2;

//   // LED 매트릭스에 원형 표시
//   drawConcentricCircle(level);

//   delay(100);
// }

// // 교통 신호 상태 변경 함수
// void changeTrafficLight() {
//   if (trafficState == RED) trafficState = GREEN;
//   else if (trafficState == GREEN) trafficState = YELLOW;
//   else if (trafficState == YELLOW) trafficState = RED;
// }

// // 실제 LED 신호등 업데이트
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

// // 네오픽셀 LED 색상 업데이트
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

// // RGB LED 색상 업데이트
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

// // LED 매트릭스에 동심원 원형 표시
// void drawConcentricCircle(int level) {
//   matrix.clear();
//   if (level >= 1) {
//     matrix.drawPixel(3, 3, LED_ON);
//     matrix.drawPixel(3, 4, LED_ON);
//     matrix.drawPixel(4, 3, LED_ON);
//     matrix.drawPixel(4, 4, LED_ON);
//   }
//   if (level >= 2) {
//     matrix.drawRect(2, 2, 4, 4, LED_ON);
//   }
//   if (level >= 3) {
//     matrix.drawRect(1, 1, 6, 6, LED_ON);
//   }
//   if (level >= 4) {
//     matrix.drawRect(0, 0, 8, 8, LED_ON);
//   }
//   matrix.writeDisplay();
// }

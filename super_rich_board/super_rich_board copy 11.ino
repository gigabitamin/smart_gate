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
// #include <LiquidCrystal_I2C.h>
// #include <HUSKYLENS.h>

// // 허스키렌즈
// HUSKYLENS huskylens;

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
// #define TOUCH_PIN 24

// #ifndef LED_ON
// #define LED_ON 1
// #endif

// #define SAMPLE_COUNT 10 // 가속도 센서 샘플링 횟수

// // FND 핀 정의
// #define CLK_PIN 29
// #define DIO_PIN 30

// // I2C LED 매트릭스 핀 정의
// TM1637Display fndDisplay(CLK_PIN, DIO_PIN);

// // I2C OLED 디스플레이 핀 정의
// Adafruit_SSD1306 oledDisplay(128, 64, &Wire, -1);

// // I2C LED 매트릭스 주소
// LiquidCrystal_I2C lcd(0x20, 16, 2); // 주소 0x20, 16x2 LCD

// // 전역 변수 선언
// int16_t last_ax = 0, last_ay = 0, last_az = 0;
// int16_t axSum = 0, aySum = 0, azSum = 0;
// int16_t ax, ay, az;
// bool systemEnabled = false;

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

// // temp, humidity 변수 선언 (초기화는 loop에서)
// float temp, humidity;

// void drawConcentricCircle(int level);
// void changeTrafficLight();
// void updateTrafficLights(bool overrideRed);
// void updateNeoPixel(bool overrideRed);
// void updateRGBLed(bool overrideRed);

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
  
//   // 터치패드 핀 설정
//   pinMode(TOUCH_PIN, INPUT);

//   if (!huskylens.begin(Wire)) {
//     Serial.println("HuskyLens 초기화 실패!");
//   } else {
//     Serial.println("HuskyLens 시작됨");
//   }

//   // LCD 초기화
//   lcd.init();           // LCD 초기화
//   lcd.backlight();      // 백라이트 켜기
//   lcd.clear();          // 화면 지우기

//   // FND 초기화
//   fndDisplay.setBrightness(0x0f);

//   // I2C LED 매트릭스 초기화
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

//   if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("OLED 초기화 실패");
//   }
//   oledDisplay.clearDisplay();
//   oledDisplay.display();
// }

// void loop() {

//   // 터치패드 활성화 상태(on/off) 확인
//   systemEnabled = digitalRead(TOUCH_PIN);

//   // 터치패드가 활성화되지 않은 경우 모든 LED와 서보를 끄고 초기화
//   if (!systemEnabled) {
//     digitalWrite(RED_LED, LOW);
//     digitalWrite(YELLOW_LED, LOW);
//     digitalWrite(GREEN_LED, LOW);
//     servo1.write(0);
//     digitalWrite(BUZZER_PIN, LOW);
//     analogWrite(R_PIN, 0);
//     analogWrite(G_PIN, 0);
//     analogWrite(B_PIN, 0);

//     for (int i = 0; i < NEOPIXEL_COUNT; i++) {
//       pixels.setPixelColor(i, 0);
//     }
//     pixels.show();

//     fndDisplay.clear();

//     oledDisplay.clearDisplay();
//     oledDisplay.display();

//     matrix.clear();
//     matrix.writeDisplay();

//     // 센서값 읽기 및 출력
//     temp = sht31.readTemperature();
//     humidity = sht31.readHumidity();

//   // 허스키렌즈 요청
//   if (!huskylens.request()) {
//       Serial.println("HuskyLens 요청 실패");
//   } else if (!huskylens.available()) {
//       Serial.println("인식된 타겟 없음");

//       // 타겟 없을 때 차단기 내림
//       servo1.write(0);
//       gateOpen = false;
//   } else {
//       HUSKYLENSResult result = huskylens.read();

//       Serial.print("ID: ");
//       Serial.print(result.ID);
//       Serial.print(" | X: ");
//       Serial.print(result.xCenter);
//       Serial.print(" | Y: ");
//       Serial.println(result.yCenter);

//       // 특정 ID(예: 1)를 인식했을 때만 차단기 열기
//       if (result.ID == 1) {
//           servo1.write(90);
//           gateOpen = true;
//           Serial.println("허용된 타겟 인식됨 → 게이트 열림");
//       } else {
//           servo1.write(0);
//           gateOpen = false;
//           Serial.println("타겟은 있지만 허용되지 않음 → 게이트 닫힘");
//       }
//   }

//     delay(100);
//     return;
//   }

//   // 비동기적 처리
//   unsigned long currentMillis = millis();

//   // 초음파
//   unsigned int distance = sonar.ping_cm();
//   bool carDetected = (distance > 0 && distance < 20);

//   // FND
//   fndDisplay.showNumberDec(distance, false);

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

//   // 센서값 갱신 (여기서 매번 읽음)
//   temp = sht31.readTemperature();
//   humidity = sht31.readHumidity();

//   oledDisplay.clearDisplay();
//   oledDisplay.setTextSize(1);
//   oledDisplay.setTextColor(SSD1306_WHITE);
//   oledDisplay.setCursor(0, 0);

//   oledDisplay.print("Traffic: ");
//   switch (trafficState) {
//     case RED: oledDisplay.println("RED"); break;
//     case GREEN: oledDisplay.println("GREEN"); break;
//     case YELLOW: oledDisplay.println("YELLOW"); break;
//   }
//   oledDisplay.print("Distance: "); oledDisplay.print(distance); oledDisplay.println(" cm");
//   oledDisplay.print("Temp: "); oledDisplay.print(temp, 1); oledDisplay.println(" C");
//   oledDisplay.print("Humidity: "); oledDisplay.print(humidity, 1); oledDisplay.println(" %");
//   oledDisplay.print("Accel: ");
//   oledDisplay.print(ax); oledDisplay.print(", ");
//   oledDisplay.print(ay); oledDisplay.print(", ");
//   oledDisplay.println(az);

//   oledDisplay.display();

//   updateTrafficLights(carDetected);
//   updateNeoPixel(carDetected);
//   updateRGBLed(carDetected);

//   // 거리값 출력
//   Serial.print("DIST:");
//   Serial.println(distance);

//   // 차단기 상태 출력
//   Serial.print("GATE:");
//   Serial.println(gateOpen ? "PASS" : "STOP");

  
//   Serial.print("temp:");
//   Serial.println(temp, 1);

//   Serial.print("humidity:");
//   Serial.println(humidity, 1);

//   int delta = abs(ax - last_ax) + abs(ay - last_ay) + abs(az - last_az);
//   last_ax = ax;
//   last_ay = ay;
//   last_az = az;

//   int level = 1;
//   if (delta > 3000) level = 4;
//   else if (delta > 2000) level = 3;
//   else if (delta > 1000) level = 2;

//   drawConcentricCircle(level);

//   // LCD 출력
//   lcd.setCursor(0, 0); // 첫 번째 줄
//   lcd.print("Gate: ");
//   if (gateOpen) {
//     lcd.print("PASS ");
//   } else {
//     lcd.print("STOP ");
//   }

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
//     color = pixels.Color(55, 0, 0);
//   } else if (trafficState == YELLOW) {
//     color = pixels.Color(55, 50, 0);
//   } else {
//     color = pixels.Color(0, 55, 0);
//   }
//   for (int i = 0; i < NEOPIXEL_COUNT; i++) {
//     pixels.setPixelColor(i, color);
//   }
//   pixels.show();
// }

// void updateRGBLed(bool overrideRed) {
//   if (overrideRed || trafficState == RED) {
//     analogWrite(R_PIN, 55);
//     analogWrite(G_PIN, 0);
//     analogWrite(B_PIN, 0);
//   } else if (trafficState == YELLOW) {
//     analogWrite(R_PIN, 55);
//     analogWrite(G_PIN, 50);
//     analogWrite(B_PIN, 0);
//   } else {
//     analogWrite(R_PIN, 0);
//     analogWrite(G_PIN, 55);
//     analogWrite(B_PIN, 0);
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

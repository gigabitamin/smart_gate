// #include <Wire.h>
// #include <MPU6050.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_SHT31.h>
// #include <NewPing.h>
// #include <Adafruit_NeoPixel.h>
// #include <Servo.h>

// // 핀 및 객체 선언 (기존에 맞게 설정하세요)
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

// // 센서, 액추에이터 객체 선언
// MPU6050 mpu;
// Adafruit_SHT31 sht31 = Adafruit_SHT31();
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Adafruit_SSD1306 display(128, 64, &Wire, -1);
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Servo servo1;

// enum TrafficLightState { RED, GREEN, YELLOW };
// TrafficLightState trafficState = RED;

// unsigned long previousMillis = 0;
// const long interval = 10000; // 신호등 변경 간격 (예: 10초)

// // 차단기 상태 변수
// bool gateOpen = true;  // 초기 상태: 차단기 열려있음 (서보 90도)
// bool lastButtonState = HIGH;  // 버튼은 INPUT_PULLUP이므로 눌리지 않으면 HIGH
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;  // 디바운스 시간(ms)

// // setup 함수: 초기화

// void setup() {

//   // d22 버튼 : 차단기
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);  // D22 핀
//   servo1.attach(SERVO1_PIN);
//   servo1.write(90); // 차단기 열기 초기 위치

//   Serial.begin(115200);

//   Wire.begin();
//   mpu.initialize();
//   sht31.begin(0x44);

//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);

//   pinMode(BUZZER_PIN, OUTPUT);
//   pinMode(R_PIN, OUTPUT);
//   pinMode(G_PIN, OUTPUT);
//   pinMode(B_PIN, OUTPUT);

//   pinMode(LASER_PIN, OUTPUT);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);

//   pixels.begin();
//   pixels.show();

//   servo1.attach(SERVO1_PIN);
//   servo1.write(0); // 초기 위치

//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("OLED 초기화 실패");
//   }
//   display.clearDisplay();
//   display.display();
// }

// void loop() {
//   unsigned long currentMillis = millis();

//  int reading = digitalRead(BUTTON1_PIN);

//   // 디바운스 처리
//   if (reading != lastButtonState) {
//     lastDebounceTime = millis();
//   }

//   if ((millis() - lastDebounceTime) > debounceDelay) {
//     static bool buttonPressed = false;

//     // 버튼이 눌린 순간을 감지 (HIGH -> LOW)
//     if (reading == LOW && !buttonPressed) {
//       buttonPressed = true;

//       // 토글 동작
//       if (gateOpen) {
//         servo1.write(0);    // 차단기 내림
//         Serial.println("Gate closed");
//       } else {
//         servo1.write(90);   // 차단기 올림
//         Serial.println("Gate opened");
//       }
//       gateOpen = !gateOpen;  // 상태 변경
//     } 
//     else if (reading == HIGH) {
//       buttonPressed = false; // 버튼 뗐을 때 다시 눌릴 준비
//     }
//   }

//   lastButtonState = reading;

//   // 차량 감지: 초음파 센서로 거리 측정
//   unsigned int distance = sonar.ping_cm();

//   // 신호등 자동 제어 (간단 타이머 기반)
//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;
//     changeTrafficLight();
//   }

//   // 버튼 수동 제어
//   if (digitalRead(BUTTON1_PIN) == LOW) {
//     trafficState = RED;
//     previousMillis = currentMillis; // 타이머 초기화
//   }
//   if (digitalRead(BUTTON2_PIN) == LOW) {
//     trafficState = GREEN;
//     previousMillis = currentMillis;
//   }

//   // MPU6050 센서 데이터 (가속도, 자이로) 읽기
//   int16_t ax, ay, az, gx, gy, gz;
//   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//   // 온습도 센서 읽기
//   float temp = sht31.readTemperature();
//   float humidity = sht31.readHumidity();

//   // OLED 출력
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

//   display.print("Distance: ");
//   display.print(distance);
//   display.println(" cm");

//   display.print("Temp: ");
//   display.print(temp, 1);
//   display.println(" C");

//   display.print("Humidity: ");
//   display.print(humidity, 1);
//   display.println(" %");

//   display.print("Accel: ");
//   display.print(ax); display.print(", ");
//   display.print(ay); display.print(", ");
//   display.println(az);

//   display.display();

//   // 신호등 LED 업데이트
//   updateTrafficLights();

//   // // 네오픽셀 색상 변경 (예: 신호등 상태 표시)
//   // updateNeoPixel();

// // // 부저 경고 (예: 차량 감지시)
// // if (distance > 0 && distance < 20) {
// //   analogWrite(BUZZER_PIN, 128);  // 50% 듀티 사이클 PWM 신호 (값 0~255)
// // } else {
// //   analogWrite(BUZZER_PIN, 0);    // PWM OFF
// // }

//   // RGB LED 예: 신호등 상태별 색상 변경
//   updateRGBLed();

//   delay(100); // 루프 주기 조절
// }

// void changeTrafficLight() {
//   if (trafficState == RED) trafficState = GREEN;
//   else if (trafficState == GREEN) trafficState = YELLOW;
//   else if (trafficState == YELLOW) trafficState = RED;
// }

// void updateTrafficLights() {
//   digitalWrite(RED_LED, trafficState == RED ? HIGH : LOW);
//   digitalWrite(YELLOW_LED, trafficState == YELLOW ? HIGH : LOW);
//   digitalWrite(GREEN_LED, trafficState == GREEN ? HIGH : LOW);
// }

// // void updateNeoPixel() {
// //   uint32_t color;
// //   if (trafficState == RED) color = pixels.Color(255, 0, 0);
// //   else if (trafficState == YELLOW) color = pixels.Color(255, 150, 0);
// //   else color = pixels.Color(0, 255, 0);

// //   for (int i = 0; i < NEOPIXEL_COUNT; i++) {
// //     pixels.setPixelColor(i, color);
// //   }
// //   pixels.show();
// // }

// void updateRGBLed() {
//   if (trafficState == RED) {
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

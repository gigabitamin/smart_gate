// // 통합 스마트 게이트 제어 (허스키렌즈 + 홈페이지 수동 제어)

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

// HUSKYLENS huskylens;

// // 핀 정의
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
// #define CLK_PIN 29
// #define DIO_PIN 30

// #define ALLOWED_ID 1
// #define SERIAL_GATE_COMMAND "GATE:OPEN"

// bool gateOpen = false;
// bool systemEnabled = false;

// // 객체 선언
// MPU6050 mpu;
// Adafruit_SHT31 sht31 = Adafruit_SHT31();
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Servo servo1;
// Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
// TM1637Display fndDisplay(CLK_PIN, DIO_PIN);
// LiquidCrystal_I2C lcd(0x20, 16, 2);
// Adafruit_SSD1306 oledDisplay(128, 64, &Wire, -1);

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();

//   // 센서/디스플레이 초기화
//   sht31.begin(0x44);
//   oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//   fndDisplay.setBrightness(0x0f);
//   matrix.begin(0x70);
//   lcd.init(); lcd.backlight();

//   // 핀 설정
//   pinMode(TOUCH_PIN, INPUT);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);
//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);
//   pinMode(BUZZER_PIN, OUTPUT);
//   pinMode(R_PIN, OUTPUT);
//   pinMode(G_PIN, OUTPUT);
//   pinMode(B_PIN, OUTPUT);
//   pinMode(LASER_PIN, OUTPUT);

//   pixels.begin(); pixels.show();
//   servo1.attach(SERVO1_PIN);
//   closeGate();

//   mpu.initialize();

//   if (!huskylens.begin(Wire)) Serial.println("❌ HuskyLens 실패");
//   else Serial.println("✅ HuskyLens 연결됨");
// }

// void loop() {
//   systemEnabled = digitalRead(TOUCH_PIN);

//   if (!systemEnabled) {
//     resetOutputs();
//     return;
//   }

//   // 시리얼 명령 수신
//   if (Serial.available()) {
//     String command = Serial.readStringUntil('\n');
//     command.trim();
//     if (command == SERIAL_GATE_COMMAND) {
//       Serial.println("🌐 원격 명령 수신 → 게이트 열기");
//       openGate();
//     }
//   }

//   // 허스키렌즈 인식
//   if (huskylens.request()) {
//     if (huskylens.available()) {
//       HUSKYLENSResult result = huskylens.read();
//       if (result.ID == ALLOWED_ID) {
//         openGate();
//       } else {
//         closeGate();
//       }
//     } else {
//       closeGate();
//     }
//   }

//   // 초음파 측정
//   int distance = sonar.ping_cm();
//   fndDisplay.showNumberDec(distance);

//   // OLED 출력 예시
//   oledDisplay.clearDisplay();
//   oledDisplay.setTextSize(1);
//   oledDisplay.setTextColor(SSD1306_WHITE);
//   oledDisplay.setCursor(0, 0);
//   oledDisplay.print("Distance: "); oledDisplay.println(distance);
//   oledDisplay.print("Gate: "); oledDisplay.println(gateOpen ? "OPEN" : "CLOSED");
//   oledDisplay.display();

//   delay(200);
// }

// void openGate() {
//   if (!gateOpen) {
//     servo1.write(90);
//     gateOpen = true;
//     Serial.println("✅ 차단기 열림");
//   }
// }

// void closeGate() {
//   if (gateOpen) {
//     servo1.write(0);
//     gateOpen = false;
//     Serial.println("⛔ 차단기 닫힘");
//   }
// }

// void resetOutputs() {
//   digitalWrite(RED_LED, LOW);
//   digitalWrite(YELLOW_LED, LOW);
//   digitalWrite(GREEN_LED, LOW);
//   digitalWrite(BUZZER_PIN, LOW);
//   analogWrite(R_PIN, 0);
//   analogWrite(G_PIN, 0);
//   analogWrite(B_PIN, 0);
//   for (int i = 0; i < NEOPIXEL_COUNT; i++) pixels.setPixelColor(i, 0);
//   pixels.show();
//   matrix.clear(); matrix.writeDisplay();
//   fndDisplay.clear();
//   oledDisplay.clearDisplay(); oledDisplay.display();
//   closeGate();
// }
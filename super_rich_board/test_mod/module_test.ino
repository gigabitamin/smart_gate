// #include <Wire.h>
// #include <MPU6050.h>
// #include <Adafruit_GFX.h>
// #include <SoftwareSerial.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_NeoPixel.h>
// #include <Servo.h>
// #include <NewPing.h>
// #include <Adafruit_SHT31.h>
// #include <Adafruit_LEDBackpack.h>
// #include <LiquidCrystal_I2C.h>

// // LCD 주소 0x20, 16x2 크기
// LiquidCrystal_I2C lcd(0x20, 16, 2);

// // MPU6050 객체
// MPU6050 mpu;

// // 신호등 핀
// #define RED_LED 44
// #define YELLOW_LED 45
// #define GREEN_LED 46

// // 초음파 센서 핀
// #define TRIG_PIN 25
// #define ECHO_PIN 26
// #define MAX_DISTANCE 200

// // 부저
// #define BUZZER_PIN 7

// // RGB LED 핀
// #define R_PIN 11
// #define G_PIN 10
// #define B_PIN 9

// // 레이저 센서
// #define LASER_PIN 12

// // 터치패드
// #define TOUCH_PIN 24

// // 버튼
// #define BUTTON1_PIN 22
// #define BUTTON2_PIN 23

// // 네오픽셀
// #define NEOPIXEL_PIN 16
// #define NEOPIXEL_COUNT 16

// // FND 핀 (임의)
// #define FND_PIN 29

// // 조이스틱 아날로그 핀
// #define JOYSTICK_X A2
// #define JOYSTICK_Y A3

// // 아날로그 슬라이더, 다이얼
// #define SLIDER_PIN A5
// #define DIAL_PIN A1

// // 서보
// Servo servo1;
// Servo servo2;
// #define SERVO1_PIN 2
// #define SERVO2_PIN 3

// // MP3 모듈 시리얼
// SoftwareSerial mp3Serial(14, 15); // RX, TX

// // I2C 온습도 센서
// Adafruit_SHT31 sht31 = Adafruit_SHT31();

// // I2C LED 매트릭스
// Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// // OLED (128x64)
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// // 네오픽셀 객체
// Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// // 초음파 센서 객체
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// void setup()
// {
//   Serial.begin(115200);
//   mp3Serial.begin(9600);

//   // I2C 시작
//   Wire.begin();

//   // LCD 초기화
//   lcd.init();      // LCD 초기화
//   lcd.backlight(); // 백라이트 켜기
//   lcd.clear();
//   lcd.setCursor(0, 0); // 첫번째 행, 첫번째 열
//   lcd.print("abcd");

//   // MPU6050 초기화
//   Serial.println("Initializing MPU6050...");
//   mpu.initialize();
//   if (mpu.testConnection())
//   {
//     Serial.println("MPU6050 connection successful");
//   }
//   else
//   {
//     Serial.println("MPU6050 connection failed");
//   }

//   // 신호등 LED
//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);

//   // 부저
//   pinMode(BUZZER_PIN, OUTPUT);

//   // RGB LED
//   pinMode(R_PIN, OUTPUT);
//   pinMode(G_PIN, OUTPUT);
//   pinMode(B_PIN, OUTPUT);

//   // 레이저 센서, 터치패드, 버튼
//   pinMode(LASER_PIN, INPUT);
//   pinMode(TOUCH_PIN, INPUT);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);

//   // FND (필요시 수정)
//   pinMode(FND_PIN, OUTPUT);

//   // 서보 모터 초기화
//   servo1.attach(SERVO1_PIN);
//   servo2.attach(SERVO2_PIN);

//   // 온습도 센서 초기화
//   if (!sht31.begin(0x44))
//   {
//     Serial.println("SHT31 temperature/humidity sensor init failed!");
//   }

//   // LED 매트릭스 초기화
//   matrix.begin(0x70);

//   // OLED 초기화
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
//   {
//     Serial.println("OLED init failed!");
//   }
//   display.clearDisplay();
//   display.display();

//   // 네오픽셀 초기화
//   pixels.begin();
//   pixels.show();

//   Serial.println("Setup complete");

//   // 매트릭스 출력
//   Wire.begin();
//   matrix.begin(0x70);
//   matrix.clear();
//   matrix.drawPixel(0, 0, LED_ON);
//   matrix.writeDisplay();

//   // 레이저 출력
//   pinMode(LASER_PIN, OUTPUT);
// }

// void loop()
// {

//   // 레이저 출력
//   digitalWrite(LASER_PIN, HIGH); // 레이저 ON
//   delay(1000);
//   digitalWrite(LASER_PIN, LOW); // 레이저 OFF
//   delay(1000);

//   // 신호등 예시: 초록 LED 켜기
//   digitalWrite(RED_LED, LOW);
//   digitalWrite(YELLOW_LED, LOW);
//   digitalWrite(GREEN_LED, HIGH);

//   // 초음파 센서 거리 측정
//   unsigned int distance = sonar.ping_cm();
//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");

//   // 온습도 센서 읽기
//   float temp = sht31.readTemperature();
//   float humidity = sht31.readHumidity();
//   Serial.print("Temp: ");
//   Serial.print(temp);
//   Serial.print(" C, Humidity: ");
//   Serial.print(humidity);
//   Serial.println(" %");

//   // MPU6050 센서 읽기
//   int16_t ax, ay, az;
//   int16_t gx, gy, gz;
//   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//   Serial.print("Accel [ax, ay, az]: ");
//   Serial.print(ax);
//   Serial.print(", ");
//   Serial.print(ay);
//   Serial.print(", ");
//   Serial.println(az);

//   Serial.print("Gyro [gx, gy, gz]: ");
//   Serial.print(gx);
//   Serial.print(", ");
//   Serial.print(gy);
//   Serial.print(", ");
//   Serial.println(gz);

//   // OLED에 표시
//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0, 0);

//   display.clearDisplay();
//   display.setCursor(0, 0);
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
//   display.print(ax);
//   display.print(", ");
//   display.print(ay);
//   display.print(", ");
//   display.println(az);

//   display.print("Gyro: ");
//   display.print(gx);
//   display.print(", ");
//   display.print(gy);
//   display.print(", ");
//   display.println(gz);

//   display.display();

//   // 네오픽셀 빨강 불 켜기
//   for (int i = 0; i < NEOPIXEL_COUNT; i++)
//   {
//     pixels.setPixelColor(i, pixels.Color(255, 0, 0));
//   }
//   pixels.show();

//   // RGB LED 파랑 불 켜기
//   analogWrite(R_PIN, 0);
//   analogWrite(G_PIN, 0);
//   analogWrite(B_PIN, 255);

//   // 서보 모터 위치 설정
//   servo1.write(0);
//   servo2.write(90);

//   // 버튼 상태 확인
//   if (digitalRead(BUTTON1_PIN) == LOW)
//   {
//     Serial.println("Button1 pressed");
//   }
//   if (digitalRead(BUTTON2_PIN) == LOW)
//   {
//     Serial.println("Button2 pressed");
//   }

//   // 터치패드 상태
//   int touchState = digitalRead(TOUCH_PIN);
//   Serial.print("Touch: ");
//   Serial.println(touchState);

//   // 레이저 센서 상태
//   int laserState = digitalRead(LASER_PIN);
//   Serial.print("Laser: ");
//   Serial.println(laserState);

//   // 아날로그 센서 읽기
//   int sliderVal = analogRead(SLIDER_PIN);
//   int dialVal = analogRead(DIAL_PIN);
//   int joyX = analogRead(JOYSTICK_X);
//   int joyY = analogRead(JOYSTICK_Y);

//   Serial.print("Slider: ");
//   Serial.print(sliderVal);
//   Serial.print(", Dial: ");
//   Serial.print(dialVal);
//   Serial.print(", JoyX: ");
//   Serial.print(joyX);
//   Serial.print(", JoyY: ");
//   Serial.println(joyY);

//   delay(1000); // 1초 대기
// }

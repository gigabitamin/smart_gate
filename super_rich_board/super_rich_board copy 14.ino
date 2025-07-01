// #include <Wire.h>
// #include <HUSKYLENS.h>
// #include <Servo.h>

// HUSKYLENS huskylens;
// Servo gateServo;

// #define SERVO_PIN 2
// #define ALLOWED_ID 1  // 허용된 타겟 ID

// bool gateOpen = false; // 현재 차단기 상태 저장

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();

//   if (!huskylens.begin(Wire)) {
//     Serial.println("❌ HuskyLens 초기화 실패!");
//     while (1);
//   } else {
//     Serial.println("✅ HuskyLens 연결됨");
//   }

//   gateServo.attach(SERVO_PIN);
//   gateServo.write(0); // 처음엔 차단기 닫힘
//   gateOpen = false;
// }

// void loop() {
//   if (!huskylens.request()) {
//     Serial.println("📡 HuskyLens 요청 실패");
//     closeGate();
//     delay(500);
//     return;
//   }

//   if (!huskylens.available()) {
//     Serial.println("🚫 인식된 타겟 없음");
//     closeGate();
//   } else {
//     HUSKYLENSResult result = huskylens.read();
//     Serial.print("🎯 인식된 ID: ");
//     Serial.println(result.ID);

//     if (result.ID == ALLOWED_ID) {
//       openGate();
//     } else {
//       closeGate();
//     }
//   }

//   delay(300);
// }

// // 게이트 여는 함수 (이미 열려 있으면 아무 것도 안함)
// void openGate() {
//   if (!gateOpen) {
//     Serial.println("✅ 허용된 차량 → 게이트 열기");
//     gateServo.write(90);
//     gateOpen = true;
//   }
// }

// // 게이트 닫는 함수 (이미 닫혀 있으면 아무 것도 안함)
// void closeGate() {
//   if (gateOpen) {
//     Serial.println("⛔️ 차단 → 게이트 닫기");
//     gateServo.write(0);
//     gateOpen = false;
//   }
// }

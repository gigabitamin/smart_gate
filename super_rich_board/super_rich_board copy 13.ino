// #include <Wire.h>
// #include <HUSKYLENS.h>
// #include <Servo.h>

// HUSKYLENS huskylens;
// Servo gateServo;

// #define SERVO_PIN 2
// #define ALLOWED_ID 1  // 학습된 타겟 ID (예: 차량 태그 ID)

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
//   gateServo.write(0); // 시작은 닫힌 상태
// }

// void loop() {
//   if (!huskylens.request()) {
//     Serial.println("📡 HuskyLens 요청 실패");
//     gateServo.write(0); // 차단기 닫음
//     delay(500);
//     return;
//   }

//   if (!huskylens.available()) {
//     Serial.println("🚫 인식된 타겟 없음");
//     gateServo.write(0); // 차단기 닫음
//   } else {
//     HUSKYLENSResult result = huskylens.read();

//     Serial.print("🎯 인식된 ID: ");
//     Serial.println(result.ID);

//     if (result.ID == ALLOWED_ID) {
//       Serial.println("✅ 허용된 차량 인식 → 게이트 열림");
//       gateServo.write(90); // 차단기 올림
//     } else {
//       Serial.println("⛔️ 허용되지 않은 ID → 게이트 닫힘");
//       gateServo.write(0); // 차단기 닫음
//     }
//   }

//   delay(300); // 300ms 주기로 확인
// }

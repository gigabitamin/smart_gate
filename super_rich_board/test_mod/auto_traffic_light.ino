// #define RED_LED 44
// #define YELLOW_LED 45
// #define GREEN_LED 46
// #define BUZZER 7

// void setup() {
//   Serial.begin(9600);

//   pinMode(RED_LED, OUTPUT);
//   pinMode(YELLOW_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);
//   pinMode(BUZZER, OUTPUT);

//   Serial.println("🚦 스마트 교통 시스템 시작");
// }

// void loop() {
//   // 1. 빨간불 (정지)
//   Serial.println("RED ON");
//   digitalWrite(RED_LED, HIGH);
//   digitalWrite(YELLOW_LED, LOW);
//   digitalWrite(GREEN_LED, LOW);
//   delay(5000);

//   // 2. 초록불 (진행)
//   Serial.println("GREEN ON");
//   digitalWrite(RED_LED, LOW);
//   digitalWrite(GREEN_LED, HIGH);
//   delay(5000);

//   // 3. 노란불 (주의 + 부저)
//   Serial.println("YELLOW ON + BUZZER");
//   digitalWrite(GREEN_LED, LOW);
//   digitalWrite(YELLOW_LED, HIGH);
// //   tone(BUZZER, 1000, 500);  // 1kHz 삑
//   delay(2000);
  
//   // 4. 부저 끄기
// //   noTone(BUZZER);
// //   digitalWrite(YELLOW_LED, LOW);
// //   delay(1000); // 잠시 대기
// }
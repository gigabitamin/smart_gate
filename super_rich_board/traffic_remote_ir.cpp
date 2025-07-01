// #include <IRremote.h>

// #define RECV_PIN 13 // 리모컨
// #define RED_LED 44
// #define GREEN_LED 46
// #define BUZZER 7

// IRrecv irrecv(RECV_PIN);
// decode_results results;

// void setup() {
//   Serial.begin(9600);
//   irrecv.enableIRIn();

//   pinMode(RED_LED, OUTPUT);
//   pinMode(GREEN_LED, OUTPUT);
//   pinMode(BUZZER, OUTPUT);
// }

// void loop() {
//   if (irrecv.decode(&results)) {
//     unsigned long code = results.value;
//     Serial.println(code, HEX);

//     if (code == 0xFFA25D) {
//       digitalWrite(GREEN_LED, HIGH);
//       digitalWrite(RED_LED, LOW);
//     } else if (code == 0xFF629D) {
//       digitalWrite(GREEN_LED, LOW);
//       digitalWrite(RED_LED, HIGH);
//       tone(BUZZER, 1000, 500);
//     }

//     irrecv.resume();
//   }
// }

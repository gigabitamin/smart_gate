// #include <Wire.h>
// #include <Adafruit_SHT31.h>

// Adafruit_SHT31 sht31 = Adafruit_SHT31();

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   if (!sht31.begin(0x44)) {
//     Serial.println("SHT31 초기화 실패!");
//     while (1) delay(1);
//   }
// }

// void loop() {
//   float temp = sht31.readTemperature();
//   float humidity = sht31.readHumidity();

//   if (!isnan(temp) && !isnan(humidity)) {
//     Serial.print("Temperature: ");
//     Serial.print(temp);
//     Serial.print(" C, Humidity: ");
//     Serial.print(humidity);
//     Serial.println(" %");
//   } else {
//     Serial.println("센서 값 읽기 실패");
//   }

//   delay(1000);
// }

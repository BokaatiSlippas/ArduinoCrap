#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

Servo myServo;
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial
  
  // Initialize RFID
  SPI.begin();
  if (!mfrc522.PCD_Init()) {
    Serial.println("RFID Init Failed!");
    while (1); // Halt if RFID fails
  }
  Serial.println("RFID Ready");
  mfrc522.PCD_DumpVersionToSerial();

  // LCD Setup
  lcd.begin(16, 2);
  lcd.print("Scan Card...");

  // Servo Setup
  myServo.attach(2);
  myServo.write(70); // Locked
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("Card Detected!");
  lcd.clear();
  lcd.print("Access Granted!");
  myServo.write(160); // Unlock
  delay(5000);
  myServo.write(70); // Relock
}

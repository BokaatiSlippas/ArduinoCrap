#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Crypto.h>
#include <SHA256.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// LCD setup (4-bit mode: RS, EN, D's)
LiquidCrystal lcd(53, 52, 13, 12, 11, 10);

Servo myServo;
const int servoPin = 51;
const String storedHash = "2f77668a9dfbf8d5848b9eeb4a7145ca94c6ed9236e4a773f6dcfa51304b2b1d"; // SHA-256 hash of "A1B2C3"
String inputText = "";

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Enter Password:");
  myServo.attach(servoPin);
  myServo.write(0);
}

String computeHash(const String &input) {
  SHA256 sha256;
  uint8_t hash[32];
  
  sha256.reset();
  sha256.update(input.c_str(), input.length());
  sha256.finalize(hash, sizeof(hash));

  String result;
  for (int i = 0; i < 32; i++) {
    char buf[3];
    sprintf(buf, "%02x", hash[i]); // Convert to hex
    result += buf;
  }
  return result;
}

void loop() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.println(customKey);
    
    if (customKey == '#') {  // Clear input
      inputText = "";
      lcd.clear();
      lcd.print("Enter Password:");
    } else if (customKey == '*') {  // Submit password
      String inputHash = computeHash(inputText);
      if (inputHash == storedHash) {
        lcd.clear();
        lcd.print("Access Granted!");
        myServo.write(90);  // Unlock door by turning servo 90 degrees
        delay(5000); // 5 seconds to actually enter the door
        myServo.write(0);
        inputText = "";
        lcd.clear();
        lcd.print("Enter Password:");
      } else { // Add character to screen
        lcd.clear();
        lcd.print("Wrong Password!");
        delay(1000);
        inputText = "";
        lcd.clear();
        lcd.print("Enter Password:");
      }
    } else {
      inputText += customKey;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(inputText);
    }
  }
}

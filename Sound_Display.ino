// Segment pins (A to G, DP)
const int segmentPins[] = {
  13, // A
  12, // B
  11, // C
  10, // D
  9,  // E
  8,  // F
  7,  // G
  6   // DP (optional)
};

// Digit select pins (D1 to D4)
const int digitPins[] = {
  5, // D1 (leftmost digit)
  4, // D2
  3, // D3
  2  // D4 (rightmost digit)
};

// Segment patterns for digits 0-9 (common cathode)
// Format: A, B, C, D, E, F, G (DP is ignored here)
const byte digitPatterns[10] = {
  B11111100, // 0 (A,B,C,D,E,F)
  B01100000, // 1 (B,C)
  B11011010, // 2 (A,B,G,E,D)
  B11110010, // 3 (A,B,G,C,D)
  B01100110, // 4 (F,G,B,C)
  B10110110, // 5 (A,F,G,C,D)
  B10111110, // 6 (A,F,G,C,D,E)
  B11100000, // 7 (A,B,C)
  B11111110, // 8 (A,B,C,D,E,F,G)
  B11110110  // 9 (A,B,C,D,F,G)
};

// Sound sensor analog input
const int soundSensorPin = A0;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // 1 second in milliseconds
int currentDisplayValue = 0; // Stores the value to display

void setup() {
  Serial.begin(9600);
  
  // Initialize segment pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Initialize digit pins as OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // Only update the value once per second
  if (currentTime - lastUpdateTime >= updateInterval) {
    // Read and map the new value
    int soundValue = analogRead(soundSensorPin);
    currentDisplayValue = map(soundValue, 0, 1023, 0, 9999);
    currentDisplayValue = constrain(currentDisplayValue, 0, 9999);
    
    Serial.print("New Sound Level: ");
    Serial.println(currentDisplayValue);
    
    lastUpdateTime = currentTime; // Reset the timer
  }
  
  // Continuously display the current value
  displayNumber(currentDisplayValue);
}

// Function to display a 4-digit number
void displayNumber(int number) {
  number = constrain(number, 0, 9999);
  
  int digits[4];
  digits[0] = number / 1000;        // Thousands
  digits[1] = (number / 100) % 10;  // Hundreds
  digits[2] = (number / 10) % 10;   // Tens
  digits[3] = number % 10;          // Ones

  for (int digit = 0; digit < 4; digit++) {
    digitalWrite(digitPins[digit], LOW);
    
    byte pattern = digitPatterns[digits[digit]];
    for (int seg = 0; seg < 7; seg++) {
      digitalWrite(segmentPins[seg], bitRead(pattern, 7 - seg));
    }
    
    delay(5);
    digitalWrite(digitPins[digit], HIGH);
  }
}

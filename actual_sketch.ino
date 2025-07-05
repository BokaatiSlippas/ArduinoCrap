#include <LedControl.h>

const int CLK_R = 13;
const int DT = 12;
const int SW = 11;
const int DIN = 10;
const int CS = 9;
const int CLK_M = 8;
LedControl l_matrix = LedControl(DIN, CLK_M, CS, 1);

byte pic[8] = {1, 0, 0, 0, 0, 0, 0, 0}; // Each byte represents row 0 to 7
int row = 0;
int col = 0;
bool cursorVisible = false;
unsigned long lastBlinkTime = 0;
const int blinkInterval = 300; // Blink speed (ms)

void setup() {  
  pinMode(CLK_R, INPUT_PULLUP);  
  pinMode(DT, INPUT_PULLUP);  
  pinMode(SW, INPUT_PULLUP);  
  l_matrix.shutdown(0, false);
  l_matrix.setIntensity(0, 8);
  l_matrix.clearDisplay(0);
  Serial.begin(9600);  
}

void toggleLed() {
  pic[row] ^= (1 << col); // Permanently toggle the LED
  displayPic(); // Refresh display
}

void displayPic() {
  for (int r = 0; r < 8; r++) {
    byte rowData = pic[r];
    // Override cursor position if blinking is active
    if (r == row && cursorVisible) {
      rowData ^= (1 << col); // Invert the cursor LED
    }
    l_matrix.setRow(0, r, rowData);
  }
}

void loop() {  
  // Handle button press (permanent toggle)
  if (digitalRead(SW) == LOW) {
    toggleLed();
    delay(200); // Debounce
  }

  // Handle rotary encoder
  static int lastCLK_R = HIGH;  
  int currentCLK_R = digitalRead(CLK_R);  
  if (currentCLK_R != lastCLK_R) {  
    if (digitalRead(DT) == currentCLK_R) {
      row = (row + 1) % 8; // CW (wrap around)
    } else {
      col = (col + 1) % 8; // CCW (wrap around)
    }
    lastCLK_R = currentCLK_R;
    Serial.print("Cursor: ("); Serial.print(row); 
    Serial.print(","); Serial.print(col); Serial.println(")");
  }

  // Blink cursor
  if (millis() - lastBlinkTime > blinkInterval) {
    cursorVisible = !cursorVisible; // Toggle cursor visibility
    lastBlinkTime = millis();
    displayPic(); // Refresh display
  }
}

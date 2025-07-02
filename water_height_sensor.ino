// for some reason my sensor would not go above 350 ish despite being fully submerged so I assumed a range of 0 to 400 instead of 0 to 1024 (included, excluded)


int waterSensorPin = A0;
int redPin = 13;
int yellowPin = 12;
int greenPin = 11;
int bluePin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(waterSensorPin);

  Serial.print("Water level: ");
  Serial.println(sensorValue);

  if (sensorValue >= 300 && sensorValue < 400) {
    digitalWrite(bluePin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  }
  else if (sensorValue >= 200 && sensorValue < 300) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(bluePin, LOW);
  }
  else if (sensorValue >= 100 && sensorValue < 200) {
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
  else {
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }

  // Wait for a second before taking the next reading
  delay(1000);
}

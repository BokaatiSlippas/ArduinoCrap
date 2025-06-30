#include <Servo.h>

// this constant won't change. It's the pin number of the sensor's output:
const int servoPin = 7;
const int trigPin = 9;
const int echoPin = 10;

float duration; // Stores pulse duration
float distanceCM;
float distanceIN;

Servo myServo;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}

void loop() {
  // start with a clean signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // send a trigger signal
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // return pulse duration in microseconds
  // if set to HIGH, pulseIn() waits for the pin to go from LOW to HIGH
  // stops timing when pin goes back to LOW
  duration = pulseIn(echoPin, HIGH);
  // convert m/s to in/microseconds
  distanceCM = (duration*0.034)/2;
  // convert to inches, 1in = 2.54cm
  distanceIN = distanceCM/2.54;
  // print distance

  Serial.print("Distance: ");
  Serial.print(distanceCM);
  Serial.print(" cm | ");
  Serial.print(distanceIN);
  Serial.print(" in");

  // if object is detected at 6 inches or less
  if (distanceIN <= 6) {
    // rotate 90 degrees
    myServo.write(90);
  }
  // or else if the object is further than 6 inches away
  else {
    // rotate / keep the servo at 0 degrees
    myServo.write(0);
  }
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

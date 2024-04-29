#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int buzzerPin = 13; // Buzzer connected to pin 13
const int trigPin = 12;   // Ultrasonic sensor trig pin connected to pin 12
const int echoPin = 11;   // Ultrasonic sensor echo pin connected to pin 11
long duration;
int distance;
const int greenLedPin = 6; // Green LED connected to pin 6
const int redLedPin = 7;   // Red LED connected to pin 7
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;
const int ENA = 9; // ENA connected to pin 9
const int ENB = 10; // ENB connected to pin 10

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(ENA, OUTPUT);  // ENA pin
  pinMode(ENB, OUTPUT); // ENB pin
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  int distance = measureDistance();
  Serial.print("Distance:");
  Serial.println(distance);

  displayDistance(distance);
  updateLEDs(distance);
  delay(1000); // Delay for 1 second (adjust as needed)
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Increased trigger pulse duration
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("Distance:");
  Serial.println(distance);
  return distance;
}

void displayDistance(int distance) {
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Set the cursor to the first column and first row
  lcd.print("Distance:"); // Print the label
  lcd.setCursor(0, 1); // Set the cursor to the first column and second row
  lcd.print(distance); // Print the distance
}

void updateLEDs(int distance) {
  if (distance < 50) {
    // Object detected, turn on red LED and turn off green LED
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    activateBuzzer(); // Activate the buzzer
    // Motor control: move forward
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(ENA, 255); // full speed
    analogWrite(ENB, 255); // full speed
  } else {
    // No object detected, turn off red LED and turn on green LED
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    deactivateBuzzer();
    // Motor control: turn left
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(ENA, 200); // lower speed for turning
    analogWrite(ENB, 200); // lower speed for turning
  }
}

void activateBuzzer() {
  tone(buzzerPin, 1000); // Turn on the buzzer at 1000Hz frequency
}

void deactivateBuzzer() {
  noTone(buzzerPin); // Turn off the buzzer
}
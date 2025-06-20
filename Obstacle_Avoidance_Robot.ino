#include <Servo.h>

#define trigPin 9
#define echoPin 8

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

#define buzzerPin 6

#define LED1 A0
#define LED2 A1
#define LED3 A2

Servo scanServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  scanServo.attach(10); // Servo motor pin

  Serial.begin(9600);
  scanServo.write(90); // Center position
  delay(1000);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void playNaaginTone() {
  for (int freq = 1000; freq <= 2000; freq += 50) {
    tone(buzzerPin, freq, 100);
    delay(100);
  }
  for (int freq = 2000; freq >= 1000; freq -= 50) {
    tone(buzzerPin, freq, 100);
    delay(100);
  }
  noTone(buzzerPin);
}

void blinkLEDs() {
  // Flowing effect: ON one-by-one
  digitalWrite(LED1, HIGH);
  delay(250);
  digitalWrite(LED2, HIGH);
  delay(250);
  digitalWrite(LED3, HIGH);
  delay(250);

  // Flowing effect: OFF one-by-one
  digitalWrite(LED1, LOW);
  delay(250);
  digitalWrite(LED2, LOW);
  delay(250);
  digitalWrite(LED3, LOW);
  delay(250);
}

void loop() {
  blinkLEDs(); // Stylish slow LED blinking

  scanServo.write(90); // Face forward
  delay(300);
  long front = getDistance();

  Serial.print("Front: ");
  Serial.println(front);

  if (front > 25) {
    moveForward();
  } else {
    stopMotors();
    delay(200);

    // Naagin buzzer sound
    playNaaginTone();

    // Move backward
    moveBackward();
    delay(1000);
    stopMotors();
    delay(200);

    // Scan left
    scanServo.write(150);
    delay(500);
    long left = getDistance();

    // Scan right
    scanServo.write(30);
    delay(500);
    long right = getDistance();

    // Return to center
    scanServo.write(90);
    delay(300);

    Serial.print("Left: ");
    Serial.print(left);
    Serial.print(" | Right: ");
    Serial.println(right);

    if (left > 10 && left > right) {
      moveLeft();
      delay(800);
    } else if (right > 10 && right > left) {
      moveRight();
      delay(800);
    } else {
      stopMotors();
    }
  }

  delay(200);
}

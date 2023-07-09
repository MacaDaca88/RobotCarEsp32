#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <BluetoothSerial.h>

// Blynk Setup
char auth[] = "P8MOcCxUZSXh52mYOBd8ocaTkq-SPA2W";  
char ssid[] = "Tip-jar";
char password[] = "PASSWORD1234LOL";            

// Motor Pins
int EN_A = 12;  // Enable pin for the first motor
int IN1 = 14;   // Control pin 1 for the first motor
int IN2 = 27;   // Control pin 2 for the first motor
int EN_B = 33;  // Enable pin for the second motor
int IN3 = 26;   // Control pin 1 for the second motor
int IN4 = 25;   // Control pin 2 for the second motor

// Light Pins
const int headlightPin = 2;
const int spotlight = 0;
const int leftindercator = 4;
const int rightindercator = 15;

// Floating variables to store data
int motor_speed;
int motor_speed1;

// Fixed variables to store data
int headlightsState = 0;
int rightindercatorstate = 0;
int leftindercatorstate = 0;
int spotlightstate = 0;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(headlightPin, OUTPUT);
  pinMode(leftindercator, OUTPUT);
  pinMode(rightindercator, OUTPUT);
  pinMode(spotlight, OUTPUT);

  SerialBT.begin("The Worlds End");
}

void loop() {
  Blynk.run();
  if (SerialBT.available()) {                  // Check if data is available from Bluetooth device
    processBluetoothCommand(SerialBT.read());  // Process the received command
    Serial.println(SerialBT.read());
  }
}

BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  Serial.print("Y = ");
  Serial.println(y);
  Serial.print("X = ");
  Serial.println(x);

  if (x < 120) {
    motor_speed = map(x, 120, 0, 0, 255);
    motor_speed1 = map(x, 120, 0, 0, 255);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed1);
    Serial.print("Motor A Speed: ");
    Serial.println(motor_speed);
    Serial.print("Motor B Speed: ");
    Serial.println(motor_speed1);

  } else if (x > 170 && x < 255) {
    motor_speed = map(x, 170, 255, 0, 255);
    motor_speed1 = map(x, 170, 255, 0, 255);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed1);
    Serial.println("Motor A Speed: ");
    Serial.print(motor_speed);
    Serial.println("Motor B Speed: ");
    Serial.print(motor_speed1);
  }

  if (y < 120) {
    motor_speed1 = map(y, 120, 0, 0, 255);
    motor_speed = map(y, 120, 0, 0, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN_B, motor_speed1);
    analogWrite(EN_A, motor_speed);
    Serial.print("Motor A Speed: ");
    Serial.println(motor_speed);
    Serial.print("Motor B Speed: ");
    Serial.println(motor_speed1);

    // Motors forward
  } else if (y > 170 && y < 255) {
    motor_speed1 = map(y, 170, 255, 0, 255);
    motor_speed = map(y, 170, 255, 0, 255);
    digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN_A, motor_speed);
    analogWrite(EN_B, motor_speed1);
    Serial.print("Motor A Speed: ");
    Serial.println(motor_speed);
    Serial.print("Motor B Speed: ");
    Serial.println(motor_speed1);

    // Motors OFF
  } else if ((x > 120 && x < 170) && (y > 120 && y < 170)) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Motor A Stopped");
    Serial.println("Motor B Stopped");
  }
}

BLYNK_WRITE(V3) {
  int speedControl = param.asInt();  

  // analogWrite(motor_speed, speedControl);
  // analogWrite(motor_speed1, speedControl);
  Blynk.virtualWrite(V5, "speed");

  Serial.print("Speed Control: ");
  Serial.println(speedControl);
}

BLYNK_WRITE(V4) {
  int headlightsStateNew = param.asInt();

  if (headlightsStateNew == 1) {
    digitalWrite(headlightPin, HIGH);
    Blynk.virtualWrite(V5, "Headlights: ON");
    Serial.println("Headlights: ON");
  } else {
    digitalWrite(headlightPin, LOW);
    Blynk.virtualWrite(V5, "Headlights: OFF");
    Serial.println("Headlights: OFF");
  }
}

BLYNK_WRITE(V6) {
  int LeftIndercatorStateNew = param.asInt();

  if (LeftIndercatorStateNew == 1) {
    digitalWrite(leftindercator, HIGH);
    Blynk.virtualWrite(V5, "leftindercator: ON");
    Serial.println("leftindercator: ON");
  } else {
    digitalWrite(leftindercator, LOW);
    Blynk.virtualWrite(V5, "leftindercator: OFF");
    Serial.println("leftindercator: OFF");
  }
}

BLYNK_WRITE(V7) {
  int RightIndercatorStateNew = param.asInt();

  if (RightIndercatorStateNew == 1) {
    digitalWrite(rightindercator, HIGH);
    Blynk.virtualWrite(V5, "rightindercator: ON");
    Serial.println("rightindercator: ON");
  } else {
    digitalWrite(rightindercator, LOW);
    Blynk.virtualWrite(V5, "rightindercator: OFF");
    Serial.println("rightindercator: OFF");
  }
}

void processBluetoothCommand(char command) {

  Serial.println(command);

  switch (command) {
    case 'F':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      // Move forward
      break;

    case 'B':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      // Move backward
      break;

    case 'L':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      // Turn left
      break;

    case 'R':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      // Turn right
      break;

    case 'S':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      // Stop
      break;

    case 'H':
      // Toggle headlights
      break;


    default:
      // Invalid command
      break;
  }
}

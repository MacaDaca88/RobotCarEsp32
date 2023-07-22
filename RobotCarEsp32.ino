#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <BlynkSimpleEsp32.h>
#include <BluetoothSerial.h>

// Blynk Setup
char auth[] = "P8MOcCxUZSXh52mYOBd8ocaTkq-SPA2W";
char ssid[] = "Tip-jar";
char password[] = "PASSWORD1234LOL";

#define maxChar 32
char message[maxChar];
char readChar;
byte index1 = 0;
int i;

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

// BlinkWithoutDelay variables for indicators
unsigned long previousLeftIndercatorMillis = 0;
unsigned long previousRightIndercatorMillis = 0;
const unsigned long leftIndercatorInterval = 500;   // Interval for left indicator (in milliseconds)
const unsigned long rightIndercatorInterval = 500;  // Interval for right indicator (in milliseconds)
bool leftIndercatorState = false;
bool rightIndercatorState = false;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
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
  unsigned long currentMillis = millis();

  if (SerialBT.available()) {
    processBluetoothCommand(SerialBT.read());
  } else if (SerialBT.available()) {
    messageBluetoothCommand();
    Serial.println(SerialBT.read());
  }
  if (currentMillis - previousLeftIndercatorMillis >= leftIndercatorInterval) {
    previousLeftIndercatorMillis = currentMillis;
    leftIndercatorState = !leftIndercatorState;  // Toggle the state
    digitalWrite(leftindercator, leftIndercatorState);
  }

  // Check and update right indicator
  if (currentMillis - previousRightIndercatorMillis >= rightIndercatorInterval) {
    previousRightIndercatorMillis = currentMillis;
    rightIndercatorState = !rightIndercatorState;  // Toggle the state
    digitalWrite(rightindercator, rightIndercatorState);
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
  int speed = motor_speed;
  int speed1 = motor_speed1;

  Serial.println(command);


  switch (command) {
    case 'f':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN_A, motor_speed);
      analogWrite(EN_B, motor_speed1);
      // Move forward slow
      break;

    case 'F':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN_A, 255);
      analogWrite(EN_B, 255);
      // Move forward fast
      break;

    case 'b':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 100);
      analogWrite(EN_B, 100);
      // Move backward SLOW
      break;

    case 'B':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 255);
      analogWrite(EN_B, 255);
      // Move backward Fast
      break;

    case 'l':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN_A, 150);
      analogWrite(EN_B, 150);
      // Turn left SLOW
      break;

    case 'L':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN_A, 255);
      analogWrite(EN_B, 255);
      // Turn left Fast
      break;

    case 'r':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 150);
      analogWrite(EN_B, 150);
      // Turn right SLOW
      break;

    case 'R':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 255);
      analogWrite(EN_B, 255);
      // Turn right
      break;

    case 'S':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 0);
      analogWrite(EN_B, 0);
      // Stop
      break;

    case 'h':
      digitalWrite(headlightPin, LOW);
      Serial.println("Headlights: off");
      // Toggle headlights
      break;

    case 'H':
      digitalWrite(headlightPin, HIGH);
      Serial.println("Headlights: ON");
      // Toggle headlights
      break;

    case '1':
      // limit EN_A Pin to 100
      motor_speed = 100;
      motor_speed1 = 100;

      // Toggle Speed 1
      break;

    case '2':
      // limit EN_A Pin to 150
      motor_speed = 150;
      motor_speed1 = 150;

      // Toggle Speed 2
      break;

    case '3':
      // limit EN_A Pin to 255
      motor_speed = 255;
      motor_speed1 = 255;

      // Toggle Speed 3
      break;

    default:
      // Invalid command
      break;
  }
}

void messageBluetoothCommand(){

if (SerialBT.available()) {
    for (i = 0; i < 31; i++) {
      message[i] = '\0';
    }
    index1 = 0;
  }
  while (SerialBT.available() > 0) {
    if (index1 < (maxChar - 1)) {
      readChar = SerialBT.read();
      message[index1] = readChar;
      index1++;
      message[index1] = '\0';
    }
  }
  Serial.print(message);
}
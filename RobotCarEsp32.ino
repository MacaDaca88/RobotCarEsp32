#include "PINS.h"
#include "OTA.H"

#include <BluetoothSerial.h>

#define maxChar 3
char message[maxChar];
char readChar;
byte index1 = 0;
int i;


// Floating variables to store data
int motor_speed;
int motor_speed1;

// Fixed variables to store data
bool headlightsState = 0;
bool rightindercatorstate = 0;
bool leftindercatorstate = 0;
bool spotlightstate = 0;

// BlinkWithoutDelay variables for indicators
unsigned long previousLeftIndercatorMillis = 0;
unsigned long previousRightIndercatorMillis = 0;
const unsigned long leftIndercatorInterval = 500;   // Interval for left indicator (in milliseconds)
const unsigned long rightIndercatorInterval = 500;  // Interval for right indicator (in milliseconds)
bool leftIndercatorState = false;
bool rightIndercatorState = false;

BluetoothSerial SerialBT;

void setup() {

  otaSetup();
//  blynkSetup();
  pinSetup();


  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  SerialBT.begin("The Worlds End");
}

void loop() {
  otaLoop();
 // blynkRun();
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

void messageBluetoothCommand() {

  if (SerialBT.available()) {
    for (i = 0; i < 3; i++) {
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
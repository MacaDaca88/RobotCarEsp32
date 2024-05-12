


#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL287iALslr"
#define BLYNK_TEMPLATE_NAME "Robot"
#define BLYNK_AUTH_TOKEN "P8MOcCxUZSXh52mYOBd8ocaTkq-SPA2W"


#include <BlynkSimpleEsp32.h>
// Blynk Setup

char auth[] = "P8MOcCxUZSXh52mYOBd8ocaTkq-SPA2W";

void blynkSetup() {
  Blynk.begin(auth, ssid, password);
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

void blynkRun() {
  Blynk.run();
}
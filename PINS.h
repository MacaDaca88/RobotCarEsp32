#ifdef

#define BLYNK_H

#endif
// Motor Pins
#define EN_A 12  // Enable pin for the first motor
#define IN1 14   // Control pin 1 for the first motor
#define IN2 27   // Control pin 2 for the first motor
#define EN_B 33  // Enable pin for the second motor
#define IN3 26   // Control pin 1 for the second motor
#define IN4 25   // Control pin 2 for the second motor

// Light Pins
#define headlightPin 2
#define spotlight 0
#define leftindercator 4
#define rightindercator 15

void pinSetup() {

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
}
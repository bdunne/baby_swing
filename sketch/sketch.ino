const int sensePinA = 2; // Digital 2
const int sensePinB = 3; // Digital 3
const int motorPinA = 4; // Digital 4
const int motorPinB = 5; // Digital 5

int directionInt = 0; // 0 == None, 1 == Forward, 2 == Backward
int pushRequired = 0; // 0 == No, 1 == Yes, run motor
int motorOnTime = 750;
unsigned long motorOffMillis = 0;

void setup() {
  // Set up Serial
//  Serial.begin(9600);

  // Setup rotary encoder
  pinMode(sensePinA, INPUT);
  pinMode(sensePinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensePinA), directionInterrupt, RISING);

  // Setup Motor
  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  motorOff();
}

void directionInterrupt() {
  if (digitalRead(sensePinB) == HIGH) {
    directionEvent(1);
  } else {
    directionEvent(2);
  }
}

void directionEvent( int d ) {
  if (directionInt != d) {
    directionInt = d;
    pushRequired = 1;
  }
}

void motorForward() {
//  Serial.println("Forward");
  digitalWrite(motorPinB, LOW);
  digitalWrite(motorPinA, HIGH);
}

void motorReverse() {
//  Serial.println("Reverse");
  digitalWrite(motorPinA, LOW);
  digitalWrite(motorPinB, HIGH);
}

void motorOff() {
  digitalWrite(motorPinA, LOW);
  digitalWrite(motorPinB, LOW);
}

void loop() {
  if (millis() >= motorOffMillis) {
    motorOff();
  }

  if (pushRequired == 1) {
    switch(directionInt) {
      case 0:
        motorOff();
        break;
      case 1:
        motorForward();
        break;
      case 2:
        motorReverse();
        break;
    }
    motorOffMillis = millis() + motorOnTime;
    pushRequired = 0;
  }
}

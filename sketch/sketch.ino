const int sensePinA = 2; // Digital 2
const int sensePinB = 3; // Digital 3
const int motorPinA = 4; // Digital 4
const int motorPinB = 5; // Digital 5
const int speedSwitchPin0 = 6; // Digital 6
const int speedSwitchPin1 = 7; // Digital 7
const int speedSwitchPin2 = 8; // Digital 8
const int speedSwitchPin3 = 9; // Digital 9
const int speedSwitchPin4 = 10; // Digital 10
const int speedSwitchPin5 = 11; // Digital 11
const int speedSwitchPin6 = 12; // Digital 12

int directionInt = 0; // 0 == None, 1 == Forward, 2 == Backward
int pushRequired = 0; // 0 == No, 1 == Yes, run motor
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

  // Setup Speed Selector Switch
  pinMode(speedSwitchPin0, INPUT_PULLUP);
  pinMode(speedSwitchPin1, INPUT_PULLUP);
  pinMode(speedSwitchPin2, INPUT_PULLUP);
  pinMode(speedSwitchPin3, INPUT_PULLUP);
  pinMode(speedSwitchPin4, INPUT_PULLUP);
  pinMode(speedSwitchPin5, INPUT_PULLUP);
  pinMode(speedSwitchPin6, INPUT_PULLUP);
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

int readSpeedSwitch() {
  if (digitalRead(speedSwitchPin0) == LOW) {
    return 0;
  } else if (digitalRead(speedSwitchPin1) == LOW) {
    return 1;
  } else if (digitalRead(speedSwitchPin2) == LOW) {
    return 2;
  } else if (digitalRead(speedSwitchPin3) == LOW) {
    return 3;
  } else if (digitalRead(speedSwitchPin4) == LOW) {
    return 4;
  } else if (digitalRead(speedSwitchPin5) == LOW) {
    return 5;
  } else if (digitalRead(speedSwitchPin6) == LOW) {
    return 6;
  } else {
    return 0; // There are other switch positions that we should never hit, but just in case we do...
  }
}

int motorOnTime() {
  int onTime = readSpeedSwitch() * 250;
  return onTime;
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
    motorOffMillis = millis() + motorOnTime();
    pushRequired = 0;
  }
}

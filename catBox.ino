const int motionPin = 9;
const int fanPin = 8;
const int solenoidPin = 13;
const bool DEBUG = false;
unsigned long sprayOnTime;

void setup() {
  pinMode(motionPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  sprayOnTime = millis();
}

void loop() {
  int catInBox = digitalRead(motionPin);
  updateFan(catInBox);
  spray(catInBox);
  performPeriodicTasks();
}


int fanState = 0; //default to off.

void updateFan(int catInBox) {
  if (catInBox == 1) {
    //turn the fan on
    fanState = 1;
  } else {
    fanState = 0;
  }
  digitalWrite(fanPin, fanState);
}

const unsigned int SPRAY_ON_DELAY = 1000;
int lastMotion = 0;
int solenoidState = 0;

void spray(int catInBox) {
  if (catInBox == 0 && lastMotion != catInBox) {
    solenoidState = 1;
    sprayOnTime = millis();
  } else if (solenoidState == 1 && millis() - sprayOnTime > SPRAY_ON_DELAY) {
    solenoidState = 0;
    sprayOnTime = millis();
  }
  digitalWrite(solenoidPin, solenoidState);
  lastMotion = catInBox;
}

const int PERIODIC_TASK_DELAY = 1000 * 60 * 60; //once an hour
const int FAN_ON_DELAY = 300000; //once the fan is on, how long do we leave it that way.
int periodicFanState = 0;
unsigned long startTime = 0;

void performPeriodicTasks() {
  //TODO: Add in spraying here as well.
  unsigned long now = millis();
  unsigned long delta = now - startTime;
  if (now < startTime || delta > PERIODIC_TASK_DELAY) { //now < loopTime means overflow.
    periodicFanState = 1;
    startTime = now;
  } else if (periodicFanState == 1 && delta < FAN_ON_DELAY) {
    //do nothing, we're waiting for 5 minutes
  } else {
    periodicFanState = 0;
  }
  digitalWrite(fanPin, periodicFanState);
}

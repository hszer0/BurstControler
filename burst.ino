#define triggerPin 3
#define selectorPin 4
#define gearPin 5
#define motorPin 6
#define ledPin 13

bool watchTrigger = 1;
bool runMotor = 0;
bool watchShots = 1;
int lastGearState = 0;
int lastTriggerState = 0;
bool tempGearState;
bool gearState;
bool tempTriggerState = 1;
bool triggerState = 1;
int shotState = 0;

long lastTriggerTime = 0;
long lastGearTime = 0;

long debounceDelay = 50;
long debounceDelayGear = 1;
int burstCount = 3;

void setup()
{
  Serial.begin(9600);
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(selectorPin, INPUT_PULLUP);
  pinMode(gearPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  lastGearState = digitalRead(gearPin);
  lastTriggerState = digitalRead(triggerPin);
}

void loop()
{
  if (digitalRead(selectorPin))
  {
    burstFire(1);
  }
  else
  {
    burstFire(burstCount);
  }
}

void burstFire(int maxShots)
{
  // debounce gear
  bool gearReading = digitalRead(gearPin);
  if (gearReading != lastGearState) lastGearTime = millis();
  if ((millis() - lastGearTime) > debounceDelayGear)
  {
    if (gearReading != tempGearState)
    {
      tempGearState = gearReading;
      gearState = !tempGearState;
    }
  }

  // debounce trigger
  bool triggerReading = digitalRead(triggerPin);
  if (triggerReading != lastTriggerState) lastTriggerTime = millis();
  if ((millis() - lastTriggerTime) > debounceDelay)
  {
    if (triggerReading != tempTriggerState)
    {
      tempTriggerState = triggerReading;
      triggerState = !tempTriggerState;
    }
  }

  if (watchTrigger and triggerState)
  {
    watchTrigger = 0;
    runMotor = 1;
    shotState = 0;
    watchShots = 1;
  }

  if (!gearState)
  {
    watchShots = 1;
  }
  else
  {
    if (watchShots);
    {
      watchShots = 0;
      shotState++;
    }
  }

  if (shotState >= maxShots && gearState)
  {
    runMotor = 0;
  }

  if (!runMotor and !triggerState)
  {
    watchTrigger = 1;
  }

  digitalWrite(motorPin, runMotor);
  digitalWrite(ledPin, runMotor);

  // debounce end
  lastTriggerState = triggerReading;
  lastGearState = gearReading;

  Serial.println(shotState);
}

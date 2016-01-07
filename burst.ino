#define triggerPin 3
#define selectorPin 4
#define gearPin 5
#define motorPin 6
#define ledPin 13

bool ignoreTrigger = 0;
bool ignoreGear = 1;
bool runMotor = 0;
int lastGearState = 0;
int lastTriggerState = 0;
int gearState;
int triggerState = 1;

long lastTriggerTime = 0;
long lastGearTime = 0;
long debounceDelay = 50;  
long debounceDelayGear = 5;

void setup()
{
    pinMode(triggerPin, INPUT_PULLUP);
    pinMode(selectorPin, INPUT_PULLUP);
    pinMode(gearPin, INPUT_PULLUP);
    pinMode(motorPin, OUTPUT);
    lastGearState = digitalRead(gearPin);
    lastTriggerState = digitalRead(triggerPin);
}


void loop() 
{
    int gearReading = digitalRead(gearPin);
    int triggerReading = digitalRead(triggerPin);
  
    if (gearReading != lastGearState)
    {
        lastGearTime = millis();
    }

    if ((millis() - lastGearTime) > debounceDelayGear) 
    {
        if (gearReading != gearState) 
        {
            gearState = gearReading;
        }
    }

    if (triggerReading != lastTriggerState)
    {
        lastTriggerTime = millis();
    }

    if ((millis() - lastTriggerTime) > debounceDelay) 
    {
        if (triggerReading != triggerState) 
        {
            triggerState = triggerReading;
        } 
    }

    if (gearState)
    {
       ignoreGear = 0; 
    }

    if (!ignoreTrigger and !triggerState)
    {
        runMotor = 1;
        ignoreTrigger = 1;
    }

    if (!gearState and !ignoreGear)
    {
        runMotor = 0;
        if (triggerState)
        {
            ignoreTrigger = 0;
            ignoreGear = 1;
        }
    }
    else
    {
        if (triggerState)
        {
            ignoreTrigger = 0; 
        }
    }

    digitalWrite(motorPin, runMotor);
    digitalWrite(ledPin, runMotor);
    lastTriggerState = triggerReading;
    lastGearState = gearReading;
}

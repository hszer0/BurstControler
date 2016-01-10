#define triggerPin 3
#define selectorPin 4
#define gearPin 5
#define motorPin 6
#define resetPin 7
#define ledPin 13

bool ignoreTrigger = 0;
bool ignoreGear = 1;
bool runMotor = 0;
int lastGearState = 0;
int lastTriggerState = 0;
int gearState;
int triggerState = 1;
int resetState = 1;
int shotState = 0;
int shotCounter = 0;

long lastTriggerTime = 0;
long lastGearTime = 0;
long lastResetTime = 0;
long lastResetState = 0;
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
    lastResetState = digitalRead(resetPin);
}


void loop() 
{
    // debounce reset
    int resetReading = digitalRead(resetPin);
    if (resetReading != lastResetState) lastResetTime = millis();
    if ((millis() - lastResetTime) > debounceDelay) 
    {
        if (resetReading != resetState) resetState = resetReading;
    }

    if (!resetState)
    {
        shotCounter = 0;
        shotState = 0;
        runMotor = 0;
    }

    if (digitalRead(selectorPin))
    {
        burstFire(1);
    }
    else
    {
        burstFire(3);
    }

    lastResetState = resetReading; 
}

void burstFire(int maxShots)
{
    // debounce gear
    int gearReading = digitalRead(gearPin);
    if (gearReading != lastGearState) lastGearTime = millis();
    if ((millis() - lastGearTime) > debounceDelayGear) 
    {
        if (gearReading != gearState) gearState = gearReading;
    }

    // debounce trigger
    int triggerReading = digitalRead(triggerPin);
    if (triggerReading != lastTriggerState) lastTriggerTime = millis();
    if ((millis() - lastTriggerTime) > debounceDelay) 
    {
        if (triggerReading != triggerState) triggerState = triggerReading; 
    }

    // if gear is not detected commence watching gear
    if (gearState) ignoreGear = 0; 

    // if trigger is held and is not ignored
    if (!ignoreTrigger and !triggerState)
    {
        // start motor and stop watching the trigger
        runMotor = 1;
        shotState = 0;
        ignoreTrigger = 1;
    }

    // if gear is detected and not ignored
    if (!gearState and !ignoreGear)
    {
        shotState++;
        shotCounter++;

        if (shotState >= maxShots)
        {
            // stop the motor
            runMotor = 0;
            shotState = 0;

            // if trigger is released
            if (triggerState)
            {
                // start watching the trigger and ignore the gear
                ignoreTrigger = 0;
                ignoreGear = 1;
            }
        }
    }
    else // in all other cases
    {
        // if trigger is released
        if (triggerState) ignoreTrigger = 0; 
    }

    digitalWrite(motorPin, runMotor);
    digitalWrite(ledPin, runMotor);

    // debounce end
    lastTriggerState = triggerReading;
    lastGearState = gearReading;   
}

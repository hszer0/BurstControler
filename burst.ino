#define triggerPin 3
#define selectorPin 4
#define gearPin 5
#define motorPin 6

int shotDelay = 250;
int burstCount = 3;
int shotCount = 0;
int bbCount = 0;
int bbMax = 120;
bool cycleFinished = 1;
bool triggerAvailable = 1;
long previousTime = 0;

void setup()
{
	pinMode(triggerPin, INPUT_PULLUP);
	pinMode(selectorPin, INPUT_PULLUP);
	pinMode(gearPin, INPUT_PULLUP);
	pinMode(motorPin, OUTPUT);
	bbCount = bbMax;
}

void loop()
{
	unsigned long currentTime = millis();
	if (digitalRead(selectorPin))
	{
		if(!digitalRead(triggerPin) && cycleFinished && triggerAvailable)
		{
			digitalWrite(motorPin, HIGH);
			cycleFinished = 0;
			triggerAvailable = 0;
		}

		if(!digitalRead(gearPin) && !cycleFinished)
		{
			digitalWrite(motorPin, LOW);
			bbCount--;
			cycleFinished = 1;
		}

		if(digitalRead(triggerPin)
		{
			triggerAvailable = 1;
		}
	}
	else
	{
		if(!digitalRead(triggerPin) && cycleFinished && triggerAvailable)
		{
			digitalWrite(motorPin, HIGH);
			cycleFinished = 0;
			triggerAvailable = 0;
			shotCount = 0;
		}

		if(!digitalRead(gearPin) && !cycleFinished)
		{
			shotCount++;
			if(shotCount >= burstCount)
			{
				digitalWrite(motorPin, LOW);
				previousTime = currentTime;
				if(!digitalRead(triggerPin))
				{
					if(currentTime - previousTime > shotDelay)
					{
						digitalWrite(motorPin, HIGH);
					}
					else
					{
						digitalWrite(motorPin, LOW);
						cycleFinished = 1;
					}
				}
			}
		}

		if(digitalRead(triggerPin)
		{
			triggerAvailable = 1;
		}
	}
}
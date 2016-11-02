#define trigger 3
#define semi 4
#define gear 5
#define motor 6
#define reset 7
#define led 13
#define ACTIVE LOW
#define INACTIVE HIGH

enum state {
    initialized,
    pending,
    firing,
    completed
};

state cycleState = pending;

void setup()
{
    pinMode(trigger, INPUT_PULLUP);
    pinMode(semi, INPUT_PULLUP);
    pinMode(gear, INPUT_PULLUP);
    pinMode(motor, OUTPUT);
    pinMode(reset, INPUT_PULLUP);
    setMotor(0);
}

void loop() 
{
    if (cycleState == initialized)
    {
        if (digitalRead(trigger) == ACTIVE)
        {
            cycleState = pending;
        }
        else
        {
            setMotor(0);
        }
    }
    else
    {
        if (digitalRead(semi) == ACTIVE)
        {
            shootOnce();
        }
        else
        {
            //Burst 'n shit
        }
    }
}

void shootOnce()
{
    if (digitalRead(trigger) == ACTIVE)
    {
        if (cycleState == pending)
        {
            cycleState = firing;
            setMotor(1);
        }
    }

    if (cycleState == completed && digitalRead(trigger) == INACTIVE)
        cycleState == pending;

    if (digitalRead(gear) == ACTIVE)
    {
        cycleState = completed;
        setMotor(0);
    }
}

void setMotor(bool status)
{
    digitalWrite(motor, status);
    digitalWrite(led, status);
}

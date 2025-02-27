#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.attachCallOnPosVerKnock(printMessageA);
    bionic.attachCallOnNegVerKnock(printMessageB);
    bionic.setAllThresholdPercentage(20);
    bionic.start();
}

void loop()
{
    bionic.read();
}

void printMessageA()
{
    if (bionic.getFclosedStatus(FINGER_INDEX))
    {
        Serial.println("1");
    }
    else if (bionic.getFclosedStatus(FINGER_MIDDLE))
    {
        Serial.println("3");
    }
}
void printMessageB()
{
    if (bionic.getFclosedStatus(FINGER_INDEX))
    {
        Serial.println("2");
    }
    else if (bionic.getFclosedStatus(FINGER_MIDDLE))
    {
        Serial.println("4");
    }
}
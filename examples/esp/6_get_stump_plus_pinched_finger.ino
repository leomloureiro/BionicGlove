    #include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.start();
    bionic.attachCallOnVerticalPositiveKnock(printMessageA);
    bionic.attachCallOnVerticalNegativeKnock(printMessageB);
    bionic.setAllThresholdPercentage(20);
}

void loop()
{
    bionic.read();
}

void printMessageA()
{
    if (bionic.getClosedFingerStatus(DATA_F_INDEX))
    {
        Serial.println("1");
    }
    else if (bionic.getClosedFingerStatus(DATA_F_MIDDLE))
    {
        Serial.println("3");
    }
}
void printMessageB()
{
    if (bionic.getClosedFingerStatus(DATA_F_INDEX))
    {
        Serial.println("2");
    }
    else if (bionic.getClosedFingerStatus(DATA_F_MIDDLE))
    {
        Serial.println("4");
    }
}
#include <BionicGlove.h>

BionicGlove bionic;

String dataPack = "";
String initString = ">";
String endString = "<";
void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        dataPack = initString +
                   String(bionic.getF(DATA_F_INDEX)) + " " +
                   String(bionic.getF(DATA_F_MIDDLE)) + " " +
                   String(bionic.getF(DATA_F_RING)) + " " +
                   String(bionic.getF(DATA_F_LITTLE)) + " " +
                   String(bionic.getF(DATA_SMOOTHFACTOR)) + endString;
        Serial.println(dataPack);
    }
}
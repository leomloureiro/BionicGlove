/********************************************************************
* This is a library for the Bionic Glove.
*
* You'll find an example which should enable you to use the library.
*
* You are free to use it, change it or build on it. In case you like
* it, it would be cool if you give it a star.
*
* If you find bugs, please open an Issue  https://github.com/PantalaLabs/BionicGlove
* If you want new features , please open a Issue with title Feature Request at https://github.com/PantalaLabs/BionicGlove
*
* Gibran Curtiss Salomão 2023
* http://www.pantalalabs.com
*
TODO :

 * Bionic Glove.cpp
 * Author: Gibran Curtiss Salomão 2023


DATAPACK
0->

1-finger[0].fingerRead
2-finger[1].fingerRead
3-finger[2].fingerRead
4-finger[3].fingerRead

X
5-accel[0].raw
6-accel[0].g
7-accel[0].ang

Y
8-accel[1].raw
9-accel[1].g
10-accel[1].ang

Z
11-accel[2].raw
12-accel[2].g
13-accel[2].ang

14-smoothness

15-<*


Never use Serial.begin(9600) with oudrate above 38400!!!!!!!!!!!

**********************************************************************/

// https://github.com/cubiwan/Regressino
#include <LinearRegression.h>

#ifndef BionicGlove_h
#define BionicGlove_h

#define BULTINLED 2

#define MAXBTDATAPACK 14

#define DATA_F_INDEX 0
#define DATA_F_MIDDLE 1
#define DATA_F_RING 2
#define DATA_F_LITTLE 3
#define DATA_A_X 4
#define DATA_A_X_G 5
#define DATA_A_X_ANG 6
#define DATA_A_Y 7
#define DATA_A_Y_G 8
#define DATA_A_Y_ANG 9
#define DATA_A_Z 10
#define DATA_A_Z_G 11
#define DATA_A_Z_ANG 12
#define DATA_SMOOTHFACTOR 13

#define FINGER_INDEX 0
#define FINGER_MIDDLE 1
#define FINGER_RING 2
#define FINGER_LITTLE 3

#define AXL_X 0
#define AXL_Y 1
#define AXL_Z 2

#define AZGOFFSET 10.0

#define MAXFINGERCHANNELS 4
#define MAXACCELCHANNELS 3

#define MINPERCENTAGE 10          // closed fingers
#define MAXPERCENTAGE 90          // opened fingers
#define DEFTHRESHOLDPERCENTAGE 15 // default for all fingers
#define DEFTHRESHOLDANGLE 30      // default for all axles

#define MINANGLE 10
#define MAXANGLE 170

#define MAXKNOCKLOG 10                           // 10 samples @ 10ms master Sample Rate = 100ms to take a picture of the mountain ** preciselly adjusted , do not touch
#define MINKNOCKTHRESHOLD 2                      // min value expected out of LR calcs
#define MAXKNOCKTHRESHOLD 50.0                   // max value expected out of LR calcs
#define DEFKNOCKVERTICALPOSITIVETHRESHOLD 15.0   //  ** preciselly adjusted , do not touch
#define DEFKNOCKVERTICALNEGATIVETHRESHOLD 15.0   //  ** preciselly adjusted , do not touch
#define DEFKNOCKHORIZONTALPOSITIVETHRESHOLD 13.0 //  ** preciselly adjusted , do not touch
#define DEFKNOCKHORIZONTALNEGATIVETHRESHOLD 15.0 //  ** preciselly adjusted , do not touch
#define DEFKNOCKDEBOUNCEINTERVAL 400             // time in ms to considert next knock
#define MAXKNOCKLINEARREGRESSIONLEARNS 4         // 90us each LR round

#define MAXFLICKLOG 9                    // 9 samples @ 10ms master Sample Rate = 90ms to climb the flick  ** preciselly adjusted , do not touch
#define MINFLICKTHRESHOLD 0.5            // min value expected out of LR calcs
#define MAXFLICKTHRESHOLD 5.0            // max value expected out of LR calcs
#define DEFFLICKTHRESHOLD 3.0            //  ** preciselly adjusted , do not touch
#define DEFFLICKDEBOUNCEINTERVAL 400     // time in ms to considert next flick
#define MAXFLICKLINEARREGRESSIONLEARNS 3 // 90us each LR round

#define CLOSED 0
#define OPENED 1

#define MAXRES 0b111111111

#define SCHMITTTRIGGERPERCENTAGE 5
#define SCHMITTTRIGGERANGLE 5

#define ALPHAFILTER(tar, amt, alpha)                                            \
  {                                                                             \
    tar = (((float)(alpha) * (float)(amt)) + ((1.0 - (alpha)) * (float)(tar))); \
  }

#define GETITEM(idx) btDataPack[idx].toFloat()

class BionicGlove
{
public:
  BionicGlove();

  void start();                                                                                   // begin BT communication
  void end();                                                                                     // end BT communication
  bool read();                                                                                    // read BT serial
  bool active();                                                                                  // return if BT is active
  void freeze(uint32_t ms);                                                                       // freeze any callback for n ms
  void setBuiltInLed(bool status);                                                                // use led built in to visual debug
  void detachAll();                                                                               // detach all callbacks
  String getSerialData();                                                                         // return one line of serial data pack
  float getRaw(uint8_t raw);                                                                      // get raw value from each one value at BT pack
  float getUnit(uint8_t raw);                                                                     // get all values from -1 to 0 to +1 where signal is applicables
  float getAGsmoothed(uint8_t axl);                                                               // get smoothed accel G raw values
  float getAAngsmoothed(uint8_t axl);                                                             // get smoothed accel Angle values
  uint16_t getF(uint8_t f);                                                                       // get expanded finger value
  float getFaccel(uint8_t f);                                                                     // return acceleration based on linear regression coeff
  void setAllFingersThresholdPercentage(uint8_t pct);                                             // set all closed and opened bounds
  void setAllClosedFingersThresholdPercentage(uint8_t pct);                                       // set all closed Percentage for all fingers
  void setAllOpenedFingersThresholdPercentage(uint8_t pct);                                       // set all opened Percentage for all fingers
  void setClosedFingerThresholdPercentage(uint8_t f, uint8_t pct);                                // set closed Percentage for individual finger
  void setOpenedFingerThresholdPercentage(uint8_t f, uint8_t pct);                                // set opened Percentage for individual finger
  bool getFingerClosedStatus(uint8_t f);                                                          // return if the finger is still inside closed area
  bool getFingerOpenedStatus(uint8_t f);                                                          // return if the finger is still inside opened area
  void setKnockThreshold(float val_verPos, float val_verNeg, float val_horPos, float val_horNeg); // set new knock treshold
  void setKnockDebounceInterval(uint32_t val);                                                    // set new knock debounce interval
  void setFlickAllThreshold(float trs);                                                           // set all new flick treshold
  void setFlickOpenedThreshold(uint8_t f, float trs);                                             // set new finger positive flick treshold
  void setFlickClosedThreshold(uint8_t f, float trs);                                             // set new  finger negative flick treshold
  void setFlickDebounceInterval(uint32_t val);                                                    // set new flick  debounce interval
  float getAZGlastKnock();                                                                        // return last knock treshold
  void setAllAxleThresholdAngle(uint8_t ang);                                                     // set all axels threshold angles
  void setAxleMinThresholdAngle(uint8_t axl, uint8_t ang);                                        // set axel min threshold angle
  void setAxleMaxThresholdAngle(uint8_t axl, uint8_t ang);                                        // set axel min threshold angle
  bool getAxleMinStatus(uint8_t axl);                                                             // get axel min status
  bool getAxleMaxStatus(uint8_t axl);                                                             // get axel max status

  // attach and detach
  void attachCallOnWideClosedFingerLittle(void (*onRise)(void));
  void detachCallOnWideClosedFingerLittle();
  void attachCallOnWideClosedFingerRing(void (*onRise)(void));
  void detachCallOnWideClosedFingerRing();
  void attachCallOnWideClosedFingerMiddle(void (*onRise)(void));
  void detachCallOnWideClosedFingerMiddle();
  void attachCallOnWideClosedFingerIndex(void (*onRise)(void));
  void detachCallOnWideClosedFingerIndex();

  void attachCallOnWideOpenedFingerLittle(void (*onRise)(void));
  void detachCallOnWideOpenedFingerLittle();
  void attachCallOnWideOpenedFingerRing(void (*onRise)(void));
  void detachCallOnWideOpenedFingerRing();
  void attachCallOnWideOpenedFingerMiddle(void (*onRise)(void));
  void detachCallOnWideOpenedFingerMiddle();
  void attachCallOnWideOpenedFingerIndex(void (*onRise)(void));
  void detachCallOnWideOpenedFingerIndex();

  void attachCallOnMinXangle(void (*onRise)());
  void detachCallOnMinXangle();
  void attachCallOnMaxXangle(void (*onRise)());
  void detachCallOnMaxXangle();
  void attachCallOnMinYangle(void (*onRise)());
  void detachCallOnMinYangle();
  void attachCallOnMaxYangle(void (*onRise)());
  void detachCallOnMaxYangle();

  void attachCallOnFlickClosedFingerLittle(void (*onRise)(void));
  void detachCallOnFlickClosedFingerLittle();
  void attachCallOnFlickClosedFingerRing(void (*onRise)(void));
  void detachCallOnFlickClosedFingerRing();
  void attachCallOnFlickClosedFingerMiddle(void (*onRise)(void));
  void detachCallOnFlickClosedFingerMiddle();
  void attachCallOnFlickClosedFingerIndex(void (*onRise)(void));
  void detachCallOnFlickClosedFingerIndex();

  void attachCallOnFlickOpenedFingerLittle(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerLittle();
  void attachCallOnFlickOpenedFingerRing(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerRing();
  void attachCallOnFlickOpenedFingerMiddle(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerMiddle();
  void attachCallOnFlickOpenedFingerIndex(void (*onRise)(void));
  void detachCallOnFlickOpenedFingerIndex();

  void attachCallOnVerticalPositiveKnock(void (*onRise)(void));
  void detachCallOnVerticalPositiveKnock();
  void attachCallOnVerticalNegativeKnock(void (*onRise)(void));
  void detachCallOnVerticalNegativeKnock();

  void attachCallOnHorizontalPositiveKnock(void (*onRise)(void));
  void detachCallOnHorizontalPositiveKnock();
  void attachCallOnHorizontalNegativeKnock(void (*onRise)(void));
  void detachCallOnHorizontalNegativeKnock();

private:
  LinearRegression lr = LinearRegression();
  double values[2];
  const float zerof = 0.0;
  uint32_t frozen = 0;
  uint32_t ts_frozen = 0;
  uint32_t ts_nextLed;
  void ledOnAsync();
  void ledOffAsync();
  bool ledBuiltInActive = false;
  String serialData = "";                                                       // receive the BT string line on each READ
  String btDataPack[MAXBTDATAPACK];                                             // receive splitted dataSerial string
  bool on = false;                                                              // flags if BT is active
  const char *pin = "1234";                                                     // default slave pin
  String device_name = "BIONICSlave";                                           // default slave name
  float logF[MAXFINGERCHANNELS][MAXFLICKLOG] = {0};                             // log finger readings to apply  offset removal
  float flickThreshold[MAXFINGERCHANNELS][2] = {0};                             // flick activation limiar
  float knockVerticalPositiveThreshold = DEFKNOCKVERTICALPOSITIVETHRESHOLD;     // positive knock activation limiar
  float knockVerticalNegativeThreshold = DEFKNOCKVERTICALNEGATIVETHRESHOLD;     // negative knock activation limiar
  float knockHorizontalPositiveThreshold = DEFKNOCKHORIZONTALPOSITIVETHRESHOLD; // positive knock activation limiar
  float knockHorizontalNegativeThreshold = DEFKNOCKHORIZONTALNEGATIVETHRESHOLD; // negative knock activation limiar
  float lastKnockAZG = 0;                                                       // last G value when knock was unlocked
  float smoothFactor = 0;                                                       // smooth factor received from MASTER
  float fixedSmoothCoeffToKnock = 0.05;                                         // smooth factor received from MASTER
  float logAZG[MAXKNOCKLOG] = {0};                                              // last Accel Z G readings to compute knock
  float logAZGsmoothed[MAXKNOCKLOG] = {0};                                      // smoothed accel Z G readings to define if the hand is in normal or twisted posiction
  float lastAGsmoothed[MAXACCELCHANNELS] = {0};                                 // smoothed RAW accels
  float lastAAngsmoothed[MAXACCELCHANNELS] = {0};                               // smoothed RAW accels
  float logAG[MAXACCELCHANNELS][MAXKNOCKLOG] = {0};                             // log all accel axles G readings to aplly offset removal
  uint32_t knockDebounceInterval = DEFKNOCKDEBOUNCEINTERVAL;                    // time in ms between to allowed knocks
  uint32_t ts_lastKnock = 0;                                                    // millis() + knockInterval
  uint32_t flickDebounceInterval = DEFFLICKDEBOUNCEINTERVAL;                    // time in ms between to allowed knocks
  uint32_t ts_lastFlick = 0;                                                    // millis() + knockInterval
  bool receiveDataPack();                                                       // receive BT serial string and split
  void callbackClosedFinger();                                                  // check if any finger reached closed area and callback them
  void callbackOpenedFinger();                                                  // check if any finger reached opened area and callback them
  void logFremoveOffset();                                                      // log all fingers
  void callbackFlick();                                                         // remove offset accumullating the difference btween x - (x-1)
  void callbackFlickLr();                                                       // apply linear regression to 4 readings
  void callbackAxles();                                                         // check if any finger reached closed area and callback them
  void updateNewLimits();                                                       // compare if new readings are outside preset area and update to new ones
  void logAGremoveOffset();                                                     // stores last MAXLOGs values of 3 G accell axle to eventually remove its offsets
  void logAZGknock();                                                           // put new finger read into knock array
  void callbackKnock();                                                         // integrate ZG signal to find knock condition
  void callbackKnockLr();                                                       // integrate ZG signal to find knock condition
  void updateClosedThreshold(uint8_t f);                                        // update individual closed finger area and recalculate all limits
  void updateOpenedThreshold(uint8_t f);                                        // update individual opened finger area and recalculate all limits
  void updateAxleMinThreshold(uint8_t axl);
  void updateAxleMaxThreshold(uint8_t axl);
  void logAZGclear();
  void logFclear(uint8_t f); // clear all last finger readings
  bool doneMs(uint32_t t0, uint32_t dt);
  bool doneUs(uint32_t t0, uint32_t dt);
  bool done(uint32_t t, uint32_t t0, uint32_t dt);

  // callbacks
  void (*callClosedLittle)(void);
  void (*callClosedRing)(void);
  void (*callClosedMiddle)(void);
  void (*callClosedIndex)(void);

  void (*callOpenedLittle)(void);
  void (*callOpenedRing)(void);
  void (*callOpenedMiddle)(void);
  void (*callOpenedIndex)(void);

  void (*callMinX)(void);
  void (*callMaxX)(void);
  void (*callMinY)(void);
  void (*callMaxY)(void);

  void (*callFlickClosedLittle)(void);
  void (*callFlickClosedRing)(void);
  void (*callFlickClosedMiddle)(void);
  void (*callFlickClosedIndex)(void);

  void (*callFlickOpenedLittle)(void);
  void (*callFlickOpenedRing)(void);
  void (*callFlickOpenedMiddle)(void);
  void (*callFlickOpenedIndex)(void);

  void (*callVerticalPositiveKnock)(void);
  void (*callVerticalNegativeKnock)(void);
  void (*callHorizontalPositiveKnock)(void);
  void (*callHorizontalNegativeKnock)(void);

  static void isrDefaultUnused(); // to dettach callbacks

  typedef struct
  {
    int16_t fingerRead = 0;                                     // raw finger read value
    uint8_t closedThresholdPercentage = DEFTHRESHOLDPERCENTAGE; // percent value set by user to calculate internal closed critical area
    uint8_t openedThresholdPercentage = DEFTHRESHOLDPERCENTAGE; // percent value set by user to calculate internal iopened critical area
    uint16_t closedThresholdIn = 0;                             // finger closed critical zone in
    uint16_t closedThresholdOut = 0;                            // finger closed critical zone out
    uint16_t openedThresholdIn = 0;                             // finger opened critical zone in
    uint16_t openedThresholdOut = 0;                            // finger opened critical zone out
    bool closedFingerStatus = false;                            // status flagging that you are inside critical area
    bool openedFingerStatus = false;                            // status flagging that you are inside critical area
    float accel = 0;
  } record_finger;
  record_finger finger[MAXFINGERCHANNELS];

  typedef struct
  {
    float raw = 0;                                 // raw accel value
    float g = 0;                                   // g accel value
    float ang = 0;                                 // corrected angle accel value
    uint8_t minThresholdAngle = DEFTHRESHOLDANGLE; // x axle critical zone point
    uint8_t minThresholdIn = 0;                    // x axle critical zone in
    uint8_t minThresholdOut = 0;                   // x axle critical zone out
    bool minThresholdStatus = false;               // status flagging that you are inside critical area
    uint8_t maxThresholdAngle = DEFTHRESHOLDANGLE; // x axle critical zone point
    uint8_t maxThresholdIn = 0;                    // x axle critical zone in
    uint8_t maxThresholdOut = 0;                   // x axle critical zone out
    bool maxThresholdStatus = false;               // status flagging that you are inside critical area
  } record_accel;
  record_accel accel[MAXACCELCHANNELS];
  bool firstReading = true;
  bool knockAllowed = true;
  uint32_t turnOffLed = 0;
};

#endif

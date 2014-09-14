#include <TinyGPS.h>
#include <DualGPS.h>
#include <SoftwareSerial.h>
#include <twoSerialComm.h>


#include <VarSpeedServo.h> 
VarSpeedServo tailServo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

const int tailPin = 7;  // the digital pin used for the servo

int tailPos = 0;    // variable to store the servo position char c ;


// sequences are defined as an array of points in the sequence
// each point has a position from 0 - 180, and a speed to get to that position
servoSequencePoint allgood[] = {
  {
    66,5              }
  ,{
    90,5              }
};

servoSequencePoint uneasy[] = {
  {
    30,15              }
  ,{
    70,15              }
};

servoSequencePoint uncomfortable[] = {
  {
    20,15              }
  ,{
    120,15              }
};

servoSequencePoint thrashing[] = {
  {
    0, 145              }
  ,{
    150, 145              }
};




DualGPS GPS1;
DualGPS GPS2;
DualGPS logging;

SoftwareSerial logger(2, 3); // RX, TX
twoSerialComm serCom(logger);


unsigned long previousLogTime = 0;        
unsigned long logInterval = 1100;  //  >=1000 to avoid duplicates, GPS updates at 1Hz

long lat1,lon1, lat2, lon2;
unsigned long GPSDifferenceValue;

unsigned long currentLogTime = 0;

//SoftwareSerial logger(2,3); // RX, TX to 2nd arduino

//SoftwareSerial tailDriver(7,8); // RX, TX to 2nd arduino
//boolean currentConfined = 0; //for storing whether currently feel confined or not

// <-- all good    --> cL1 <-- uneasy              --> cL2 <-- uncomfortable          --> cL3 <-- GETME OUT OF HERE!
// <-- slow twitch --> cL1 <-- faster, small range --> cL2 <-- faster, large range    --> cL3 <-- thrashing
int cl1 = 50;
int cl2 = 100;
int cl3 = 150;


int cl1Pow2 = cl1*cl1; //for use in code to avoid square rootz
int cl2Pow2 = cl2*cl2;
int cl3Pow2 = cl3*cl3;

// used in a previous version of the code - might come back in later
/*boolean previousConfined = 0;
 boolean distressedFlag = 0;
 unsigned long confinedThreshold = 100; //how much is confined?
 unsigned long confinedThresholdPow2 = confinedThreshold * confinedThreshold;
 int confinedMaxDuration = 200000; //amount of time can tolerate being confined before gets distressed
 
 unsigned long confinedStart = 0;
 unsigned long confinedTimer = 0; */

void setup()
{
  pinMode(14,INPUT_PULLUP); //tx3, was trying to solve problems with OpenLog - still a work in progress
  Serial.begin(115200);
  Serial3.begin(4800);
 // tailDriver.begin(4800); // communicatjon wih 2nd arduino
  GPS1.serialBegin(1, 1, 4800);
  GPS2.serialBegin(2, 2, 4800);
   serCom.begin(4800); // baud rate 
serCom.test(); // prints a test message to serial 1
  serCom.printToSerial("Device ready dfdsawe",0); // prints Device ready to both serial lines


  tailServo.attach(tailPin);


  // print field names
  Serial.println("GPS1Label,GPS1date,GPS1time,GPS1hdop,GPS1sats,GPS1flat,GPS1flon,GPS1alt,GPS1course,GPS1speed,GPS2Label,GPS2date,GPS2time,GPS2hdop,GPS2sats,GPS2flat,GPS2flon,GPS2alt,GPS2course,GPS2speed,GPSDifference,currentConfined,confinedTimer,distressedFlag,solenoidDriver");
  Serial3.println("GPS1Label,GPS1date,GPS1time,GPS1hdop,GPS1sats,GPS1flat,GPS1flon,GPS1alt,GPS1course,GPS1speed,GPS2Label,GPS2date,GPS2time,GPS2hdop,GPS2sats,GPS2flat,GPS2flon,GPS2alt,GPS2course,GPS2speed,GPSDifference,currentConfined,confinedTimer,distressedFlag,solenoidDriver");


}

void loop()
{
  currentLogTime = millis();

  if(currentLogTime - previousLogTime > logInterval) { // if we've waited long enough since the last reading, then log the data
    previousLogTime = currentLogTime;  // save the last time you logged

    GPS1.gpsdump(); // fill variables with new data
    GPS2.gpsdump();  // fil variables with new data

    lat1 = GPS1.get_most_recentLat(1); 
    lon1 = GPS1.get_most_recentLng(1); 
    lat2 = GPS2.get_most_recentLat(2); 
    lon2 = GPS2.get_most_recentLng(2); 

    GPSDifferenceValue = GPS1.getGPSDifference(lon1, lon2, lat1, lat2);
    
serCom.printToSerial("sercomtest from sketch",2);



 // logger.print(F(","));

    if (GPSDifferenceValue < cl1Pow2){
      Serial.print(0); 
   //   tailDriver.print("0");
//      logger.print(0);     

      tailServo.sequencePlay(allgood, 2); 

    }
    else if (GPSDifferenceValue >= cl1Pow2 && GPSDifferenceValue < cl2Pow2){
      Serial.print(1); 
  //    tailDriver.print("1");
  //    logger.print(1);     

      tailServo.sequencePlay(uneasy, 2); 

    }
    else if (GPSDifferenceValue >= cl2Pow2 && GPSDifferenceValue < cl3Pow2){
      Serial.print(2); 
 //     tailDriver.print("2");
 //     logger.print(2);       

      tailServo.sequencePlay(uncomfortable, 2); 



    }

    else{
      Serial.print(3); 
 //     tailDriver.print("3");
 //     logger.print(3);     

      tailServo.sequencePlay(thrashing, 2);

    }




    Serial.println();
//    logger.println();


  } // end logging timer



  // continuously executed code in here

  GPS1.feedgps();
  GPS2.feedgps();

}//end loop



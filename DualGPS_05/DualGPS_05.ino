#include <TinyGPS.h>
#include <DualGPS.h>
#include <SoftwareSerial.h>


DualGPS GPS1;
DualGPS GPS2;

long previousLogTime = 0;        
long logInterval = 1100;  //  >=1000 to avoid duplicates

long lat1,lon1, lat2, lon2;
unsigned long GPSDifferenceValue;

unsigned long currentLogTime = 0;





SoftwareSerial tailDriver(7,8); // RX, TX
boolean currentConfined = 0; //for storing whether currently feel confined or not
int confinedLevel;

// <-- all good    --> cL1 <-- uneasy              --> cL2 <-- uncomfortable          --> cL3 <-- GETME OUT OF HERE!
// <-- slow twitch --> cL1 <-- faster, small range --> cL2 <-- faster, large range    --> cL3 <-- thrashing
int cl1 = 200;
int cl2 = 300;
int cl3 = 400;


int cl1Pow2 = cl1*cl1;
int cl2Pow2 = cl2*cl2;
int cl3Pow2 = cl3*cl3;


boolean previousConfined = 0;
boolean distressedFlag = 0;
unsigned long confinedThreshold = 100; //how much is confined?
unsigned long confinedThresholdPow2 = confinedThreshold * confinedThreshold;
int confinedMaxDuration = 200000; //amount of time can tolerate being confined before gets distressed

unsigned long confinedStart = 0;
unsigned long confinedTimer = 0;

void setup()
{
  pinMode(14,INPUT_PULLUP);
  Serial.begin(115200);
  Serial3.begin(4800);
  tailDriver.begin(4800);
  GPS1.serialBegin(1, 1, 4800);
  GPS2.serialBegin(2, 2, 4800);

  Serial.println("GPS1Label,GPS1date,GPS1time,GPS1hdop,GPS1sats,GPS1flat,GPS1flon,GPS1alt,GPS1course,GPS1speed,GPS2Label,GPS2date,GPS2time,GPS2hdop,GPS2sats,GPS2flat,GPS2flon,GPS2alt,GPS2course,GPS2speed,GPSDifference,currentConfined,confinedTimer,distressedFlag,solenoidDriver");
  Serial3.println("GPS1Label,GPS1date,GPS1time,GPS1hdop,GPS1sats,GPS1flat,GPS1flon,GPS1alt,GPS1course,GPS1speed,GPS2Label,GPS2date,GPS2time,GPS2hdop,GPS2sats,GPS2flat,GPS2flon,GPS2alt,GPS2course,GPS2speed,GPSDifference,currentConfined,confinedTimer,distressedFlag,solenoidDriver");


}

void loop()
{
  currentLogTime = millis();

  if(currentLogTime - previousLogTime > logInterval) { // if we've waited long enough since the last reading, then log the data
    previousLogTime = currentLogTime;  // save the last time you logged

    GPS1.gpsdump(); // fill variables with new data

    // GPS2.feedgps();
    GPS2.gpsdump();  // fil variables with new data

    lat1 = GPS1.get_most_recentLat(1); 
    lon1 = GPS1.get_most_recentLng(1); 
    lat2 = GPS2.get_most_recentLat(2); 
    lon2 = GPS2.get_most_recentLng(2); 

    GPSDifferenceValue = GPS1.getGPSDifference(lon1, lon2, lat1, lat2);
    //decide whether this is too confined or not (and for how long)

    if (GPSDifferenceValue < cl1Pow2){
      Serial.print(0); 
      tailDriver.print("0");
      Serial3.print(0);       
    }
    else if (GPSDifferenceValue >= cl1Pow2 && GPSDifferenceValue < cl2Pow2){
      Serial.print(1); 
      tailDriver.print("1");
      Serial3.print(1);       
    }
    else if (GPSDifferenceValue >= cl2Pow2 && GPSDifferenceValue < cl3Pow2){
      Serial.print(2); 
      tailDriver.print("2");
      Serial3.print(2);       
    }

    else{
      Serial.print(3); 
      tailDriver.print("3");
      Serial3.print(3);       
    }

 


    Serial.println();
    Serial3.println();


  } // end logging timer



  // continuously executed code in here

  GPS1.feedgps();
  GPS2.feedgps();

}//end loop

















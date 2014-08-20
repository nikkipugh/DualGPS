#include <TinyGPS.h>
#include <DualGPS.h>
#include <SoftwareSerial.h>


DualGPS GPS1;
DualGPS GPS2;

long previousLogTime = 0;        
long logInterval = 1500;  //  >=1000 to avoid duplicates

long lat1,lon1, lat2, lon2;
unsigned long GPSDifferenceValue;

unsigned long currentLogTime = 0;

bool newGPS1Data = false;
bool newGPS2Data = false;



SoftwareSerial solenoidDriver(47, 49); // RX, TX
boolean currentConfined = 0; //for storing whether currently feel confined or not
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
  solenoidDriver.begin(4800);
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

    if(   GPSDifferenceValue >= confinedThresholdPow2  && previousConfined == 0)   //transitioned to being too confined
    {
      currentConfined = 1;
      confinedStart = millis(); 
      solenoidDriver.print("1");

    }

    if (GPSDifferenceValue < confinedThresholdPow2 && previousConfined == 1)  // transitioned from being too confined
    {  
      currentConfined = 0;  
      confinedStart = 0;  
      confinedTimer = 0;  
      // solenoidDriver.print("0");


    }

    if (currentConfined ==1 && previousConfined == 1)  {  //still confined - time it
      confinedTimer = millis();   

      if (confinedTimer - confinedStart >= confinedMaxDuration) {  // have been confined for too long!
        distressedFlag = 1;
        //  solenoidDriver.print("2");
      }


    }  


    previousConfined = currentConfined;  // update previousConfined ready for next time around


      Serial.print(currentConfined); 
    Serial.print(F(","));  
    Serial.print(confinedTimer); 
    Serial.print(F(","));  
    Serial.print(distressedFlag); 
    Serial.print(F(","));  

    Serial3.print(currentConfined); 
    Serial3.print(F(","));  
    Serial3.print(confinedTimer); 
    Serial3.print(F(","));  
    Serial3.print(distressedFlag); 
    Serial3.print(F(",")); 



    //affect the heart rate
    if (currentConfined == 0 && distressedFlag == 0)  // all good - relaxed heartrate
    { 
      Serial.print(0); 
      solenoidDriver.print("0");
      Serial3.print(0); 
    }

    if (currentConfined == 1 && distressedFlag == 0) // feeling a bit twitchy
    {
      Serial.print(1); 
      solenoidDriver.print("1");
      Serial3.print(1); 
    }


    if (distressedFlag == 1) // game over
    { 
      Serial.print(2); 
      solenoidDriver.print("2");
      Serial3.print(2); 
    }

    Serial.println();
    Serial3.println();


    if (distressedFlag == 1) 
    {

      //   while(true) {
      //just resting
      // }

    }



  } // end logging timer



  // continuously executed code in here
  
  GPS1.feedgps();
  GPS2.feedgps();

}//end loop















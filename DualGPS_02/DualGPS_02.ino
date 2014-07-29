#include <TinyGPS.h>
#include <DualGPS.h>
#include <SoftwareSerial.h>


DualGPS GPS1;
DualGPS GPS2;

long previousLogTime = 0;        
long logInterval = 1000;  //>=1000 to avoid duplicates

long lat1,lon1, lat2, lon2;
unsigned long GPSDifferenceValue;


SoftwareSerial solenoidDriver(47, 49); // RX, TX
boolean currentConfined = 0; //for storing whether currently feel confined or not
boolean previousConfined = 0;
boolean distressedFlag = 0;
unsigned long confinedThreshold = 100; //how much is confined?
unsigned long confinedThresholdPow2 = confinedThreshold * confinedThreshold;
int confinedMaxDuration = 20000; //amount of time can tolerate being confined before gets distressed

unsigned long confinedStart = 0;
unsigned long confinedTimer = 0;

void setup()
{
  Serial.begin(115200);
  Serial3.begin(4800);

  GPS1.serialBegin(1, 1, 4800);
  GPS2.serialBegin(2, 2, 4800);

  Serial.println("--------------------------------------------------------------------------------------------------------------------------------------");
}

void loop()
{
  unsigned long currentLogTime = millis();
  GPS2.feedgps();
  if (GPS1.feedgps() ){
    if(currentLogTime - previousLogTime > logInterval) {
      // save the last time you logged
      previousLogTime = currentLogTime;  
      GPS1.gpsdump();
      GPS2.gpsdump();

      // Serial.print(F("Difference, "));
      lat1 = GPS1.get_most_recentLat(1); 
      lon1 = GPS1.get_most_recentLng(1); 
      lat2 = GPS2.get_most_recentLat(2); 
      lon2 = GPS2.get_most_recentLng(2); 

      GPSDifferenceValue = GPS1.getGPSDifference(lon1, lon2, lat1, lat2);
      //decide whether this is too confined or not (and for how long)

      if(   GPSDifferenceValue >= confinedThresholdPow2  && previousConfined == 0)    
      {
        currentConfined = 1;
        confinedStart = millis(); 
      }

      if (GPSDifferenceValue < confinedThresholdPow2 && previousConfined == 1) 
      {  
        currentConfined = 0;  
        confinedStart = 0;  
        confinedTimer = 0;  
      }

      if (currentConfined ==1 && previousConfined == 1)  {
        confinedTimer = millis();   

        if (confinedTimer - confinedStart >= confinedMaxDuration) {
          distressedFlag = 1;
          solenoidDriver.print("2");
        }


      }  


      previousConfined = currentConfined;


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
      if (currentConfined == 0 && distressedFlag == 0)
      { 
        Serial.print(0); 

        Serial3.print(0); 
      }
      if (currentConfined == 1 && distressedFlag == 0)
      {
        Serial.print(1); 

        Serial3.print(1); 
      }


      if (distressedFlag == 1)
      { 
        Serial.print(2); 

        Serial3.print(2); 
      }

      Serial.println();
      Serial3.println();


      if (distressedFlag == 1) 
      {

        while(true) {
          //just resting
        }

      }


    } // end logging timer


  }  //end GPS1 feed gps

  // continuously executed code in here

}//end loop








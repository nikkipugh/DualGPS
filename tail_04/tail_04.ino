#include <VarSpeedServo.h> 
VarSpeedServo tailServo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

const int tailPin = 7;  // the digital pin used for the servo

int tailPos = 0;    // variable to store the servo position char c ;


// sequences are defined as an array of points in the sequence
// each point has a position from 0 - 180, and a speed to get to that position
servoSequencePoint allgood[] = {
  {
    66,5            }
  ,{
    90,5            }
};

servoSequencePoint uneasy[] = {
  {
    60,15            }
  ,{
    110,15            }
};

servoSequencePoint uncomfortable[] = {
  {
   110,35            }
  ,{
    180,45            }
};

servoSequencePoint thrashing[] = {
  {
    0, 200            }
  ,{
    177, 145            }
    ,{
    180, 50            }
};

char c=0;
int randNumber;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(4800);

  tailServo.attach(tailPin);


  randomSeed(analogRead(0));


}

void loop()
{





  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.println(c); 
}

    if (c == '0')
    {

      tailServo.sequencePlay(allgood, 2); 

    }
    if (c == '1')
    {
      tailServo.sequencePlay(uneasy, 2); 
    }
    if (c == '2')
    {
      tailServo.sequencePlay(uncomfortable, 2); 
    }
        if (c == '3')
    {
      tailServo.sequencePlay(thrashing, 3);
    }

  

  //tailServo.sequencePlay(allgood, 7, true, 2); // play sequence "twitchy", loop, start at third position

  //  tailServo.sequencePlay(allgood, 2); // play sequence "twitchy", loop, start at third position
  //    Serial.print("upperExtreme   ");
  //        Serial.print(upperExtreme);
  //    Serial.print("   lowerExtreme   ");
  //    Serial.println(lowerExtreme);




} //end loop







#include <VarSpeedServo.h> 
VarSpeedServo tailServo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

const int tailPin = 9;  // the digital pin used for the servo

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
    30,15            }
  ,{
    70,15            }
};

servoSequencePoint uncomfortable[] = {
  {
    20,15            }
  ,{
    120,15            }
};

servoSequencePoint thrashing[] = {
  {
    0, 145            }
  ,{
    150, 145            }
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
      tailServo.sequencePlay(thrashing, 2);
    }

  

  //tailServo.sequencePlay(allgood, 7, true, 2); // play sequence "twitchy", loop, start at third position

  //  tailServo.sequencePlay(allgood, 2); // play sequence "twitchy", loop, start at third position
  //    Serial.print("upperExtreme   ");
  //        Serial.print(upperExtreme);
  //    Serial.print("   lowerExtreme   ");
  //    Serial.println(lowerExtreme);




} //end loop







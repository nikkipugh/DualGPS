#include <VarSpeedServo.h> 
VarSpeedServo tailServo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

const int tailPin = 9;  // the digital pin used for the servo

int tailPos = 0;    // variable to store the servo position char c ;

int upperExtreme = 120;
int lowerExtreme = 80;



// sequences are defined as an array of points in the sequence
// each point has a position from 0 - 180, and a speed to get to that position
servoSequencePoint slow[] = {
  {
    100,20  }
  ,{
    20,20  }
  ,{
    60,50  }
}; // go to position 100 at speed of 20, position 20 speed 20, position 60, speed 50
servoSequencePoint twitchy[] = {
  {
    0,255  }
  ,{
    180,40  }
  ,{
    90,127  }
  ,{
    120,60  }
};
//servoSequencePoint allgood[] = {{90,10},{110,10},{120,5},{110,5},{80,10},{70,5},{80,5}};
/*servoSequencePoint allgood[] = {
  {
    90,10  }
  ,{
    upperExtreme-10,10  }
  ,{
    upperExtreme,5  }
  ,{
    upperExtreme-10,5  }
  ,{
    lowerExtreme+10,10  }
  ,{
    lowerExtreme,5  }
  ,{
    lowerExtreme+10,5  }
};*/


char c;
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
  servoSequencePoint allgood[] = {
  {
    90,10  }
  ,{
    upperExtreme+random(-5,+5),10  }
  ,{
    upperExtreme,5  }
  ,{
    upperExtreme-10,5  }
  ,{
    lowerExtreme+10,10  }
  ,{
    lowerExtreme,5  }
  ,{
    lowerExtreme+10,5  }
  };
  /*
  while (Serial1.available())
   {
   c = Serial1.read();
   Serial.println(c); // uncomment this line if you want to see the GPS data flowing
   }
   
   if (c == '0')
   {
   
   }
   if (c == '1')
   {
   }
   
   if (c == '2')
   {
   }
   
   if (c == '3')
   {
   }
   
   */


  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.println(c); 
  }

  if (c == '0')
  {
 

  }
  if (c == '1')
  {
    int upperExtreme = 180;
    int lowerExtreme = 00;
      tailServo.sequencePlay(allgood, 7); // play sequence "twitchy", loop, start at third position

  }

  if (c == '2')
  {
    int upperExtreme = 120;
    int lowerExtreme = 80;
    
          tailServo.sequencePlay(twitchy, 4); // play sequence "twitchy", loop, start at third position

  }

  if (c == '3')
  {
    int upperExtreme = 120;
    int lowerExtreme = 80;
    
              tailServo.sequencePlay(slow, 3); // play sequence "twitchy", loop, start at third position

  }

//  tailServo.sequencePlay(allgood, 7, true, 2); // play sequence "twitchy", loop, start at third position

} //end loop





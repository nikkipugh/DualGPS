#include <twoSerialComm.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

twoSerialComm serCom(mySerial);

void setup() {
  // put your setup code here, to run once:

//mySerial.begin(4800);
  serCom.begin(4800); // baud rate serial 1, baud rate serial2
  serCom.test(); // prints a test message to serial 1
  serCom.printToSerial("Device ready",0); // prints Device ready to both serial lines


}

void loop() {
  // put your main code here, to run repeatedly: 



serCom.printToSerial("mySerial println sldkflsdkjf ",2);
delay(1000);

}


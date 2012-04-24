/* Example of using the Tx433_Proove lib */
#include "tx433_proove.h"

// The transmitter unique address (you will need to chage this)
char *transmitter = "1010100101101001010101100101011001010101010101010110";

// The transmitter channel (might need to change this)
char *channel="0101";

// Create an instance of the Proove class
Tx433_Proove Proove(4, transmitter, channel);


void setup() {
  // put your setup code here, to run once:
  Proove.Device_On(3);	// Turn on group
  delay(700);
  Proove.Device_Off(3);	// Turn off group
  delay(700);
}

void loop() {
  // put your main code here, to run repeatedly: 
  Proove.Device_On(1);
  delay(150);
  Proove.Device_On(2);
  delay(150);
  Proove.Device_On(0);
  delay(700);
  Proove.Device_Off(1);
  delay(150);
  Proove.Device_Off(0);
  delay(150);
  Proove.Device_Off(2);
  delay(700);
}

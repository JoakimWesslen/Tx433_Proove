/*
  Joakim Wesslen
  2012-04-24

  A library to handle the TX433 Proove device.
  Version 1.0

  Here is a link to some overview of some different protocols:
  http://tech.jolowe.se/home-automation-rf-protocols/
  
  Summary of the Proove protocol.

  Proove packet structure (32 bits):
  HHHH HHHH HHHH HHHH HHHH HHHH HHGO CCEE
  H = The first 26 bits are transmitter unique codes, and it is this code that the reciever “learns” to recognize.
  G = Group code. Set to 0 for on, 1 for off.
  O = On/Off bit. Set to 0 for on, 1 for off.
  C = Channel bits.
  E = Unit bits. Device to be turned on or off. Unit #1 = 00, #2 = 01, #3 = 10.  

  Physical layer.
  Every bit in the packets structure is sent as two physical bits. 
  Where the second bit is the inverse of the first, i.e. 0 -> 01 and 1 -> 10.
  Example: 10101110 is sent as 1001100110101001 
  The sent packet length is thus 64 bits.
  
  A message is made up by a Sync bit followed by the Packet bits and ended by a Pause bit.
  Every message is repeated four times.
  
  The API:s below handles code parameter of the longer format, i.e. with redundancy bits.

*/

#ifndef TX433_PROOVE_H
#define TX433_PROOVE_H

#define LIB_VERSION 1.0

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

class Tx433_Proove
{
  public:
	/*
	@digitalpin - the digitalpin to send data on to transmitter
	@transmittercode - the unique code of the transmitter (26/52 bits)
	@channelcode - the channel code (2/4 bits)
	*/
	Tx433_Proove(int digitalpin, char *transmittercode, char *channelcode);

	/*
	@unit - the device to turn on. 
		0,1,2 are the three separate devices.
		3 is the complete group.
	*/
	void Device_On(int unit);	  

	/*
	@unit - the device to turn off. 
		0,1,2 are the three separate devices.
		3 is the complete group.
	*/
	void Device_Off(int unit);	  
	
  private:
	int txpin;
	
	static char *TxCode;
	static char *ChCode;
	static char *GrpOn;
	static char *GrpOff;
	static char *On;
	static char *Off;
	static char *Unit[];

	const static int pulse_high;
	const static int pulse_one_low;
	const static int pulse_zero_low;
	const static int pulse_sync_low;
	const static int pulse_pause_low;
  
	// Protocol layer
	void sendCode(char *str, int len);
	void sendPackets(char *grp, char *dev, char *onoff);

	// Physical layer interfaces
	void sendZero(void);
	void sendOne(void);
	void sendSync(void);
	void sendPause(void);
};
#endif	// TX433_PROOVE_H

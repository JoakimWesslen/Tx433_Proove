/*
  Joakim Wesslen
  2012-04-24

  A library to handle the TX433 Proove device.
  Version 1.0

  http://tech.jolowe.se/home-automation-rf-protocols/
  
*/
#include "tx433_proove.h"

extern "C" {
  // AVR LibC Includes
  #include <inttypes.h>
  #include <avr/interrupt.h>
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
}

#define RETRANSMIT  4

char *Tx433_Proove::TxCode="0000000000000000000000000000000000000000000000000000";
char *Tx433_Proove::ChCode="0000";
char *Tx433_Proove::GrpOn="01";
char *Tx433_Proove::GrpOff="10";
char *Tx433_Proove::On="01";
char *Tx433_Proove::Off="10";

char *Tx433_Proove::Unit[] = {
	"0101",
	"0110",
	"1001",
	"0101"
};

const int Tx433_Proove::pulse_high = 250;
const int Tx433_Proove::pulse_one_low = 250;
const int Tx433_Proove::pulse_zero_low = 1250;
const int Tx433_Proove::pulse_sync_low = 2500;
const int Tx433_Proove::pulse_pause_low = 10000;


/* Public */

Tx433_Proove::Tx433_Proove(int digitalpin, char *transmittercode, char *channelcode)
{
	txpin = digitalpin;
	pinMode(txpin, OUTPUT);

	if (strlen(transmittercode) == strlen(TxCode))
		memcpy(TxCode, transmittercode, strlen(TxCode));
	if (strlen(channelcode) == strlen(ChCode))
		memcpy(ChCode, channelcode, strlen(ChCode));
}

void Tx433_Proove::Device_On(int dev)
{
	if (dev == 3) {
		sendPackets(GrpOn, Unit[dev], On);
	} else
		sendPackets(GrpOff, Unit[dev], On);
}	  

void Tx433_Proove::Device_Off(int dev)
{
	if (dev == 3) {
		sendPackets(GrpOn, Unit[dev], Off);
	} else
		sendPackets(GrpOff, Unit[dev], Off);
}	  


/* Private */

// Protocol layer
void Tx433_Proove::sendCode(char *str, int len) {
  char *p = str;
  int i = 0;
  while (i <= len) {
    if (*p == '0') {
      sendZero();
    }
    if (*p == '1') {
      sendOne();
    }
    p++;
    i++;
  }
}

void Tx433_Proove::sendPackets(char *grp, char *dev, char *onoff) {
  for (int i = 0; i < RETRANSMIT; i++) {
	  sendSync();
	  sendCode(TxCode, strlen(TxCode));
	  sendCode(grp, strlen(grp));
	  sendCode(onoff, strlen(onoff));
	  sendCode(ChCode, strlen(ChCode));
	  sendCode(dev, strlen(dev));
	  sendPause();
  }
}

// Physical layer
void Tx433_Proove::sendZero() {
  digitalWrite(txpin, HIGH);
  delayMicroseconds(pulse_high);
  digitalWrite(txpin, LOW);
  delayMicroseconds(pulse_zero_low);
}

void Tx433_Proove::sendOne() {
  digitalWrite(txpin, HIGH);
  delayMicroseconds(pulse_high);
  digitalWrite(txpin, LOW);
  delayMicroseconds(pulse_one_low);
}

void Tx433_Proove::sendSync() {
  digitalWrite(txpin, HIGH);
  delayMicroseconds(pulse_high);
  digitalWrite(txpin, LOW);
  delayMicroseconds(pulse_sync_low);
}

void Tx433_Proove::sendPause() {
  digitalWrite(txpin, HIGH);
  delayMicroseconds(pulse_high);
  digitalWrite(txpin, LOW);
  delayMicroseconds(pulse_pause_low);
}

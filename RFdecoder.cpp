/*
  RCSdecoder - Ultra light Arduino libary for RF remote control receiver
  Copyright (c) 2020 Vincent Horn.  All right reserved.
  
  Project home: https://github.com/sui77/rc-switch/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* This library has been designed for one specific 24 bits RF protocole
 * to make it as light as possible.
 *
 * Sync sync : 1 high pulse and 31 low pulses
 *     (perceived as a 31*pulselength long pulse, total length of sync bit is
 *     32*pulselength microseconds)
 *     _
 *    | |_______________________________ (don't count the vertical bars)
 *
 * Pulselength: pulse length is approximatly 390 microseconds
 *
 * "0" bit: waveform for a data bit of value "0", 
 *     1 high pulse and 3 low pulses, 
 *     1 pulse length (390 microseconds) + 3 pulses length (3x390 = 1170 microseconds)
 *     Can be adjusted by using the delayTolerance variable
 *      _
 *     | |___
 * 
 * "1" bit: waveform for a data bit of value "1", 
 *     3 high pulse and 1 low pulses
 *      ___
 *     |   |_
 * 
 */

#include "RFdecoder.h"

volatile unsigned long RFdecoder::ReceivedValue = 0;


RFdecoder::RFdecoder(int interrupt) {
  RFdecoder::ReceivedValue = 0;
  attachInterrupt(interrupt, handleInterrupt, CHANGE);  
}

bool RFdecoder::available() {
  return RFdecoder::ReceivedValue != 0;
}

unsigned long RFdecoder::getValue() {
  return RFdecoder::ReceivedValue;
}

void RFdecoder::reset() {
  RFdecoder::ReceivedValue = 0;
}

void RFdecoder::handleInterrupt() {

  volatile static unsigned long long raw = 0;				// contains all short (1) and long (0) delays (2x24 bits)
  volatile static unsigned int rawPosition = 0;				// raw counter
  volatile static unsigned int startdetect = 0;				// Sync detection number
  volatile static unsigned long lastTime = 0;

  
  const long time = micros();
  const unsigned int duration = time - lastTime;
  const unsigned int Tolerance = 250;
  const unsigned int shortPulse = 390;						// Short pulse length
  const unsigned int longPulse = 1170;						// 3x shortPulse
  
  if ((duration > 12000) && (duration < 12200)) {			// Detect synchro 
    startdetect ++;  
    rawPosition = 0;
    raw = 0;
  }

  if ((startdetect == 2) && (duration < 2000)){				// catch 2 rd data flow send
    rawPosition ++;
    raw <<= 1;												
    if (diff(duration, shortPulse) < Tolerance){			// Add 1 (shortPulse)
    } else if (diff(duration, longPulse) < Tolerance){
      raw |= 1;												// Add 1 (longPulse)
    } else {
      startdetect = 0; 
    }
  } 

  if (rawPosition == 48){									// All 48 pulses hav been received (2x24 bits)
    RFdecoder::ReceivedValue = findCode(raw);				// Convert 48 Pulses in 24 bits data
    rawPosition = 0;   
    startdetect = 0;
  }
  
  lastTime = time; 
}

unsigned long RFdecoder::findCode(unsigned long long Dec) {	
  
  unsigned long addByte =0;
  unsigned long code = 0;
  int shiftIndex =0;
  
  while (Dec != 0) {
    addByte = ((Dec & 3) == 1 ) ? 0 : 1;					// take bits 2 by 2. if "1 0" (short long) then add a 0 else add a 1 
    Dec = Dec >> 2;  										// move to next couple 
    addByte = addByte << shiftIndex;						// shift x time to the left ()
    code += addByte;    									// add the new couple
    shiftIndex++;
  }
  return code;
}

unsigned int RFdecoder::diff(int A, int B) {
  return abs(A - B);
}
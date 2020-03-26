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

#ifndef RFdecoder_h
#define RFdecoder_h

#include "Arduino.h"

class RFdecoder {

  public:
  
    RFdecoder(int interrupt);

    bool available();
    void reset();
    unsigned long getValue();

  private:
	
    static void handleInterrupt();
	static unsigned int diff(int A, int B);
	static unsigned long findCode(unsigned long long Dec);
    volatile static unsigned long ReceivedValue;
	
	const int interrupt;
    
};

#endif

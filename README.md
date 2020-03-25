# RF-decoder

Inspired by different existing RF ibraries, I wrote my own light library with restrains fonctions and protocols. 
Only receiving function is implemented with fixed protocol.

This library has been designed for one specific 24 bits RF protocole
 to make it as light as possible.

 Sync sync : 1 high pulse and 31 low pulses
     (perceived as a 31*pulselength long pulse, total length of sync bit is
     32*pulselength microseconds)
     _
    | |_______________________________ (don't count the vertical bars)

 Pulselength: pulse length is approximatly 390 microseconds

 "0" bit: waveform for a data bit of value "0", 
     1 high pulse and 3 low pulses, 
     1 pulse length (390 microseconds) + 3 pulses length (3x390 = 1170 microseconds)
     Can be adjusted by using the delayTolerance variable
      _
     | |___
 
 "1" bit: waveform for a data bit of value "1", 
     3 high pulse and 1 low pulses
      ___
     |   |_

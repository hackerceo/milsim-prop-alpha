# COMMS Standard
The serial communications standard for Milsim-Prop Alpha1 and accessories will be as follows:

### Cabling and Connectors

The standard cabling will use [Speakon 4-pole](https://www.aliexpress.com/w/wholesale-speakon-4-pole.html?catId=0&SearchText=speakon+4+pole) connectors.  On those connectors the following pins will be wired as:
- Speakon -1 : GND (Ground direct connection)
- Speakon +1 : VCC (Fuse-protected connection to battery positive [22.2v])
- Speakon -2 : RX (RS232 Receiving signal)
- Speakon +2 : TX (RS232 Transmitting signal)

It is expected that all compatible cables shall have a crossover pairing with regards to connections -2 and +2.  
This means that a cable having sides A and B shall be wired such that:
- A/-2 connects to B/+2
- A/+2 connects to B/-2


### Electrical and Protocol
The COMMS standard follows [RS232 standard](https://en.wikipedia.org/wiki/RS-232) for physical voltages and shall dictate that +5v represents a "1" logic and -5v represents a "0" logic.
The baud rate shall be "19200" using the default Arduino configuration of "SERIAL_8N1" meaning 8 bits, no parity, 1 stop bit.   

The current communications chip on the Milsim-Prop Alpha1 prototype is a [MaxLinear SP232EEN](https://www.lcsc.com/product-detail/RS232-ICs_MaxLinear-SP232EEN-L-TR_C7078.html). 


### Daughter Devices
The following daughter devices use this standard:
- 4-Team Domination Box
- Strike Target(s)
- Bio-Freezer Box
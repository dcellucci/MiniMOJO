# Central Computer Control

All control is done directly by the computer. The function of the microcontrollers is to pass messages with the minimum of parsing.

This document describes a few of the commands used to interface with the USB Bridge and the Coordinator

## Command Reference 

 1. **l:** toggles the LED pin on the **bridge** board. Good for debugging.
 2. **s:** prompts the bridge to send a *status request* packet, whose format is TBD.
 3. **w:** reads in the next **five** (5) bytes to the payload and sends a *motor update* packet with this information to the coordinator. 
 4. **!:** the modify settings command. Reads in bytes until either the payload array is full or the buffer runs out. Sends a *settings modification* packet with these data.
 5. **c:** prompts the bridge to send a *current measurement request* packet. The format of the resulting string is a comma-separated list of ASCII integers with the currents in mA, with the first value of the string being the char 'c', i.e. `c,100,500`

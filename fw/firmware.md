# Firmware Specification

This document assumes you've successfully assembled a MOJO, or you have an assembled MOJO already.

## Definitions
Each MOJO consists of two halves, a top and a bottom, each of which has 3 boards: a wireless coordinator board, and a servo control board/ battery board sandwich. One of the router boards is there purely for structure (for now). The boards are connected using 4-pin flat flexible cables (FFC) with an I2C bus. 

MOJO is defined by its *state*, which is the full description of the current positions and control interfaces for the robot. See the **packet format** section for more information.

## Startup Procedure

Startup of MOJO involves first turning the switch on the control/battery sandwich on. This will cause a blue LED on the battery board light up, indicating that there is power for the microprocessors. If the coordinator board is connected to the control board, then a red LED on this board will also light and a blue led on this board will begin blinking at 1 Hz. 

If at any point the coordinator status light ceases to blink, the board is frozen and should be restarted. Freezing can occur when the I2C bus is locked. 

The bridge board can be independently connected to the laptop. While it is connected, this board will blink its LED at 2 Hz.

## Communication Interface
The bridge board will appear as a COM port on the computer. The baudrate of this port is 115200 and the format is 8N1 with no flow control.

## Packet Communication

The bridge is currently configured to receive raw data packets. The format of these packets is described more fully in the `comms.md` document, but a summary is listed below. 

### Command Packet

A data packet consists of a two-byte header, followed by the payload information. The first byte in the header is the destination of the packet, and the second byte in the header is the command.

#### Destination Byte: 
the options for the destinations are as follows:
	* `w`: coordinator
	* `!`: bridge

####Command Byte: 
The command byte can be only `s` right now, which means the entire robot state is to be written. This is to maintain one-to-one state parity between the computer and robot.

The format for the coordinator write state packet is as follows:

`[header][5 motorval bytes][1 config byte]`
 * *code:* currently, 'ws' is the only option. (Stands for write state)
 * *motorval bytes:* each byte is in order of  

    ``` 
    [Top Outer | Top Inner | Bot Outer | Bot Inner | Hip]  
     (A handy figure)  
         TOP OUTER  
         TOP INNER  
            HIP  
         BOT INNER  
         BOT OUTER  
     (NB: The top set of motors is the one 
        closest to the functioning coordinator)
    ```

 * *config byte:* The byte is the union of 3 booleans: top power state, bot power state, and stream state (room for 5 more!) Currently the register config is:
    `00000 | Top Power | Bot Power | Stream State`

### Response Packet

When a command packet is sent to MOJO, the coordinator will respond with a packet detailing the current robot state, including the on-board sensor values. The format of this response packet is as follows:

`[5 motorval bytes][10 current bytes][10 voltage bytes][4 soc bytes]`  
    * The format of the motorval bytes section are identical to the format for the write state packet
    * current bytes: 16-bit (2 byte) values associated with each servo's current consumption. The order of these 16-bit chomps is the same as the motor vals, and the format is MSB. The LSB corresponds to 0.16 mA.  
    * voltage bytes: 16-bit (2 byte) values associated with the supply voltage to the servo. The order is the same as the current bytes and is MSB. the LSB corresponds to 1 mV. 
    * SoC bytes: the SoC bytes correspond to the capacity of the batteries. The first chomp refers to the top servoset battery, the second chomp refers to the bottom servoset battery. The LSB of the chomp is 1/256%. 

## Locomotion

Locomotion through the lattice occurs by moving MOJO through a set of 6 discrete positions. These positions can be described as the outer product of two orthogonal state vectors: `{retracted, extended}` and `{outer, middle, inner}`.

    ``` 
    |        | Retracted | Extended  |
    | Outer  | [ 40,184] | [ 40, 80] |
    | Middle | [ 40,200] | [ 80,178] |
    | Inner  | [ 40,220] | [190,220] |
    HIP HIP HIP
    ```

The `retracted` state refers to when the arms are not in contact with the lattice, the `extended` state refers to when the arms are gripping the lattice.

The `outer` state refers to when the arms are furthest from the hip. This state corresponds to when the robot is in the middle of a cuboctahedron.

The `inner` state refers to when the arms are closest to the hip. This state correspond to when the robot is passing off from one set of arms to the other during a climb.

The `middle` state refers to when the arms are halfway between the `inner` and `outer` states. This state is mainly for debug right now.

By switching between these locomotion states for the top and bottom, it is possible to perform a climb. 



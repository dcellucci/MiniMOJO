# MiniMOJO DEBUG Communication Architecture: 

 1. **4 levels:** Computer, Bridge, Coordinator, Motor Controller
	1. **Computer -> Bridge:**
		* **Format**: JSON Packets
		* **Update Freq:** TBD
		* **Keys:**
			* **motorvals:** contains 5 ints 0-255
			* **power:** flags for turning power on for top/bottom motors
			* **updating:** flag for turning on the automatic feed from the robot to the computer (for data acquisition purposes)
	2. **Bridge -> Computer:**
		* **Format:** JSON Packets
		* **Update Freq:**
		* **Keys:**
			* *vshunts:* array of 5 voltage drops across shunt resistors
			* *vbuses:* array of 5 voltages at the bus
			* *battery SoC:* percentage of power in battery
			* *motorvals:*
			* *IMU Vals:*
	3.	**Bridge -> Coordinator:**
		*	**Format:** bytestrings
		* 	**Update Freq:** 1 Hz, or when new packet is received
		*	`[code][5 motorval bytes][1 config byte]`
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

			* **config byte:** The byte is the union of 3 booleans: top power state, bot power state, and stream state (room for 5 more!) Currently the register config is  
				`00000 | Top Power | Bot Power | Stream State`
	4. **Coordinator -> Bridge:**
		* **Format:** bytestrings
		* **Update Freq:**
		* `[code][9 current bytes][2 bytes battery]`	
			* only code is 'us' (update state)
			* current bytes: Are actually 6 12-bit values that are glommed together. The bus and shunt registers for a given channel share a middle byte which holds their LSB values. So for example  
			`[MS VShunt Byte][LS VShunt Nibble | LS VBus Nibble][MS Vbus Byte]`

# Network Latency Tests

In order to quantify the relative speed of the system, I attempted to quantify the latency of each of the hops in the network, and the time required to do serial/deserialization of the input strings.





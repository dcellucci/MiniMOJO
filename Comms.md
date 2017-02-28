# MiniMOJO DEBUG Communication Architecture: 

 1. 4 levels: Computer, Bridge, Coordinator, Motor Controller
	1. 	Computer -> Bridge:
		* 	JSON Packets
		*	Update Freq:
		* 	Keys:
			> 	motorvals: contains 5 ints 0-255
			>	power: flags for turning power on for top/bottom motors
			> 
		Bridge -> Computer:
		*	JSON Packets
		*	Update Freq:
		*	Keys:
			>	vshunts: array of 5 voltage drops across shunt resistors
			>	vbuses: array of 5 voltages at the bus
			>	battery SoC: percentage of power in battery
			>	motorvals: 
			>	IMU Vals:
	2.	Bridge -> Coordinator:
		*	bytestrings
		* 	Update Freq: 1 Hz, or when new packet is received
		*	[code][5 motorval bytes][1 config byte]
			> 	code: currently, 'ws' is the only option. (Stands for write state)
			>	motorval bytes: each byte is in order of
				1)	top outer motor
				2)	top inner motor
				3)	bot outer motor
				4) 	bot inner motor
				5)	hip motor
					(A handy figure)
						TOP OUTER
						TOP INNER
						   HIP
						BOT INNER
						BOT OUTER
			>	The top set of motors is the one closest to the functioning coordinator
			>	config byte: The byte is the union of 3 booleans: top power state, bot power state, and stream state (room for 5 more!) Currently the register config is
				(5 Empty Bits) Top Power | Bot Power | Stream State
	3.	Coordinator -> Bridge:
		*	bytestrings
		*	[code][9 current bytes][2 bytes battery]	
			>	only code is 'us' (update state)
			> 	current bytes: Are actually 6 12-bit values that are glommed together. The bus and shunt registers for a given channel share a middle byte which holds their LSB values. So for example. 






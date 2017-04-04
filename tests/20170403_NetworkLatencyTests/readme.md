# Network Latency Tests

This set of tests is designed to ascertain the maximum data bandwidth/latency of the system as configured. The setup of the system is as follows:

    WSS Client
    <-{WSS}->
    WSS Host
    <-{112500 baud USB Serial}->
    USB Bridge
    <-{802.15.4 Wireless Connection}->
    MOJO Coordinator
    <-{I2C 400 kHz}->
    MOJO Servo Controller

The variables we have to play with are:
 * WSS Host update period (given in milliseconds)
 * USB Bridge send period (ms as well)
 * 

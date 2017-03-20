import serial
import time
import multiprocessing

## Change this to match your local settings
SERIAL_PORT = '/dev/ttyACM0'
SERIAL_BAUDRATE = 115200

class SerialProcess(multiprocessing.Process):
 
    def __init__(self, input_queue, output_queue):
        multiprocessing.Process.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial()
        self.sp.port = SERIAL_PORT
        self.sp.baudrate = SERIAL_BAUDRATE
        self.portOpen = False
 
    def open(self):
        self.portOpen = True
        self.sp.open()

    def close(self):
        self.portOpen = False
        self.sp.close()
 
    def writeSerial(self, data):
        self.sp.write(data)
        # time.sleep(1)
        
    def readSerial(self):
        return self.sp.readline().replace("\n", "")
    
    def set(self,port,baudrate):
        self.sp.port = port
        self.sp.baudrate = baudrate

    def run(self):
        if self.portOpen:
            self.sp.flushInput()
            
            while True:
                # look for incoming tornado request
                if not self.input_queue.empty():
                    data = self.input_queue.get()
                    # send it to the serial device
                    self.writeSerial(data)
                    print "writing to serial: " + data
     
                # look for incoming serial data
                if (self.sp.inWaiting() > 0):
                    data = self.readSerial()
                    print "reading from serial: " + data
                    # send it back to tornado
                    self.output_queue.put(data)

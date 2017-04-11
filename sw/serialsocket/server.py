import tornado.httpserver
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.gen
from tornado.options import define, options
import os
import time
import multiprocessing
import json
import json.encoder
import serialworker
import serial
import serial.tools.list_ports
import binascii
import struct


##
## Just a default port
##
define("port", default=8080, help="run on the given port", type=int)

##
## Maintain an array of currently connected clients
##
clients = []

##
## Two queues, one for input and one for output messages
##
input_queue = multiprocessing.Queue()
output_queue = multiprocessing.Queue()

##
## Starting a serial process with the two queues
##
sp = serialworker.SerialProcess(input_queue, output_queue)

##Debug flag
verbose = True

##
## DATA PACKET FORMAT
##
dataformat  = "="       # Native-endian, standard size
dataformat += "LL"      # 2 4-byte timestamps
dataformat += "BBBBB"   # 5 1-byte motor values
dataformat += "B"       # 1 1-byte status byte
dataformat += "HHHHH"   # 5 2-byte shunt voltages
dataformat += "HHHHH"   # 5 2-byte bus voltages
dataformat += "HH"      # 2 2-byte battery states
dataformat += "HHH"     # 3 2-byte gyro values
dataformat += "HHH"     # 3 2-byte accelerometer values
dataformat += "HHH"     # 3 2-byte magnetometer values


class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')

class StaticFileHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('main.js')

class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True

    def open(self):
        print 'new connection'
        clients.append(self)
        self.write_message("connected")

    def on_message(self, message):
        print 'tornado received from client: %s' % json.dumps(message)
        #self.write_message('ack')
        self.parseMessage(message)

    def on_close(self):
        print 'connection closed'
        clients.remove(self)

    def parseMessage(self, message):
        try:
            jsonmess = json.loads(message)
            if 'command' in jsonmess:
                #
                # scan open ports
                #
                if(jsonmess['command'] == "scan"):
                    portlist = serial.tools.list_ports.comports()
                    devicelist = []
                    for port in portlist:
                        devicelist.append(port.device)
                    out = {}
                    out['SerialPorts'] = devicelist
                    self.write_message(json.dumps(out))
                #
                # open a port
                #
                if(jsonmess['command'] == "open"):
                    sp.set(jsonmess['port'],jsonmess['baudrate'])
                    if(not sp.sp.is_open):
                        print 'opening port'
                        sp.open()
                    else:
                        print 'port already open. closing then reopening.'
                        sp.close()
                        sp.open()
                    if(not sp.is_alive()):
                        sp.start()
                #
                # close serial port (doesn't work though...)
                #
                if(jsonmess['command'] == "close"):
                    if(sp.sp.is_open):
                        print 'closing port'
                        sp.close()
                    else:
                        print 'port was not open'
                #
                # send a state packet
                #
                if(jsonmess['command'] == "state"):
                    command = bytearray([119,115]) #corresponds to ws
                    motvals = bytearray(jsonmess['motorvals'])
                    p = jsonmess['power']+[jsonmess['sync']]
                    status = bytearray([sum(v<<i for i, v in enumerate(p[::-1]))])
                    outarray = command+motvals+status+bytearray([59])
                    if verbose:
                        print "sending message:"
                        print list(outarray)
                    input_queue.put(outarray)
                #
                # Echo packet
                #
                if(jsonmess['command'] == "echo"):
                    #send the message: e;
                    outarray = bytearray([ord('e'),59])
                    if verbose:
                        print "sending message: %s" % outarray
                    input_queue.put(outarray)
            else:
                print "no command here"
        except ValueError:
            print "%s wasn't a json string you sent." % message



##
## check the queue for pending messages, and relay that to all connected clients
##
def checkQueue():
    if not output_queue.empty():
        message = output_queue.get()
        outmess = parseOutputMessage(message)
        if "command" in outmess:
            if verbose:
                print "Sending JSON: %s" % json.dumps(outmess)
            for c in clients:
                 c.write_message(json.dumps(outmess))
        elif verbose:
            print "Did not receive a valid JSON packet"

##
## Read the raw message in the queue, decide what to do with it.
##
def parseOutputMessage(message):
    if verbose:
        print "Received Message: %s" % message
    output = {}

    if message[0] == 'p':
        output["command"] = "ping"
        output["count"] = int(message[1:])
    elif message[0] == 's':
        output["command"] = "state"
        output["motorvals"] = [message[i+1] for i in range(5)]
    elif message[0] == 'l':
        print "Network latency: %s" % message[1:]
    elif message[0] == 'd':
        if verbose:
            print "it's a data packet."
            print "length: %d" % len(message)
        if (len(message) == 57):
            output["command"] = "data"
            unpkvals = struct.unpack(dataformat,message[1:])
            output["timestamps"] = unpkvals[0:2]
            output["motorvals"] = unpkvals[2:7]
            statusbools = [bool(int(i)) for i in bin(unpkvals[7])[2:].zfill(8)]
            output["power"] = statusbools[-3:-1]
            output["sync"] = statusbools[-1]
            output["vshunts"] = unpkvals[8:13]
            output["vbuses"] = unpkvals[13:18]
            output["socs"] = unpkvals[18:20]
            output["imu"] = {
                "gyro" : {
                    "x" : unpkvals[20],
                    "y" : unpkvals[21],
                    "z" : unpkvals[22]
                },
                "acc" : {
                    "x" : unpkvals[23],
                    "y" : unpkvals[24],
                    "z" : unpkvals[25]
                },
                "mag" : {
                    "x" : unpkvals[26],
                    "y" : unpkvals[27],
                    "z" : unpkvals[28]
                }
            }
    else:
        print "Could not parse the command"

    return output

##
##
##
if __name__ == '__main__':
    sp.daemon = True
    ## start the serial worker in background (as a daemon)
    tornado.options.parse_command_line()
    app = tornado.web.Application(
        handlers=[
            (r"/", IndexHandler),
            (r"/static/(.*)", tornado.web.StaticFileHandler, {'path':  './'}),
            (r"/ws", WebSocketHandler)
        ]
    )
    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)
    print "Listening on port:", options.port

    mainLoop = tornado.ioloop.IOLoop.instance()
    ## adjust the scheduler_interval according to the frames sent by the serial port
    scheduler_interval = 10 #milliseconds
    scheduler = tornado.ioloop.PeriodicCallback(checkQueue, scheduler_interval, io_loop = mainLoop)
    scheduler.start()
    mainLoop.start()

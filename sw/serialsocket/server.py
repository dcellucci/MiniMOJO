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

 
define("port", default=8080, help="run on the given port", type=int)
 
clients = [] 

input_queue = multiprocessing.Queue()
output_queue = multiprocessing.Queue()

sp = serialworker.SerialProcess(input_queue, output_queue)

 
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
				if(jsonmess['command'] == "scan"):
					portlist = serial.tools.list_ports.comports()
					devicelist = []
					for port in portlist:
						devicelist.append(port.device)
					out = {}
					out['SerialPorts'] = devicelist
					self.write_message(json.dumps(out))
				if(jsonmess['command'] == "open"):
					print 'opening port'
					sp.set(jsonmess['port'],jsonmess['baudrate'])
					sp.open()
					sp.start()
				if(jsonmess['command'] == "close"):
					sp.close()
				if(jsonmess['command'] == "data"):
					print "received data"
					command = bytearray([119,115]) #corresponds to ws
					motvals = bytearray(jsonmess['motorvals'])
					p = jsonmess['power']+[jsonmess['sync']]
					status = bytearray([sum(v<<i for i, v in enumerate(p[::-1]))])
					outarray = command+motvals+status+bytearray([59])
					print "sending message: %s" % outarray
					input_queue.put(outarray)
			else:
				print "no command here"
		except ValueError:
			print "%s wasn't a json string you sent." % message




## check the queue for pending messages, and relay that to all connected clients
def checkQueue():
	if not output_queue.empty():
		message = output_queue.get()
		readOutputMessage(message)
		jsonout = {}
		jsonout["command"] = "packet"
		jsonout["motorvals"] = []
		for i in range(5):
			jsonout["motorvals"].append(message[i])
		for c in clients:
 			c.write_message(json.dumps(jsonout))

def readOutputMessage(message):
	print message


if __name__ == '__main__':
	sp.daemon = True
	## start the serial worker in background (as a deamon)
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
	scheduler_interval = 100
	scheduler = tornado.ioloop.PeriodicCallback(checkQueue, scheduler_interval, io_loop = mainLoop)
	scheduler.start()
	mainLoop.start()
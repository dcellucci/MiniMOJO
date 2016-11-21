//
// serialserver.js
//    WebSocket serial server
//
// Daniel Cellucci modding Neil Gershenfeld's code
// NASA Ames Research Center 
//
// This work may be reproduced, modified, distributed, performed, and 
// displayed for any purpose, but must acknowledge the mods
// project. Copyright is retained and must be preserved. The work is 
// provided as is; no warranty is provided, and users accept all 
// liability.
//
// check command line
//
if (process.argv.length < 4) {
   console.log("command line: node serialserver.js client_address server_port")
   process.exit(-1)
   }
//
// start server
//
var client_address = process.argv[2]
var server_port = process.argv[3]
console.log("listening for connection from client address "+client_address+" on server port "+server_port)
var fs = require("fs")

var WebSocketServer = require('ws').Server
var SerialPort = require("serialport");

wss = new WebSocketServer({port:server_port})
var connections = new Array;

// list serial ports:
SerialPort.list(function (err, ports) {
  ports.forEach(function(port) {
    console.log(port.comName);
  });
});

var port 

/* 
 *PORT FUNCTIONS
 */
// Let us know when a port opens
function showPortOpen() {
   console.log('port open. Data rate: ' + port.options.baudRate);
}
//Send the data back to fabmods
function sendSerialData(data) {
   console.log('Received Data: '+ data);
   broadcast(data)
}
//Yea
function broadcast(data){
   for(myConnection in connections){
      connections[myConnection].send(data);
   }
}
// Let us know when the port closes
function showPortClose() {
   console.log('port closed.');
}
//Tell us when an error happens
function showError(error) {
   console.log('Serial port error: ' + error);
}


/*
 *WEB SOCKET STUFF
 */

//
// handle connection
//
wss.on('connection', handleConnection);

function handleConnection(ws) {
   //
   // check address
   //
   if (ws._socket.remoteAddress != client_address) {
      console.log("connection rejected from "+ws._socket.remoteAddress)
      ws.send('socket closed')
      ws.close()
      }
   else {
      console.log("connection accepted from "+ws._socket.remoteAddress)
      connections.push(ws); // add this client to the connections array
      }
   //
   // handle messages
   //

   var cancel
   ws.on("message", handleMessage);

   ws.on('close', function() { // when a client closes its connection
   console.log("connection closed"); // print it out
   var position = connections.indexOf(client); // get the client's position in the array
   connections.splice(position, 1); // and delete it from the array
   });


}

function handleMessage(msg){
   //
   // cancel job
   //
   if (msg == 'cancel') {
      cancel = true
      }
   //
   // start job
   //
   else {
      var count = 0
      var file
      var job = JSON.parse(msg)
      if(job.type == 'scan'){
            console.log('Scanning...')
            //console.log(fs.readdir('/dev/'))
         }
      else if(job.type == 'open'){
         port = new SerialPort('/dev/'+job.device, { 
            parser: SerialPort.parsers.readline('\n')
         });
         port.on('open', showPortOpen);
         port.on('data', sendSerialData);
         port.on('close', showPortClose);
         port.on('error', showError);
      }
      else if(job.type == 'string'){
         port.write(''+job.string)
      }
   }
}

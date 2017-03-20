//
// serial server module
//
// Neil Gershenfeld 
// (c) Massachusetts Institute of Technology 2016
// 
// This work may be reproduced, modified, distributed, performed, and 
// displayed for any purpose, but must acknowledge the mods
// project. Copyright is retained and must be preserved. The work is 
// provided as is; no warranty is provided, and users accept all 
// liability.
//
// closure
//
(function(){
//
// module globals
//
var mod = {}
//
// name
//
var name = 'WebSocket serial'
//
// initialization
//
var init = function() {
   mod.address.value = '127.0.0.1'
   mod.port.value = 8080
   mod.device = ''
   mod.baud.value = 115200
   mod.flow_rtscts.checked = false
   mod.socket = null
   }
//
// inputs
//
var inputs = {
   transmit:{type:'string',
      event:function(evt){
         serial_send_string(evt.detail)
         }
      }
   }
//
// outputs
//
var outputs = {
   receive:{type:'string',
      event:function(str){
         mods.output(mod,'receive',str)
         }
      }
   }
//

//
// interface
//
var interface = function(div){
   mod.div = div
   //
   // server
   //
   div.appendChild(document.createTextNode('serialserver:'))
   div.appendChild(document.createElement('br'))
   //
   // open/close
   //
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('open'))
      btn.addEventListener('click',function() {
         socket_open()
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('close'))
      btn.addEventListener('click',function() {
         socket_close()
         })
      div.appendChild(btn)
   div.appendChild(document.createElement('br'))
   //
   // address
   //
   div.appendChild(document.createTextNode('address: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 10
      div.appendChild(input)
      mod.address = input
   div.appendChild(document.createElement('br'))
   //
   // port
   //
   div.appendChild(document.createTextNode('\u00a0\u00a0\u00a0\u00a0\u00a0port: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 10
      div.appendChild(input)
      mod.port = input
   div.appendChild(document.createElement('br'))
   //
   // status
   //
   div.appendChild(document.createTextNode('\u00a0\u00a0status: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 10
      div.appendChild(input)
      mod.status = input
   div.appendChild(document.createElement('br'))
   //
   // serial
   //
   div.appendChild(document.createTextNode('serial device:'))
   div.appendChild(document.createElement('br'))
   //
   // open/close
   //
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('scan'))
      btn.addEventListener('click',function() {
         serial_scan()
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('open'))
      btn.addEventListener('click',function() {
         serial_open()
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('close'))
      btn.addEventListener('click',function() {
         serial_close()
         })
      div.appendChild(btn)
   div.appendChild(document.createElement('br'))
   //
   // device
   //
   div.appendChild(document.createTextNode('/dev/'))
   mod.device = ""
   var sel = document.createElement('select')
   sel.style.padding = mods.ui.padding
   sel.addEventListener(('change'),function(evt){
      mod.device = evt.target.value
      })
   sel.style.width = "100px"
   mod.portlist = sel
   div.appendChild(mod.portlist)
   div.appendChild(document.createElement('br'))   
   //
   // baud rate
   //
   div.appendChild(document.createTextNode('baud rate:'))
   div.appendChild(document.createElement('br'))   
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 10
      div.appendChild(input)
      mod.baud = input
   div.appendChild(document.createElement('br'))   
   //
   // flow control
   //
   div.appendChild(document.createTextNode('flow control:'))
   div.appendChild(document.createElement('br'))   
   var input = document.createElement('input')
      input.type = 'radio'
      input.name = mod.div.id+'flow'
      input.id = mod.div.id+'flow_none'
      div.appendChild(input)
      mod.flow_none = input
   div.appendChild(document.createTextNode('none\u00A0\u00A0\u00A0\u00A0'))
   div.appendChild(document.createElement('br'))   
   var input = document.createElement('input')
      input.type = 'radio'
      input.name = mod.div.id+'flow'
      input.id = mod.div.id+'flow_rtscts'
      div.appendChild(input)
      mod.flow_rtscts = input
   div.appendChild(document.createTextNode('RTSCTS'))
   div.appendChild(document.createElement('br'))   
   var input = document.createElement('input')
      input.type = 'radio'
      input.name = mod.div.id+'flow'
      input.id = mod.div.id+'flow_dsrdtr'
      div.appendChild(input)
      input.disabled = true
      mod.flow_dsrdtr = input
   div.appendChild(document.createTextNode('DSRDTR'))
   div.appendChild(document.createElement('br'))  
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      var span = document.createElement('span')
         var text = document.createTextNode('scan')
            mod.label = text
            span.appendChild(text)
         mod.labelspan = span
      btn.appendChild(span)
      btn.addEventListener('click',function(){
         serial_scan();
      })
   div.appendChild(btn)
   div.appendChild(document.createElement('br'))  
   //
   // file button
   //
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      var span = document.createElement('span')
         var text = document.createTextNode('waiting for file')
            mod.label = text
            span.appendChild(text)
         mod.labelspan = span
         btn.appendChild(span)
      btn.addEventListener('click',function(){
         if (mod.socket == null) {
            mod.status.value = "can't send, not open"
            }
         else if (mod.label.nodeValue == 'send file') {
            socket_send(JSON.stringify(mod.job))
            mod.label.nodeValue = 'cancel'
            }
         else if (mod.label.nodeValue == 'cancel') {
            mod.command = {}
            mod.command.type = 'cancel'
            socket_send(JSON.stringify(mod.command))
            }
         })
      div.appendChild(btn)
   }
//
// local functions
//

function parseMessage(event){
   //console.log("Received these data: ")
   console.log(event)
   if("data" in event){
        var message = {}
        try {
            message = JSON.parse(event.data)
            }
        catch(e){
            console.log("JSON Parse of event data failed.")
            }
        if("SerialPorts" in message){
            populate_portlist(message.SerialPorts)
            }
        if(!("Cmd" in message)){
            if("D" in message){
                outputs.receive.event(message.D)
            }
        }
      }
   }

function socket_open() {
   var url = "ws://"+mod.address.value+':'+mod.port.value+'/ws'
   mod.socket = new WebSocket(url)
   mod.socket.onopen = function(event) {
      mod.status.value = "socket opened"
      //serial_open()
      }
   mod.socket.onerror = function(event) {
      mod.status.value = "cannot open"
      console.log(event)
      mod.socket = null
      }
   mod.socket.onmessage = function(event){
      parseMessage(event)
      }
   mod.socket.onclose = function(event) {
      delete mod.socket;
      }
}

function socket_close() {
   mod.socket.close()
   mod.status.value = "socket closed"
   }


/* 
function socket_send(msg) {
   if (mod.socket != null) {
      mod.status.value = "send"
      mod.socket.send(msg)
      }
   else {
      mod.status.value = "can't send, not open"
      }
   }
*/
function serial_open() {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
       var msg = 'open '
       msg = msg + mod.device + ' ' + mod.baud.value
      mod.socket.send(msg)
      }
   }
function serial_close() {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      var msg = 'close '+ mod.device
      mod.socket.send(msg)
      }
   }
function serial_send_string(str) {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
       var msg = 'sendnobuf '
       msg = msg + mod.device
       msg = msg + " " + str
       mod.socket.send(msg)
      }
   }
   
function serial_scan(){
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      message = {}
      message.command = "scan"
      mod.socket.send(JSON.stringify(message))
   }
}

function populate_portlist(portlist){
   mod.portlist.options.length = 0
   for(port in portlist){
      var opt = document.createElement('option')
         opt.value = portlist[port].Name
         opt.text = portlist[port].Name.substring(5)
      mod.portlist.add(opt)   
   }
}
//
// return values
//
return ({
   name:name,
   init:init,
   inputs:inputs,
   outputs:outputs,
   interface:interface
   })
}())

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
   mod.port.value = 1234
   mod.device = ''
   mod.baud.value = 9600
   mod.flow_rtscts.checked = false
   mod.socket = null
   socket_open()
   }
//
// inputs
//
var inputs = {
   transmit:{type:'string',
      event:function(evt){
         serial_send_string(evt.detail)
         }
      },
   file:{type:'object',
      event:function(evt){
         if (evt.detail.type == 'command') {
            mod.command = evt.detail
            socket_send(JSON.stringify(mod.command))
            }
         else if (evt.detail.type == 'file') {
            mod.job = evt.detail
            mod.job.type = 'file'
            mod.label.nodeValue = 'send file'
            mod.labelspan.style.fontWeight = 'bold'
            }
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
   console.log(event)
   if(typeof event.data !== 'undefined'){
      var message = JSON.parse(event.data)
      switch(message.type){
         case 'data':
            mod.status.value = 'passing data'
            outputs.receive.event(message.data)
            break;
         case 'ports':
            mod.status.value = 'populating portlist...'
            populate_portlist(message.data)
            break;
         default:
            mod.status.value = message.data
            mod.label.nodeValue = 'waiting for file'
            mod.labelspan.style.fontWeight = 'normal'
      }
   }
}

function socket_open() {
   var url = "ws://"+mod.address.value+':'+mod.port.value
   mod.socket = new WebSocket(url)
   mod.socket.onopen = function(event) {
      mod.status.value = "socket opened"
      //serial_open()
      }
   mod.socket.onerror = function(event) {
      mod.status.value = "cannot open"
      mod.socket = null
      }
   mod.socket.onmessage = function(event){
      parseMessage(event)
      }
}


function socket_close() {
   var msh = {}
   msg.type = 'close'
   mod.socket.send(JSON.stringify(msg))
   mod.socket.close()
   mod.status.value = "socket closed"
   mod.socket = null
   }
function socket_send(msg) {
   if (mod.socket != null) {
      mod.status.value = "send"
      mod.socket.send(msg)
      }
   else {
      mod.status.value = "can't send, not open"
      }
   }
function serial_open() {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      var msg = {}
      msg.type = 'open'
      msg.port = mod.device
      msg.baud = parseInt(mod.baud.value)
      msg.contents = "start"
      if (mod.flow_none.checked)
         msg.flow = 'none'
      else if (mod.flow_rtscts.checked)
         msg.flow = 'rtscts'
      else if (mod.flow_dsrdtr.checked)
         msg.flow = 'dsrdtr'
      mod.socket.send(JSON.stringify(msg))
      }
   }
function serial_close() {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      var msg = {}
      msg.type = 'close'
      msg.device = mod.device
      mod.socket.send(JSON.stringify(msg))
      }
   }
function serial_send_string(str) {
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      var msg = {}
      msg.type = 'send'
      msg.string = str
      mod.socket.send(JSON.stringify(msg))
      mod.status.value = 'transmitting'
      }
   }
function serial_scan(){
   if (mod.socket == null) {
      mod.status.value = "socket not open"
      }
   else {
      var msg = {}
      msg.type = 'scan'
      mod.socket.send(JSON.stringify(msg))
      mod.status.value = 'transmit'
   }
}
function populate_portlist(portlist){
   mod.portlist.options.length = 0
   for(port in portlist){
      var opt = document.createElement('option')
         opt.value = portlist[port]
         opt.text = portlist[port].substring(5)
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

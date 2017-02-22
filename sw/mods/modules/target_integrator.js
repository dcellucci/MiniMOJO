//
// step and delay
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
var name = 'Target Integrator'
//
// initialization
//
var init = function() {
   mod.value.value = ''
   mod.start.value = 0
   mod.stop.value = 10
   mod.step.value = 1
   mod.delay.value = 0.1
   mod.timeout == ''
   mod.curmotvals = [74,184,74,184,185]
   mod.tarmotvals = [74,184,74,184,185]
   mod.moving = false
   }
//
// inputs
//
var inputs = {
   start:{type:'string',
      event:function(evt) {
         mod.tarvalstring = evt.detail 
         parseMotString()
         //outputs.motstring.event()
         mod.value.value = mod.tarmotvals
         update_value()
         }
      }
   }
//
// outputs
//
var outputs = {
   motstring:{type:'string',
      event:function() {
         encodeMotorString()
         mods.output(mod,'motstring',mod.outvalue)
         }
      }
   }
//
// interface
//
var interface = function(div) {
   mod.div = div
   div.appendChild(document.createTextNode('value: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.value = input
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('start: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.start = input
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('stop: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.stop = input
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('step: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.step = input
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('delay (s): '))
   input= document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.delay = input
   div.appendChild(document.createElement('br'))
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('start'))
      btn.addEventListener('click',function(){
         mod.nextvalue = parseFloat(mod.start.value)
         update_value()
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('stop'))
      btn.addEventListener('click',function(){
         window.clearTimeout(mod.timeout)
         mod.value.value = ''
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      var text = document.createTextNode('pause')
         mod.pause = text
         btn.appendChild(text)
      btn.addEventListener('click',function(){
         if (mod.timeout != '') {
            window.clearTimeout(mod.timeout)
            mod.pause.nodeValue = 'resume'
            mod.timeout = ''
            }
         else {
            mod.pause.nodeValue = 'pause'
            var delay = 1000*parseFloat(mod.delay.value)
            mod.timeout = window.setTimeout(update_value,delay)
            }
         })
      div.appendChild(btn)
   }
//
// local functions
//
function parseMotString(){
   for(i = 0; i < mod.tarmotvals.length; i++){
      mod.tarmotvals[i] = mod.tarvalstring.charCodeAt(i+1)
   }  
}

function encodeMotorString(){
   mod.outvalue = 'w'
   for(motorval in mod.curmotvals){
      if(mod.curmotvals[motorval] > 255){
         motbyte = String.fromCharCode(255)
      }
      else{
         motbyte = String.fromCharCode(mod.curmotvals[motorval])
      }
      mod.outvalue += motbyte
   }
   mod.value.value = mod.outvalue
}

function update_value() {
   var step  = parseFloat(mod.step.value)
   var delay = 1000*parseFloat(mod.delay.value)
   var tmpcurval = 0
   var tmptarval = 0
   mod.moving = false

   for(motindex = 0; motindex < mod.curmotvals.length; motindex++){
      tmpcurval = mod.curmotvals[motindex]
      tmptarval = mod.tarmotvals[motindex]
      mod.value.value = mod.moving

      if(tmpcurval != tmptarval){
         mod.moving = true
         if(Math.abs(tmptarval-tmpcurval)<step) {
            tmpcurval = tmptarval
         } else if(tmptarval > tmpcurval) {
            tmpcurval += step
         } else {
            tmpcurval -= step
         }
         mod.curmotvals[motindex] = tmpcurval
      }
   }
   if(mod.moving){
      outputs.motstring.event()
      mod.timeout = window.setTimeout(update_value,delay)
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

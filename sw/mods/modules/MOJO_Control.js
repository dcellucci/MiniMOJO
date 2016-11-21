//
// MOJO Motor Control
//
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
var name = 'MOJO Motor Control'
//
// initialization
//
var init = function() {
   }
//
// inputs
//
var inputs = {
   input:{type:'character',
      event:function(evt){
         var str = evt.detail
         mod.character.value = str.slice(-1)
         outputs.output.event()
         }}}
//
// outputs
//
var outputs = {
   output:{type:'string',
      event:function(){
         mods.output(mod,'output',mod.value)}}}
//
// interface
//


var interface = function(div){
   mod.motorvals = new Array
   mod.div = div
   div.appendChild(document.createTextNode('Motor 1: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 1
   div.appendChild(input)
   mod.motorvals.push(input)
   div.appendChild(document.createElement('br'))

   div.appendChild(document.createTextNode('Motor 2: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 1
   div.appendChild(input)
   mod.motorvals.push(input)
   div.appendChild(document.createElement('br'))

   div.appendChild(document.createTextNode('Motor 3: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 1
   div.appendChild(input)
   mod.motorvals.push(input)
   div.appendChild(document.createElement('br'))

   div.appendChild(document.createTextNode('Motor 4: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 1
   div.appendChild(input)
   mod.motorvals.push(input)
   div.appendChild(document.createElement('br'))
   
   div.appendChild(document.createTextNode('Motor 5: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 1
   div.appendChild(input)
   mod.motorvals.push(input)
   div.appendChild(document.createElement('br'))  

   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('send'))
      btn.addEventListener('click', parseMotVals)
   div.appendChild(btn)         
   }



//
// local functions
//
function parseMotVals(){
   mod.value = 'w'
   for(motorval in mod.motorvals){
      if(mod.motorvals[motorval].value > 255){
         motbyte = String.fromCharCode(255)
      }
      else{
         motbyte = String.fromCharCode(mod.motorvals[motorval].value)
      }
      mod.value = mod.value + motbyte
   }
   outputs.output.event()
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

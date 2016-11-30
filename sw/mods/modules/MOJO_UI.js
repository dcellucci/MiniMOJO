//
// ui button
//
// Neil Gershenfeld 
// (c) Massachusetts Institute of Technology 2015,6
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
var name = 'MOJOUI'
//
// initialization
//
var init = function() {
   openUI()
   }
//
// inputs
//
var inputs = {
   }
//
// outputs
//
var outputs = {
   output:{type:'string',
      event:function(){
         mods.output(mod,'output',mod.value)}}
}
//
// interface
//
var interface = function(div){
   mod.div = div
   //
   // view UI
   //
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('view window'))
      btn.addEventListener('click',function(){
         openUI()
         })
      div.appendChild(btn)
   }
//
// local functions
//
function openUI() {
   mod.motorvals = new Array
   var win = window.open('')
   //
   // close
   //
   var btn = document.createElement('button')
      btn.appendChild(document.createTextNode('close window'))
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.addEventListener('click',function(){
         win.close()
         })
      win.document.body.appendChild(btn)
   win.document.body.appendChild(document.createElement('br'))
   win.document.body.appendChild(document.createElement('br'))
   //
   // button 1
   //
   var btn = document.createElement('button')
      btn.appendChild(document.createTextNode('Open'))
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.addEventListener('click',openMOJO)
      win.document.body.appendChild(btn)
   win.document.body.appendChild(document.createTextNode(' '))

   var btn = document.createElement('button')
      btn.appendChild(document.createTextNode('Close'))
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.addEventListener('click',closeMOJO)
      win.document.body.appendChild(btn)
   win.document.body.appendChild(document.createTextNode(' '))
   //
   // button 2
   //
   var btn = document.createElement('button')
      btn.appendChild(document.createTextNode('send'))
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.addEventListener('click',function(){
         outputs.output.event()
         })
      win.document.body.appendChild(btn)
   win.document.body.appendChild(document.createElement('br'))
   //
   // text 1
   //
   win.document.body.appendChild(document.createTextNode('Motor 1: '))
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      input.addEventListener('input',parseMotVals)
      input.value = 128
      win.document.body.appendChild(input)
      mod.motorvals.push(input)

   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      win.document.body.appendChild(input)
      mod.outputdisplay = input
   win.document.body.appendChild(document.createElement('br'))
   //
   // text 2
   //
   win.document.body.appendChild(document.createTextNode('Motor 2: '))
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      input.addEventListener('input',parseMotVals)
      input.value = 128
      win.document.body.appendChild(input)
      mod.motorvals.push(input)
   win.document.body.appendChild(document.createElement('br'))

   //
   // text 3
   //
   win.document.body.appendChild(document.createTextNode('Motor 3: '))
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      input.addEventListener('input',parseMotVals)
      input.value = 128
      win.document.body.appendChild(input)
      mod.motorvals.push(input)
   win.document.body.appendChild(document.createElement('br'))
   //
   // text 2
   //
   win.document.body.appendChild(document.createTextNode('Motor 4: '))
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      input.addEventListener('input',parseMotVals)
      input.value = 128
      win.document.body.appendChild(input)
      mod.motorvals.push(input)
   win.document.body.appendChild(document.createElement('br'))
   //
   // text 2
   //
   win.document.body.appendChild(document.createTextNode('Motor 5: '))
   var input = document.createElement('input')
      input.type = 'text'
      input.size = 6
      input.addEventListener('input',parseMotVals)
      input.value = 128
      win.document.body.appendChild(input)
      mod.motorvals.push(input)
   }  

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
   mod.outputdisplay.value = mod.value

}

function openMOJO(){
   mod.motorvals[0].value = 74
   mod.motorvals[1].value =184
   mod.motorvals[2].value = 74
   mod.motorvals[3].value =184
   mod.motorvals[4].value =180
   parseMotVals()
   outputs.output.event()
}

function closeMOJO(){
   mod.motorvals[0].value = 30
   mod.motorvals[1].value = 60
   mod.motorvals[2].value = 30
   mod.motorvals[3].value = 60
   mod.motorvals[4].value =180
   parseMotVals()
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

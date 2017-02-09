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
var mod = {
   motorvals: new Array
}


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
      }
   }
}
//
// outputs
//
var outputs = {
   output:{type:'string',
      event:function(){
         mods.output(mod,'output',mod.value)
      }
   }
}
//
// interface
//


var interface = function(div){
   mod.div = div///*
   var openspan = document.createElement('span')
      openspan.innerHTML = 'open'
      openspan.style.fontWeight = 'normal'
      openspan.addEventListener('mouseover', function(event){
         openspan.style.fontWeight = 'bold'})
      openspan.addEventListener('mouseout', function(event){
         openspan.style.fontWeight = 'normal'})
      openspan.addEventListener('mousedown', openMOJO)
   div.appendChild(openspan)
   var closespan = document.createElement('span')
      closespan.innerHTML = ' close'
      closespan.style.fontWeight = 'normal'
      closespan.addEventListener('mouseover', function(event){
         closespan.style.fontWeight = 'bold'})
      closespan.addEventListener('mouseout', function(event){
         closespan.style.fontWeight = 'normal'})
      closespan.addEventListener('mousedown', closeMOJO)
   div.appendChild(closespan)
   div.appendChild(document.createElement('br'))
//*/
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

   function openMOJO(){
      mod.motorvals[0].value = 74
      mod.motorvals[1].value =184
      mod.motorvals[2].value = 74
      mod.motorvals[3].value =184
      mod.motorvals[4].value =180
      parseMotVals()
      //outputs.output.event()
   }

   function closeMOJO(){
      mod.motorvals[0].value = 30
      mod.motorvals[1].value = 60
      mod.motorvals[2].value = 30
      mod.motorvals[3].value = 60
      mod.motorvals[4].value =180
      parseMotVals()
      //outputs.output.event()
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

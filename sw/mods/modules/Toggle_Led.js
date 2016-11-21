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
mod.bridge_led_col = 'red'
mod.coord_led_col = 'red'
//
// name
//
var name = 'Toggle Led'
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
   mod.div = div
   div.appendChild(document.createTextNode('Bridge LED'))
   div.appendChild(document.createElement('br'))
   input = document.createElement('div')
      input.style = 'background:'+mod.coord_led_col+'; border-radius: 25px; width: 50px; height: 50px;'
   div.appendChild(input)
   div.appendChild(document.createElement('br'))   

   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('Toggle Bridge. LED'))
      btn.addEventListener('click', sendBridgeToggle)
   div.appendChild(btn)         

   div.appendChild(document.createTextNode('Coord. LED'))   
   div.appendChild(document.createElement('br'))

   input = document.createElement('div')
      input.style = 'background:'+mod.bridge_led_col+'; border-radius: 25px; width: 50px; height: 50px;'
    div.appendChild(input)
   div.appendChild(document.createElement('br'))

   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('Toggle Coord. LED'))
      btn.addEventListener('click', sendCoordToggle)
   div.appendChild(btn)         
   }



//
// local functions
//
function sendBridgeToggle(){
   if(mod.bridge_led_col == 'black'){
      mod.bridge_led_col = 'red'
   }
   else{
      mod.bridge_led_col = 'black'
   }

   mod.value = 'l'
   outputs.output.event()
}
function sendCoordToggle(){
   if(mod.coord_led_col == 'black'){
      mod.coord_led_col = 'red'
   }
   else{
      mod.coord_led_col = 'black'
   }
   mod.value = '!l'
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

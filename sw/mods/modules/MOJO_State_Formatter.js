//
// string variables
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
var name = 'MOJO State Formatter'
//
// initialization
//
var init = function() {
   mod.state = {}
   mod.outputstring = ""
   }
//
// inputs
//
var inputs = {
   state:{type:'string',
      event:function(evt){
         parseStateJSON(evt.detail)
         outputs.outputstring.event()
         }
      }
   }
//
// outputs
//
var outputs = {
   outputstring:{type:'string',
      event:function(){
         mods.output(mod,'outputstring',mod.outputstring)}}}
//
// interface
//
var interface = function(div){
   mod.div = div
   div.appendChild(document.createTextNode('var1: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 20
      input.addEventListener('keydown',function(evt){
         if (evt.key == 'Enter')
            parseStateJSON(input.value)
         })
      div.appendChild(input)
    mod.text1 = input

   div.appendChild(document.createTextNode(' (enter)'))
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('var2: '))
   
   input = document.createElement('input')
      mod.text2 = input
      input.type = 'text'
      input.size = 20
      input.value = mod.outputstring
      input.addEventListener('keydown',function(evt){
         if (evt.key == 'Enter')
            mod.outputstring = input.value
            outputs.outputstring.event()
         })
      div.appendChild(input)
   div.appendChild(document.createTextNode(' (enter)'))
   }
//
// local functions
//
function parseStateJSON(str){
    mod.text1.value = str
   var state = {}
   try{
      state = JSON.parse(str) 
      }
   catch(e){
      console.log("There was an error.")
      }
   mod.state = state
   
   opstr = 'wa '
   for(var i = 0; i < 5; i++){
        opstr = opstr + mod.state.motorvals[i] + " "
   }
   mod.outputstring = opstr + ";"
   mod.text2.value = mod.outputstring
   outputs.outputstring.event()
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
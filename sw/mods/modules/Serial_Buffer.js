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
var name = 'Serial Buffer'
//
// initialization
//
var init = function() {
   mod.delay.value = 0.1
   mod.timeout == ''
   mod.updating = false
   mod.outbotstate = ""
   setTimeout(update_value,1000)
   }
//
// inputs
//
var inputs = {
   start:{type:'string',
      event:function(evt) {
            mod.inbotstate = evt.detail
         }
      }
   }
//
// outputs
//
var outputs = {
   botstate:{type:'string',
      event:function() {
         mods.output(mod,'botstate',mod.outbotstate + ";")
         }
      }
   }
//
// interface
//
var interface = function(div) {
   mod.div = div
   div.appendChild(document.createTextNode('State: '))
   input = document.createElement('input')
      input.type = 'text'
      input.size = 50
      div.appendChild(input)
      mod.disp = input
   div.appendChild(document.createElement('br'))
   div.appendChild(document.createTextNode('Interval (s): '))
   input= document.createElement('input')
      input.type = 'text'
      input.size = 6
      div.appendChild(input)
      mod.delay = input
      input.addEventListener('keydown',function(evt){
         if (evt.key == 'Enter')
            outputs.botstate.event
         })
         
    inputckbox = document.createElement('input')
        inputckbox.type = 'checkbox'
        inputckbox.checked = mod.updating
        inputckbox.addEventListener('change', function(){
            mod.updating = inputckbox.checked
         if(mod.updating){
             update_value()
         }
            })
   div.appendChild(document.createElement('br'))
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('start'))
      btn.addEventListener('click',function(){
         mod.updating = true
         inputckbox.checked = true
         update_value()
         })
      div.appendChild(btn)
   var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('stop'))
      btn.addEventListener('click',function(){
         mod.updating = false
         inputckbox.checked = false;
         })
      div.appendChild(btn)
       div.appendChild(document.createElement('br'))  
      div.appendChild(document.createTextNode('Updating'))
    div.appendChild(inputckbox)
   div.appendChild(document.createElement('br'))
   }

function update_value() {
    if(mod.updating){
        timer = mod.delay.value*1000
        if(mod.outbotstate == mod.inbotstate){
          timer = 1000
        }
        else{
          mod.outbotstate = mod.inbotstate
        }
        outputs.botstate.event()
        mod.disp.value = mod.outbotstate
        setTimeout(update_value,parseFloat(mod.delay.value)*1000)
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

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
   motorvals: [0,0,0,0,0],
   motordisp: [0,0,0,0,0],
   trimvals: [5,-15,10,-10,-15],
   liveupdate: false,
   toppow: false, 
   botpow: false,
   sync: false,
   state: {},
   pos:{
      or:[0,0], //outer retracted
      oe:[0,0], //outer extended
      mr:[0,0], //mid retracted
      me:[0,0], //mid extended
      ir:[0,0], //inner retracted
      ie:[0,0]  //inner extended
   }
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
   motorVals:{
      type:'MOJOState',
      event:function(evt){
         var input = JSON.parse(evt.detail)
         for(var i = 0; i < 5; i++){
            if(parseInt(input.motorvals[i]) >= 0){
               mod.motorvals[i].value = parseInt(input.motorvals[i])
               mod.motordisp[i].value = parseInt(input.motorvals[i])
               }
            }
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
          mod.state.power = [mod.toppow,mod.botpow]
          mod.state.sync = mod.sync
        parseMotVals()
        console.log(JSON.stringify(mod.state))
         mods.output(mod,'output',JSON.stringify(mod.state))
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
    div.appendChild(document.createTextNode('Auto-Sync'))

    inputckbox = document.createElement('input')
        inputckbox.type = 'checkbox'
        inputckbox.checked = mod.liveupdate
        inputckbox.addEventListener('change', function(){
            mod.liveupdate = inputckbox.checked
            })
    div.appendChild(inputckbox)
   div.appendChild(document.createElement('br'))
//*/
    console.log(mod.motorsliders)
    
    div.appendChild(document.createTextNode('Motor 1: '))
    inputslider1 = document.createElement('input')
        inputslider1.type = 'range'
        inputslider1.min = 0
        inputslider1.max = 255
        inputslider1.step = 5
    div.appendChild(inputslider1)
    
    input1 = document.createElement('input')
        input1.type = 'text'
        input1.value = inputslider1.value
        input1.size = 1
    div.appendChild(input1)
    
    trim1 = document.createElement('input')
        trim1.type = 'text'
        trim1.value = mod.trimvals[0]
        trim1.size = 1
    div.appendChild(trim1)
        
    mod.motorvals[0] = inputslider1
    mod.motordisp[0] = input1
    mod.trimvals[0] = trim1
    
    input1.addEventListener('change', function(){
        inputslider1.value = input1.value
        if(mod.liveupdate){
            outputs.output.event()
            }
        })
    inputslider1.addEventListener('input', function(){
           input1.value = inputslider1.value  
           if(mod.liveupdate){
            outputs.output.event()
            }
            })
    div.appendChild(document.createElement('br'))


    div.appendChild(document.createTextNode('Motor 2: '))
    inputslider2 = document.createElement('input')
        inputslider2.type = 'range'
        inputslider2.min = 0
        inputslider2.max = 255
        inputslider2.step = 5
    div.appendChild(inputslider2)
    
    input2 = document.createElement('input')
        input2.type = 'text'
        input2.value = inputslider2.value
        input2.size = 1
    div.appendChild(input2)
    
    trim2 = document.createElement('input')
        trim2.type = 'text'
        trim2.value = mod.trimvals[1]
        trim2.size = 1
    div.appendChild(trim2)
    
    mod.motorvals[1] = inputslider2
    mod.motordisp[1] = input2
    mod.trimvals[1] = trim2
    
    input2.addEventListener('change', function(){
        inputslider2.value = input2.value
        if(mod.liveupdate){
            outputs.output.event()
            }
        })
        
    inputslider2.addEventListener('input', function(){
           input2.value = inputslider2.value
           if(mod.liveupdate){
            outputs.output.event()
            }
            })
            
    div.appendChild(document.createElement('br'))
    
    
    div.appendChild(document.createTextNode('Motor 3: '))
    inputslider3 = document.createElement('input')
        inputslider3.type = 'range'
        inputslider3.min = 0
        inputslider3.max = 255
        inputslider3.step = 5
    div.appendChild(inputslider3)
    
    input3 = document.createElement('input')
        input3.type = 'text'
        input3.value = inputslider3.value
        input3.size = 1
    div.appendChild(input3)
    
    trim3 = document.createElement('input')
        trim3.type = 'text'
        trim3.value = mod.trimvals[2]
        trim3.size = 1
    div.appendChild(trim3)
    
    mod.motorvals[2] = inputslider3
    mod.motordisp[2] = input3
    mod.trimvals[2] = trim3
    
    input3.addEventListener('change', function(){
        inputslider3.value = input3.value
        if(mod.liveupdate){
            outputs.output.event()
            }
        })
    inputslider3.addEventListener('input', function(){
           input3.value = inputslider3.value 
           if(mod.liveupdate){
            outputs.output.event()
            }
            })

    div.appendChild(document.createElement('br'))
    
    
    div.appendChild(document.createTextNode('Motor 4: '))
    inputslider4 = document.createElement('input')
        inputslider4.type = 'range'
        inputslider4.min = 0
        inputslider4.max = 255
        inputslider4.step = 5
    div.appendChild(inputslider4)
    
    input4 = document.createElement('input')
        input4.type = 'text'
        input4.value = inputslider4.value
        input4.size = 1
    div.appendChild(input4)
    
    trim4 = document.createElement('input')
        trim4.type = 'text'
        trim4.value = mod.trimvals[3]
        trim4.size = 1
    div.appendChild(trim4)
        
    mod.motorvals[3] = inputslider4
    mod.motordisp[3] = input4
    mod.trimvals[3] = trim4
    
    input4.addEventListener('change', function(){
        inputslider4.value = input4.value
        if(mod.liveupdate){
            outputs.output.event()
            }
        })
    inputslider4.addEventListener('input', function(){
           input4.value = inputslider4.value 
           if(mod.liveupdate){
            outputs.output.event()
            }
            })

    div.appendChild(document.createElement('br'))
    
    div.appendChild(document.createTextNode('Hip Motor: '))
    inputslider5 = document.createElement('input')
        inputslider5.type = 'range'
        inputslider5.min = 0
        inputslider5.max = 255
        inputslider5.step = 5
    div.appendChild(inputslider5)
    
    input5 = document.createElement('input')
        input5.type = 'text'
        input5.value = inputslider5.value
        input5.size = 1
    div.appendChild(input5)
    
    trim5 = document.createElement('input')
        trim5.type = 'text'
        trim5.value = mod.trimvals[4]
        trim5.size = 1
    div.appendChild(trim5)
    
    mod.motorvals[4] = inputslider5
    mod.motordisp[4] = input5
    mod.trimvals[4] = trim5
    
    input5.addEventListener('change', function(){
        inputslider5.value = input5.value
        if(mod.liveupdate){
            outputs.output.event()
            }
        })
    inputslider5.addEventListener('input', function(){
        input5.value = inputslider5.value  
        if(mod.liveupdate){
            outputs.output.event()
            }
        })

    div.appendChild(document.createElement('br'))
    
   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('send'))
      btn.addEventListener('click', 
            outputs.output.event)
   div.appendChild(btn)  

    div.appendChild(document.createElement('br'))
    
    div.appendChild(document.createTextNode('Top Motor Power: '))
    toppowckbox = document.createElement('input')
        toppowckbox.type = 'checkbox'
        toppowckbox.checked = mod.toppow
        toppowckbox.addEventListener('change', function(){
            mod.toppow = toppowckbox.checked
            outputs.output.event()
            })
    div.appendChild(toppowckbox)
   div.appendChild(document.createElement('br'))
   
   div.appendChild(document.createTextNode('Bot Motor Power: '))
    botpowckbox = document.createElement('input')
        botpowckbox.type = 'checkbox'
        botpowckbox.checked = mod.botpow
        botpowckbox.addEventListener('change', function(){
            mod.botpow = botpowckbox.checked
           outputs.output.event()
            })
    div.appendChild(botpowckbox)
   div.appendChild(document.createElement('br'))
   
   div.appendChild(document.createTextNode('Status Updates: '))
    updatesckbox = document.createElement('input')
        updatesckbox.type = 'checkbox'
        updatesckbox.checked = mod.botpow
        updatesckbox.addEventListener('change', function(){
            mod.sync = updatesckbox.checked
           outputs.output.event()
            })
    div.appendChild(updatesckbox)
   div.appendChild(document.createElement('br'))
   

    

   }



//
// local functions
//
   function togglePower(top){
       mod.value = 'w'
        if(top && mod.toppow){
            mod.value = mod.value+'+'
            //console.log('w+')
        }
        if(top && !mod.toppow){
            mod.value = mod.value+'-'
            console.log('w-')
        }
        if(!top && mod.botpow){
            mod.value = mod.value+'='
            console.log('w=')
        }
        if(!top && !mod.botpow){
            mod.value = mod.value+'_'
            console.log('w_')
        }
        
        }
        
   function parseMotVals(){
      mod.state.motorvals = [0,0,0,0,0]
      for(i = 0; i < 5; i ++){
         if(mod.motorvals[i].value > 255){
            mod.state.motorvals[i] = 255
            }
         else{
            mod.state.motorvals[i] = (parseInt(mod.motorvals[i].value)+parseInt(mod.trimvals[i].value))
            }
         
        }
   }

   function openMOJO(){
      mod.motorvals[0].value = 74
      mod.motorvals[1].value =184
      mod.motorvals[2].value = 74
      mod.motorvals[3].value =184
      mod.motorvals[4].value =180

      mod.motordisp[0].value = 74
      mod.motordisp[1].value =184
      mod.motordisp[2].value = 74
      mod.motordisp[3].value =184
      mod.motordisp[4].value =180
      parseMotVals()
      outputs.output.event()
   }

   function closeMOJO(){
      mod.motorvals[0].value = 30
      mod.motorvals[1].value = 60
      mod.motorvals[2].value = 30
      mod.motorvals[3].value = 60
      mod.motorvals[4].value =180

      mod.motordisp[0].value = 30
      mod.motordisp[1].value = 60
      mod.motordisp[2].value = 30
      mod.motordisp[3].value = 60
      mod.motordisp[4].value =180
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



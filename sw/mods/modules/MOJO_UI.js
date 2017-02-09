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
var mod = {bloop:["deep","dop"]}
//
// name
//
var name = 'MOJOUI'
//
// initialization
//
var init = function() {
   mod.data = [{
      x: ['2013-10-04 22:23:00', '2013-11-04 22:23:00', '2013-12-04 22:23:00'],
      y: [1, 3, 6],
      type: 'scatter'
   }];
   mod.layout = {
      autosize:true,
      height:500,
      margin:{
         l:20,
         r:20,
         b:50,
         t:20,
         pad:10
      }
   };
   openUI()
   }
//
// inputs
//
var inputs = {/*
   input:{type:'array',
   event:function(){
      mods}}*/
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
   var link = document.createElement('link')
      link.rel = "stylesheet"
      link.href = "https://unpkg.com/purecss@0.6.0/build/pure-min.css"
   win.document.head.appendChild(link)
   var link = document.createElement('link')
      link.rel = "stylesheet"
      link.href = "https://unpkg.com/purecss@0.6.0/build/grids-responsive-min.css"
   win.document.head.appendChild(link)

   var meta = document.createElement('meta')
      meta.name = "viewport"
      meta.setAttribute("content","width=device-width, initial-scale=1")
   win.document.head.appendChild(meta)

   var script = document.createElement('script')
      script.src = "MOJOUI_handler.js"
   win.document.head.appendChild(script)

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
   var div = document.createElement('div')
   div.setAttribute("class","pure-g")
   var divcol = document.createElement('div')
   divcol.setAttribute("class", "pure-u-1 pure-u-md-1-2")

      var subdiv = document.createElement('div')
      subdiv.setAttribute("class","pure-u-1-3")
          var btn = document.createElement('button')
             btn.appendChild(document.createTextNode('Open Motors'))
             btn.setAttribute('class','pure-button pure-button-primary')
             btn.style.padding = mods.ui.padding
             btn.style.margin = 1
             btn.addEventListener('click',openMOJO)
      subdiv.appendChild(btn)
   divcol.appendChild(subdiv)

      var subdiv = document.createElement('div')
      subdiv.setAttribute("class","pure-u-1-3")
      var btn = document.createElement('button')
         btn.appendChild(document.createTextNode('Close Motors'))
         btn.setAttribute('class','pure-button pure-button-primary')
         btn.style.padding = mods.ui.padding
         btn.style.margin = 1
         btn.addEventListener('click',closeMOJO)
      subdiv.appendChild(btn)
   divcol.appendChild(subdiv)
   //
   // button 2
   //
      var subdiv = document.createElement('div')
      subdiv.setAttribute("class","pure-u-1-3")
      var btn = document.createElement('button')
         btn.appendChild(document.createTextNode('Send Positions'))
         btn.setAttribute('class','pure-button pure-button-primary')
         btn.style.padding = mods.ui.padding
         btn.style.margin = 1
         btn.addEventListener('click',function(){
            outputs.output.event()
            })
      subdiv.appendChild(btn)
   divcol.appendChild(subdiv)
   
   //
   // text 1
   //
      var subdiv = document.createElement('div')
         subdiv.setAttribute("class","pure-u-1-2")
         subdiv.appendChild(document.createTextNode('Motor 1: '))
   divcol.appendChild(subdiv)
      var subdiv = document.createElement('div')
         subdiv.setAttribute("class","pure-u-1-2")
         var input = document.createElement('input')
            input.type = 'text'
            input.size = 6
            input.addEventListener('input',parseMotVals)
            input.value = 128
      subdiv.appendChild(input)
      mod.motorvals.push(input)
   divcol.appendChild(subdiv)
   
  
   win.document.body.appendChild(divcol)   

   var divcol = document.createElement('div')
   divcol.setAttribute("class", "pure-u-1 pure-u-md-1-2")

      var subdiv = document.createElement('div')
      subdiv.setAttribute("id","myPlot")
      subdiv.setAttribute("class","pure-u-1")
      subdiv.setAttribute("style","padding:0px; margin:0px")
   divcol.appendChild(subdiv)
   win.document.body.appendChild(divcol)
   win.document.body.setAttribute("onload","bodyLoad()")

   win.document.body.appendChild(document.createElement('br'))
   Plotly.plot(win.document.getElementById('myPlot'),mod.data,mod.layout,{staticPlot:true})
   win.onresize = function(){
      Plotly.Plots.resize(win.document.getElementById('myPlot'))
   }
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

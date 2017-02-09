//
// Chart Display
// Takes a datastream in, outputs a chart
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

var input
//
// name
//
var name = 'Chart'
//
// initialization
//
var init = function() {
    mod.data = [{
        x: ['2013-10-04 22:23:00', '2013-11-04 22:23:00', '2013-12-04 22:23:00'],
        y: [1, 3, 6],
        type: 'scatter'
    }];
    mod.layout = [{
      autosize: true,
      margin: {
        l: 0,
        r: 0,
        b: 0,
        t: 0,
        pad: 0
      }
    }];
    plot()
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
   var canvas = document.createElement('canvas')
      canvas.width = 300
      canvas.height = 0
      canvas.style.backgroundColor = 'rgb(255,255,255)'
      div.appendChild(canvas)
    var subdiv = document.createElement('div')
      subdiv.setAttribute("id","myPlot")
   div.appendChild(subdiv)
   //
}



//
// local functions
//
function plot(){
  Plotly.plot(document.getElementById('myPlot'),mod.data)  
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

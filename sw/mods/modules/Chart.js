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
var newScript = document.createElement('script')
newScript.src = "https://d3js.org/d3.v4.js"
document.body.appendChild(newScript)
//
// module globals
//
var mod = {}
var data = [4, 8, 15, 16, 23, 42];
var input
//
// name
//
//
// initialization
//
var init = function() {
   //createChart()
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
   chart = document.createElement('div')
   chart.style.cssText = "font: 10px sans-serif;background-color: steelblue;text-align: right;padding: 3px;margin: 1px;color: white;"
   chart.className = ".chart"
   div.appendChild(chart)

   div.appendChild(document.createTextNode('Bridge LED'))
   div.appendChild(document.createElement('br'))
   input = document.createElement('div')
      input.style = 'background:red; border-radius: 25px; width: 50px; height: 50px;'
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
      input.style = 'background: black; border-radius: 25px; width: 50px; height: 50px;'
    div.appendChild(input)
   div.appendChild(document.createElement('br'))

   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('Toggle Coord. LED'))
      btn.addEventListener('click', createChart)
   div.appendChild(btn)         
   }



//
// local functions
//
function sendBridgeToggle(){
   mod.value = 'l'
   outputs.output.event()
}
function sendCoordToggle(){
   mod.value = '!l'
   outputs.output.event()
}

function createChart(){
   d3.select(".chart")
     .selectAll("div")
       .data(data)
     .enter().append("div")
       .style("width", function(d) { return d + "px"; })
       .text(function(d) { return d+"\n"; });
       input.style = 'background: red; border-radius: 25px; width: 50px; height: 50px;'
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

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
var mod = {
  counter: 0,
  data:[
          ['Time', 'Arm A', 'Arm B']
       ],
  options:{
            title: 'Current Draw',
            hAxis: {title: 'Time', minValue: 0, maxValue: 15},
            vAxis: {title: 'MotorVal', minValue: 0, maxValue: 15},
            legend: 'none'
          },
    armAVals : [0,0, -0.00670299, -0.602193,211.1],
    armBVals : [0,0, 0.00682128, -1.967,173.629]
}

var input
//
// name
//
var name = 'Chart'
//
// initialization
//
var init = function() {
}

//
// inputs
//
var inputs = {
  data:{
    type:'data',
    event:function(evt){
      console.log(evt)
        outarr = [mod.counter]
        evt.detail.forEach(function(entry){
          outarr.push(parseInt(entry))
        })
       mod.data.push(outarr)
       mod.counter++
       plotChart()
       outputs.output.event()
       }
    },
  options:{
    type:'str',
    event:function(evt){

      }
    } 
  }

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
    canvas.width =500
    canvas.height =0
    canvas.style.backgroundColor = 'rgb(255,255,255)'
    div.appendChild(canvas)
  
  mod.subdiv = document.createElement('div')
    div.appendChild(mod.subdiv)

  google.charts.load('current', {'packages':['corechart']});      
  google.charts.setOnLoadCallback(plotChart);
   //

    input_t4a = document.createElement('input')
        input_t4a.type = 'text'
        input_t4a.value = mod.armAVals[0]
        input_t4a.size = 1
    input_t4a.addEventListener('change', function(){
        mod.armAVals[0] = parseFloat(input_t4a.value)
        plotChart()
        })
    div.appendChild(input_t4a)
    
    div.appendChild(document.createTextNode('t^4 + '))
    
    input_t3a = document.createElement('input')
        input_t3a.type = 'text'
        input_t3a.value = mod.armAVals[1]
        input_t3a.size = 1
    input_t3a.addEventListener('change', function(){
        mod.armAVals[1] = parseFloat(input_t3a.value)
        plotChart()
        })
    div.appendChild(input_t3a)
    
    div.appendChild(document.createTextNode('t^3 + '))
    
    input_t2a = document.createElement('input')
        input_t2a.type = 'text'
        input_t2a.value = mod.armAVals[2]
        input_t2a.size = 1
    input_t2a.addEventListener('change', function(){
        mod.armAVals[2] = parseFloat(input_t2a.value)
        plotChart()
        })
    div.appendChild(input_t2a)
    
    div.appendChild(document.createTextNode('t^2 + '))
    
    input_t1a = document.createElement('input')
        input_t1a.type = 'text'
        input_t1a.value = mod.armAVals[3]
        input_t1a.size = 1
    input_t1a.addEventListener('change', function(){
        mod.armAVals[3] = parseFloat(input_t1a.value)
        plotChart()
        })

    div.appendChild(input_t1a)
    
    div.appendChild(document.createTextNode('t^1 + '))
    
    input_t0a = document.createElement('input')
        input_t0a.type = 'text'
        input_t0a.value = mod.armAVals[4]
        input_t0a.size = 1
    input_t0a.addEventListener('change', function(){
        mod.armAVals[4] = parseFloat(input_t0a.value)
        plotChart()
        })
 
    div.appendChild(input_t0a)
    
    //
    // LINE BREAK
    //
    div.appendChild(document.createElement('br'))
    
    //
    // New Set of inputs (B arm)
    //
        input_t4b = document.createElement('input')
        input_t4b.type = 'text'
        input_t4b.value = mod.armBVals[0]
        input_t4b.size = 1
    input_t4b.addEventListener('change', function(){
        mod.armBVals[0] = parseFloat(input_t4b.value)
        plotChart()
        })
    div.appendChild(input_t4b)
    
    div.appendChild(document.createTextNode('t^4 + '))
    
    input_t3b = document.createElement('input')
        input_t3b.type = 'text'
        input_t3b.value = mod.armBVals[1]
        input_t3b.size = 1
    input_t3b.addEventListener('change', function(){
        mod.armBVals[1] = parseFloat(input_t3b.value)
        plotChart()
        })
    div.appendChild(input_t3b)
    
    div.appendChild(document.createTextNode('t^3 + '))
    
    input_t2b = document.createElement('input')
        input_t2b.type = 'text'
        input_t2b.value = mod.armBVals[2]
        input_t2b.size = 1
    input_t2b.addEventListener('change', function(){
        mod.armBVals[2] = parseFloat(input_t2b.value)
        plotChart()
        })
    div.appendChild(input_t2b)
    
    div.appendChild(document.createTextNode('t^2 + '))
    
    input_t1b = document.createElement('input')
        input_t1b.type = 'text'
        input_t1b.value = mod.armBVals[3]
        input_t1b.size = 1
    input_t1b.addEventListener('change', function(){
        mod.armBVals[3] = parseFloat(input_t1b.value)
        plotChart()
        })
    div.appendChild(input_t1b)
    
    div.appendChild(document.createTextNode('t^1 + '))
    
    input_t0b = document.createElement('input')
        input_t0b.type = 'text'
        input_t0b.value = mod.armBVals[4]
        input_t0b.size = 1
    input_t0b.addEventListener('change', function(){
        mod.armBVals[4] = parseFloat(input_t0b.value)
        plotChart()
        })

    div.appendChild(input_t0b)
    
    //
    // LINE BREAK
    //
    div.appendChild(document.createElement('br'))
    
  var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('view window'))
      btn.addEventListener('click',plotChart)
      div.appendChild(btn)
  }

function plotChart(){
    mod.data = [['Time', 'Arm A', 'Arm B']]
    for(var t = 0; t < 100; t++){
        mod.data.push([t,armA(t), armB(t)])
    }
    console.log(mod.data)
    
    chart_data = google.visualization.arrayToDataTable(mod.data);
    
    chart_data.addColumn('number', 'location');
    chart_data.addRow([50, null, null,0]);
    chart_data.addRow([50, null, null,100]);
    mod.chart = new google.visualization.ScatterChart(mod.subdiv);

    mod.chart.draw(chart_data, mod.options);   
  }
function armA(tval){
    var total = 0;
    for(var i = 4; i >= 0; i--){
        total = total + Math.pow(tval,i)*mod.armAVals[4-i]
    }
    return total
}

function armB(tval){
    var total = 0;
    for(var i = 4; i >= 0; i--){
        total = total + Math.pow(tval,i)*mod.armBVals[4-i]
    }
    return total
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

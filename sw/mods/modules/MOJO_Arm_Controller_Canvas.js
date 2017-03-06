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
            hAxis: {title: 'Time', minValue: 0, maxValue: 100},
            vAxis: {title: 'MotorVal', viewWindowMode:'explicit',
    viewWindow: {
        max:255,
        min:0
    }},
            legend: 'none'
          },
    armAVals : [0,0, -0.00670299, -0.602193,211.1],
    armBVals : [0,0, 0.00682128, -1.967,173.629],
    tval : 0,
    canvas : null,
    ctx: null
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
          var out = {}
          out.motorvals = [parseInt(mod.data[mod.tval][2]), parseInt(mod.data[mod.tval][1]),0,0,0]
         mods.output(mod,'output',JSON.stringify(out))}}}
//
// interface
//

var interface = function(div){
  mod.div = div
  
  mod.canvas = document.createElement('canvas')
    mod.canvas.width =500
    mod.canvas.height =300
    mod.canvas.style.backgroundColor = 'rgb(255,255,255)'
    div.appendChild(mod.canvas)

    mod.ctx = mod.canvas.getContext('2d')
    
    //
    // LINE BREAK
    //
    div.appendChild(document.createElement('br'))

    input_t4a = document.createElement('input')
        input_t4a.type = 'text'
        input_t4a.value = mod.armAVals[0]
        input_t4a.size = 1
    input_t4a.addEventListener('change', function(){
        mod.armAVals[0] = parseFloat(input_t4a.value)
        updateChartData()
        })
    div.appendChild(input_t4a)
    
    div.appendChild(document.createTextNode('t^4 + '))
    
    input_t3a = document.createElement('input')
        input_t3a.type = 'text'
        input_t3a.value = mod.armAVals[1]
        input_t3a.size = 1
    input_t3a.addEventListener('change', function(){
        mod.armAVals[1] = parseFloat(input_t3a.value)
        updateChartData()
        })
    div.appendChild(input_t3a)
    
    div.appendChild(document.createTextNode('t^3 + '))
    
    input_t2a = document.createElement('input')
        input_t2a.type = 'text'
        input_t2a.value = mod.armAVals[2]
        input_t2a.size = 1
    input_t2a.addEventListener('change', function(){
        mod.armAVals[2] = parseFloat(input_t2a.value)
        updateChartData()
        })
    div.appendChild(input_t2a)
    
    div.appendChild(document.createTextNode('t^2 + '))
    
    input_t1a = document.createElement('input')
        input_t1a.type = 'text'
        input_t1a.value = mod.armAVals[3]
        input_t1a.size = 1
    input_t1a.addEventListener('change', function(){
        mod.armAVals[3] = parseFloat(input_t1a.value)
        updateChartData()
        })

    div.appendChild(input_t1a)
    
    div.appendChild(document.createTextNode('t^1 + '))
    
    input_t0a = document.createElement('input')
        input_t0a.type = 'text'
        input_t0a.value = mod.armAVals[4]
        input_t0a.size = 1
    input_t0a.addEventListener('change', function(){
        mod.armAVals[4] = parseFloat(input_t0a.value)
        updateChartData()
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
        updateChartData()
        })
    div.appendChild(input_t4b)
    
    div.appendChild(document.createTextNode('t^4 + '))
    
    input_t3b = document.createElement('input')
        input_t3b.type = 'text'
        input_t3b.value = mod.armBVals[1]
        input_t3b.size = 1
    input_t3b.addEventListener('change', function(){
        mod.armBVals[1] = parseFloat(input_t3b.value)
        updateChartData()
        })
    div.appendChild(input_t3b)
    
    div.appendChild(document.createTextNode('t^3 + '))
    
    input_t2b = document.createElement('input')
        input_t2b.type = 'text'
        input_t2b.value = mod.armBVals[2]
        input_t2b.size = 1
    input_t2b.addEventListener('change', function(){
        mod.armBVals[2] = parseFloat(input_t2b.value)
        updateChartData()
        })
    div.appendChild(input_t2b)
    
    div.appendChild(document.createTextNode('t^2 + '))
    
    input_t1b = document.createElement('input')
        input_t1b.type = 'text'
        input_t1b.value = mod.armBVals[3]
        input_t1b.size = 1
    input_t1b.addEventListener('change', function(){
        mod.armBVals[3] = parseFloat(input_t1b.value)
        updateChartData()
        })
    div.appendChild(input_t1b)
    
    div.appendChild(document.createTextNode('t^1 + '))
    
    input_t0b = document.createElement('input')
        input_t0b.type = 'text'
        input_t0b.value = mod.armBVals[4]
        input_t0b.size = 1
    input_t0b.addEventListener('change', function(){
        mod.armBVals[4] = parseFloat(input_t0b.value)
        updateChartData()
        })

    div.appendChild(input_t0b)
    
    //
    // LINE BREAK
    //
    div.appendChild(document.createElement('br'))
    
  var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('update'))
      btn.addEventListener('click',updateChartData)
      div.appendChild(btn)  
    
    var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('go'))
      btn.addEventListener('click',function(){
          mod.incrementing = true
          increment()
      })
      div.appendChild(btn)
  }

function plotChart(){
    
    mod.ctx.clearRect(0,0,mod.canvas.width,mod.canvas.height)
    mod.ctx.beginPath()
    mod.ctx.moveTo(10,mod.canvas.height-armA(0))
    for(var t = 0; t < 100; t++){
        mod.ctx.lineTo(t*(mod.canvas.width-20)/100.0+10,mod.canvas.height-mod.data[t][1])
    }
    mod.ctx.stroke()
    
    mod.ctx.moveTo(10,mod.canvas.height-armB(0))
    mod.ctx.beginPath()
    for(var t = 0; t < 100; t++){
        mod.ctx.lineTo(t*(mod.canvas.width-20)/100.0+10,mod.canvas.height-mod.data[t][2])
    }
    mod.ctx.stroke()
    
    
    mod.ctx.moveTo(mod.tval*(mod.canvas.width-20)/100.0+10,mod.canvas.height)
    mod.ctx.beginPath()
        mod.ctx.lineTo(mod.tval*(mod.canvas.width-20)/100.0+10,mod.canvas.height)
        mod.ctx.lineTo(mod.tval*(mod.canvas.width-20)/100.0+10,0)
    mod.ctx.stroke()
    
    
  }
function increment(){
    mod.tval = mod.tval + 5
    if(mod.tval > 100){
        mod.incrementing = false
        mod.tval = 0
    }
    plotChart()
    outputs.output.event()
    
    if(mod.incrementing)
        setTimeout(increment,100)
        
        
}
function updateChartData(){
    mod.data = []
    
    for(var t = 0; t < 100; t++){
        mod.data.push([t,armA(t),armB(t)])
    }
    
    
    plotChart()
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

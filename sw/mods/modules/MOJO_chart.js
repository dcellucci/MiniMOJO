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
  data1:[
          ['Time', 'Current']
        ],
  data2:[
          ['Time', 'Current']
        ],
  options:{
            title: 'Current Draw',
            hAxis: {title: 'Time',
                minValue: 0,
                maxValue: 15,
                viewWindowMode:'explicit',
                viewWindow:{
                    max:50,
                    min:0
                },
            },
            vAxis: {
                title: 'Current (A)',
                minValue: 0,
                maxValue: 15,
            },
            legend: 'none',
            width: 900
          }
}

var input
//
// name
//
var name = 'MOJO_Chart'
//
// initialization
//
var init = function() {
}

//
// inputs
//
var inputs = {
    data1:{
        type:'data',
        event:function(evt){
            console.log(evt)
            outarr = [mod.counter]
            evt.detail.forEach(function(entry){
                outarr.push(parseInt(entry))
            })
            mod.data1.push(outarr)
            mod.counter++
            plotChart()
            outputs.output.event()
        }
    },
    data2:{
        type:'data',
        event:function(evt){
            console.log(evt)
            outarr = [mod.counter]
            evt.detail.forEach(function(entry){
                outarr.push(parseInt(entry))
            })
            mod.data2.push(outarr)
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
            mods.output(mod,'output',mod.value)
        }
    }
}
//
// interface
//

var interface = function(div){
  mod.div = div
  
  var canvas = document.createElement('canvas')
    canvas.width = 500
    canvas.height = 0
    canvas.style.backgroundColor = 'rgb(255,255,255)'
    div.appendChild(canvas)
  
  mod.subdiv = document.createElement('div')
    div.appendChild(mod.subdiv)

  //google.charts.load('current', {'packages':['corechart']});
  google.charts.load('current', {'packages':['line']});    
  google.charts.setOnLoadCallback(plotChart);

  var btn = document.createElement('button')
      btn.style.padding = mods.ui.padding
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('view window'))
      btn.addEventListener('click',plotChart)
      div.appendChild(btn)
  }

function plotChart(){
    // test looping through data to create data table (not working)
    // for (var i=0; i<mod.data1.length; i++)
    // {
        // data.addRows([i, mod.data1[i], mod.data1[i], mod.data1[i]]);
    // }
    
    // example
    // var data = new google.visualization.DataTable();
    // data.addColumn('number', 'Elapsed Time [sec]');
    // data.addColumn('number', 'Motor_Current_1');
    // data.addColumn('number', 'Motor_Current_2');
    // data.addColumn('number', 'Motor_Current_3');
    // data.addRows([
        // [1,  37.8, 80.8, 41.8],
        // [2,  30.9, 69.5, 32.4],
        // [3,  25.4,   57, 25.7],
        // [4,  11.7, 18.8, 10.5],
        // [5,  11.9, 17.6, 10.4],
        // [6,   8.8, 13.6,  7.7],
        // [7,   7.6, 12.3,  9.6],
        // [8,  12.3, 29.2, 10.6],
        // [9,  16.9, 42.9, 14.8],
        // [10, 12.8, 30.9, 11.6],
        // [11,  5.3,  7.9,  4.7],
        // [12,  6.6,  8.4,  5.2],
        // [13,  4.8,  6.3,  3.6],
        // [14,  4.2,  6.2,  3.4]
    // ]);
    
    
    chart_data = google.visualization.arrayToDataTable(mod.data1);
    console.log(chart_data)
    // mod.chart = new google.visualization.LineChart(mod.subdiv);
    mod.chart = new google.visualization.ScatterChart(mod.subdiv);
    // mod.chart = new google.charts.Line(mod.subdiv);
    mod.chart.draw(chart_data, mod.options);
    
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

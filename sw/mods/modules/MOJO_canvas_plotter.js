// Steven Hu [20170317] in collaboration with Daniel Cellucci (NASA/Cornell)
// 
// using mods project by Neil Gershenfeld (c) Massachusetts Institute of Technology 2016
//
(function(){
//
// module globals
//
var mod = {}
var name = 'MOJO_CANVAS_PLOTTER'
              
var canv = document.createElement('canvas');
canv.height = 300
canv.width = 500
var ctx = canv.getContext("2d")

// data array to be plotted
var dataX = [0]
var dataY = [0] 

// plot area size/location
var plot_area = {}
plot_area.margin = 50
plot_area.height = canv.height - plot_area.margin
plot_area.width = canv.width - plot_area.margin
plot_area.xrange = 30
plot_area.yrange = 10
plot_area.xticklength = 5
plot_area.yticklength = 5

//
// initialization
//
var init = function(){
    console.log("Initializing... " + name)
}
//
// inputs
//
var inputs = {
    data_input_1:{type:'string',
        event:function(evt){
            mod.lastval = parseFloat(evt.detail[0])
            add_data(mod.lastval)
            plot_data(ctx)
        }
    }
}
//
// outputs
//

var outputs = {}

//
// interface
//
var interface = function(div){
    mod.lastval = "<null>"
    
    input_x_range = document.createElement('input')
    input_x_range.type = 'text'
    input_x_range.size = 3
    input_x_range.addEventListener('change', function(){
        plot_area.xrange = input_x_range.value
        console.log('xrange updated: ' + plot_area.xrange)
    })
    
    input_y_lim = document.createElement('input')
    input_y_lim.type = 'text'
    input_y_lim.size = 3
    input_y_lim.addEventListener('change', function(){
        plot_area.yrange = input_y_lim.value
        console.log('ylim updated: ' + plot_area.yrange)
    })
   
    // test button
    var btn = document.createElement("BUTTON")
    btn.style.margin = 1
    btn.appendChild(document.createTextNode('TEST_PLOT'))
    btn.addEventListener('click', function() {
        add_data(5)
        plot_data(ctx)
    })
    
    // DOM
    div.appendChild(canv)
    div.appendChild(document.createElement('br'))
    div.appendChild(document.createTextNode(' x_range: '))
    div.appendChild(input_x_range)
    div.appendChild(document.createTextNode(' y_lim: '))
    div.appendChild(input_y_lim)
    // div.appendChild(document.createElement('br'))
    // div.appendChild(btn)  // test button
};


function plot_data(x){
    x.clearRect(0, 0, canv.width, canv.height);  // clearing previous canvas
    
    // draw box for chart area
    x.lineWidth = 1;
    x.beginPath();
    x.strokeStyle = "black";
    x.rect(plot_area.margin/2, plot_area.margin/2, plot_area.width, plot_area.height);
    x.stroke();
    
    // adjusting plotting for plot_area size
    x_increment = plot_area.width/plot_area.xrange            // canvas width == 30 sec.
    y_increment = -1 * plot_area.height/plot_area.yrange      // canvas height == ? ; reverse y axis
    
    // adjusting plotting for canvas relationship with plot_area
    x_offset = plot_area.margin/2
    y_offset = plot_area.margin/2 + plot_area.height
    
    // draw x and y axis
    x.strokeStype = "blue";
    for (var i = 0; i < plot_area.xrange; i++){
        x.moveTo(x_offset + i*x_increment, y_offset - plot_area.xticklength)
        x.lineTo(x_offset + i*x_increment, y_offset + plot_area.xticklength)
        x.stroke()
    }
    x.strokeStype = "blue";
    for (var i = 0; i < plot_area.yrange; i++){
        x.moveTo(x_offset - plot_area.yticklength, y_offset + i*y_increment)
        x.lineTo(x_offset + plot_area.yticklength, y_offset + i*y_increment)
        x.stroke()
    }
    
    // update last value
    x.font = '20px Arial'
    x.fillText(mod.lastval, plot_area.margin, plot_area.margin)
        
    // draw line
    x.beginPath();
    x.moveTo(x_offset,y_offset);
    
    console.log(dataX)
    console.log(dataY)

    x.strokeStyle = "red";
    for (var i = 0; i < dataX.length; i++) {
        dx = x_offset + x_increment*dataX[i]
        dy = y_offset + y_increment*dataY[i]
        
        x.lineTo(dx,dy);
        x.stroke();
    }
    x.closePath();
}

function add_data(y){
    dataX.push(dataX[dataX.length-1] + 1)
    dataY.push(y)
    
    if (dataX.length > plot_area.xrange) {
        dataX.pop();
        dataY.shift();
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

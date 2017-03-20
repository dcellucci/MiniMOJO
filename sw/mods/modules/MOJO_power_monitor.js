// Steven Hu [20170302] in collaboration with Daniel Cellucci (NASA/Cornell)
// 
// using mods project by Neil Gershenfeld (c) Massachusetts Institute of Technology 2016
//
(function(){
//
// module globals
//
var mod = {}
var name = 'MOJO_POWER_MONITOR'
var params = ['power_1',
              'power_2',
              'motorvals_1',
              'motorvals_2',
              'motorvals_3',
              'motorvals_4',
              'motorvals_5',
              'vbus_1',
              'vbus_2',
              'vbus_3',
              'vbus_4',
              'vbus_5',
              'vshunt_1',
              'vshunt_2',
              'vshunt_3',
              'vshunt_4',
              'vshunt_5',
              'soc_1',
              'soc_2'
              ]
              
var canv = document.createElement('canvas');
canv.height = 300
canv.width = 160
var ctx = canv.getContext("2d")
    
//
// conversions
//
var conv_vshunt = 0.16  // to mA
var conv_vbus = 0.001   // to V
var conv_soc = 1/256    // to %

// power guage size/location
var powerbar1 = {}
powerbar1.offset_x = 30
powerbar1.offset_y = 50
powerbar1.width = 30
powerbar1.height = 200

var powerbar2 = {}
powerbar2.offset_x = powerbar1.offset_x + 80
powerbar2.offset_y = powerbar1.offset_y
powerbar2.width = 30
powerbar2.height = 200

var poweron1 = {}
poweron1.centerX = powerbar1.offset_x + powerbar1.width/2
poweron1.centerY = powerbar1.offset_y/2;
poweron1.radius = 10;

var poweron2 = {}
poweron2.centerX = powerbar2.offset_x + powerbar2.width/2
poweron2.centerY = powerbar2.offset_y/2;
poweron2.radius = 10;

              
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
    json_input:{type:'string',
        event:function(evt){
        if (evt.detail.slice(-1) == "}"){           // ensuring properly formed JSON
                if (evt.detail.slice(0,1) == "{"){  // previous WebsocketSerial removed starting bracket '{'
                    var x = evt.detail
                } else {
                    var x = '{'.concat(evt.detail)
                }
                var obj = JSON.parse(x)
                // add loop here with params in json 
                if (obj.power){
                    // mod.text1.value = [obj.power[0]]
                    // mod.text2.value = [obj.power[1]]
                    // outputs.power_1.event()
                    // outputs.power_2.event()
                }
                if (obj.motorvals){
                    // mod.text3.value = [parseInt(obj.motorvals[0])]
                    // mod.text4.value = [parseInt(obj.motorvals[1])]
                    // mod.text5.value = [parseInt(obj.motorvals[2])]
                    // mod.text6.value = [parseInt(obj.motorvals[3])]
                    // mod.text7.value = [parseInt(obj.motorvals[4])]
                    // outputs.motorvals_1.event()
                    // outputs.motorvals_2.event()
                    // outputs.motorvals_3.event()
                    // outputs.motorvals_4.event()
                    // outputs.motorvals_5.event()
                }
                if (obj.vbus){
                    // mod.text8.value = [parseInt(obj.vbus[0])*conv_vbus]
                    // mod.text9.value = [parseInt(obj.vbus[1])*conv_vbus]
                    // mod.text10.value = [parseInt(obj.vbus[2])*conv_vbus]
                    // mod.text11.value = [parseInt(obj.vbus[3])*conv_vbus]
                    // mod.text12.value = [parseInt(obj.vbus[4])*conv_vbus]
                    // outputs.vbus_1.event()
                    // outputs.vbus_2.event()
                    // outputs.vbus_3.event()
                    // outputs.vbus_4.event()
                    // outputs.vbus_5.event()
                }
                if (obj.vshunt){
                    // mod.text13.value = [parseInt(obj.vshunt[0])*conv_vshunt]
                    // mod.text14.value = [parseInt(obj.vshunt[1])*conv_vshunt]
                    // mod.text15.value = [parseInt(obj.vshunt[2])*conv_vshunt]
                    // mod.text16.value = [parseInt(obj.vshunt[3])*conv_vshunt]
                    // mod.text17.value = [parseInt(obj.vshunt[4])*conv_vshunt]
                    // outputs.vshunt_1.event()
                    // outputs.vshunt_2.event()
                    // outputs.vshunt_3.event()
                    // outputs.vshunt_4.event()
                    // outputs.vshunt_5.event()
                }
                if (obj.soc){
                    // var soc1 = [parseInt(obj.soc[0])*conv_soc]
                    // var soc2 = [parseInt(obj.soc[1])*conv_soc]
                }
                
                var p1 = [obj.power[0]]
                var p2 = [obj.power[1]]
                var soc1 = [parseInt(obj.soc[0])*conv_soc]
                var soc2 = [parseInt(obj.soc[1])*conv_soc]
                
                // update_canvas()
                update_canvas(canv, ctx, p1, p2, soc1, soc2) 
                    
                              
            } else {
                console.log("WARNING - JSON input may not be properly formed")
            }
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
    update_canvas(canv, ctx, false, false, 0, 0)     // initialize empty power guages
    
    // test button
    var btn = document.createElement("BUTTON")
    btn.style.margin = 1
    btn.appendChild(document.createTextNode('SET to 40%, 50%'))
    btn.addEventListener('click', function() {
        update_canvas(canv, ctx, true, true, 40, 50)
    })
    
    // DOM
    div.appendChild(canv)
    div.appendChild(document.createElement('br'))
    // div.appendChild(btn)  // test button
};


function update_canvas(canv, x, p1, p2, soc1, soc2){
    x.clearRect(0, 0, canv.width, canv.height);  // clearing previous canvas
    // drawing power on lights
    var p1_fill = "#CCCCCC"
    var p2_fill = "#CCCCCC"
    
    if (p1 == true){
        p1_fill = "#00FF00"
    } else {
        p1_fill = "#CCCCCC"
    }
    
    if (p2 == true){
        p2_fill = "#00FF00"
    } else {
        p2_fill = "#CCCCCC"
    }
    
    x.beginPath();
    x.arc(poweron1.centerX, poweron1.centerY, poweron1.radius, 0, 2 * Math.PI, false);
    x.fillStyle = p1_fill
    x.fill();
    x.stroke();
    
    x.beginPath();
    x.arc(poweron2.centerX, poweron2.centerY, poweron2.radius, 0, 2 * Math.PI, false);
    x.fillStyle = p2_fill
    x.fill();
    x.stroke();
    
    // drawing power guage boundary
    x.beginPath();
    x.lineWidth = 3;
    x.strokeStyle="black";
    x.rect(powerbar1.offset_x, powerbar1.offset_y, powerbar1.width, powerbar1.height);
    x.rect(powerbar2.offset_x, powerbar2.offset_y, powerbar2.width, powerbar2.height);
    x.stroke();
    // filling power guage
    powerbar1.value = soc1/100
    powerbar2.value = soc2/100
    // adding % text
    x.fillStyle = "#000000"
    x.font = '20px Arial'
    x.fillText((powerbar1.value*100).toFixed(0).toString().concat(' %'), powerbar1.offset_x-5,powerbar1.offset_y + powerbar1.height + 25)
    x.fillText((powerbar2.value*100).toFixed(0).toString().concat(' %'), powerbar2.offset_x-5,powerbar2.offset_y + powerbar2.height + 25)
    // max fill bar to 100
    if (powerbar1.value > 1){
        powerbar1.value = 1
    }
    if (powerbar2.value > 1){
        powerbar2.value = 1
    }
    x.fillStyle = "#FF0000"
    x.fillRect(powerbar1.offset_x, powerbar1.offset_y + powerbar1.height - (powerbar1.value*powerbar1.height), powerbar1.width, powerbar1.value*powerbar1.height)
    x.fillRect(powerbar2.offset_x, powerbar2.offset_y + powerbar2.height - (powerbar2.value*powerbar2.height), powerbar2.width, powerbar2.value*powerbar2.height)
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

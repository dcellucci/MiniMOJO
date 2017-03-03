// Steven Hu [20170302] in collaboration with Daniel Cellucci (NASA/Cornell)
// 
// using mods project by Neil Gershenfeld (c) Massachusetts Institute of Technology 2016
//
(function(){
//
// module globals
//
var mod = {}
var name = 'get_MOJO_JSON_Data'
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
//
// initialization
//
var init = function() {
    for (i = 0; i < params.length; i++){
        eval("mod.text" + parseInt(i+1) + ".value = 'value " + parseInt(i+1) + "';");
    }
   }
//
// inputs
//
var inputs = {
    json_input:{type:'string',
        event:function(evt){
        if (evt.detail.slice(-1) == "}"){  // ensuring properly formed JSON
                var x = '{'.concat(evt.detail)
                var obj = JSON.parse(x)
                // add loop here with params in json 
                if (obj.power){
                    mod.text1.value = [obj.power[0]]
                    mod.text2.value = [obj.power[1]]
                    outputs.power_1.event()
                    outputs.power_2.event()
                }
                if (obj.motorvals){
                    mod.text3.value = [parseInt(obj.motorvals[0])]
                    mod.text4.value = [parseInt(obj.motorvals[1])]
                    mod.text5.value = [parseInt(obj.motorvals[2])]
                    mod.text6.value = [parseInt(obj.motorvals[3])]
                    mod.text7.value = [parseInt(obj.motorvals[4])]
                    outputs.motorvals_1.event()
                    outputs.motorvals_2.event()
                    outputs.motorvals_3.event()
                    outputs.motorvals_4.event()
                    outputs.motorvals_5.event()
                }
                if (obj.vbus){
                    mod.text8.value = [parseInt(obj.vbus[0])]
                    mod.text9.value = [parseInt(obj.vbus[1])]
                    mod.text10.value = [parseInt(obj.vbus[2])]
                    mod.text11.value = [parseInt(obj.vbus[3])]
                    mod.text12.value = [parseInt(obj.vbus[4])]
                    outputs.vbus_1.event()
                    outputs.vbus_2.event()
                    outputs.vbus_3.event()
                    outputs.vbus_4.event()
                    outputs.vbus_5.event()
                }
                if (obj.vshunt){
                    mod.text13.value = [parseInt(obj.vshunt[0])]
                    mod.text14.value = [parseInt(obj.vshunt[1])]
                    mod.text15.value = [parseInt(obj.vshunt[2])]
                    mod.text16.value = [parseInt(obj.vshunt[3])]
                    mod.text17.value = [parseInt(obj.vshunt[4])]
                    outputs.vshunt_1.event()
                    outputs.vshunt_2.event()
                    outputs.vshunt_3.event()
                    outputs.vshunt_4.event()
                    outputs.vshunt_5.event()
                }
                if (obj.soc){
                    mod.text18.value = [parseInt(obj.soc[0])]
                    mod.text19.value = [parseInt(obj.soc[1])]
                    outputs.soc_1.event()
                    outputs.soc_2.event()
                }                
            } else {
                //console.log("WARNING - JSON input may not be properly formed")
            }
        }
    }
}
//
// outputs
//
for (i = 0; i < params.length; i++){
    // assign output
    if (i == 0){
        var assign_output = "var outputs = {"
    } else {
        assign_output += ","
    }
    assign_output += params[i] + ":{type:'int', event:function(){mods.output(mod,'" + params[i] + "',[mod.text" + parseInt(i+1) + ".value])}}"
}
assign_output += "}"
eval(assign_output)

//
// interface
//
var interface = function(div){
    mod.div = div
    
    for (i = 0; i < params.length; i++){
        // creating input element
        eval("input" + parseInt(i+1) + " = document.createElement('input')")
        eval("input" + parseInt(i+1) + ".type = 'text'")
        eval("input" + parseInt(i+1) + ".size = 5")
        eval("input" + parseInt(i+1) + ".addEventListener('keydown',function(evt){if (evt.key == 'Enter'){outputs." + params[i] +".event()}})")
        // adding element to html
        eval("div.appendChild(document.createElement('br'))")
        eval("div.appendChild(document.createTextNode('" + params[i] + ": '))")
        eval("div.appendChild(input" + parseInt(i+1) + ")")
        // assign value
        eval("mod.text" + parseInt(i+1) + " = input" + parseInt(i+1));
    }
}
;
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

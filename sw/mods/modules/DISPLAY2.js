// Steven Hu [20170302] in collaboration with Daniel Cellucci (NASA/Cornell)
// 
// using mods project by Neil Gershenfeld (c) Massachusetts Institute of Technology 2016
//
(function(){
//
// module globals
//
var mod = {}
var name = 'DISPLAY'
var params = ['input_1',
              'input_2',
              'input_3',
              'input_4',
              'input_5'
              ]
          
var display_width = 120
//
// initialization
//
var init = function() {
    for (i = 0; i < params.length; i++){
        eval("mod.text" + parseInt(i+1) + ".value = 'value " + parseInt(i+1) + "'");
    }
}
//
// inputs
//       
for (i = 0; i < params.length; i++){
    if (i == 0){
        var assign_input = "var inputs = {"
    } else {
        assign_input += ","
    }
    assign_input += params[i] + ":{type:'string', event:function(evt){mod.text" + parseInt(i+1) + ".value = evt.detail"
    assign_input += "\noutputs." + params[i] + ".event()}}"
}
assign_input += "}"
// console.log(assign_input)
eval(assign_input)
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
    assign_output += params[i] + ":{type:'string', event:function(){mods.output(mod,'" + params[i] + "',[mod.text" + parseInt(i+1) + ".value])}}"
}
assign_output += "}"
// console.log(assign_output)
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
        eval("input" + parseInt(i+1) + ".size = " + parseInt(display_width))
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

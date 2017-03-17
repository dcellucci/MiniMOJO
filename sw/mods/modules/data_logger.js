// Steven Hu [20170316] in collaboration with Daniel Cellucci (NASA/Cornell)
// 
// using mods project by Neil Gershenfeld (c) Massachusetts Institute of Technology 2016
//
(function(){

// module globals
var mod = {}
var auto_save_handle = 0  // initializing global handle
var name = 'DATA LOGGER'  // GUI name

// initialization
var init = function() {
    var date = new Date();
    mod.text1.value = "<null>";
}

// inputs
var inputs = {
    input_1:{
        type:'string',
        event:function(evt){
            mod.text1.value = evt.detail
            outputs.input_1.event()
        }
    }
}

// outputs
var outputs = {
        input_1:{
            type:'string',
            event:function(){
                mods.output(mod,'input_1',mod.text1.value)
                data_logger.value = mod.text1.value + '\n' + data_logger.value
            }
        }
}

// interface
var interface = function(div){
    mod.div = div
    
    // input data stream
    div.appendChild(document.createTextNode('input_1: '))
    input = document.createElement('input')
    input.type = 'text'
    input.size = 150
    mod.text1 = input
    
    // data logger textbox
    data_logger = document.createElement('textarea');
    data_logger.cols = 150;
    data_logger.rows = 10;

    // auto save checkbox
    var auto_save = document.createElement('input')
    auto_save.setAttribute("type", "checkbox")
    auto_save.addEventListener('click', function(){
        auto_save_checkbox_handler(auto_save)
    })    

    // manual save button
    var btn = document.createElement("BUTTON")
    btn.id = "save_button"
    btn.style.margin = 1
    btn.appendChild(document.createTextNode('SAVE_LOG'))
    btn.addEventListener('click', function() {
        saveFile(data_logger)
    })
   
    // DOM
    div.appendChild(input);
    div.appendChild(document.createElement('br'));
    div.appendChild(document.createElement('br'));
    div.appendChild(data_logger);
    div.appendChild(document.createElement('br'))
    div.appendChild(document.createTextNode('AUTO SAVE (60 sec.)'))
    div.appendChild(auto_save)
    div.appendChild(document.createElement('br'))
    div.appendChild(btn)
    
}

// local functions
function auto_save_checkbox_handler(x){
    console.log('auto data logger: ' + x.checked)
    if (x.checked){
        auto_save_handle = setInterval(function(){document.getElementById("save_button").click()}, 60*1000);
    } else {
        clearInterval(auto_save_handle)
    }
}

function saveFile(data_logger){
    var textToWrite = data_logger.value;
    var textFileAsBlob = new Blob([textToWrite], {type:'text/xml'});
    
    var date = new Date();
    var year = (date.getFullYear())
    var month = (("0" + (date.getMonth() + 1).toString()).slice(-2))
    var day = (("0" + (date.getDate()).toString()).slice(-2))
    var hour = (("0" + (date.getHours()).toString()).slice(-2))
    var min = (("0" + (date.getMinutes()).toString()).slice(-2))
    var datestr = year + month + day + hour + min
    
    var fileNameToSaveAs = "mojo_output_".concat(datestr).concat(".log");

    var downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File";

    downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
    downloadLink.click();
    data_logger.value = "";  // once saved; clears data logger textbox
}

// return values
return ({
    name:name,
    init:init,
    inputs:inputs,
    outputs:outputs,
    interface:interface
    })
}())

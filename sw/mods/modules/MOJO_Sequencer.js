// MOJO Motor Control
//
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
    seqIndex:0,
    moving:true,
    currentSequence:"climbSequence",
    direction:1,
    timeout:1000,
    sequences:{
        climbSequence:[['oe', 'oe', 'st'],
                       ['oe', 'or', 'st'],
                       ['ie', 'ir', 'st'],
                       ['ie', 'ie', 'st'],
                       ['ir', 'ie', 'st'],
                       ['or', 'oe', 'st'],
                       ['oe', 'oe', 'st']],
        turnSequence: [['oe', 'oe', 'st'],
                       ['oe', 'or', 'st'],
                       ['oe', 'ir', 'st'],
                       ['ie', 'ir', 'st'],
                       ['ie', 'ie', 'st'],
                       ['ir', 'ie', 'st'],
                       ['or', 'oe', 'st'],
                       ['oe', 'oe', 'st']]
    }
}

//
// name
//
var name = 'MOJO Sequencer'
//
// initialization
//
var init = function() {

    }

//
// inputs
//
var inputs = {
   }
//
// outputs
//
var outputs = {
   frame:{
      type:'MOJOFrame',
      event:function(){
           var out = {}
           out.topFrame = mod.sequences[mod.currentSequence][mod.seqIndex][0]
           out.botFrame = mod.sequences[mod.currentSequence][mod.seqIndex][1]
           out.hipFrame = mod.sequences[mod.currentSequence][mod.seqIndex][2]
           mods.output(mod,'frame',JSON.stringify(out))
           }
      }
}
//
// interface
//


var interface = function(div){
    mod.div = div///*

    div.appendChild(document.createTextNode('Timeout: '))

    input_to = document.createElement('input')
        input_to.type = 'text'
        input_to.value = mod.timeout
        input_to.size = 5
        //input_to.align="right"
        input_to.addEventListener('change',
            function(){
                mod.timeout = parseInt(input_to.value)
            })
    div.appendChild(input_to)

    div.appendChild(document.createTextNode('ms'))

    var toptable = document.createElement('table')
        var tr = document.createElement('tr')
            var td = document.createElement('td')
        tr.appendChild(td)
            td = document.createElement('td')
                var btn = document.createElement('button')
                    btn.style.margin = 1
                    btn.appendChild(document.createTextNode('forward'))
                    btn.addEventListener('click',
                        function(event){
                            moveForward()
                        })
            td.appendChild(btn)
        tr.appendChild(td)
            td = document.createElement('td')
        tr.appendChild(td)
    toptable.appendChild(tr)
        tr = document.createElement('tr')
            td = document.createElement('td')
                var btn = document.createElement('button')
                    btn.style.margin = 1
                    btn.appendChild(document.createTextNode('left'))
                    btn.addEventListener('click',
                        function(event){
                            turnLeft()
                        })
            td.appendChild(btn)
        tr.appendChild(td)
            td = document.createElement('td')
        tr.appendChild(td)
            td = document.createElement('td')
                var btn = document.createElement('button')
                    btn.style.margin = 1
                    btn.appendChild(document.createTextNode('right'))
                    btn.addEventListener('click',
                        function(event){
                            turnRight()
                        })
            td.appendChild(btn)
        tr.appendChild(td)
    toptable.appendChild(tr)
          tr = document.createElement('tr')
              var td = document.createElement('td')
          tr.appendChild(td)
              td = document.createElement('td')
                  var btn = document.createElement('button')
                      btn.style.margin = 1
                      btn.appendChild(document.createTextNode('backward'))
                      btn.addEventListener('click',
                          function(event){
                              moveBackward()
                          })
              td.appendChild(btn)
        tr.appendChild(td)
            td = document.createElement('td')
        tr.appendChild(td)
    toptable.appendChild(tr)
    div.appendChild(toptable)

    div.appendChild(document.createElement('br'))

    var btn = document.createElement('button')
        btn.style.margin = 1
        btn.appendChild(document.createTextNode('pause'))
        btn.addEventListener('click',
            function(event){
                pauseMovement()
                outputs.motorVals.event()
            })
    div.appendChild(btn)
}

//
// local functions
//

function moveForward(){
   mod.moving = true
   mod.direction = 1
   mod.currentSequence="climbSequence"
   mod.seqIndex = 0
   move()
   }

function moveBackward(){
   mod.moving = true
   mod.direction = -1
   mod.currentSequence="climbSequence"
   mod.seqIndex = 0
   move()
   }

function pauseMovement(){
   mod.moving = false
   }

function move(){
    if(mod.moving){
        outputs.frame.event()
        mod.seqIndex = (mod.seqIndex + mod.direction) % mod.sequences[mod.currentSequence].length
        setTimeout(move,1000)
    }
    if(mod.seqIndex < 0)
        mod.seqIndex = mod.seqIndex+mod.sequences[mod.currentSequence].length
    if(mod.seqIndex == 0){
        mod.moving = false
        if(mod.direction == -1){
           outputs.frame.event()
        }
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

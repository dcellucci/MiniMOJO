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
   curTopPos:-1,
   curBotPos:-1,
   curHipPos:-1,
   posVals:{
      or:[ 40,184], //outer retracted
      oe:[ 40, 80], //outer extended
      mr:[ 40,200], //mid retracted
      me:[ 80,178], //mid extended
      ir:[ 40,220], //inner retracted
      ie:[190,220],  //inner extended
      st:180,
      bt:15
   },
   topPos:{},
   botPos:{},
   hipPos:{},
   motorVals:[-1,-1,-1,-1,-1]
}



//
// name
//
var name = 'MOJO Position Control'
//
// initialization
//
var init = function() {

    }

//
// inputs
//
var inputs = {
    frameList:{
        type:'framelist',
        event:function(evt){
            var input = JSON.parse(evt.detail)
            mod.motorVals = [-1,-1,-1,-1,-1]
            if("topFrame" in input){
                val = input.topFrame
                if(mod.curTopPos != -1)
                    mod.topPos[mod.curTopPos].style.backgroundColor = 'transparent'
                mod.topPos[val].style.backgroundColor = 'red'
                mod.curTopPos = val
                mod.motorVals[0] = mod.posVals[val][0]
                mod.motorVals[1] = mod.posVals[val][1]
            }
            if("botFrame" in input){
                val = input.botFrame
                if(mod.curBotPos != -1)
                    mod.botPos[mod.curBotPos].style.backgroundColor = 'transparent'
                mod.botPos[val].style.backgroundColor = 'red'
                mod.curBotPos = val
                mod.motorVals[2] = mod.posVals[val][0]
                mod.motorVals[3] = mod.posVals[val][1]
            }
            if("hipFrame" in input){
                val = input.hipFrame
                if(mod.curHipPos != -1)
                    mod.hipPos[mod.curHipPos].style.backgroundColor = 'transparent'
                mod.hipPos[val].style.backgroundColor = 'red'
                mod.curHipPos = val
                mod.motorVals[4] = mod.posVals[val]
            }
            outputs.motorVals.event()
        }
    }
}
//
// outputs
//
var outputs = {
   motorVals:{
      type:'MOJOState',
      event:function(){
           var out = {}
           out.motorvals = mod.motorVals
           mods.output(mod,'motorVals',JSON.stringify(out))
           }
      }
}
//
// interface
//


var interface = function(div){
    mod.div = div///*

    var btn = document.createElement('button')
        btn.style.margin = 1
        btn.appendChild(document.createTextNode('init'))
        btn.addEventListener('click', function(event){
            goTopMOJO('mr')
            goBotMOJO('mr')
            goHipMOJO('st')
            goCurMOJO()
            outputs.motorVals.event()
        })
   div.appendChild(btn)

   div.appendChild(document.createElement('br'))

   div.appendChild(document.createTextNode('Top: '))
      var toptable = document.createElement('table')
         var tr = document.createElement('tr')
            var ttdor = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('or'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('or')
                          outputs.motorVals.event()
                      })
            ttdor.appendChild(btn)
            mod.topPos.or = ttdor
         tr.appendChild(ttdor)
            ttdoe = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('oe'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('oe')
                          outputs.motorVals.event()
                      })
            ttdoe.appendChild(btn)
            mod.topPos.oe = ttdoe
         tr.appendChild(ttdoe)
      toptable.appendChild(tr)
         var tr = document.createElement('tr')
            var ttdmr = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('mr'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('mr')
                          outputs.motorVals.event()
                      })
            ttdmr.appendChild(btn)
            mod.topPos.mr = ttdmr
         tr.appendChild(ttdmr)
            ttdme = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('me'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('me')
                          outputs.motorVals.event()
                      })
            ttdme.appendChild(btn)
            mod.topPos.me = ttdme
         tr.appendChild(ttdme)
      toptable.appendChild(tr)
         var tr = document.createElement('tr')
            var ttdir = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('ir'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('ir')
                          outputs.motorVals.event()
                      })
            ttdir.appendChild(btn)
            mod.topPos.ir = ttdir
         tr.appendChild(ttdir)
            ttdie = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('ie'))
                  btn.addEventListener('click',
                      function(event){
                          goTopMOJO('ie')
                          outputs.motorVals.event()
                      })
            ttdie.appendChild(btn)
            mod.topPos.ie = ttdie
         tr.appendChild(ttdie)
      toptable.appendChild(tr)
   div.appendChild(toptable)

   div.appendChild(document.createElement('br'))

   div.appendChild(document.createTextNode('Hip: '))


      var toptable = document.createElement('table')
         var tr = document.createElement('tr')
            var htdst = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('straight'))
                  btn.addEventListener('click',
                      function(event){
                          goHipMOJO('st')
                          outputs.motorVals.event()
                      })
            htdst.appendChild(btn)
            mod.hipPos.st = htdst
         tr.appendChild(htdst)
            var htdbt = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('bent'))
                  btn.addEventListener('click',
                      function(event){
                          goHipMOJO('bt')
                          outputs.motorVals.event()
                      })
            htdbt.appendChild(btn)
            mod.hipPos.bt = htdbt
         tr.appendChild(htdbt)
      toptable.appendChild(tr)
   div.appendChild(toptable)

   div.appendChild(document.createTextNode('Bot: '))
      var toptable = document.createElement('table')
         var tr = document.createElement('tr')
            var btdir = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('ir'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('ir')
                          outputs.motorVals.event()
                      })
            btdir.appendChild(btn)
            mod.botPos.ir = btdir
         tr.appendChild(btdir)
            btdie = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('ie'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('ie')
                          outputs.motorVals.event()
                      })
            btdie.appendChild(btn)
            mod.botPos.ie = btdie
         tr.appendChild(btdie)
      toptable.appendChild(tr)
         var tr = document.createElement('tr')
            var btdmr = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('mr'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('mr')
                          outputs.motorVals.event()
                      })
            btdmr.appendChild(btn)
            mod.botPos.mr = btdmr
         tr.appendChild(btdmr)
            btdme = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('me'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('me')
                          outputs.motorVals.event()
                      })
            btdme.appendChild(btn)
            mod.botPos.me = btdme
         tr.appendChild(btdme)
      toptable.appendChild(tr)
         var tr = document.createElement('tr')
            var btdor = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('or'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('or')
                          outputs.motorVals.event()
                      })
            btdor.appendChild(btn)
            mod.botPos.or = btdor
         tr.appendChild(btdor)
            btdoe = document.createElement('td')
               var btn = document.createElement('button')
                  btn.style.margin = 1
                  btn.appendChild(document.createTextNode('oe'))
                  btn.addEventListener('click',
                      function(event){
                          goBotMOJO('oe')
                          outputs.motorVals.event()
                      })
            btdoe.appendChild(btn)
            mod.botPos.oe = btdoe
         tr.appendChild(btdoe)
      toptable.appendChild(tr)
   div.appendChild(toptable)

   div.appendChild(document.createElement('br'))

   var btn = document.createElement('button')
      btn.style.margin = 1
      btn.appendChild(document.createTextNode('send state'))
      btn.addEventListener('click', function(event){
         goCurMOJO()
         outputs.motorVals.event()
      })
   div.appendChild(btn)
   }

//
// local functions
//

function goCurMOJO(){
    if(mod.curTopPos != -1){
        mod.motorVals[0] = mod.posVals[mod.curTopPos][0]
        mod.motorVals[1] = mod.posVals[mod.curTopPos][1]
    }
    if(mod.curBotPos != -1){
        mod.motorVals[2] = mod.posVals[mod.curBotPos][0]
        mod.motorVals[3] = mod.posVals[mod.curBotPos][1]
    }
    if(mod.curHipPos != -1){
        mod.motorVals[4] = mod.posVals[mod.curHipPos]
    }
}

   function goTopMOJO(val){
      mod.motorVals[0] = mod.posVals[val][0]
      mod.motorVals[1] = mod.posVals[val][1]
      mod.motorVals[2] = -1
      mod.motorVals[3] = -1
      mod.motorVals[4] = -1
      if(mod.curTopPos != -1)
        mod.topPos[mod.curTopPos].style.backgroundColor = 'transparent'
      mod.topPos[val].style.backgroundColor = 'red'
      mod.curTopPos = val
   }

   function goBotMOJO(val){
      mod.motorVals[2] = mod.posVals[val][0]
      mod.motorVals[3] = mod.posVals[val][1]
      mod.motorVals[0] = -1
      mod.motorVals[1] = -1
      mod.motorVals[4] = -1
      if(mod.curBotPos != -1)
        mod.botPos[mod.curBotPos].style.backgroundColor = 'transparent'
      mod.botPos[val].style.backgroundColor = 'red'
      mod.curBotPos = val
   }

   function goHipMOJO(val){
      mod.motorVals[4] = mod.posVals[val]
      mod.motorVals[0] = -1
      mod.motorVals[1] = -1
      mod.motorVals[2] = -1
      mod.motorVals[3] = -1
      if(mod.curHipPos != -1)
         mod.hipPos[mod.curHipPos].style.backgroundColor = 'transparent'
      mod.hipPos[val].style.backgroundColor = 'red'
      mod.curHipPos = val
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

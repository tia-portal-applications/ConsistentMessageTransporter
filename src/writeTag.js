/* eslint-disable no-multiple-empty-lines */
/* eslint-disable no-trailing-spaces */
/* eslint-disable eqeqeq */
/* eslint-disable brace-style */

let http = require('http');
const myconvert = require('./convertdata');

const openpipe = require('./Openpipe');
const winccXsltPathAll = __dirname + '/winccValueconfig.xslt';
const xsltPathAll = __dirname + '/configtag.xslt';
let odk = null;

const ServerError = require('./ServerError');
let buFunc = require('./backUpFunc.js');
const serverjs = require('./Server.js');

/******************************************/
//All additional functions for  middleware
/*****************************************/

/**fiter unidentified tagname for wincc classic funtion
* @param {array} arr1 ['wincc1','errortagname','wincc2']
 * @param {array} arr2 ['testSigned32','errortagname','testFloat64']
 * @returns {array} item ['errortagname']
 */
function arrsame(arr1, arr2) {
  return arr1.filter(item => {
    if (arr2.indexOf(item) > -1) {
      return item;
    }
  });
}

/**filter unidentified tagvalue for wincc classic function
 * @param {array} valuearr ['123','error','hello']
 * @param {array} namearr ['testSigned32','errortagname','testText8']
 * @param {array} filterarr ['errortagname']
 * @returns {array} valuearr ['123','hello']
 */
function valuefilter(valuearr, namearr, filterarr) {
  const indicies = [];
  namearr.forEach((element, index) => {
    if (filterarr.includes(element)) {
      indicies.push(index);
    }
  });
  let newIndexs = indicies.map(function(val, idx) { return val - idx; });
  newIndexs.forEach(function(index) {
    valuearr.splice(index, 1);
  });

  return valuearr;
}

/**
 * 
 * @param {IncomingMessage} req 
 * @param {ServerResponse} res 
 * @param {function} next 
 * @param {boolean} Serverflag 
 * @param {string} Branchflag 
 * @param {Array} clientAll 
 * @param {Array<int>} rseq 
 * @param {Record<string, string | number | boolean>} ackvalue 
 * @returns 
 */
exports.writeTag = function(req, res, next, Serverflag, Branchflag, clientAll, rseq, ackvalue) {
  /*//SAP jsondata=[{"TagName":"wincc2","TagValue":"123", }]
  //Opcenter jsondata=[{"variableName":"Order_Tag_4","value":"start"},{"variableName":"Order_Tag_5","value":"liangbai1"}]
  //handshake Opcenter client0 jsondata={"OrderName": "ON00056", "Equipment": "Screwer", "PlannedStartTime": 56421324562, "PlannedEndTime": 56421324562}
  //handshake Opcenter client1 jsondata = {
  //  "EquipmentId": "Equipment_1",
  //  "OrderData": {
  //    "ExecutionGroupid": "EXGR1",
  //    "ParentWorkOrderIds": [
  //      "Order1",
  //      "Order1",
  //      "Order3"
  //    ],
  //    "WorkOrderIds": [
  //      "WO1",
  //      "WO2",
  //      "WO3",
  //      "WO4",
  //      "WO5"
  //    ],
  //    "OperationsIds": [
  //      "OperationIdExample1",
  //      "OperationIdExample2",
  //      "OperationIdExample3"
  //    ],
  //    "SerialNumbers": [
  //      "SN001",
  //      "SN002",
  //      "SN003"
  //    ]
  //  }
  //}*/

  if (Branchflag === '1' && !odk) {
    odk = require('./odk');
  }

  console.log(ackvalue);

  let jsondata;
  let clientid;
  //configure successed response data
  let successres = ServerError.success();
  let errres;
  if (req.headers['content-type'] === 'text/plain') {  //text ruquest convert to json
    jsondata = JSON.parse(req.body);
  } else {
    jsondata = req.body;
  }

  console.log('************************************************************************\n*RestServers receive writedata:');
  console.log(jsondata);

  if (buFunc.handeljsondata(jsondata)[2]) {
    let errmsg = buFunc.handeljsondata(jsondata)[2];
    errres = ServerError.failed(501, errmsg);
    return next(errres);
  }

  //get jsondata keys and value
  let jsonkeys = buFunc.handeljsondata(jsondata)[0];
  let value = buFunc.handeljsondata(jsondata)[1];
  console.log('The all jsondata keys(Tagname)===========', jsonkeys);
  console.log('The all jsondata values(Tagvalue)===========', value);

  // start find webclient
  clientid = clientAll.findIndex(x => x.URL === req.originalUrl);
  // tell if client is found
  if (clientid >= 0)
  {
    console.log();
    console.log('****************The WebClient %d is matched****************', clientid);
  } else {
    errres = ServerError.failed(502, 'Request property matching Templatebody failed');
    return next(errres);
  }
  // end find webclient

  // TODO: tell whether enable handshake
  let tagconfigxml;
  //transfer request tagname to wincc/unified  identified tagname
  //if convertdata.js error,return server internal error
  try {
    tagconfigxml = clientAll[clientid].alltags;
  } catch (error) {
    console.error(error);
    return next(ServerError.failed(500, 'Server Internal Error'));
  }


  //rebuild jsondata like [{name:"tagname",value:"tagvalue"},{name:"tagname",value:"tagvalue"}] for translate value
  let newjsondata = [];
  for (let i in jsonkeys) {
    let newobj = {};
    newobj.name = jsonkeys[i];
    newobj.value = value[i];
    newjsondata.push(newobj);
  }
  console.log('****************The new jsondata  changed to****************', newjsondata);
  //build jsonobj for configtag/winccValueconfig.xslt
  let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
  let subobj = {};
  subobj.tag = newjsondata;
  jsonobj.Tags = subobj;

  //get tagconfigxml placeholder
  let placeholder = [];
  //delete [$i] in each palceholder if it has
  for (let i in tagconfigxml) {
    let str1 = tagconfigxml[i].PlaceHolder;
    if (str1.indexOf('[') !== -1) {
      let slicestr = '[$i]';
      str1 = str1.replace(slicestr, '');
      placeholder.push(str1);
    }
    else {
      placeholder.push(str1);
    }
  }
  //rebuild jsonkeys into jsonkey to match placeholder and get valueindex from jsonkeys
  let jsonkey = [];
  let valueindex;
  let cutstring;
  let subname = {};
  for (let j in jsonkeys) {
    let index = jsonkeys[j].indexOf('.');
    //if there is a '.' in jsonkeys
    if (index != -1) {
      //if there is seconde '.' in jsonkeys
      if (jsonkeys[j].indexOf('.', index + 1) > 0) {
        //find the index of seconde'.'
        let index1 = jsonkeys[j].indexOf('.', index + 1);
        //cutstring=.num(.1 .2 .3 .n)
        cutstring = jsonkeys[j].substring(index, index1);
      }
      else {
        //cutstring=.num(.1 .2 .3 .n)
        cutstring = jsonkeys[j].substring(index);
      }
      //newstring will a string  without .index
      let newstring = jsonkeys[j].replace(cutstring, '');
      subname = { name: newstring };
    }
    else {
      subname = { name: jsonkeys[j] };
    }
    jsonkey.push(subname);
  }
  let jsonkeycopy = [];
  for (let k in jsonkey) {
    jsonkeycopy.push(jsonkey[k].name);
  }

  //get jsonname via tagconfig and jsondata.name which is identified for wincc or unified
  let jsonname = [];
  jsonname = jsonkey.map((item, index) => {
    for (let i in placeholder) {
      if (placeholder[i] == item.name) {
        //if there is a '.'in jsonkeys
        if (jsonkeys[index].indexOf('.') !== -1) {
          let index1 = jsonkeys[index].indexOf('.');
          //if there is seconde'.'in jsonkeys
          if (jsonkeys[index].indexOf('.', index1 + 1) > 0) { //find the index of seconde'.'
            let index2 = jsonkeys[index].indexOf('.', index1 + 1);
            //valueinde=.num(.1 .2 .3 .n)
            valueindex = jsonkeys[index].substring(index1, index2);
          }
          else {
            //valueinde=.num(.1 .2 .3 .n)
            valueindex = jsonkeys[index].substring(index1);
          }
          //replace '$i' into 'num'(0,1,2...n)
          item.name = tagconfigxml[i].Name.replace('$i', valueindex.split('.')[1]);
        }
        else {
          //else dont have to replace
          item.name = tagconfigxml[i].Name;
        }
      }
    }
    return item;
  });

  console.log('transform output jsonname via Tagconfigxml');
  console.log(jsonname); //["tag1","tag2"]

  //*************put following branch function into resetful server !!!!!********************//
  if (Branchflag !== '0')//write tag to wincc or IIH
  {
    //seq tag value +1
    rseq[clientid]++;
    let jsoncopy = [];
    let rebuildjson = [];
    let error = [];
    let writeseqflag = false;
    let odkvalue = [];

    //rebuild jsonname into odk form
    for (let i in jsonkey) {
      jsoncopy.push(jsonkeycopy[i]);
      rebuildjson.push(jsonname[i].name);
    }

    //call arrsame to get unidentified tagname
    let diff = arrsame(rebuildjson, jsoncopy);

    //save diff tagarray into error array
    for (let i = 0; i < diff.length; i++) {
      error.push(diff[i]);
    }
    let odkname = [];
    //function to filter out unidentified tagname
    rebuildjson.forEach((a) => {
      let c = diff.findIndex(b => a === b);
      if (c > -1) delete diff[c];
      else odkname.push(a);
    });

    console.log('fiter out name:');
    console.log(error);

    // //transfer Boolean value into ture or false
    for (let i in odkname) {
      if (odkname[i].search('testBool') !== -1) {
        console.log('start to transfer Boolean value!');
        myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function(jsonvalue) {
          console.log(jsonvalue);
          for (let j in jsonvalue) {
            if (jsonvalue[j].search('true') !== -1 || jsonvalue[j].search('false') !== -1) {
              value[j] = jsonvalue[j];
            }
          }
        });
      }
    }

    odkvalue = valuefilter(value, jsoncopy, error);
    console.log('The identified webrequest name:');
    console.log(odkname);
    console.log('The identified webrequest value');
    console.log(odkvalue);
    if (error.length > 0) {
      let errmsg = 'unidentified Property:' + '[' + error + ']';
      errres = ServerError.failed(503, errmsg);
      return next(errres);
    }
    //if no usecase3 write tag into wincc and return
    if (Serverflag == false) {
      // eslint-disable-next-line no-shadow
      odk.writetag(odkname, odkvalue, function(res, err) {
        if (err) {
          errres = ServerError.failed(800, err);
          return next(errres);
        }
        //else return secceeded=true
        return next(successres);
      });
    }
    else { //usecase3
      if (Branchflag === '2')//write tag into IIH
      {
        let iihids = [];
        let iihbody = [];
        let errary = [];
        //get corresponding IIHvariable IDs from started up process
        console.log(variableids);
        for (let i in odkname) {
          for (let j in variablereq) {
            for (let k in variablereq[j]) {
              if (odkname[i] ==  variablereq[j][k].variableName) {
                iihids.push(variableids[j][k]);
                errary.push(jsonkeys[i]);
              }
            }
          }
        }
        console.log('The corresponding IIHvariable IDs are:', iihids);
        //error handling for iih get variablename filed
        if (iihids.length == 0) {
          errres = ServerError.failed(506, 'All properties matches variable name in assets.xml failed');
          return next(errres);
        }
        else if (iihids.length < odkname.length) {
          let mismatch = jsonkeys.concat(errary).filter(function(v, i, arr) {
            return arr.indexOf(v) === arr.lastIndexOf(v);
          });
          errres = ServerError.failed(506, ' properties: [' + mismatch + '] matches variable name in assets.xml failed');
          return next(errres);
        }

        // if no error,configure the request body and option
        for (let i in iihids) {
          let iihobj = {'variableId': iihids[i],
            'values': [
              {'timestamp': buFunc.getRealTime(),
                'value': odkvalue[i],
                'qualitycode': 0
              }
            ]
          };
          iihbody.push(iihobj);
        }
        console.log('The request post body to IIH are:', JSON.stringify(iihbody));
        let reqoption = {
          hostname: '10.31.1.2',
          port: 4203,
          method: 'POST',
          path: '/Dataservice/Data',
          headers: {
            'content-Type': 'Application/json',
            'content-Length': JSON.stringify(iihbody).length
          }
        };
        // create request
        let iihreq = http.request(reqoption, function(resdata) {
          let data = '';
          let dataobj = {};
          //lsten data
          resdata.on('data', function(chunk) {
            data += chunk;
          });
          //listen to the end and return data
          resdata.on('end', function() {
            dataobj = JSON.parse(data);
            console.log(dataobj);
            //console.log('The received data from IIH and sent to webclient data is:', JSON.parse(data));
            if (dataobj.success ) {
            //return secceeded=true
              return next(successres);
            }//JSON.stringify(errary).replace(/"/g, '')
            let iiherrmsg = JSON.stringify(dataobj.debugInfo.description).replace(/\\"/g, '');
            errres = ServerError.failed(901, JSON.parse(iiherrmsg));
            return next(errres);
          });
        });
        //write body
        iihreq.write(JSON.stringify(iihbody));
        iihreq.on('error', (e)=>{
          errres = ServerError.failed(900, e.message);
          return next(errres);
        });
        iihreq.end();
        //if listen error, retrun reject
      }
      else {//handshake for wincc, write seq and wait ack
        // eslint-disable-next-line no-shadow
        odk.writetag(odkname, odkvalue, function(res, err) {
          console.log(res);
          if (err) {
            errres = ServerError.failed(800, err);
            return next(errres);
          }
        });
        writeseqflag = clientAll[clientid].readseq !== undefined;
        //no error then start to write seq and wait ack
        if (writeseqflag) {
          //configure seqtag and seqvalue
          let count = 1;
          const readseq = clientAll[clientid].readseq;
          //start first time writting seqvalue into wincc
          // eslint-disable-next-line no-shadow
          odk.writetag([readseq], [rseq[clientid]], function(res, err) {
            console.log(res);
            if (err) {
              errres = ServerError.failed(800, err);
              return next(errres);
            }
            console.log('first writting seq finished! waiting for ack sync');
          });
          //wait 5s to get ack resoponse
          // if ack==seq, return handshake success ;else wirte two more times seq
          //after 3 times writting,ack still not sync, return handshake failed
          let timer = setInterval(function() {
            const neededAckValue = Object.entries(ackvalue).find(x => x[0] === clientAll[clientid].suback);
            if (neededAckValue && neededAckValue[1] == rseq[clientid]) {
              console.log('Webclient %d ack value is:%d, seq value is:%d, handshake success!', clientid, neededAckValue[1], rseq[clientid]);
              //end loop
              clearInterval(timer);
              //return successres
              return next(successres);
            }

            //Read the new ackvalue
            const aux_ = serverjs[clientid];
            console.log(ackvalue);
            console.log(aux_);

            if (count == 15) {
              //end loop
              clearInterval(timer);
              console.log('15s passed, %d times writting seq finished,ack still not equals to seq', count);
              errres = ServerError.failed(801, 'handshake failed');
              return next(errres);
            }

            console.log('5s passed, ack still not synced, start %d times writting seq', count + 1);
            count++;
          }, 1000);//end timer
        }//end writeseqflag
        else {
          console.log('no SequenceTag found -> no handshake, value written only');
          return next(successres);
        }
      }//end else
    }//end else
  }//end if
  else { //write tag to unified
    myconvert.convertjsonviaxsl(jsonobj, xsltPathAll, function(jsonvalue) {  //change body value via xslt
      console.log('transform output jsonvalue via xslt');
      console.log(jsonvalue);                     //the changed write value  jsonvalue=[ '1', '0', '300' ]
      myconvert.convertw(jsonname, jsonvalue, function(wcommandstr) { //change xml name and xslt value output openpipte datastring
        openpipe.writetag(wcommandstr, function(reserr, resdata) {   //get the write resualt via openpipe
          //build the response data
          if (reserr.length == 0 || reserr == '') {
            return next(successres);
          }

          if (typeof(reserr) == 'string') {
            errres = ServerError.failed(600, reserr);
            return next(errres);
          }
          let errmsg = [];
          for (let i in reserr) {
            if (reserr[i].ErrorCode == 2148545551) {
              let errvalue = value.filter((item, index)=>{
                if (reserr[i].Name == jsonname[index].name) {
                  return item;
                }
              });
              errmsg.push({value: errvalue[0], error: reserr[i].ErrorDescription});
            }
            else {
              errmsg.push({variable: reserr[i].Name, error: reserr[i].ErrorDescription});
            }
          }
          errres = ServerError.failed(600, errmsg);
          return next(errres);
        });
      });
    });
  }//end else */
};

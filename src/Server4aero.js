
/* eslint-disable eqeqeq */
/* eslint-disable brace-style */
/* start this server Example
REM example1: connect OpCenter to IIH
node Server.js --port=12345  --southboundservice=iih --serverconfig=opclient --clientconfig=iih2opcenter  --pollingcycle=1000 --log=verbose/error/info

REM example2: connect OpCenter to WinCC Unified
node Server.js --port=23456 --southboundservice=unified --clientconfig=unified2opcenter --log=verbose/error/info

REM example3: connect OpCenter to WinCC V7.5
node Server.js --port=34567 --southboundservice=classic  --clientconfig=wincc2opcenter --log=verbose/error/info

REM example4: connect OpCenter to WinCC V7.5(enable handshake )
node Server.js --port=34567 --southboundservice=classic --serverconfig=opclient --clientconfig=wincc2opcenter --log=verbose/error/info
*/

let express = require('express');
let app = express();
let https = require('https');
let fs = require('fs');

const {URL} = require('url');
const myconvert = require('./convertdata');
const openpipe = require('./Openpipe');
const RestServersOpcenterxmlpath = __dirname + '/RestServerConfiguration_OpCenter.xml';
const tagxmlpath = __dirname + '/TagConfiguration.xml';
const wincctagxmlpath = __dirname + '/WinccTagConfiguration.xml';
const iihjsondata = require('./dataservice-backup-config.json');
const iihtagxmlpath = __dirname + '/IIHTagConfiguration.xml';
const xsltPathAll = __dirname + '/configtag.xslt';
const winccXsltPathAll = __dirname + '/winccValueconfig.xslt';
const util = require('util');
const  exec = util.promisify(require('child_process').execFile);
//const odk = require('./odk');
const { argv } = require('process');
//let fork = require('child_process').fork;
//let odksubscribe = fork('./odksubscribe.js');
const ServerError = require('./ServerError');
const log4js = require('./log4jsconfig.js');
const jwt = require('express-jwt');

const logger = log4js.getLogger();
const logger2 = log4js.getLogger('Error');


/*********************************/
//Rest Server
/********************************/

//Set flag for initialization
let Serverflag = false;
let Branchflag = '0';
let tagconfigflag = '0';
let logflag = 'verbose';
let port = 23456;
let delay = 1000;
// parse arguments passed by the user
for (let i = 2; i < argv.length; i++) {
  const [key, value] = process.argv[i].split('=');
  switch (key.toLowerCase()) {
  case '--southboundservice':
    if (value.toLowerCase() === 'unified') {
      Branchflag = '0';
    }
    else if (value.toLowerCase() === 'classic') {
      Branchflag = '1';
    }
    else if (value.toLowerCase() === 'iih') {
      Branchflag = '2';
    }
    break;
  case '--port':
    port = value.toLowerCase();
    break;
  case '--clientconfig':
    if (value.toLowerCase() === 'unified2opcenter') {
      tagconfigflag = '0';
    }
    else if (value.toLowerCase() === 'wincc2opcenter') {
      tagconfigflag = '1';
    }
    else if  (value.toLowerCase() === 'iih2opcenter') {
      tagconfigflag = '2';
    }

    break;
  case '--serverconfig':
    Serverflag = value.toLowerCase() === 'opclient';
    break;
  case '--log':
    if (value.toLowerCase() === 'error') {
      logflag = 'error';
    }
    else if (value.toLowerCase() === 'info') {
      logflag = 'info';
    }
    else if (value.toLowerCase() === 'verbose') {
      logflag = 'verbose';
    }
    break;
  case '--pollingcycle':
    delay = value.toLowerCase();
    break;
  default:
    break;
  }
}
//rewrite console function
console.log = function() {
  if (logflag == 'info' || logflag == 'verbose') {
    return logger.info.apply(logger, arguments);
  }
};
console.error = function() {
  if (logflag == 'error' || logflag == 'verbose') {
    return logger2.error.apply(logger2, arguments);
  }
};
//https server options
const serverOptions = {
  key: fs.readFileSync(__dirname + '/certificate/server.key'),
  ca: [fs.readFileSync(__dirname + '/certificate/cert.pem')],
  cert: fs.readFileSync(__dirname + '/certificate/server.crt'),
  requestCert: true
};
// start https server
if (port == 12345) {
  console.log('*************************************************************************');
  console.log('start server connectes to IIH,port is %d', port);
  console.log('*************************************************************************');
}
else if (port == 23456) {
  console.log('*************************************************************************');
  console.log('start server connectes to Unified,port is %d', port);
  console.log('*************************************************************************');
}
else if (port == 34567) {
  console.log('*************************************************************************');
  console.log('start server connectes to WinCC V7.5,port is %d', port);
  console.log('*************************************************************************');
}
else {
  port = 23456;
  console.log('start default server connects to Unified, port is %d', port);
}
https.createServer(serverOptions, app).listen(port); //start the server and listen at port
/******************************************/
//local test codes for our server
/*****************************************/
app.use(express.urlencoded({ extended: false }));
app.use(express.text());
app.use(express.json());

const authcert = fs.readFileSync(__dirname + '/certificate/server.crt');
const rurl = '/WinCCRestService/readtags';
const wurl = '/WinCCRestService/TagManagement/Values';
const surl = '/subscribe';
const burl = '/browseTags';

/******************************************/
//All additional functions for  middleware
/*****************************************/

/**
 * readhandlejsonobj function specfy the jsonobj into app(rurl)
 * @param {Object} json {"variableName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
 * @returns [value, err]; value:[ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ],err:different err obj
 */
let readhandlejsonobj = function(json) {
  let value = [];
  let err;
  //first tell if json is JSON Object ,array is not acceptiable
  if (Object.prototype.toString.call(json) !== '[object Object]') {
    err = {errcode: 504, errmsg: 'JSON body should be a JSON Object'};
  } else {
    //get keys
    keys = Object.keys(json);
    //no keys is not acceptiable
    if (keys.length === 0) {
      err = {errcode: 504, errmsg: 'property lacks'};
    }
    //has one key
    else if (keys.length === 1) {
      //tell if key matchs defaut
      if (keys[0] == 'variableName' || keys[0] == 'TagName') {
        //tell if parameter is array
        if (Object.prototype.toString.call(json[keys]) === '[object Array]') {
          //tell if no parameter
          if (json[keys].length == 0) {
            err = {errcode: 504, errmsg: 'parameter lacks'};
          }
          // get  keyvalue(tagname for wincc or unified)
          value = json[keys];
        }
        // parameter not a arry, reject
        else {
          err = {errcode: 504, errmsg: 'parameter has to be a JSON Array'};
        }
      }
      //else not accept
      else {
        err = {errcode: 504, errmsg: 'unaccpetiable property: ' + keys[0] };
      }
    }
    //has more key is not acceptiable
    else {
      err = {errcode: 504, errmsg: 'unaccpetiable for more than one property'};
    }
  }
  return [value, err];
};

/**the function deal with different type of jsondata
 * @param {obj} jsondata different type of jsondata from client
 * @returns {array} [keys,value,err] handeljsondata(jsondata)[0] is key of jsondata,handeljsondata(jsondata)[1] is value of jsondata,handeljsondata(jsondata)[2] is err msg
 */
function handeljsondata(json) {
  let value = [];
  let errary = [];
  let err = '';
  //keys=[ 'EquipmentId', 'OrderData' ]
  keys = Object.keys(json);
  //case1: jsondata is a array
  if (Object.prototype.toString.call(json) === '[object Array]') {
    let arrayvalue = [];
    for (let i in json) {
      let arraykey = Object.keys(json[i]);
      if (arraykey.length !== 2) {
        errary.push(json[i]);
      }
      for (let j in arraykey) {
        arrayvalue.push(json[i][arraykey[j]]);
      }
      //keys equal to even number of array elements
      keys = arrayvalue.filter((item, index) => index % 2 == 0);
      //value equal to odd number of array elements
      value = arrayvalue.filter((item, index) => index % 2 !== 0);
    }
    if (errary.length != 0) {
      err = 'Object: ' + JSON.stringify(errary).replace(/"/g, '') + ', error:should has two properties.';
    } else if (json.length == 0) {
      err = 'property lacks';
    }
  }
  //case2:jsondata.key is a array
  else if (Object.prototype.toString.call(json[keys]) === '[object Array]') {
    keys = json[keys];
  }
  else {//case3:jsondata is just a object
    keys = [];
    handleobj(json, 'jsondata', '.', keys);
  }
  //the handleobj function serch for jsonobj tree
  function handleobj(jsons, name, sign, keys) {
    for (let key in jsons) {
      let path = name + sign + key;
      //if not a obj, put it into keys[] and value[]
      if (!(jsons[key] instanceof Object)) {
        value.push(jsons[key]);
        keys.push(path);
      }
      else {
        //if a obj, recursion
        handleobj(jsons[key], path, sign, keys);
      }
    }
  }
  //function substring change path into key which without 'jsondata.' and 'jsondata.first propety'
  function subString(item) {
    function find(str, cha, num) {
      let x = str.indexOf(cha);
      for (let i = 0; i < num; i++) {
        x = str.indexOf(cha, x + 1);
        if (x == -1) {
          return x = str.indexOf(cha);
        }
      }
      return x;
    }
    let index = find(item, '.', 1);
    return item.substring(index + 1);
  }
  keys = keys.map(subString);
  //return new array[key,value]
  return [keys, value, err];
}
/**fiter unidentified tagname for wincc classic funtion
* @param {array} arr1 ['wincc1','errortagname','wincc2']
 * @param {array} arr2 ['testSigned32','errortagname','testFloat64']
 * @returns {array} item ['errortagname']
 */
arrsame = (arr1, arr2) => {
  return arr1.filter(item => {
    if (arr2.indexOf(item) > -1) {
      return item;
    }
  });
};
/**filter unidentified tagvalue for wincc classic function
 * @param {array} valuearr ['123','error','hello']
 * @param {array} namearr ['testSigned32','errortagname','testText8']
 * @param {array} filterarr ['errortagname']
 * @returns {array} valuearr ['123','hello']
 */
valuefilter = (valuearr, namearr, filterarr) => {
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
};

/**
 *function for get realtime
 * @param {string/number/null} n 1 or'1h'or null or 0.1
 * @returns ISO Date realtime or n hours before realtime
 */
function getRealTime(n) {
  let timestamp = new Date().getTime();
  let Rt;
  //if there is daygap
  if (n) {
    //Rt is n hours ago
    Rt = new Date(timestamp - 60 * 60 * 1000 * parseFloat(n, 10)).toISOString();
  } else { //get realtime
    Rt = new Date(timestamp).toISOString();
  }
  return Rt;
}

/**
 * the connetion and get Token from IIH
 * @returns promise obj token
 */
function gettokenfromIIH() {
  //set client option
  let options = {
    cert: fs.readFileSync(__dirname + '/certificate/certificate.crt'),
    hostname: '10.120.6.63',
    method: 'GET',
    path: '/device/edge/a.service/api/v1/auth/token?grant_type=password&username=avc@siemens.com&password=Siemens1!&client_id=d6b80eb290eb4820b2bab0a19824c532',
    rejectUnauthorized: false,
    headers: {
    }
  };
    //use promise to synchronous
  return new Promise(function(resolve, reject) {
    //start to enable https request
    let req = https.request(options, function(res) {
      let data = '';
      //listen data
      res.on('data', function(chunk) {
        data += chunk;
      });
      //listen to the end and return data
      res.on('end', function() {
        data = JSON.parse(data);
        if (data.access_token) {
          let token = data.access_token;
          resolve(token);
        }
        else {
          let errmsg = 'get token failed';
          reject(errmsg);
        }
      });
    });
      //write body
      //req.write(body);
    req.end();
    //if listen error, retrun reject
    req.on('error', function(error) {
      reject(error);
    });
  });
}
/**
 * @param {string} url IIH URL
 * @param {string} token the token get from the response message of function gettokenfromIIH()
 * @param {obj} callback return response
 */
function getdatafromIIH(url, token, callback) {
  //set client option
  let options = {
    cert: fs.readFileSync(__dirname + '/certificate/certificate.crt'),
    hostname: '10.120.6.63',
    method: 'GET',
    path: url,
    rejectUnauthorized: false,
    headers: {
      'Cookie': 'authToken=' + token
    }
  };
    //start to enable https request
  let req = https.request(options, function(res) {
    let data = '';
    //listen data
    res.on('data', function(chunk) {
      data += chunk;
    });
    //listen to the end and return data
    res.on('end', function() {
      callback(data);
    });
  });
  req.end();
  //if listen error, retrun reject
  req.on('error', function(error) {
    console.error(error);
  });
}
/**
 * @param {string} token the token get from the response message of function gettokenfromIIH()
 * @param {obj} body JSON body message
 * @param {obj} callback return response
 */
function postdata2iih(token, body, callback) {
  let reqoption = {
    cert: fs.readFileSync(__dirname + '/certificate/certificate.crt'),
    hostname: '10.120.6.63',
    method: 'POST',
    path: '/Dataservice/Data',
    rejectUnauthorized: false,
    headers: {
      'Cookie': 'authToken=' + token,
      'content-Type': 'Application/json',
      'content-Length': JSON.stringify(body).length
    }
  };
  // create request
  let iihreq = https.request(reqoption, function(resdata) {
    let data = '';
    let err = '';
    let dataobj;
    //listen data
    resdata.on('data', function(chunk) {
      data += chunk;
    });
    //listen to the end and return data
    resdata.on('end', function() {
      dataobj = JSON.parse(data);
      if (dataobj.success ) {
      //return secceeded=true
        callback(dataobj, err);
      }
      else {
        err = JSON.stringify(dataobj.debugInfo.description).replace(/\\"/g, '');
        callback(dataobj, err);
      }
    });
  });
  //write body
  iihreq.write(JSON.stringify(body));
  iihreq.on('error', (e)=>{
    console.error(new ServerError(900, e.message));
  });
  iihreq.end();
}
/**
 * @param {obj} options the clientoption based on https
 * @param {*} body the request JSON body
 * @returns
 */
function opcenterconnect(options, body) {
  //use promise to synchronous
  return new Promise(function(resolve, reject) {
    //start to enable http request
    let req = https.request(options, function(res) {
      let data = '';
      //listen data
      res.on('data', function(chunk) {
        data += chunk;
      });
      //listen to the end and return data
      res.on('end', function() {
        resolve(data);
      });
    });
      //write body
    req.write(body);
    req.end();
    //if listen error, retrun reject
    req.on('error', function(error) {
      reject(error);
    });
  });
}


//verify token from Server.crt unless '/*','/subscribe', '/browseTags' url
app.use(jwt({ secret: authcert, algorithms: ['RS256'] })
  .unless({ path: ['/*', '/subscribe', '/browseTags'] }));
/******************************************/
//start write tag from opcenter
/*****************************************/
//test for connection and Certificate
app.get('/*', function(req, res) {
  res.send('trusted page');
});
/******************************************/
//write tag from opcenterclient
/*****************************************/
app.put(wurl,  function(req, res, next) {
  //SAP jsondata=[{"TagName":"wincc2","TagValue":"123", }]
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
  //}
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

  if (handeljsondata(jsondata)[2]) {
    let errmsg = handeljsondata(jsondata)[2];
    errres = ServerError.failed(501, errmsg);
    return next(errres);
  }
  //get jsondata keys and value
  let jsonkeys = handeljsondata(jsondata)[0];
  let value = handeljsondata(jsondata)[1];
  console.log('The all jsondata keys(Tagname)===========', jsonkeys);
  console.log('The all jsondata values(Tagvalue)===========', value);

  //first tell whether enable handshake
  if (Serverflag == true) {
  // start to match webclient templatebody
    let tempbody = [];
    let tempkey = [];
    for (let i in serverAll) {
      tempbody.push(JSON.parse(serverAll[i].templatebody));
      tempkey.push(handeljsondata(tempbody[i])[0]);
      if (tempkey[i].every(item => jsonkeys.includes(item)) == true) {
        clientid = parseInt(i, 10);
      }//end if
      //iih case
      else if (Branchflag == '2') {
        if (jsonkeys.every(item => tempkey[i].includes(item)) == true) {
          clientid = parseInt(i, 10);
        }//end if
      }
    } //end for
    // tell if client is matched
    if (clientid !== undefined)
    {
      console.log();
      console.log('****************The WebClient %d is matched****************', clientid);
    }
    //if all webclients match jsondata failed, res.end(error)
    else {
      errres = ServerError.failed(502, 'Request property matching Templatebody failed');
      return next(errres);
    }
  }
  else {//do not enable handshake,match ip in tagconfig.xml instead
    //get the write client , get IP address
    for (let i in clientAll) {
      if (clientAll[i].URL === req.url) {
        clientid = parseInt(i, 10);
      }
    }
    //test 1
    clientid = parseInt(0, 10);
    console.log('****************The WebServer %d is matched****************\n', clientid);
  }
  let tagconfigxml;
  //transfer request tagname to wincc/unified  identified tagname
  //if convertdata.js error,return server internal error
  if (Serverflag == true) {
    try {
      tagconfigxml = serverAll[clientid].alltags;
    } catch (error) {
      console.error(error);
      return next(ServerError.failed(500, 'Server Internal Error'));
    }
  }
  else {
    try {
      tagconfigxml = clientAll[clientid].alltags;
    } catch (error) {
      console.error(error);
      return next(ServerError.failed(500, 'Server Internal Error'));
    }
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
  console.log('transferm output jsonname via Tagconfigxml');
  console.log(jsonname); //["tag1","tag2"]
  //*************put following branch function into resetful server !!!!!********************//
  if (Branchflag !== '0')//write tag to wincc or IIH
  {
    //seq tag value +1
    rseq[clientid] = rseq[clientid] + 1;
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
    //if no serverflag write tag into wincc and return
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
    else {// has serverflag enable handshake with wincc or connect to IIH
      if (Branchflag === '2') //connect to IIH
      {
        let iihids = [];
        let iihbody = [];
        let errary = [];
        //get corresponding IIHvariable IDs from started up process
        for (let i in odkname) {
          for (let j in variablenames) {
            if (odkname[i] ==  variablenames[j]) {
              iihids.push(variableids[j]);
              errary.push(jsonkeys[i]);
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
              {'timestamp': getRealTime(),
                'value': odkvalue[i],
                'qualitycode': 0
              }
            ]
          };
          iihbody.push(iihobj);
        }
        console.log('The request post body to IIH are:', JSON.stringify(iihbody));
        //first get token from IIH
        let gettokenofIIH = async function() {
          let IIHtoken = await gettokenfromIIH();
          return IIHtoken;
        };
        //main function to call IIH connetion and create asset.
        gettokenofIIH().then((IIHtoken)=>{
        // configurer request option
          let reqoption = {
            cert: fs.readFileSync(__dirname + '/certificate/certificate.crt'),
            hostname: '10.120.6.63',
            method: 'POST',
            path: '/Dataservice/Data',
            rejectUnauthorized: false,
            headers: {
              'Cookie': 'authToken=' + IIHtoken,
              'content-Type': 'Application/json',
              'content-Length': JSON.stringify(iihbody).length
            }
          };
          // create request
          let iihreq = https.request(reqoption, function(resdata) {
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
        });
      }
      else { //handshake for wincc, write seq and wait ack
        // eslint-disable-next-line no-shadow
        odk.writetag(odkname, odkvalue, function(res, err) {
          console.log(res);
          if (err) {
            errres = ServerError.failed(800, err);
            return next(errres);
          }
          //else enable handshake
          writeseqflag = true;
        });
        //no error then start to write seq and wait ack
        if (writeseqflag == true) {
          //configure seqtag and seqvalue
          let count = 1;
          let readseq = clientAll[clientid].readseq;
          let seqname = [];
          let seqvalue = [];
          seqname.push(readseq);
          seqvalue.push(rseq[clientid]);
          //start first time writting seqvalue into wincc
          // eslint-disable-next-line no-shadow
          odk.writetag(seqname, seqvalue, function(res, err) {
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
            if (ackvalue[clientid] == seqvalue) {
              console.log('Webclient %d ack value is:%d, seq value is:%d, handshake success!', clientid, ackvalue[clientid], seqvalue);
              //end loop
              clearInterval(timer);
              //return successres
              return next(successres);
            }

            if (count == 3) {
              //end loop
              clearInterval(timer);
              console.log('15s passed, %d times writting seq finished,ack still not equals to seq', count);
              errres = ServerError.failed(801, 'handshake failed');
              return next(errres);
            }

            console.log('5s passed, ack still not synced, start %d times writting seq', count + 1);
            odk.writetag(seqname, seqvalue, function(response) {
              count++;
            });
          }, 5000);//end timer
        }//end writeseqflag
      //end else
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
  }//end else
});
/******************************************/
//read tag from opcenterclient
/*****************************************/
app.put(rurl, function(req, res, next) {
  //Opcenter jsondata={"variableName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
  //SAP jsondata= {"TagName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
  let jsondata;
  let resput = [];
  let tem = '';
  let clientid = 0;
  let errres;
  if (req.headers['content-type'] === 'text/plain') {  //convert the text to json
    jsondata = JSON.parse(req.body);
  } else {                                           //json baody
    jsondata = req.body;
  }

  console.log('************************************************************************\n*RestServers receive readdata:');
  console.log(jsondata);
  console.log('*RestServers receive readdata\n************************************************************************');
  let error = readhandlejsonobj(jsondata)[1];
  if (error !== undefined) {
    errres = ServerError.failed(error.errcode, error.errmsg);
    return next(errres);
  }
  //get jsondata keys
  let jsonkeys = readhandlejsonobj(jsondata)[0];
  console.log('The all jsondata keys(Tagname)===========', jsonkeys);


  //get the write client , get IP address
  for (let i in clientAll) {
    if (clientAll[i].URL === req.URL) {
      clientid = i;
    }
  }

  //test 1
  clientid = 0;

  let tagconfigxml;
  //transfer request tagname to wincc/unified  identified tagname
  //if convertdata.js error,return server internal error
  try {
    tagconfigxml = clientAll[clientid].alltags;
  // eslint-disable-next-line no-shadow
  } catch (error) {
    console.error(error);
    return next(ServerError.failed(500, 'Server Internal Error'));
  }

  //transfer request tagname to wincc/unified identified tagname
  let jsonname = [];
  for (let i in jsonkeys) {
    jsonname[i] = jsonkeys[i];
  }
  //the changed read name
  for (let i in jsonkeys) {
    for (let j in tagconfigxml) {
      if (jsonkeys[i] === tagconfigxml[j].PlaceHolder) {
        jsonname[i] = tagconfigxml[j].Name;
      }
    }
  }
  console.log('transform output jsonname via Tagconfigxml');
  console.log(jsonname);    //jsonname=['abcd''testsigned32','HMI_Tag_5']

  if (Branchflag !== '0') {//read tag from wincc
    console.log('read tag from wincc');
    odk.readtag(jsonname, function(resdata, err) {
      if (err) {
        errres = ServerError.failed(406, err);
        return next(errres);
      }
      //build the response data
      for (let i in jsonkeys) {
        tem = {name: jsonkeys[i], value: '' + resdata.Value[i] + ''};
        resput.push(tem);
      }
      //build jsonobj1 for configtag/winccValueconfig.xslt
      let jsonobj1 = {};    //jsonobj1={ Tags: { tag: [ [Object], [Object], [Object] ] } }
      let subobj1 = {};
      subobj1.tag = resput;
      jsonobj1.Tags = subobj1;

      // reverse Boolean value ture or false into sever identified boolean
      for (let i in jsonname) {
        if (jsonname[i].search('testBool') !== -1) {
          console.log('start to reverse Boolean value!');
          myconvert.convertjsonviaxsl(jsonobj1, winccXsltPathAll, function(jsonvalue) {
            resput[i].value = jsonvalue[i];
          });
        }
      }
      res.send(resput);
    });//end odk.readtag()
  }//end if branch
  else {// read tag from unified
    console.log('read tag from unified');
    let datar = JSON.stringify(jsonname); //var datar = '["abcd","HMI_Tag_4","HMI_Tag_5"]';
    openpipe.readtag(datar, function(reserr, resdata) {   //get the read resualt
      //build the response data
      if (reserr.length == 0 || reserr == '') {
        res.statusCode = 200;

        for (let i in jsonkeys) {
          //tem +='{"variableName":'+'"'+jsondata[i].variableName+'"'+',"dataType":'+'"1"'+',"value":'+'"'+resdata[i].Value+'"'+',"timestamp":'+'"'+resdata[i].TimeStamp+'"'+',"qualitycode":'+'"'+resdata[i].QualityCode+'"'+',"dataType":"errorcode":'+'"'+resdata[i].ErrorCode+'"},';
          tem = {variableName: jsonkeys[i], dataType: 1, value: resdata[i].Value, timestamp: resdata[i].TimeStamp, qualitycode: resdata[i].QualityCode, errorcode: resdata[i].ErrorCode};
          resput.push(tem);
        }
        //resdata="["+tem.substr(0,tem.length-1)+"]";
        console.log(resput);
        res.send(JSON.stringify(resput));            //send the response body
      }
      else {
        if (typeof(reserr) == 'string') {
          errres = ServerError.failed(601, reserr);
          return next(errres);
        }
        let errTag = [];
        for (let i in reserr) {
          errTag.push(reserr[i].Name);
        }
        let errmsg = {variable: errTag, error: reserr[0].ErrorDescription};
        errres = ServerError.failed(601, errmsg);
        return next(errres);
      }
    });
  }//end else
});

/******************************************/
//subscribe tag from opcenterclient
/*****************************************/
app.put(surl, function(req, res) {
  let jsondata;
  let resput = [];
  let tem = '';

  if (req.headers['content-type'] === 'text/plain') {  //convert the text to json
    jsondata = JSON.parse(req.body);
  } else {                                           //json data
    jsondata = req.body;
  }
  //put body {"variableNames" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
  console.log('************************************************************************\n*RestServers receive readdata:');
  console.log(jsondata);
  console.log('*RestServers receive readdata\n************************************************************************');
  //convert the data,get the openpope data formate
  myconvert.converts(jsondata, function(commandtext) {
    //var datar = '[{"Name":"HMI_Tag_4","Value":"50"},{"Name":"HMI_Tag_5","Value":"40"}]';
    let datar = commandtext;
    openpipe.subscribetag(datar, function(reserr, resdata) {   //call subscribetag to get the subscribe resualt
      if (reserr === '') {
        // openpipe.unsubscribetag();
        res.statusCode = 200;

        for (let i in jsondata.variableNames) {                  //build the tag name, error code ...
          tem = {variableName: jsondata.variableNames[i], dataType: 1, value: resdata[i].Value, timestamp: resdata[i].TimeStamp, qualitycode: resdata[i].QualityCode, errorcode: resdata[i].ErrorCode};
          resput.push(tem);
        }

        console.log(resput);
        res.send(JSON.stringify(resput));           //send the response body
      } else {                                        //if openpipe subscribe error,send the error code
        //console.log(reserr);
        res.send(JSON.parse(reserr));
      }
    });
  });
});
///******************************************/
//browser tag from opcenterclient
/*****************************************/
app.put(burl, function(req, res) {
  let jsondata;

  if (req.headers['content-type'] === 'text/plain') {  //convert text to json
    jsondata = JSON.parse(req.body);
    //console.log(JSON.parse(req.body));
    //console.log(jsondata.length);
    //console.log(jsondata[0].variableName);
  } else {                                           //json data
    jsondata = req.body;
    //console.log(req.body);
  }
  console.log('************************************************************************\n*RestServers browseTags receive data:');
  console.log(jsondata);
  console.log('*RestServers browseTags receive data\n************************************************************************');

  //convert the data, get the openpipe needed data formate
  myconvert.convertb(jsondata, function(commandtext) {
    let datab = commandtext;
    openpipe.browseTags(datab, function(reserr, resdata) {   //call the brossetags to get browser tags
      if (reserr === '') {
        res.statusCode = 200;

        console.log(resdata);
        res.send(resdata);            //send the response body
      } else {                       //if the openpipe browser error,send the error code
        //console.log(reserr);
        res.send(JSON.parse(reserr));
      }
    });
  });
});

//app next middleware send response to opcenterclient */
app.use(function(err, req, res, next) {
  if (err.status == 400) {
    err = ServerError.failed(400, 'Webclient request error');
  }
  if (err.name === 'UnauthorizedError') {
    err = ServerError.failed(401, 'token recognized failed');
  }
  let resdata = {
    '@odata.context': req.protocol + '://' + req.get('host') + req.originalUrl,
    'Succeeded': err.status,
    'Error': { Errorcode: err.code, Errormessage: err.msg}
  };
  if (resdata.Succeeded == 'true') {
    console.log(JSON.stringify(resdata));
  }
  else {
    console.error(JSON.stringify(resdata));
  }
  res.end(JSON.stringify(resdata));
});


/*********************************/
//Rest Client
/********************************/

//Creat bearertoken from certificate
const authcertificatepath = __dirname + '/certificate/opcenter_cert.pfx';
const authcertificatePassword = fs.readFileSync(__dirname + '/authcertificatepassword.txt');
const authcreatetokenpath = __dirname + '/CreateTokenFromCertificate_V1.0/CreateTokenFromCertificate.exe';
let bearerToken;


exec(authcreatetokenpath, ['certificate_path=' + authcertificatepath, 'password=' + authcertificatePassword ], function(error, stdout, stderr) {
  if (error) {
    console.error('Creating a bearer token failed:' + stderr);
    return error.code;
  }
  bearerToken = stdout.trim();
  console.log('The Token for Opcenter_Server is:', bearerToken);
  clientOptions.headers.Authorization = 'Bearer ' + bearerToken;
});
//the client options

let clientOptions = {
  hostname: '192.168.0.102',
  port: 9001,
  path: '/api/tags/OnChange3',
  method: 'POST',
  cert: fs.readFileSync(__dirname + '/certificate/opcenter_cert.pem'),
  rejectUnauthorized: false,
  requestCert: true,
  //agent:false
  headers: {
    //'username': 'admin',
    //'password': '123456',
    'Authorization': 'Bearer ' + bearerToken,
    //'Cookie': 'locale=zh_CN',
    //'X-BuildTime': '2015-01-01 20:04:11',
    //'Autologin': '4',
    //'Accept-Encoding': 'gzip, deflate',
    //'X-Timeout': '3600000',
    'Content-Type': 'Application/json'   //"text/plain"//'Application/json',
    //"Content-Length":reqdata.length
  }
};

/******************************************/
//start subscribe tag from TagConfig.xml
/*****************************************/

let num = 0;
let num1 = 0;             //The number of client in tagconfiguration.xml
let clientAll = [];     //all data tagconfiguration.xml [{CertificateID:$,URL:$,method:$,templatebody:$,tags:$,readtags:$},{},{}]
let serverAll = [];   //all data restserverconfigtagconfiguration.xml [{CertificateID:$,URL:$,method:$,templatebody:$,tags:$,readtags:$},{},{}]
let subdataObj = {};  //sub tags name of json
let subdata = [];      //all needed sub tags [tag1,tag2,tag3]
let readdata = [];     //all needed read tags[[tag1,tag2,tag3],[],[]]
let readsubtrg = [];   //all subscribe triggered read tags.
let readflag = [];     //all needed flage ,true is need
let readbodyflag = []; //templatebody include tags, but tag not have onchange,and tags include the tag
let sendbodyflag = []; //if have subdata changed and the value is satisfacte the filter is ture ,send databody, else false not send databody.
let subresult = [];    //all subscribe tag value  [[PlaceHolder:$PlaceHolder,name:$name,value:$value],[],[]]
let subResultOld = [];//all subscribe tag old value. like subresult formate.
let initflag = false;  //init the subResultOld arry
let subfilter = [];    //all subscribe tag value and satisfaction filter
let ackdata = [];
let sendflag = false;   //send to opcenter after written ack tag
let ackvalue = [];
let rseq = [];


// IIH variable
let variableids = [];
let variablenames = [];

//read tagconfiguration.xml
//Branchflag==='0' connect to unified
if (Branchflag === '0') {
  if (tagconfigflag == '0') {
    console.log('read unified tagconfiguration.xml');
    myconvert.readxml(tagxmlpath, function(fd) {
      num = JSON.parse(fd).TagList.WebClient.length;   //get how many client element (OPcenter server)
      if (typeof(JSON.parse(fd).TagList.WebClient) === 'Object') {
        num = 1;  //if the tagconfigation.xml just have one webclient
      }
      console.log('Get the data from TagConfiguration.xml');
      for (let i = 0; i < num; i++) {
        let obj = JSON.parse(fd).TagList.WebClient[i].Tags.Tag;  //get the tag name which to subscribe from unified panel
        let tags = [];             //current client include all onchange tags
        let readtags = [];        //current client include all not have onchange tags
        let alltags = [];         //current client include all tags
        for (let j in obj) {
          if (obj[j].Trigger) {
            if (obj[j].Trigger === 'Onchange') {          //parse ===trigger onchange onely onchange need to subscribe
              let subobj = obj[j].Name;
              if (subobj in subdataObj) {
                console.log('subdata had include the tag %s \n', subobj);
              } else {
                subdataObj[subobj] = '1';
                subdata = Object.keys(subdataObj);  //get the subscribe tags
              }
              subobj = {PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter};
              tags.push(subobj);              //all the onchange tags name
            } else {
              errwhat = 'The Trigger column of webclient[' + i + '] in TagConfiguration.xml';
              errmsg = {what: errwhat, value: obj[j].Trigger, reson: 'Trigger name has to be Onchange'};
              console.error(new ServerError(500, 505, errmsg));
            }
          }
          else {
            let readObj = {PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter};
            readtags.push(readObj);  //get all the read tags name
          }

          let allObj = {PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter};
          alltags.push(allObj);  //get all the tags name
        }
        //save all the webclient info
        let clientObj = {'token': JSON.parse(fd).TagList.WebClient[i].Token, 'CertificateID': JSON.parse(fd).TagList.WebClient[i].CertificateID, 'URL': JSON.parse(fd).TagList.WebClient[i].CommandName, 'method': JSON.parse(fd).TagList.WebClient[i].CommandMode, 'templatebody': JSON.parse(fd).TagList.WebClient[i].TemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags};
        clientAll.push(clientObj);
        readdata.push(readtags);

        //get the templatebody need read tags
        let tembody = clientObj.templatebody;
        let temread = [];
        //console.log("The templatebody is :%s \n",tembody);

        readbodyflag[i] = false;
        if (tembody.indexOf('$') > 0) {
          while (tembody.indexOf('$') > 0) {
            let s = tembody.indexOf('${');
            let e = tembody.indexOf('}', s);
            let str = tembody.substr(s + 2, e - s - 2);
            for (let k in readtags) {
              if (str === readtags[k].PlaceHolder ) {
                readbodyflag[i] = true;            //if template body have tags in readtags need to readtag
                temread.push(readtags[k].Name);
                console.log('The temread tags %s', temread);
              }
            }
            tembody = tembody.replace('${' + str + '}', '0');
          //console.log("The repalced %s",tembody);
          }
        } else {
          readbodyflag[i] = false; //if have not $ ,not need to read tags
        }
        readsubtrg.push(temread);
      }
      console.log('The needed suscribe tags \n');
      console.log(subdata);
      console.log('The needed read tags \n');
      console.log(readdata);
      console.log('The subscribe trigger read tags \n');
      console.log(readsubtrg);
      console.log('The all data of webclient \n');
      console.log(clientAll);
      openpipe.subscribetag(JSON.stringify(subdata), function(reserr, resdata) {  //start subscribe tags
        subresult = []; //the result of subscribe tags
        subfilter = [];

        if (reserr.length == 0 || reserr == '') {
          console.log('subscribed needed tags');
          console.log(resdata);  //output subscribed tags


          for (let i = 0; i < num; i++) {//every client
            let resultdata = [];
            let filterdata = [];
            sendbodyflag[i] = false;
            for (let k in clientAll[i].tags) { //current client all subscribe tags
              for (let j in resdata) {
                if (resdata[j].Name === clientAll[i].tags[k].Name) {
                  let subResultObj = {}; //all subscribe tag result obj
                  let filterDataObj = {}; //all satisfate filter result

                  if (clientAll[i].tags[k].Filter !== 'undefined') {
                    if (myconvert.operator(resdata[j].Value, clientAll[i].tags[k].Filter)) {
                      filterDataObj = {Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value};
                      filterdata.push( filterDataObj);

                      sendbodyflag[i] = true; //only have one statisfate needed to send
                    //console.log("The client %s subscribe data",subResultObj)
                    } else {
                      filterDataObj = {Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: 'The value is not satisfaction filter'};
                      filterdata.push( filterDataObj);
                    }
                  } else {
                  // eslint-disable-next-line no-unused-vars
                    filterDataObj = {Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value};
                    filterdata.push( filterDataObj);  //not have filter send all tag value
                  //console.log("The client %s subscribe data",subResultObj)
                  }

                  subResultObj = {Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value};
                  resultdata.push(subResultObj);
                }
              }
            }
            subresult.push(resultdata);
            subfilter.push(filterdata);
          //console.log(subresult);
          }
          console.log('The all client subscribe data', subresult);
          console.log('The all client subscribe satisfaction fliter data', subfilter);

          //first subscribe not to send any data since first restart program
          if (initflag === false) {
            initflag = true;
            subResultOld = subresult;
          }

          //tag value changed trigger the read needed tag
          for (let i = 0; i < num; i++) {
            readflag[i] = false;

            // current value is changed

            for (let n in subresult[i]) {
              if (subresult[i][n].Name === subResultOld[i][n].Name) {
                if (subresult[i][n].Value !== subResultOld[i][n].Value) {
                  for (let m in clientAll[i].tags) {
                    if (clientAll[i].tags[m].Name === subResultOld[i][n].Name) {
                      if (clientAll[i].tags[m].Filter !== 'undefined') {
                        if (myconvert.operator(subresult[i][n].Value, clientAll[i].tags[m].Filter)) {
                          readflag[i] = true;
                        }
                      } else {
                        readflag[i] = true;
                      }
                    }
                  }
                }
              }
            }

            subResultOld[i] = subresult[i];

            //if have tag changed and satisfate the filter send data

            //the client have read tags and the trigger tag value changed
            if ((readflag[i] === true) && (readbodyflag[i] === true)) {
              console.log('current client %s have tags changed,start to read', i);
              console.log(readsubtrg[i]);
              //read the tags
              // eslint-disable-next-line no-shadow
              openpipe.readtag(JSON.stringify(readsubtrg[i]), function(reserr, resdata) {
                if (reserr.length == 0 || reserr == '') {
                //console.log(resdata);
                  let writedata = []; //data formate {name:placeholder,value:value}
                  //get all the subscribe tags

                  for (let x in subresult[i]) {
                    if (subresult[i][x] !== '') {
                      let wobj = {name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value};
                      writedata.push(wobj);
                    }
                  }
                  //get all the read tags
                  for (let k in resdata) {
                    for (let m in readdata[i]) {
                      if (resdata[k].Name === readdata[i][m].Name) {
                        let robj = {name: readdata[i][m].PlaceHolder, value: resdata[k].Value};
                        writedata.push(robj);
                      }
                    }
                  }
                  console.log('The writedata is :\n');
                  console.log(writedata);
                  if (writedata.length > 0) {   //if have data needed to send data
                    let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                    let subobj = {};
                    subobj.tag = writedata;
                    jsonobj.Tags = subobj;
                    console.log(jsonobj);
                    myconvert.convertjsonviaxsl(jsonobj, xsltPathAll, function(jsonvalue) { //transform value
                      console.log(jsonvalue);
                      let writeDataJson = {};
                      for (let x in writedata) {
                        writeDataJson[writedata[x].name] = jsonvalue[x];
                      }
                      console.log('The converted writedata is %s ', writeDataJson);
                      let curentbody = clientAll[i].templatebody;
                      //console.log(curentbody);
                      //replace $ built the sendbody
                      while (curentbody.indexOf('$') > 0) {
                        let s = curentbody.indexOf('${');
                        let e = curentbody.indexOf('}', s);
                        let str = curentbody.substr(s + 2, e - s - 2);
                        for (let {} in writeDataJson) {
                          if (str in writeDataJson ) {
                            curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                          } else {
                            curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                          }
                        }
                      //console.log("The repalced %s",curentbody);
                      }
                      console.log('The repalced curentbody: %s', curentbody);

                      //send the replaced body to webclient

                      let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                      let opmethod = clientAll[i].method;             //get the OPcenter method
                      clientOptions.hostname = urlstr.hostname;
                      clientOptions.port = urlstr.port;
                      clientOptions.path = urlstr.pathname;
                      clientOptions.method = opmethod;
                      try {
                        clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                      } catch (err) {
                        console.error(err);
                      }
                      clientOptions.headers['Content-Length'] = curentbody.length;
                      if (clientAll[i].token) {
                        clientOptions.headers.Authorization = 'Bearer ' + clientAll[i].token;
                      }
                      //clientOptions.headers["Content-Type"] = 'text/plain';
                      let clientReq = https.request(clientOptions, function(res) {
                        res.setEncoding('utf-8');
                        res.on('data', function(chunk) {
                          try {
                            chunk = JSON.parse(chunk);
                          }
                          catch {
                            console.error( new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                          }
                          console.log(chunk);
                          if (chunk.error) {
                            console.error( new ServerError(500, 700, chunk.error.message));
                          }
                        });
                      });
                      clientReq.write(curentbody);
                      clientReq.on('error', function(e) {
                        console.error(new ServerError(500, 702, e.message));
                      });
                      clientReq.end();
                    });
                  }
                } else {
                  if (typeof(reserr) == 'string') {
                    console.error( new ServerError(500, 601, reserr));
                  }
                  let errTag = [];
                  for (let k in reserr) {
                    errTag.push(reserr[k].Name);
                  }
                  let errmsg = {variable: errTag, error: reserr[0].ErrorDescription};
                  console.error( new ServerError(500, 601, errmsg));
                }
              });
            } else if ((readflag[i] === true) && (readbodyflag[i] !== true)) {
              console.log('current client %s have tags changed, and no tags need to read', i);
              let writedata = []; //data formate {name:placeholder,value:value}

              //get all the tags and satisfate subscribe filter
              /*
                        for(let x in subfilter[i]){
                            if(subfilter[i][x] != ""){
                                let wobj={name:subfilter[i][x].PlaceHolder,value:subfilter[i][x].Value};
                                writedata.push(wobj);
                             }
                        }
                        */

              //get all the subscribe tags

              for (let x in subresult[i]) {
                if (subresult[i][x] !== '') {
                  let wobj = {name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value};
                  writedata.push(wobj);
                }
              }

              console.log('The writedata is :\n');
              console.log(writedata);
              if (writedata.length > 0) {   //if have data needed to send data
                let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                let subobj = {};
                subobj.tag = writedata;
                jsonobj.Tags = subobj;
                console.log(jsonobj);
                myconvert.convertjsonviaxsl(jsonobj, xsltPathAll, function(jsonvalue) { //transform value
                  console.log(jsonvalue);
                  let writeDataJson = {};
                  for (let x in writedata) {
                    writeDataJson[writedata[x].name] = jsonvalue[x];
                  }
                  console.log('The converted writedata is %s ', writeDataJson);
                  let curentbody = clientAll[i].templatebody;
                  //console.log(curentbody);

                  while (curentbody.indexOf('$') > 0) {
                    let s = curentbody.indexOf('${');
                    let e = curentbody.indexOf('}', s);
                    let str = curentbody.substr(s + 2, e - s - 2);
                    for (let {} in writeDataJson) {
                      if (str in writeDataJson ) {
                        curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                      } else {
                        curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                      }
                    }
                  //console.log("The repalced %s",curentbody);
                  }
                  console.log('The repalced curentbody: %s', curentbody);

                  //send the replaced body to webclient

                  let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                  let opmethod = clientAll[i].method;             //get the OPcenter method
                  clientOptions.hostname = urlstr.hostname;
                  clientOptions.port = urlstr.port;
                  clientOptions.path = urlstr.pathname;
                  clientOptions.method = opmethod;
                  try {
                    clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                  } catch (err) {
                    console.error(err);
                  }
                  clientOptions.headers['Content-Length'] = curentbody.length;
                  if (clientAll[i].token) {
                    clientOptions.headers.Authorization = 'Bearer ' + clientAll[i].token;
                  }
                  //clientOptions.headers["Content-Type"]='text/plain';
                  let clientReq = https.request(clientOptions, function(res) {
                    res.setEncoding('utf-8');
                    res.on('data', function(chunk) {
                      try {
                        chunk = JSON.parse(chunk);
                      }
                      catch {
                        console.error( new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                      }
                      console.log(chunk);
                      if (chunk.error) {
                        console.error( new ServerError(500, 700, chunk.error.message));
                      }
                    });
                  });
                  clientReq.write(curentbody);
                  clientReq.on('error', function(e) {
                    console.error( new ServerError(500, 702, e.message));
                  });
                  clientReq.end();
                });
              }
            }
          }
        } else {
          if (typeof(reserr) == 'string') {
            console.error( new ServerError(500, 602, reserr));
          }
          let errTag = [];
          for (let i in reserr) {
            errTag.push(reserr[i].Name);
          }
          let errmsg = {variable: errTag, error: reserr[0].ErrorDescription};
          console.error( new ServerError(500, 602, errmsg));
        }
      });
    });
  }
}
//connect to IIH
else if (Branchflag === '2') {
  console.log('enable subscription from IIH');
  //read IIH export JSON file
  //get variables json array
  const IIHdata = iihjsondata.variables;
  //get variableId
  variableids = IIHdata.map((item) => {
    return item.variableId;
  });
  //get variableName
  variablenames = IIHdata.map((item) => {
    return item.variableName;
  });

  console.log('get variablenames from IIH are:', variablenames);
  console.log('get variableids from IIH are:', variableids);

  //switch iih usecase3 if serverflag=true
  if (Serverflag == true)//read ServerConfiguration_Opcenter.xml
  {
    console.log('enable usecase3 and read ServerConfiguration_Opcenter.xml');
    // eslint-disable-next-line no-shadow
    myconvert.readxml(RestServersOpcenterxmlpath, function(fd) {
      console.log('read ServerConfiguration_Opcenter.xml finished');
      let allservertags = [];
      num1 = JSON.parse(fd).TagList.WebClient.length;//get how many client element (OPcenter server)
      if (typeof(JSON.parse(fd).TagList.WebClient) === 'Object') {
        num1 = 1;  //if the RestServerconfigation_OpCenter.xml just have one webclient
      }
      for (let i = 0; i < num1; i++) {
        let allservertag = [];
        let objs = JSON.parse(fd).TagList.WebClient[i].Tags.Tag;
        for (let j in objs) {
          let allserverObj = { PlaceHolder: '' + objs[j].PlaceHolder, Name: '' + objs[j].Name };
          allservertag.push(allserverObj);  //get all the tags name
        }
        allservertags.push(allservertag);
        //save all client infos
        let serverObj = { 'URL': JSON.parse(fd).TagList.WebClient[i].CommandName, 'method': JSON.parse(fd).TagList.WebClient[i].CommandMode, 'templatebody': JSON.parse(fd).TagList.WebClient[i].TemplateBody, 'alltags': allservertags[i] };
        serverAll.push(serverObj);
      }
      //console.log(serverAll)
    });
  }
  if (tagconfigflag == '2') {
    console.log('enable usecase 2 and 4');
    myconvert.readxml(iihtagxmlpath, function(fd) {
      console.log('read wincctagConfiguration.xml finished');
      num = JSON.parse(fd).TagList.WebClient.length;   //get how many client element (OPcenter server)
      if (typeof(JSON.parse(fd).TagList.WebClient) === 'Object') {
        num = 1;  //if the tagconfigation.xml just have one webclient
      }
      console.log('Get the data from TagConfiguration.xml');
      for (let i = 0; i < num; i++) {
        let obj = JSON.parse(fd).TagList.WebClient[i].Tags.Tag;  //get the tag name which to subscribe from unified panel
        let tags = [];             //current client include all onchange tags
        let readtags = [];        //current client include all not have onchange tags
        let alltags = [];         //current client include all tags
        let writetag = [];        //current client include write tags
        for (let j in obj) {
          if (obj[j].Trigger === 'Onchange') {          //parse ===trigger onchange onely onchange need to subscribe
            let subobj = obj[j].Name;
            if (subobj in subdataObj) {
              console.log('subdata had include the tag %s \n', subobj);
            } else {
              subdataObj[subobj] = '1';
              subdata = Object.keys(subdataObj);  //get the subscribe tags
            }
            subobj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
            tags.push(subobj);              //all the onchange tags name
          } else {
            let readObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
            readtags.push(readObj);  //get all the read tags name
          }
          if (obj[j].AcknowledgeTag) {
            let writeObj = { AcknowledgeTag: '' + obj[j].AcknowledgeTag, Name: '' + obj[j].Name };
            writetag.push(writeObj);
          }
          let allObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
          alltags.push(allObj);  //get all the tags name
        }
        //save all the webclient info
        let clientObj = {'token': JSON.parse(fd).TagList.WebClient[i].Token, 'CertificateID': JSON.parse(fd).TagList.WebClient[i].CertificateID, 'URL': JSON.parse(fd).TagList.WebClient[i].CommandName, 'method': JSON.parse(fd).TagList.WebClient[i].CommandMode, 'templatebody': JSON.parse(fd).TagList.WebClient[i].TemplateBody, 'ReturnTemplateBody': JSON.parse(fd).TagList.WebClient[i].ReturnTemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags };
        clientAll.push(clientObj);
        readdata.push(readtags);
        ackdata.push(writetag);
        //get the templatebody need read tags
        let tembody = clientObj.templatebody;
        let temread = [];
        //console.log("The templatebody is :%s \n",tembody);

        readbodyflag[i] = false;
        if (tembody.indexOf('$') > 0) {
          while (tembody.indexOf('$') > 0) {
            let s = tembody.indexOf('${');
            let e = tembody.indexOf('}', s);
            let str = tembody.substr(s + 2, e - s - 2);
            for (let k in readtags) {
              if (str == readtags[k].PlaceHolder) {
                readbodyflag[i] = true;            //if template body have tags in readtags need to readtag
                temread.push(readtags[k].Name);
                console.log('The temread tags %s', temread);
              }
            }
            tembody = tembody.replace('${' + str + '}', '0');
            //console.log("The repalced %s",tembody);
          }
        } else {
          readbodyflag[i] = false; //if have not $ ,not need to read tags
        }
        readsubtrg.push(temread);
      }
      console.log('readbodyflag is false or true\n');
      console.log(readbodyflag);
      console.log('The needed suscribe tags \n');
      console.log(subdata);
      console.log('The needed read tags \n');
      console.log(readdata);
      console.log('The subscribe trigger read tags \n');
      console.log(readsubtrg);
      //  console.log('The all data of webclient \n');
      //  console.log(clientAll);
      console.log('The needed writeack tags \n');
      console.log(ackdata);

      //select need to read variableids
      let subiih = [];
      for (let i in subdata) {
        for (let j in variablenames) {
          if (subdata[i] == variablenames[j]) {
            subiih.push(variableids[j]);
          }
        }
      }
      console.log('The needed subscribe ID from IIH\n');
      console.log(subiih);
      //set daygap variable =1dat, which means from-to daygap for IIH get variable value equal 1s
      let daygap = 1;
      let daygapflag;
      //endless loop to get variable values
      setInterval(function() {
        //async function to gettoken from IIH first
        let gettoken = async function() {
          let token = await gettokenfromIIH();
          return token;
        };
        gettoken().then((token)=>{
          console.log('get Token for IIH are:', token);
          //then can get data
          //set inital daygapflag=true;
          daygapflag = true;
          console.log('The time gap now is:%d hours', daygap);
          let geturl = '/Dataservice/Data/?variableIds=' + JSON.stringify(subiih) + '&from=' + getRealTime(daygap) + '&to=' + getRealTime();
          console.log('IIH server request url is:', geturl);
          getdatafromIIH(geturl, token, function(res)  {
            // console.log('IIH server response datastring is:', res);
            let obj = JSON.parse(res).data;
            let ressub = [];
            console.log('IIH server response data is:', obj);
            //get value
            for (let n in obj) {
              let final = {};
              //if value is not []
              if (obj[n].values.length !== 0) {
                breakflag = false;
                //get the last value(newest value)
                let latest = obj[n].values.length - 1;
                for (let k in subdata) {
                  for (let j in variablenames) {
                    if (subdata[k] == variablenames[j]) {
                      final = {Name: variablenames[j], Value: obj[n].values[latest].value};
                    }
                  }
                }
              }
              else {
                console.log('get value from iih failed, daygap +1hour and resend request');
                //daygapflag=false
                daygapflag = false;
                for (let k in subdata) {
                  for (let j in variablenames) {
                    if (subdata[k] == variablenames[j]) {
                      final = {Name: variablenames[j], Value: null};
                    }
                  }
                }
              }
              ressub.push(final);
            }//end for


            //the result of subscribe tags
            subresult = [];
            subfilter = [];
            //response of subscribed value:
            console.log('The newest subscribed Tags&Values from IIH response are:', ressub);
            //filter value for every client
            for (let i = 0; i < num; i++) {
              let resultdata = [];
              let filterdata = [];
              sendbodyflag[i] = false;
              //console.log('how may webclient in IIHtagconfiguration.xml?',num)
              //console.log('now are producing which client?',i);
              for (let k in clientAll[i].tags) { //current client all subscribe tags
                for (let j in ressub) {
                  if (ressub[j].Name === clientAll[i].tags[k].Name) {
                    let subResultObj = {}; //all subscribe tag result obj
                    let filterDataObj = {}; //all satisfate filter result

                    if (clientAll[i].tags[k].Filter !== 'undefined') {
                      if (myconvert.operator(ressub[j].Value, clientAll[i].tags[k].Filter)) {
                        filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                        filterdata.push(filterDataObj);

                        sendbodyflag[i] = true; //only have one statisfate needed to send
                        //console.log("The client %s subscribe data",subResultObj)
                      } else {
                        filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: 'The value is not satisfaction filter' };
                        filterdata.push(filterDataObj);
                      }
                    } else {
                      filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                      filterdata.push(filterDataObj);  //not have filter send all tag value
                      //console.log("The client %s subscribe data",subResultObj)
                    }

                    subResultObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                    resultdata.push(subResultObj);
                  }
                }
              }
              subresult.push(resultdata);
              subfilter.push(filterdata);
            }
            console.log('The all client subscribe satisfaction fliter data', subfilter);

            //first subscribe not to send any data since first restart program
            if (initflag === false) {
              initflag = true;
              subResultOld = subresult;
            }
            // value changed and is over the threshold for every webclient
            for (let i = 0; i < num; i++) {
              readflag[i] = false;
              // current value is changed
              for (let n in subresult[i]) {
                if (subresult[i][n].Name === subResultOld[i][n].Name) {
                  if (subresult[i][n].Value !== subResultOld[i][n].Value) {
                    for (let m in clientAll[i].tags) {
                      if (clientAll[i].tags[m].Name === subResultOld[i][n].Name) {
                        if (clientAll[i].tags[m].Filter !== 'undefined') {
                          if (myconvert.operator(subresult[i][n].Value, clientAll[i].tags[m].Filter)) {
                            readflag[i] = true;
                          }
                        } else {
                          readflag[i] = true;
                        }
                      }
                    }
                  }
                  else {
                    //else the value is not changed, do not have to send to opcenter
                    console.log('The value is same, do not need to send request');
                  }
                }
              }

              subResultOld[i] = subresult[i];
              // the client have read tags and the trigger tag value changed
              // get readtag's value from IIH and send to opcenter
              if ((readflag[i] === true) && (readbodyflag[i] === true)) {
                console.log('current client %s have tags changed,start to read', i);
                //configure needed variableId of IIH request url
                let readiih = [];
                for (let j in readsubtrg[i]) {
                  for (let k in variablenames) {
                    if (readsubtrg[i][j] == variablenames[k]) {
                      readiih.push(variableids[k]);
                    }
                  }
                }
                //configure complete iih request url
                geturl = '/Dataservice/Data/?variableIds=' + JSON.stringify(readiih) + '&from=' + getRealTime(daygap) + '&to=' + getRealTime();
                console.log('IIH server request url is:', geturl);
                //call getdatafromIIH function to get variable value of readtag from IIH and send to opcenter
                getdatafromIIH(geturl, token, function(resobj)  {
                  //console.log(res);
                  obj = JSON.parse(resobj).data;
                  //get latest one value from resobj
                  let result = [];

                  for (let j in obj)
                  {
                    if (obj[j].values.length !== 0) {
                      let lastone = obj[j].values.length - 1;
                      result.push(obj[j].values[lastone].value);


                      //data formate {name:placeholder,value:value}
                      let writedata = [];
                      let resdata = [];
                      let ackname = [];
                      // eslint-disable-next-line no-shadow
                      let ackvalue = [];
                      //build the response data
                      for (let k in readsubtrg[i]) {
                        let tem = { Name: readsubtrg[i][k], Value: '' + result[k] + '' };
                        resdata.push(tem);
                      }
                      console.log('The readed Tags&Values from IIH response are:', resdata);
                      //get all ack tags
                      for (let j in ackdata[i]) {
                        if (ackdata[i][j].AcknowledgeTag) {
                          ackname.push(ackdata[i][j].Name);
                        }
                      }
                      //get all the subscribe tags
                      for (let x in subresult[i]) {
                        if (subresult[i][x] !== '') {
                          let wobj = {name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value};
                          ackvalue.push(subresult[i][x].Value);
                          writedata.push(wobj);
                        }
                      }
                      //get all the read tags
                      for (let k in resdata) {
                        for (let m in readdata[i]) {
                          if (resdata[k].Name === readdata[i][m].Name) {
                            let robj = {name: readdata[i][m].PlaceHolder, value: resdata[k].Value};
                            writedata.push(robj);
                          }
                        }
                      }
                      console.log('The writedata is :\n');
                      console.log(writedata);
                      if (writedata.length > 0) {   //if have data needed to send data
                        let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                        let subobj = {};
                        subobj.tag = writedata;
                        jsonobj.Tags = subobj;
                        //translate value
                        myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function(jsonvalue) {
                          let writeDataJson = {};
                          for (let x in writedata) {
                            writeDataJson[writedata[x].name] = jsonvalue[x];
                          }
                          let curentbody = clientAll[i].templatebody;

                          console.log('The Templatebody is:', curentbody);
                          //replace $ built the sendbody
                          while (curentbody.indexOf('$') > 0) {
                            let s = curentbody.indexOf('${');
                            let e = curentbody.indexOf('}', s);
                            let str = curentbody.substr(s + 2, e - s - 2);
                            for (let { } in writeDataJson) {
                              if (str in writeDataJson) {
                                curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                              } else {
                                curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                              }
                            }
                          }
                          console.log('The builded Requestbody now is:', curentbody);

                          // //send the replaced body to webclient
                          let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                          let opmethod = clientAll[i].method;             //get the OPcenter method
                          clientOptions.hostname = urlstr.hostname;
                          clientOptions.port = urlstr.port;
                          clientOptions.path = urlstr.pathname;
                          clientOptions.method = opmethod;
                          try {
                            clientOptions.key = fs.readFileSync(clientAll[i].CertificateID + 'server.key');
                            clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'server.crt');
                            // eslint-disable-next-line no-shadow
                          } catch (err) {
                            console.error(err);
                          }
                          clientOptions.headers['Content-Length'] = curentbody.length;
                          if (clientAll[i].token) {
                            clientOptions.headers.Authorization = 'Bearer ' + clientAll[i].token;
                          }
                          let opcenterresult = async function() {
                            let responsedata = await opcenterconnect(clientOptions, curentbody);
                            //if Opcenter Server has  return TemplateBody
                            if (clientAll[i].ReturnTemplateBody != undefined) {
                              console.log('webclient %d has returntemplate, do comparetion with respondata from Opcenter', i);
                              let returntemplate = JSON.parse(clientAll[i].ReturnTemplateBody);
                              let temkey = Object.keys(returntemplate);
                              let responkey = Object.keys(responsedata);
                              //tell if key match
                              if (temkey.toString == responkey.toString) {
                                return (responsedata);
                              }
                              //if not match, log this error and no need to proceed
                              console.error('Responsedata does not match OpcenterServer %d RetrunTemplatebody, no need to proceed.', i);
                            }
                            //if Opcenter Server do not has retrun TemplateBody, just Console log and no need to proceed
                            console.log('client %d do not have retruntemplate,no need to proceed.', i);
                            console.log('The response Data from OpcenterServer %d is: %o', i, responsedata);
                          };
                          opcenterresult().then((v) => {
                            if (v != undefined) {
                              let responsefromOp = JSON.parse(v);
                              console.log('compartion done, the correct response from  OpcenterServer is %o', responsefromOp);
                              console.log('start to write this return message into IIH');
                              let returntemplate = JSON.parse(clientAll[i].ReturnTemplateBody);
                              // do translation tagname and value works
                              let tagconfigxml;
                              tagconfigxml = clientAll[i].alltags;
                              //get tagconfigxml placeholder
                              let placeholder = [];
                              let Name = [];
                              //delete [$i] in each palceholder if it has
                              for (let j in tagconfigxml) {
                                placeholder.push(tagconfigxml[j].PlaceHolder);
                                Name.push(tagconfigxml[j].Name);
                              }
                              let temvalue = handeljsondata(returntemplate)[1];
                              let newtemvalue = [];
                              temvalue.map((item, index) =>{
                                let str = JSON.stringify(item);
                                let s1 = str.indexOf('{');
                                let s2 = str.indexOf('}');
                                let s3 = str.slice(s1 + 1, s2);
                                newtemvalue.push(s3);
                                return newtemvalue;
                              });
                              for (let k in newtemvalue) {
                                for (let g in placeholder) {
                                  if (newtemvalue[k] == placeholder[g]) {
                                    newtemvalue[k] = Name[g];
                                  }
                                }
                              }
                              let keyary = [];
                              //delete [$s] in name and get transfered tagname
                              newtemvalue.map((item, index) => {
                                if (item.indexOf('[$i]') != -1) {
                                  let itemcopy = [];
                                  let keys = Object.keys(responsefromOp);
                                  let key = keys[index];
                                  if (Object.prototype.toString.call(responsefromOp[key]) === '[object Array]') {
                                    let length = responsefromOp[key].length;
                                    for (let j = 0; j < length; j++) {
                                      itemcopy.push(item);
                                    }
                                    console.log(itemcopy);
                                    for (let k = 0; k < length; k++) {
                                      let str = JSON.stringify(k);
                                      console.log(str);
                                      itemcopy[k] = itemcopy[k].replace('$i', str);
                                      keyary.push(itemcopy[k]);
                                    }
                                  }
                                }
                                else {
                                  keyary.push(item);
                                }
                              });

                              console.log('transfered tagname for datasource are:');
                              console.log(keyary);

                              //get tagvalue
                              let valueary = handeljsondata(responsefromOp)[1];
                              let values = [];
                              valueary.map((item, index) => {
                                if (typeof(item) == 'object') {
                                  for (let j in item) {
                                    values.push(item[j]);
                                  }
                                }
                                else {
                                  values.push(item);
                                }
                              });
                              console.log('get value from responsebody are:');
                              console.log(values);
                              //get corresponding IIHvariable IDs from started up process
                              let iihids = [];
                              let iihbody = [];
                              for (let j in keyary) {
                                for (let k in variablenames) {
                                  if (keyary[j] ==  variablenames[k]) {
                                    iihids.push(variableids[k]);
                                  }
                                }
                              }
                              //start to config IIH post body
                              for (let g in iihids) {
                                let iihobj = {'variableId': iihids[g],
                                  'values': [
                                    {'timestamp': getRealTime(),
                                      'value': values[g],
                                      'qualitycode': 0}]};
                                iihbody.push(iihobj);
                              }
                              console.log('The request post body to IIH are:', JSON.stringify(iihbody));
                              //call postdata2iih to write tag into IIH
                              postdata2iih(token, iihbody, function(res, err) {
                                //no error
                                if (err == '') {
                                  console.log('Post Data to IIH done,Response is:', res);
                                  //start to write ack into IIH
                                  console.log('start to write ack into IIH');
                                  console.log('get ackname is:', ackname);
                                  console.log('get ackvalue is', ackvalue);
                                  let ackids = [];
                                  let ackbody = [];
                                  for (let j in ackname)
                                  {
                                    for (let k in variablenames) {
                                      if (ackname[j] == variablenames[k]) {
                                        ackids.push(variableids[k]);
                                      }
                                    }
                                  }
                                  console.log('get ackid is', ackids);
                                  //config post ack request body
                                  for (let g in ackids) {
                                    let iihobj = {'variableId': ackids[g],
                                      'values': [
                                        {'timestamp': getRealTime(),
                                          'value': ackvalue[g],
                                          'qualitycode': 0}]};
                                    ackbody.push(iihobj);
                                  }
                                  postdata2iih(token, ackbody, function(res, err) {
                                    //no error
                                    if (err == '') {
                                      console.log('Finally write into IIH acktag successful,The response is:', res);
                                    }
                                    else {
                                      //log error for posting ack value
                                      console.error(new ServerError(901, JSON.parse(err)));
                                    }
                                  });
                                }
                                else {
                                  //log error for posting variable value
                                  console.error(new ServerError(901, JSON.parse(err)));
                                }
                              });
                            }
                          }).catch(function(reason) {
                            //reason.code = 'ECONNREFUSED'
                            console.error(new ServerError(500, 702, reason.message));
                          });
                        });
                      }//end if writedata.length > 0
                    }//end if obj[j].values.length != 0
                    else {
                      console.log('Value of %s is not found in IIH', readsubtrg[i][j]);
                      breakflag = true;
                    }
                  }//end for let j in obj
                });//end getvariable
              }//end if read
              //else send the subcribed tag&value from IIH to opcenter
              else if ((readflag[i] === true) && (readbodyflag[i] !== true)) {
                console.log('current client %s have tags changed, and no tags need to read', i);
                let writedata = []; //data formate {name:placeholder,value:value}

                //get all the subscribe tags

                for (let x in subresult[i]) {
                  if (subresult[i][x] !== '') {
                    let wobj = { name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value };
                    writedata.push(wobj);
                  }
                }

                console.log('The Subscribed Tags&Values from IIH response are: is :\n');
                console.log(writedata);
                if (writedata.length > 0) {   //if have data needed to send data
                  let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                  let subobj = {};
                  subobj.tag = writedata;
                  jsonobj.Tags = subobj;

                  //translate value via xslt
                  myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function(jsonvalue) {
                    let writeDataJson = {};
                    for (let x in writedata) {
                      writeDataJson[writedata[x].name] = jsonvalue[x];
                    }
                    console.log('The converted writedata is: ', writeDataJson);
                    let curentbody = clientAll[i].templatebody;


                    while (curentbody.indexOf('$') > 0) {
                      let s = curentbody.indexOf('${');
                      let e = curentbody.indexOf('}', s);
                      let str = curentbody.substr(s + 2, e - s - 2);
                      for (let { } in writeDataJson) {
                        if (str in writeDataJson) {
                          curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                        } else {
                          curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                        }
                      }
                      //console.log("The repalced %s",curentbody);
                    }
                    console.log('The builded Repuestbody now is: ', curentbody);

                    //send the replaced body to webclient

                    let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                    let opmethod = clientAll[i].method;             //get the OPcenter method
                    clientOptions.hostname = urlstr.hostname;
                    clientOptions.port = urlstr.port;
                    clientOptions.path = urlstr.pathname;
                    clientOptions.method = opmethod;
                    try {
                      clientOptions.key = fs.readFileSync(clientAll[i].CertificateID + 'server.key');
                      clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'server.crt');
                      // eslint-disable-next-line no-shadow
                    } catch (err) {
                      console.error(err);
                    }
                    clientOptions.headers['Content-Length'] = curentbody.length;
                    if (clientAll[i].token) {
                      clientOptions.headers.Authorization = 'Bearer ' + clientAll[i].token;
                    }
                    let opcenterresult = async function() {
                      let responsedata = await opcenterconnect(clientOptions, curentbody);
                      if (clientAll[i].ReturnTemplateBody != undefined) {
                        console.log('webclient %d has returntemplate, do comparetion with respondata from Opcenter', i);
                        let returntemplate = JSON.parse(clientAll[i].ReturnTemplateBody);
                        let temkey = Object.keys(returntemplate);
                        let responkey = Object.keys(responsedata);
                        if (temkey.toString == responkey.toString) {
                          return (responsedata);
                        }
                        console.log('Responsedata does not match client %d RetrunTemplatebody, no need to proceed.', i);
                      }
                      console.log('client %d do not have retruntemplate,no need to proceed.', i);
                    };
                    opcenterresult().then((v) => {
                      if (v != undefined) {
                        let responsefromOp = JSON.parse(v);
                        console.log('compartion done, the correct response from  OpcenterServer is %o', responsefromOp);
                        console.log('start to write this return message into IIH');
                        let returntemplate = JSON.parse(clientAll[i].ReturnTemplateBody);
                        // do translation tagname and value works
                        let tagconfigxml;
                        tagconfigxml = clientAll[i].alltags;
                        //get tagconfigxml placeholder
                        let placeholder = [];
                        let Name = [];
                        //delete [$i] in each palceholder if it has
                        for (let j in tagconfigxml) {
                          placeholder.push(tagconfigxml[j].PlaceHolder);
                          Name.push(tagconfigxml[j].Name);
                        }
                        let temvalue = handeljsondata(returntemplate)[1];
                        let newtemvalue = [];
                        temvalue.map((item, index) =>{
                          let str = JSON.stringify(item);
                          let s1 = str.indexOf('{');
                          let s2 = str.indexOf('}');
                          let s3 = str.slice(s1 + 1, s2);
                          newtemvalue.push(s3);
                          return newtemvalue;
                        });
                        for (let k in newtemvalue) {
                          for (let g in placeholder) {
                            if (newtemvalue[k] == placeholder[g]) {
                              newtemvalue[k] = Name[g];
                            }
                          }
                        }
                        let keyary = [];
                        //delete [$s] in name and get transfered tagname
                        newtemvalue.map((item, index) => {
                          if (item.indexOf('[$i]') != -1) {
                            let itemcopy = [];
                            let keys = Object.keys(responsefromOp);
                            let key = keys[index];
                            if (Object.prototype.toString.call(responsefromOp[key]) === '[object Array]') {
                              let length = responsefromOp[key].length;
                              for (let j = 0; j < length; j++) {
                                itemcopy.push(item);
                              }
                              console.log(itemcopy);
                              for (let k = 0; k < length; k++) {
                                let str = JSON.stringify(k);
                                console.log(str);
                                itemcopy[k] = itemcopy[k].replace('$i', str);
                                keyary.push(itemcopy[k]);
                              }
                            }
                          }
                          else {
                            keyary.push(item);
                          }
                        });

                        console.log('transfered tagname for datasource are:');
                        console.log(keyary);

                        //get tagvalue
                        let valueary = handeljsondata(responsefromOp)[1];
                        let values = [];
                        valueary.map((item, index) => {
                          if (typeof(item) == 'object') {
                            for (let j in item) {
                              values.push(item[j]);
                            }
                          }
                          else {
                            values.push(item);
                          }
                        });
                        console.log('get value from responsebody are:');
                        console.log(values);
                        //get corresponding IIHvariable IDs from started up process
                        let iihids = [];
                        let iihbody = [];
                        for (let j in keyary) {
                          for (let k in variablenames) {
                            if (keyary[j] ==  variablenames[k]) {
                              iihids.push(variableids[k]);
                            }
                          }
                        }
                        //start to config IIH post body
                        for (let g in iihids) {
                          let iihobj = {'variableId': iihids[g],
                            'values': [
                              {'timestamp': getRealTime(),
                                'value': values[g],
                                'qualitycode': 0}]};
                          iihbody.push(iihobj);
                        }
                        console.log('The request post body to IIH are:', JSON.stringify(iihbody));
                        //call postdata2iih to write tag into IIH
                        postdata2iih(token, iihbody, function(res, err) {
                          //no error
                          if (err == '') {
                            console.log('Post Data to IIH done,Response is:', res);
                            //start to write ack into IIH
                            console.log('start to write ack into IIH');
                            console.log('get ackname is:', ackname);
                            console.log('get ackvalue is', ackvalue);
                            let ackids = [];
                            let ackbody = [];
                            for (let j in ackname)
                            {
                              for (let k in variablenames) {
                                if (ackname[j] == variablenames[k]) {
                                  ackids.push(variableids[k]);
                                }
                              }
                            }
                            console.log('get ackid is', ackids);
                            //config post ack request body
                            for (let g in ackids) {
                              let iihobj = {'variableId': ackids[g],
                                'values': [
                                  {'timestamp': getRealTime(),
                                    'value': ackvalue[g],
                                    'qualitycode': 0}]};
                              ackbody.push(iihobj);
                            }
                            postdata2iih(token, ackbody, function(res, err) {
                              //no error
                              if (err == '') {
                                console.log('Finally write into IIH acktag successful,The response is:', res);
                              }
                              else {
                                //log error for posting ack value
                                console.error(new ServerError(901, JSON.parse(err)));
                              }
                            });
                          }
                          else {
                            //log error for posting variable value
                            console.error(new ServerError(901, JSON.parse(err)));
                          }
                        });
                      }
                    }).catch(function(reason) {
                      //reason.code = 'ECONNREFUSED'
                      console.error(new ServerError(500, 702, reason.message));
                    });
                  });
                }// end if writedata.length>0
              }//end else if
            }//end for
            //if daygapflag=false, daygap+1 h
            if (daygapflag === false) {
              daygap += 1;
            }
          });//end getsvariable
        }).catch(function(reason) {
          if (reason.code == 'ECONNREFUSED') {
            console.error( new ServerError(500, 900, reason.message));
          } else {
            console.error( new ServerError(500, 902, reason));
          }
        });//end postvariable.then
      }, delay);//end loop
    });//end read iihtagxml
  }
}
//connect to wincc
else if (Branchflag === '1') {
  if (Serverflag == true)//read ServerConfiguration_Opcenter.xml
  {
    console.log('start up usecase3 and read RestServerConfigureation_Opcenter.xml');
    myconvert.readxml(RestServersOpcenterxmlpath, function(fd) {
      console.log('read ServerConfiguration_Opcenter.xml finished');
      let suback = [];
      let readseq = [];
      let allservertags = [];
      num1 = JSON.parse(fd).TagList.WebClient.length;//get how many client element (OPcenter server)
      if (typeof(JSON.parse(fd).TagList.WebClient) === 'Object') {
        num1 = 1;  //if the RestServerconfigation_OpCenter.xml just have one webclient
      }
      for (let i = 0; i < num1; i++) {
        let allservertag = [];
        let objs = JSON.parse(fd).TagList.WebClient[i].Tags.Tag;
        for (let j in objs) {
          let allserverObj = { PlaceHolder: '' + objs[j].PlaceHolder, Name: '' + objs[j].Name};
          allservertag.push(allserverObj);  //get all the tags name
        }
        allservertags.push(allservertag);
        suback.push(JSON.parse(fd).TagList.WebClient[i].AcknowledgeTag);//get the ackname which need to subscribe from restserverconfig_Opcenter.xml
        readseq.push(JSON.parse(fd).TagList.WebClient[i].SequenceTag);  //get the seqname which need to read from restserverconfig_Opcenter.xml
        //save all client infos
        let serverObj = { 'URL': JSON.parse(fd).TagList.WebClient[i].CommandName, 'method': JSON.parse(fd).TagList.WebClient[i].CommandMode, 'templatebody': JSON.parse(fd).TagList.WebClient[i].TemplateBody, 'suback': suback[i], 'readseq': readseq[i], 'alltags': allservertags[i] };
        serverAll.push(serverObj);
      }
      //console.log(suback)
      //console.log(readseq)
      console.log('enable usecase3 for wincc v7.5');
      odk.readtag(readseq, function(readresult, err)
      {
        if (err) {
          console.error( new ServerError('false', 802, err));
        }

        console.log(readresult);
        for (let i in readresult.Value)
        {
          rseq.push(readresult.Value[i]);
        }
        console.log('startup process reads seq value is:', rseq);
      });

      odksubscribe.send(suback);
      odksubscribe.on('message', function(ressub) {
        //resub=[ { Name: 'NewOrder_ack', Value: 102 },{ Name: 'NewOrder_ack2', Value: 101 } ]
        //clean ackvalue array
        if (ressub.error) {
          console.error( new ServerError('false', 803, ressub));
        }
        else {
          ackvalue = [];
          //get ackvalue from ressub
          for (let i in ressub)
          {
            ackvalue.push(ressub[i].Value);
          }
          console.log('The All subscribed ackvalues are:', ackvalue);
        }
      });
    });//end readxml-restserver
  }//end  Serverflag
  if (tagconfigflag == '1') {//read wincctagconfiguration.xml instead
    console.log('start up usecase2_handshake ');
    myconvert.readxml(wincctagxmlpath, function(fd) {
      console.log('read wincctagConfiguration.xml finished');
      num = JSON.parse(fd).TagList.WebClient.length;   //get how many client element (OPcenter server)
      if (num == undefined) {
        num = 1;  //if the tagconfigation.xml just have one webclient
      }
      console.log('Get the data from TagConfiguration.xml');
      for (let i = 0; i < num; i++) {
        let obj = JSON.parse(fd).TagList.WebClient[i].Tags.Tag;  //get the tag name which to subscribe from unified panel
        let tags = [];             //current client include all onchange tags
        let readtags = [];        //current client include all not have onchange tags
        let alltags = [];         //current client include all tags
        let writetag = [];        //current client include write tags
        let errwhat;
        let errmsg;
        for (let j in obj) {
          if (obj[j].Trigger) {
            if (obj[j].Trigger === 'Onchange') {          //parse ===trigger onchange onely onchange need to subscribe
              let subobj = obj[j].Name;
              if (subobj in subdataObj) {
                console.log('subdata had include the tag %s \n', subobj);
              } else {
                subdataObj[subobj] = '1';
                subdata = Object.keys(subdataObj);  //get the subscribe tags
              }
              subobj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
              tags.push(subobj);              //all the onchange tags name
            } else {
              errwhat = 'The Trigger column of webclient[' + i + '] in WinccTagConfiguration.xml';
              errmsg = {what: errwhat, value: obj[j].Trigger, reson: 'Trigger name has to be Onchange'};
              console.error( new ServerError(500, 505, errmsg));
            }
          } else {
            let readObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
            readtags.push(readObj);  //get all the read tags name
          }
          if (obj[j].AcknowledgeTag) {
            let writeObj = { AcknowledgeTag: '' + obj[j].AcknowledgeTag, Name: '' + obj[j].Name };
            writetag.push(writeObj);
          }
          let allObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
          alltags.push(allObj);  //get all the tags name
        }
        //save all the webclient info
        let clientObj = { 'CertificateID': JSON.parse(fd).TagList.WebClient[i].CertificateID, 'URL': JSON.parse(fd).TagList.WebClient[i].CommandName, 'method': JSON.parse(fd).TagList.WebClient[i].CommandMode, 'templatebody': JSON.parse(fd).TagList.WebClient[i].TemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags };
        clientAll.push(clientObj);
        readdata.push(readtags);
        ackdata.push(writetag);
        //get the templatebody need read tags
        let tembody = clientObj.templatebody;
        let temread = [];
        //console.log("The templatebody is :%s \n",tembody);

        readbodyflag[i] = false;
        if (tembody.indexOf('$') > 0) {
          while (tembody.indexOf('$') > 0) {
            let s = tembody.indexOf('${');
            let e = tembody.indexOf('}', s);
            let str = tembody.substr(s + 2, e - s - 2);
            for (let k in readtags) {
              if (str == readtags[k].PlaceHolder) {
                readbodyflag[i] = true;            //if template body have tags in readtags need to readtag
                temread.push(readtags[k].Name);
                console.log('The temread tags %s', temread);
              }
            }
            tembody = tembody.replace('${' + str + '}', '0');
            //console.log("The repalced %s",tembody);
          }
        } else {
          readbodyflag[i] = false; //if have not $ ,not need to read tags
        }
        readsubtrg.push(temread);
      }
      console.log('readbodyflag is false or true\n');
      console.log(readbodyflag);
      console.log('The needed suscribe tags \n');
      console.log(subdata);
      console.log('The needed read tags \n');
      console.log(readdata);
      console.log('The subscribe trigger read tags \n');
      console.log(readsubtrg);
      console.log('The all data of webclient \n');
      //console.log(clientAll);
      console.log('The needed writeack tags \n');
      console.log(ackdata);
      //asyn sending subdata
      odksubscribe.send(subdata);
      //call child process to excute odk subscribe function
      odksubscribe.on('message', function(ressub) {
        if (ressub.error) {
          console.error( new ServerError('false', 803, ressub));
        }

        subresult = [];//the result of subscribe tags
        subfilter = [];
        console.log('***************The subscribed output data:************');
        console.log(ressub);  //output subscribed tags

        for (let i = 0; i < num; i++) {//every client
          let resultdata = [];
          let filterdata = [];
          sendbodyflag[i] = false;
          for (let k in clientAll[i].tags) { //current client all subscribe tags
            for (let j in ressub) {
              if (ressub[j].Name === clientAll[i].tags[k].Name) {
                let subResultObj = {}; //all subscribe tag result obj
                let filterDataObj = {}; //all satisfate filter result

                if (clientAll[i].tags[k].Filter !== 'undefined') {
                  if (myconvert.operator(ressub[j].Value, clientAll[i].tags[k].Filter)) {
                    filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                    filterdata.push(filterDataObj);

                    sendbodyflag[i] = true; //only have one statisfate needed to send
                    //console.log("The client %s subscribe data",subResultObj)
                  } else {
                    filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: 'The value is not satisfaction filter' };
                    filterdata.push(filterDataObj);
                  }
                } else {
                  filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                  filterdata.push(filterDataObj);  //not have filter send all tag value
                  //console.log("The client %s subscribe data",subResultObj)
                }

                subResultObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: ressub[j].Value };
                resultdata.push(subResultObj);
              }
            }
          }
          subresult.push(resultdata);
          subfilter.push(filterdata);
          //console.log(subresult);
        }
        //console.log('The all client subscribe data', subresult);
        console.log('The all client subscribe satisfaction fliter data', subfilter);


        //first subscribe not to send any data since first restart program
        if (initflag === false) {
          initflag = true;
          subResultOld = subresult;
        }

        for (let i = 0; i < num; i++) {
          readflag[i] = false;

          // current value is changed

          for (let n in subresult[i]) {
            if (subresult[i][n].Name === subResultOld[i][n].Name) {
              if (subresult[i][n].Value !== subResultOld[i][n].Value) {
                for (let m in clientAll[i].tags) {
                  if (clientAll[i].tags[m].Name === subResultOld[i][n].Name) {
                    if (clientAll[i].tags[m].Filter !== 'undefined') {
                      if (myconvert.operator(subresult[i][n].Value, clientAll[i].tags[m].Filter)) {
                        readflag[i] = true;
                      }
                    } else {
                      readflag[i] = true;
                    }
                  }
                }
              }
            }
          }

          subResultOld[i] = subresult[i];
          //the client have read tags and the trigger tag value changed
          //console.log('need to subscribe?', readflag, 'need to read?', readbodyflag);
          if ((readflag[i] === true) && (readbodyflag[i] === true)) {
            console.log('current client %s have tags changed,start to read', i);
            //console.log(readsubtrg[i]);
            //read the tags
            // eslint-disable-next-line no-shadow
            //call odk.read()
            console.log(readsubtrg[i]);
            odk.readtag(readsubtrg[i], function(res, err) {
              //console.log(res);
              if (err) {
                console.error( new ServerError('false', 802, err));
              }
              let writedata = []; //data formate {name:placeholder,value:value}
              //get all the tags and satisfate subscribe filter
              let resdata = [];
              let ackname = [];
              // eslint-disable-next-line no-shadow
              let ackvalue = [];
              //build the response data
              for (let k in readsubtrg[i]) {
                let tem = { Name: res.Name[k], Value: '' + res.Value[k] + '' };
                resdata.push(tem);
              }

              //get all ack tags
              for (let j in ackdata[i]) {
                if (ackdata[i][j].AcknowledgeTag) {
                  ackname.push(ackdata[i][j].Name);
                }
              }
              //get all the subscribe tags
              for (let x in subresult[i]) {
                if (subresult[i][x] !== '') {
                  let wobj = { name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value };
                  ackvalue.push(subresult[i][x].Value);
                  writedata.push(wobj);
                }
              }
              console.log('start to write ack tag into wincc, the arguments are:', ackname, ackvalue);
              //call odk.write()
              // eslint-disable-next-line no-shadow
              odk.writetag(ackname, ackvalue, function(response, err) {
                if (err) {
                  console.error( new ServerError('false', 800, err));
                }
                console.log(response);//response= write finished!"SetValue to Wincc!"
                //start to send data to opcenter
                sendflag = true;
              });
              // if write ack done
              if (sendflag === true) {
                //get all the read tags and satisfate read filter
                //get all the read tags
                for (let k in resdata) {
                  for (let m in readdata[i]) {
                    if (resdata[k].Name == readdata[i][m].Name) {
                      let robj = { name: readdata[i][m].PlaceHolder, value: resdata[k].Value };
                      writedata.push(robj);
                      // }
                    }
                  }
                }
                console.log('The writedata is :\n');
                console.log(writedata);
                if (writedata.length > 0) {   //if have data needed to send data
                  let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                  let subobj = {};
                  subobj.tag = writedata;
                  jsonobj.Tags = subobj;
                  console.log(jsonobj);
                  myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function(jsonvalue) { //transform value
                    console.log(jsonvalue);
                    let writeDataJson = {};
                    for (let x in writedata) {
                      writeDataJson[writedata[x].name] = jsonvalue[x];
                    }
                    let curentbody = clientAll[i].templatebody;

                    console.log('The  curentbody: %s', curentbody);
                    //replace $ built the sendbody
                    while (curentbody.indexOf('$') > 0) {
                      let s = curentbody.indexOf('${');
                      let e = curentbody.indexOf('}', s);
                      let str = curentbody.substr(s + 2, e - s - 2);
                      for (let { } in writeDataJson) {
                        if (str in writeDataJson) {
                          curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                        } else {
                          curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                        }
                      }
                      //console.log("The repalced %s",curentbody);
                    }
                    console.log('The repalced curentbody: %s', curentbody);

                    //send the replaced body to webclient

                    let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                    let opmethod = clientAll[i].method;             //get the OPcenter method
                    clientOptions.hostname = urlstr.hostname;
                    clientOptions.port = urlstr.port;
                    clientOptions.path = urlstr.pathname;
                    clientOptions.method = opmethod;
                    try {
                      clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                    // eslint-disable-next-line no-shadow
                    } catch (err) {
                      console.error(err);
                    }
                    clientOptions.headers['Content-Length'] = curentbody.length;
                    //clientOptions.headers["Content-Type"] = 'text/plain';
                    // eslint-disable-next-line no-shadow
                    let clientReq = https.request(clientOptions, function(res) {
                      res.setEncoding('utf-8');
                      res.on('data', function(chunk) {
                        try {
                          chunk = JSON.parse(chunk);
                        }
                        catch {
                          console.error( new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                        }
                        console.log(chunk);
                        if (chunk.error) {
                          console.error( new ServerError(500, 700, chunk.error.message));
                        }
                      });
                    });
                    clientReq.write(curentbody);
                    clientReq.on('error', function(e) {
                      console.error( new ServerError(500, 702, e.message));
                    });
                    clientReq.end();
                  });
                }
              }//end if sendflag
              else {
                console.log('write ack failed, do not send data to opcenter');
              }//end else
            });//end odk.readtag()
          } else if ((readflag[i] === true) && (readbodyflag[i] !== true)) {
            console.log('current client %s have tags changed, and no tags need to read', i);
            let writedata = []; //data formate {name:placeholder,value:value}

            //get all the subscribe tags

            for (let x in subresult[i]) {
              if (subresult[i][x] !== '') {
                let wobj = { name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value };
                writedata.push(wobj);
              }
            }

            console.log('The writedata is :\n');
            console.log(writedata);
            if (writedata.length > 0) {   //if have data needed to send data
              let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
              let subobj = {};
              subobj.tag = writedata;
              jsonobj.Tags = subobj;
              console.log(jsonobj);
              myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function(jsonvalue) { //transform value
                let writeDataJson = {};
                for (let x in writedata) {
                  writeDataJson[writedata[x].name] = jsonvalue[x];
                }
                console.log('The converted writedata is %s ', writeDataJson);
                let curentbody = clientAll[i].templatebody;
                //console.log(curentbody);

                while (curentbody.indexOf('$') > 0) {
                  let s = curentbody.indexOf('${');
                  let e = curentbody.indexOf('}', s);
                  let str = curentbody.substr(s + 2, e - s - 2);
                  for (let { } in writeDataJson) {
                    if (str in writeDataJson) {
                      curentbody = curentbody.replace('${' + str + '}', writeDataJson[str]);
                    } else {
                      curentbody = curentbody.replace('${' + str + '}', 'no macth value');
                    }
                  }
                  //console.log("The repalced %s",curentbody);
                }
                console.log('The repalced curentbody: %s', curentbody);

                //send the replaced body to webclient

                let urlstr = new URL(clientAll[i].URL);  //get the OPcenter URL
                let opmethod = clientAll[i].method;             //get the OPcenter method
                clientOptions.hostname = urlstr.hostname;
                clientOptions.port = urlstr.port;
                clientOptions.path = urlstr.pathname;
                clientOptions.method = opmethod;
                try {
                  clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                } catch (err) {
                  console.error(err);
                }
                clientOptions.headers['Content-Length'] = curentbody.length;
                //clientOptions.headers["Content-Type"]='text/plain';
                let clientReq = https.request(clientOptions, function(res) {
                  res.setEncoding('utf-8');
                  res.on('data', function(chunk) {
                    try {
                      chunk = JSON.parse(chunk);
                    }
                    catch {
                      console.error( new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                    }
                    console.log(chunk);
                    if (chunk.error) {
                      console.error(new ServerError(500, 700, chunk.error.message));
                    }
                  });
                });
                clientReq.write(curentbody);
                clientReq.on('error', function(e) {
                  console.error( new ServerError(500, 702, e.message));
                });
                clientReq.end();
              });
            }
          }
        }//end for
      });//end odksuvscribe.on
    });//end readxml-wincc
  }
}//end else branch

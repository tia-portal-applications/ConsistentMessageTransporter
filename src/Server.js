/* eslint-disable no-trailing-spaces */
/* eslint-disable no-empty */
/* eslint-disable brace-style */
/* eslint-disable eqeqeq */

/* start this server Example
REM example2: connect OpCenter to WinCC Unified
node Server.js --southboundservice=unified --serverconfig=TagConfiguration.xml --clientconfig=unified2opcenter --log=verbose/error/info

REM example4: connect OpCenter to WinCC V7.5(enable handshake )
node Server.js  --southboundservice=classic --serverconfig=WinccTagConfiguration.xml --clientconfig=wincc2opcenter --log=verbose/error/info
*/

process.title = 'RestApp_consistent_message';

const express = require('express');
const app = express();
const https = require('https');
const fs = require('fs');

const { URL } = require('url');
const myconvert = require('./convertdata');
const openpipe = require('./Openpipe');
const assetspath = __dirname + '/assets.xml';

// const RestServersOpcenterxmlpath = __dirname + '/RestServerConfiguration_OpCenter.xml';
// const tagxmlpath = __dirname + '/TagConfiguration.xml';
// const wincctagxmlpath = __dirname + '/WinccTagConfiguration.xml';

let RestServersOpcenterxmlpath = __dirname + '/RestServerConfiguration_OpCenter.xml';
let tagxmlpath = '';
let wincctagxmlpath = '';

const winccXsltPathAll = __dirname + '/winccValueconfig.xslt';
const xsltPathAll = __dirname + '/configtag.xslt';
const util = require('util');
const exec = util.promisify(require('child_process').execFile);

const { argv } = require('process');
const ServerError = require('./ServerError');
const log4js = require('./log4jsconfig.js');
const jwt = require('express-jwt');

//Added func
const funcWT = require('./writeTag.js');
const buFunc = require('./backUpFunc.js');
const funcRT = require('./readTag.js');

const logger = log4js.getLogger();
const logger2 = log4js.getLogger('Error');


/*********************************/
//Rest Server
/********************************/

//Set default flag for initialization
let Serverflag = true;  //Enables the handshake if true
let Branchflag = '0';
let tagconfigflag = '0';
let logflag = 'verbose';
let port = 23456;
let delay = 1000;

let serverConfig = '';

// parse arguments passed by the user
for (let i = 2; i < argv.length; i++) {
  const [key, value] = process.argv[i].split('=');
  switch (key.toLowerCase()) {
  case '--southboundservice':
    if (value.toLowerCase() === 'unified') {
      Branchflag = '0';
      port = 23456;
    } else if (value.toLowerCase() === 'classic') {
      Branchflag = '1';
      port=34567;
    } else if (value.toLowerCase() === 'iih') {
      Branchflag = '2';
      port=12345;
    }
    break;
  case '--clientconfig':
    if (value.toLowerCase() === 'unified2opcenter') {
      tagconfigflag = '0';
    } else if (value.toLowerCase() === 'wincc2opcenter') {
      tagconfigflag = '1';
    } else if (value.toLowerCase() === 'iih2opcenter') {
      tagconfigflag = '2';
    }

    break;
  case '--serverconfig':
    //Serverflag = value.toLowerCase() === 'opclient';
    serverConfig = value.toLowerCase();
    break;
  case '--log':
    if (value.toLowerCase() === 'error') {
      logflag = 'error';
    } else if (value.toLowerCase() === 'info') {
      logflag = 'info';
    } else if (value.toLowerCase() === 'verbose') {
      logflag = 'verbose';
    }
    break;
  case '--pollingcycle':
    delay = value.toLowerCase();
    break;
  case '--help':
    console.log(`
REM example2: connect OpCenter to WinCC Unified
node Server.js --southboundservice=unified --serverconfig=TagConfiguration.xml --clientconfig=unified2opcenter --log=verbose/error/info
    
REM example4: connect OpCenter to WinCC V7.5(enable handshake )
node Server.js  --southboundservice=classic --serverconfig=WinccTagConfiguration.xml --clientconfig=wincc2opcenter --log=verbose/error/info
      `);
    process.exit(0);
    break;
  default:
    break;
  }
}

//Get xml config file
if (Branchflag == 0 && tagconfigflag == 0) {
  //Unified
  tagxmlpath = __dirname + '/' + serverConfig;
} else if (Branchflag == 1 && tagconfigflag == 1) {
  //WinCC7
  wincctagxmlpath = __dirname + '/' + serverConfig;
} else if (Branchflag == 2 && tagconfigflag == 2) {
  //IIH
  wincctagxmlpath = __dirname + '/' + serverConfig;
}

//rewrite console function
console.log = function () {
  if (logflag == 'info' || logflag == 'verbose') {
    return logger.info.apply(logger, arguments);
  }
};
console.error = function () {
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
/* if (port == 12345) {
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
} */
https.createServer(serverOptions, app).listen(port); //start the server and listen at port


/******************************************/
//local test codes for our server
/*****************************************/

app.use(express.urlencoded({ extended: false }));
app.use(express.text());
app.use(express.json());

const authcert = fs.readFileSync(__dirname + '/certificate/server.crt');
const rurl = '/WinCCRestService/readtags';
const surl = '/subscribe';
const burl = '/browseTags';

//verify token from Server-Cert.pem
app.use(jwt({ secret: authcert, algorithms: ['RS256'] })
  .unless({ path: ['/', '/subscribe', '/browseTags'] }));

app.get('/', function (req, res, next) {
  console.log(req.body);
  res.send('trusted page');
});
/******************************************/
//read tag from opcenter
/*****************************************/
app.put(rurl, function (req, res, next) {
  //Opcenter jsondata={"variableName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
  //SAP jsondata= {"TagName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}

  funcRT.readTag(req, res, next, Branchflag, clientAll);
});

/******************************************/
//subscribe tag from opcenter
/*****************************************/
app.put(surl, function (req, res) {
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
  myconvert.converts(jsondata, function (commandtext) {
    //var datar = '[{"Name":"HMI_Tag_4","Value":"50"},{"Name":"HMI_Tag_5","Value":"40"}]';
    let datar = commandtext;
    openpipe.subscribetag(datar, function (reserr, resdata) {   //call subscribetag to get the subscribe resualt
      if (reserr === '') {
        // openpipe.unsubscribetag();
        res.statusCode = 200;

        for (let i in jsondata.variableNames) {                  //build the tag name, error code ...
          tem = { variableName: jsondata.variableNames[i], dataType: 1, value: resdata[i].Value, timestamp: resdata[i].TimeStamp, qualitycode: resdata[i].QualityCode, errorcode: resdata[i].ErrorCode };
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
//browser tag from opcenter
/*****************************************/
app.put(burl, function (req, res) {
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
  myconvert.convertb(jsondata, function (commandtext) {
    let datab = commandtext;
    openpipe.browseTags(datab, function (reserr, resdata) {   //call the brossetags to get browser tags
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

/******************************************/
//start write tag from opcenter
/*****************************************/

app.put('*', function (req, res, next) {
  //SAP jsondata=[{"TagName":"wincc2","TagValue":"123", }]
  //Opcenter jsondata=[{"variableName":"Order_Tag_4","value":"start"},{"variableName":"Order_Tag_5","value":"liangbai1"}]

  funcWT.writeTag(req, res, next, Serverflag, Branchflag, clientAll, rseq, ackvalue);
});

//app next middleware send response to webclient */
app.use(function (err, req, res, next) {
  if (err.status == 400) {
    err = ServerError.failed(400, 'Webclient request error');
  }
  if (err.name === 'UnauthorizedError') {
    err = ServerError.failed(401, 'token recognized failed');
  }
  let resdata = {
    '@odata.context': req.protocol + '://' + req.get('host') + req.originalUrl,
    'Succeeded': err.status,
    'Error': { Errorcode: err.code, Errormessage: err.msg }
  };
  if (resdata.Succeeded == 'true') {
    console.log(JSON.stringify(resdata));
  } else {
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


exec(authcreatetokenpath, ['certificate_path=' + authcertificatepath, 'password=' + authcertificatePassword], function (error, stdout, stderr) {
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

let num = 0;           //The number of server in tagconfiguration.xml
let num1 = 0;           //The number of client in restserverconfiguration.xml
let clientAll = [];   //all data tagconfiguration.xml [{CertificateID:$,URL:$,method:$,templatebody:$,tags:$,readtags:$},{},{}]
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

/** @type {Record<string, string | number | boolean>} ackvalue */
let ackvalue = {};
/** @type {Array<string | number | boolean>}  */
let rseq = [];

//other IIH variable
let assetsreq = [];
let aspectreq = [];
let variablereq = [];
let assetobj = {};
let aspectsobj = {};
let variableids = [];


//read tagconfiguration.xml
//Branchflag==='0' connect to unified
if (Branchflag === '0') {
  if (tagconfigflag == '0') {
    console.log('*************************************************************************');
    console.log('start server connected to Unified,port is %d', port);
    console.log('*************************************************************************');

    console.log('read unified tagconfiguration.xml');
    myconvert.readxml(tagxmlpath, function (fd) {
      const parsedFd = JSON.parse(fd);
      num = parsedFd.TagList.WebClient.length;   //get how many client element (OPcenter server)
      if (typeof (parsedFd.TagList.WebClient) === 'Object') {
        num = 1;  //if the tagconfigation.xml just have one webclient
      }
      console.log('Get the data from TagConfiguration.xml');
      for (let i = 0; i < parsedFd.TagList.WebClient.length; i++) {
        const client = parsedFd.TagList.WebClient[i];
        let obj = client.Tags.Tag;  //get the tag name which to subscribe from unified panel
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
              subobj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
              tags.push(subobj);              //all the onchange tags name
            } else {
              errwhat = 'The Trigger column of webclient[' + i + '] in TagConfiguration.xml';
              errmsg = { what: errwhat, value: obj[j].Trigger, reson: 'Trigger name has to be Onchange' };
              console.error(new ServerError(500, 505, errmsg));
            }
          } else {
            let readObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
            readtags.push(readObj);  //get all the read tags name
          }

          let allObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name, Filter: '' + obj[j].Filter };
          alltags.push(allObj);  //get all the tags name
        }
        //save all the webclient info
        let clientObj = { 'CertificateID': client.CertificateID, 'URL': client.CommandName, 'method': client.CommandMode, 'templatebody': client.TemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags };
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
              if (str === readtags[k].PlaceHolder) {
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
      openpipe.subscribetag(JSON.stringify(subdata), function (reserr, resdata) {  //start subscribe tags
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
                      filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value };
                      filterdata.push(filterDataObj);

                      sendbodyflag[i] = true; //only have one statisfate needed to send
                      //console.log("The client %s subscribe data",subResultObj)
                    } else {
                      filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: 'The value is not satisfaction filter' };
                      filterdata.push(filterDataObj);
                    }
                  } else {
                    // eslint-disable-next-line no-unused-vars
                    filterDataObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value };
                    filterdata.push(filterDataObj);  //not have filter send all tag value
                    //console.log("The client %s subscribe data",subResultObj)
                  }

                  subResultObj = { Name: clientAll[i].tags[k].Name, PlaceHolder: clientAll[i].tags[k].PlaceHolder, Value: resdata[j].Value };
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
              openpipe.readtag(JSON.stringify(readsubtrg[i]), function (reserr, resdata) {
                if (reserr.length == 0 || reserr == '') {
                  //console.log(resdata);
                  let writedata = []; //data formate {name:placeholder,value:value}
                  //get all the subscribe tags

                  for (let x in subresult[i]) {
                    if (subresult[i][x] !== '') {
                      let wobj = { name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value };
                      writedata.push(wobj);
                    }
                  }
                  //get all the read tags
                  for (let k in resdata) {
                    for (let m in readdata[i]) {
                      if (resdata[k].Name === readdata[i][m].Name) {
                        let robj = { name: readdata[i][m].PlaceHolder, value: resdata[k].Value };
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
                    myconvert.convertjsonviaxsl(jsonobj, xsltPathAll, function (jsonvalue) { //transform value
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
                      //clientOptions.headers["Content-Type"] = 'text/plain';
                      let clientReq = https.request(clientOptions, function (res) {
                        res.setEncoding('utf-8');
                        res.on('data', function (chunk) {
                          try {
                            chunk = JSON.parse(chunk);
                          } catch {
                            console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                          }
                          console.log(chunk);
                          if (chunk.error) {
                            console.error(new ServerError(500, 700, chunk.error.message));
                          }
                        });
                      });
                      clientReq.write(curentbody);
                      clientReq.on('error', function (e) {
                        console.error(new ServerError(500, 702, e.message));
                      });
                      clientReq.end();
                    });
                  }
                } else {
                  if (typeof (reserr) == 'string') {
                    console.error(new ServerError(500, 601, reserr));
                  }
                  let errTag = [];
                  for (let k in reserr) {
                    errTag.push(reserr[k].Name);
                  }
                  let errmsg = { variable: errTag, error: reserr[0].ErrorDescription };
                  console.error(new ServerError(500, 601, errmsg));
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
                myconvert.convertjsonviaxsl(jsonobj, xsltPathAll, function (jsonvalue) { //transform value
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
                  let clientReq = https.request(clientOptions, function (res) {
                    res.setEncoding('utf-8');
                    res.on('data', function (chunk) {
                      try {
                        chunk = JSON.parse(chunk);
                      } catch {
                        console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                      }
                      console.log(chunk);
                      if (chunk.error) {
                        console.error(new ServerError(500, 700, chunk.error.message));
                      }
                    });
                  });
                  clientReq.write(curentbody);
                  clientReq.on('error', function (e) {
                    console.error(new ServerError(500, 702, e.message));
                  });
                  clientReq.end();
                });
              }
            }
          }
        } else {
          if (typeof (reserr) == 'string') {
            console.error(new ServerError(500, 602, reserr));
          }
          let errTag = [];
          for (let i in reserr) {
            errTag.push(reserr[i].Name);
          }
          let errmsg = { variable: errTag, error: reserr[0].ErrorDescription };
          console.error(new ServerError(500, 602, errmsg));
        }
      });
    });
  }
}
//connect to IIH
else if (Branchflag === '2') {
  console.log('*************************************************************************');
  console.log('start server connected to IIH,port is %d', port);
  console.log('*************************************************************************');

  console.log('enable subscription from IIH');
  //read assets.xml file
  myconvert.readxml(assetspath, function (fd) {
    let allInfoArry = [];
    const parsedFd = JSON.parse(fd);
    num = parsedFd.assets.asset.length;
    //num=undefined when only one asset
    if (num == undefined) {
      let variableobj = {};
      let aspectobj = {};
      let variable = {};
      let variableary = [];
      let allInfo = {};
      aspectobj = parsedFd.assets.asset.children.aspect;
      variableobj = aspectobj.children.variable;
      for (let j in variableobj) {
        variable = { variableName: variableobj[j].variableName, dataType: variableobj[j].dataType, adapterId: variableobj[j].adapterId, topic: variableobj[j].topic };
        variableary.push(variable);
      }
      allInfo = { 'name': parsedFd.assets.asset.name, 'parentId': parsedFd.assets.asset.parentId, 'aspectName': aspectobj.aspectName, 'variable': variableary };
      allInfoArry.push(allInfo);
    } else {//there are two more asset in assets.xml
      for (let i = 0; i < num; i++) {
        let variableobj = {};
        let aspectobj = {};
        let variable = {};
        let variableary = [];
        let allInfo = {};

        aspectobj = parsedFd.assets.asset[i].children.aspect;
        variableobj = parsedFd.assets.asset[i].children.aspect.children.variable;
        for (let j in variableobj) {
          variable = { variableName: variableobj[j].variableName, dataType: variableobj[j].dataType, adapterId: variableobj[j].adapterId, topic: variableobj[j].topic };
          variableary.push(variable);
        }
        allInfo = { 'name': parsedFd.assets.asset[i].name, 'parentId': parsedFd.assets.asset[i].parentId, 'aspectName': aspectobj.aspectName, 'variable': variableary };
        allInfoArry.push(allInfo);
      }
    }

    //build allinfo arry to store all info from assets.xml
    for (let k in allInfoArry) {
      assetobj = { 'name': allInfoArry[k].name, 'parentId': allInfoArry[k].parentId };
      aspectsobj = { 'aspectName': allInfoArry[k].aspectName };
      assetsreq.push(assetobj);
      aspectreq.push(aspectsobj);
      variablereq.push(allInfoArry[k].variable);
    }
    console.log('**********readout JSONData from assets.xml are:*************');
    console.log(assetsreq);
    console.log(aspectreq);
    console.log(variablereq);

    //async function to post asset,aspects and variable to iih
    let postvariable = async function () {
      //configure assets post parameters
      let variableIds = [];
      let method = 'POST';

      for (let i in assetsreq) {
        //let assetid = JSON.parse(result).children[i].assetId
        let variableid = [];
        let url = '/AssetService/Assets/';
        let body = JSON.stringify(assetsreq[i]);

        //call post function and get assetid
        let result = await buFunc.connectIih(url, body, method);
        let assetid = result.assetId;
        if (assetid !== undefined) {
          //configure aspects post parameters
          body = JSON.stringify({ aspectName: aspectreq[i].aspectName, assetId: assetid });
          url = '/DataService/Aspects';

          //call post function and get aspectId
          result = await buFunc.connectIih(url, body, method);
          let aspectid = result.aspectId;

          //configure variable post  and post variable one by one
          url = '/DataService/Variables';
          for (let j = 0; j < variablereq[i].length; j++) {
            //build variable post body
            body = JSON.stringify({
              variableName: variablereq[i][j].variableName,
              dataType: variablereq[i][j].dataType,
              assetId: assetid,
              aspectId: aspectid,
              adapterId: variablereq[i][j].adapterId,
              topic: variablereq[i][j].topic
            });
            //call post function and get variableId
            result = await buFunc.connectIih(url, body, method);
            //console.log(result)
            //push variableId into array
            variableid.push(result.variableId);
          }
        }//end if result !==undefined
        else {
          let method2 = 'GET';
          let url2 = 'http://10.31.1.2:4203/Assetservice/Assets/Tree';
          result = await buFunc.connectIih(url2, body, method2);
          let children = result.children;
          //console.log("children",  result)
          //get exist assetid according to assetname
          for (let k in children) {
            if (assetsreq[i].name == children[k].name) {
              assetid = children[k].assetId;
            }
          }
          //configure the get variable url
          url2 = 'http://10.31.1.2:4203/DataService/Variables/?assetIds=[' + JSON.stringify(assetid) + ']';
          //get variableids
          result = await buFunc.connectIih(url2, body, method2);
          let variables = result.variables;
          //if get variableid failed, post aspect and variable again,to get error msg
          if (variables.length == 0) {
            url2 = 'http://10.31.1.2:4203/DataService/Aspects';
            //call post function and get aspectId
            result = await buFunc.connectIih(url2, '', method2);
            let aspect = result.aspects;
            let aspectid;
            for (let k in aspect) {
              if (aspectreq[i].aspectName == aspect[k].aspectName) {
                aspectid = aspect[k].aspectId;
              }
            }
            //configure variable post  and post variable one by one
            let url3 = '/DataService/Variables';
            for (let j = 0; j < variablereq[i].length; j++) {
              //build variable post body
              body = JSON.stringify({
                variableName: variablereq[i][j].variableName,
                dataType: variablereq[i][j].dataType,
                assetId: assetid,
                aspectId: aspectid,
                adapterId: variablereq[i][j].adapterId,
                topic: variablereq[i][j].topic
              });
              //call post function and get variableId
              result = await buFunc.connectIih(url3, body, method);
              //push variableId into array
              variableid.push(result.variableId);
            }
          } else {
            for (let k in variables) {
              variableid.push(variables[k].variableId);
            }//end for
          }//end else
        }//end else
        variableIds.push(variableid);
      }//end for
      //return promise obj
      return variableIds;
    };
    //main function to call IIH connetion and create asset.
    postvariable().then((v) => {
      //v is returned promise obj
      variableids = v;
      console.log('get variableids from IIH are:', variableids);
      console.log('IIH connection and post variable done,  enable usecase2/3');
      //switch iih usecase3 if serverflag=true

      if (Serverflag == true)//read ServerConfiguration_Opcenter.xml
      {
        console.log('enable usecase3 and read ServerConfiguration_Opcenter.xml');
        // eslint-disable-next-line no-shadow
        myconvert.readxml(RestServersOpcenterxmlpath, function (fd) {
          console.log('read ServerConfiguration_Opcenter.xml finished');
          let alltags = [];
          const parsedFd2 = JSON.parse(fd);
          num1 = parsedFd2.TagList.WebClient.length;//get how many client element (OPcenter server)
          if (num1 == undefined) {
            num1 = 1;  //if the RestServerconfigation_OpCenter.xml just have one webclient
          }
          for (let i = 0; i < parsedFd2.TagList.WebClient.length; i++) {
            const client = parsedFd2.TagList.WebClient[i];
            let alltag = [];
            let obj = client.Tags.Tag;
            for (let j in obj) {
              let allObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name };
              alltag.push(allObj);  //get all the tags name
            }
            alltags.push(alltag);
            //save all client infos
            const clientObj = { URL: client.CommandName, method: client.CommandMode, templatebody: client.TemplateBody, alltags: alltag };
            clientAll.push(clientObj);
          }
          //console.log(clientAll)
        });
      }//end if serverflag =true
      if (tagconfigflag == '2') {//read wincctagConfiguration.xml
        console.log('enable usecase2 and read wincctagconfig.xml');
        // eslint-disable-next-line no-shadow
        myconvert.readxml(wincctagxmlpath, function (fd) {
          console.log('read wincctagConfiguration.xml finished');
          const parsedFd2 = JSON.parse(fd);
          num = parsedFd2.TagList.WebClient.length;   //get how many client element (OPcenter server)
          if (typeof (parsedFd2.TagList.WebClient) === 'Object') {
            num = 1;  //if the tagconfigation.xml just have one webclient
          }
          console.log('Get the data from TagConfiguration.xml');
          for (let i = 0; i < parsedFd2.TagList.WebClient.length; i++) {
            const client = parsedFd2.TagList.WebClient[i];
            let obj = client.Tags.Tag;  //get the tag name which to subscribe from unified panel
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
            let clientObj = { 'CertificateID': client.CertificateID, 'URL': client.CommandName, 'method': client.CommandMode, 'templatebody': client.TemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags };
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
          //console.log('The all data of webclient \n');
          //console.log(clientAll);
          //console.log('The needed writeack tags \n');
          //console.log(ackdata);

          //select need to read variableids
          let subiih = [];
          for (let i in subdata) {
            for (let j in variablereq) {
              for (let k in variablereq[j]) {
                if (subdata[i] == variablereq[j][k].variableName) {
                  subiih.push(variableids[j][k]);
                }
              }
            }
          }
          console.log('The needed subscribe ID from IIH\n');
          console.log(subiih);
          //set timegap variable =1s, which means from-to timegap for IIH get variable value equal 1s
          let timegap = 1;
          let timegapflag;
          //endless loop to get variable values
          setInterval(function () {
            //set inital timegapflag=true;
            timegapflag = true;
            console.log('The time gap now is:%d s', timegap);
            let geturl = 'http://10.31.1.2:4203/Dataservice/Data/?variableIds=' + JSON.stringify(subiih) + '&from=' + buFunc.getRealTime(timegap) + '&to=' + buFunc.getRealTime();
            console.log('IIH server request url is:', geturl);
            buFunc.getvariable(geturl, function (res) {
              // console.log('IIH server response datastring is:', res);
              let obj = JSON.parse(res).data;
              let ressub = [];
              console.log('IIH server response data is:', obj);
              //get value
              for (let i in obj) {
                let final = {};
                //if value is not []
                if (obj[i].values.length !== 0) {
                  breakflag = false;
                  //get the last value(newest value)
                  let latest = obj[i].values.length - 1;
                  for (let k in subdata) {
                    for (let j in variablereq[i]) {
                      if (subdata[k] == variablereq[i][j].variableName) {
                        final = { Name: variablereq[i][j].variableName, Value: obj[i].values[latest].value };
                      }
                    }
                  }
                } else {
                  console.log('get value from iih failed, timegap +30s and resend request');
                  //timegapflag=false
                  timegapflag = false;
                  for (let k in subdata) {
                    for (let j in variablereq[i]) {
                      if (subdata[k] == variablereq[i][j].variableName) {
                        final = { Name: variablereq[i][j].variableName, Value: null };
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
                //console.log(subresult[i]);
                //console.log(subResultOld[i]);
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
                    } else {
                      //else the value is not changed, do not have to send to opcenter
                      console.log('The value is same, do not need to send request');
                    }
                  }
                }

                subResultOld[i] = subresult[i];
                //the client have read tags and the trigger tag value changed
                // get readtag's value from IIH and send to opcenter
                if ((readflag[i] === true) && (readbodyflag[i] === true)) {
                  console.log('current client %s have tags changed,start to read', i);
                  //configure needed variableId of IIH request url
                  let readiih = [];
                  for (let j in readsubtrg[i]) {
                    for (let k in variablereq) {
                      for (let l in variablereq[k]) {
                        if (readsubtrg[i][j] == variablereq[k][l].variableName) {
                          readiih.push(variableids[k][l]);
                        }
                      }
                    }
                  }
                  //configure complete iih request url
                  geturl = 'http://10.31.1.2:4203/Dataservice/Data/?variableIds=' + JSON.stringify(readiih) + '&from=' + buFunc.getRealTime(timegap) + '&to=' + buFunc.getRealTime();
                  console.log('IIH server request url is:', geturl);
                  //call getvariable function to get variable value of readtag from IIH and send to opcenter
                  buFunc.getvariable(geturl, function (resobj) {
                    //console.log(res);
                    obj = JSON.parse(resobj).data;
                    //get latest one value from resobj
                    let result = [];

                    for (let j in obj) {
                      if (obj[j].values.length !== 0) {
                        let lastone = obj[j].values.length - 1;
                        result.push(obj[j].values[lastone].value);


                        //data formate {name:placeholder,value:value}
                        let writedata = [];
                        let resdata = [];
                        //build the response data
                        for (let k in readsubtrg[i]) {
                          let tem = { Name: readsubtrg[i][k], Value: '' + result[k] + '' };
                          resdata.push(tem);
                        }
                        console.log('The readed Tags&Values from IIH response are:', resdata);
                        //get all the subscribe tags
                        for (let x in subresult[i]) {
                          if (subresult[i][x] !== '') {
                            let wobj = { name: subresult[i][x].PlaceHolder, value: subresult[i][x].Value };
                            writedata.push(wobj);
                          }
                        }
                        //get all the read tags
                        for (let k in resdata) {
                          for (let m in readdata[i]) {
                            if (resdata[k].Name === readdata[i][m].Name) {
                              let robj = { name: readdata[i][m].PlaceHolder, value: resdata[k].Value };
                              writedata.push(robj);
                            }
                          }
                        }
                        // console.log('The writedata is :\n');
                        // console.log(writedata);
                        if (writedata.length > 0) {   //if have data needed to send data
                          let jsonobj = {};    //jsonobj={ Tags: { tag: [ [Object], [Object], [Object] ] } }
                          let subobj = {};
                          subobj.tag = writedata;
                          jsonobj.Tags = subobj;
                          //translate value
                          myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function (jsonvalue) {
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
                              clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                            } catch (err) {
                              console.error(err);
                            }
                            clientOptions.headers['Content-Length'] = curentbody.length;
                            //clientOptions.headers["Content-Type"] = 'text/plain';

                            // eslint-disable-next-line no-shadow
                            let clientReq = https.request(clientOptions, function (res) {
                              res.setEncoding('utf-8');
                              res.on('data', function (chunk) {
                                try {
                                  chunk = JSON.parse(chunk);
                                } catch {
                                  console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                                }
                                console.log(chunk);
                                if (chunk.error) {
                                  console.error(new ServerError(500, 700, chunk.error.message));
                                }
                              });
                            });
                            clientReq.write(curentbody);
                            clientReq.on('error', function (e) {
                              console.error(new ServerError(500, 702, e.message));
                            });
                            clientReq.end();
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
                    myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function (jsonvalue) {
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
                        clientOptions.cert = fs.readFileSync(clientAll[i].CertificateID + 'opcenter_cert.pem');
                      } catch (err) {
                        console.error(err);
                      }
                      clientOptions.headers['Content-Length'] = curentbody.length;
                      //clientOptions.headers["Content-Type"]='text/plain';
                      // eslint-disable-next-line no-shadow
                      let clientReq = https.request(clientOptions, function (res) {
                        res.setEncoding('utf-8');
                        res.on('data', function (chunk) {
                          try {
                            chunk = JSON.parse(chunk);
                          } catch {
                            console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                          }
                          console.log(chunk);
                          if (chunk.error) {
                            console.error(new ServerError(500, 700, chunk.error.message));
                          }
                        });
                      });
                      clientReq.write(curentbody);
                      clientReq.on('error', function (e) {
                        console.error(new ServerError(500, 702, e.message));
                      });
                      clientReq.end();
                    });
                  }
                }
              }//end for
              //if timegapflag=false, timegap+30s
              if (timegapflag === false) {
                timegap += 30;
              }
            });//end getsvariable
          }, delay);//end loop
        });//end read winncctagxml
      }//end if tagconfigflag=2
    }).catch(function (reason) {
      if (reason.code == 'ECONNREFUSED') {
        console.error(new ServerError(500, 900, reason.message));
      } else {
        console.error(new ServerError(500, 902, reason));
      }
    });//end postvariable.then
  });//end assets.xml file
}
//connect to wincc
else if (Branchflag === '1') {
  const odk = require('./odk');
  const fork = require('child_process').fork;
  const odksubscribeAcknowledge = fork('./odksubscribe.js');
  const odksubscribeTrigger = fork('./odksubscribe.js');
  console.log('*************************************************************************');
  console.log('start server connected to WinCC V7.5,port is %d', port);
  console.log('*************************************************************************');
  if (Serverflag == true)//read ServerConfiguration_Opcenter.xml
  {
    console.log('start up usecase3 and read RestServerConfigureation_Opcenter.xml');
    myconvert.readxml(RestServersOpcenterxmlpath, function (fd) {
      console.log('read ServerConfiguration_Opcenter.xml finished');
      let suback = [];
      let readseq = [];
      let alltags = [];
      const parsedFd = JSON.parse(fd);
      num = parsedFd.TagList.WebClient.length;//get how many client element (OPcenter server)
      if (num == undefined) {
        num = 1;  //if the RestServerconfigation_OpCenter.xml just have one webclient
      }
      console.log(num);
      for (let i = 0; i < parsedFd.TagList.WebClient.length; i++) {
        const client = parsedFd.TagList.WebClient[i];
        let alltag = [];
        let obj = client.Tags.Tag;
        for (let j in obj) {
          let allObj = { PlaceHolder: '' + obj[j].PlaceHolder, Name: '' + obj[j].Name };
          alltag.push(allObj);  //get all the tags name
        }
        alltags.push(alltag);
        if (client.AcknowledgeTag) {
          suback.push(client.AcknowledgeTag);//get the ackname which need to subscribe from restserverconfig_Opcenter.xml
        }
        if (client.SequenceTag) {
          readseq.push(client.SequenceTag);  //get the seqname which need to read from restserverconfig_Opcenter.xml
        }
        //save all client infos
        const clientObj = { URL: client.CommandName, method: client.CommandMode, templatebody: client.TemplateBody, suback: client.AcknowledgeTag, readseq: client.SequenceTag, alltags: alltag };
        clientAll.push(clientObj);
      }
      //console.log(suback)
      console.log(readseq);
      console.log('enable usecase3 for wincc v7.5');
      odk.readtag(readseq, function (readresult, err) {
        if (err) {
          console.error(new ServerError('false', 802, err));
        }

        console.log(readresult);
        for (let i in readresult.Value) {
          rseq.push(readresult.Value[i]);
        }
        console.log('startup process reads seq value is:', rseq);
      });

      odksubscribeAcknowledge.send(suback);
      odksubscribeAcknowledge.on('message', function (ressub) {
        //resub=[ { Name: 'NewOrder_ack', Value: 102 },{ Name: 'NewOrder_ack2', Value: 101 } ]
        //clean ackvalue array
        if (ressub.error) {
          console.error(new ServerError('false', 803, ressub));
        } else {
          //get ackvalue from ressub
          for (let i in ressub) {
            ackvalue[ressub[i].Name] = ressub[i].Value;

            console.log('///////////', ackvalue);
          }
          console.log('The All subscribed ackvalues are:', ackvalue);
        }
      });
    });//end readxml-restserver
  }//end  Serverflag=true
  if (tagconfigflag == '1') {// read wincctagconfiguration.xml instead
    console.log('start up usecase2_handshake ');
    myconvert.readxml(wincctagxmlpath, function (fd) {
      console.log('read wincctagConfiguration.xml finished');
      const parsedFd = JSON.parse(fd);
      num = parsedFd.TagList.WebClient.length;   //get how many client element (OPcenter server)
      if (typeof (parsedFd.TagList.WebClient) === 'Object') {
        num = 1;  //if the tagconfigation.xml just have one webclient
      }
      console.log('Get the data from TagConfiguration.xml');
      for (let i = 0; i < parsedFd.TagList.WebClient.length; i++) {
        const client = parsedFd.TagList.WebClient[i];
        let obj = client.Tags.Tag;  //get the tag name which to subscribe from unified panel
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
              errwhat = 'The Trigger column of webclient ' + client.CommandName + ' in WinccTagConfiguration.xml';
              errmsg = { what: errwhat, value: obj[j].Trigger, reson: 'Trigger name has to be Onchange' };
              console.error(new ServerError(500, 505, errmsg));
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
        let clientObj = { 'CertificateID': client.CertificateID, 'URL': client.CommandName, 'method': client.CommandMode, 'templatebody': client.TemplateBody, 'tags': tags, 'readtags': readtags, 'alltags': alltags };
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
      odksubscribeTrigger.send(subdata);
      //call child process to excute odk subscribe function
      odksubscribeTrigger.on('message', function (ressub) {
        if (ressub.error) {
          console.error(new ServerError('false', 803, ressub));
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
            odk.readtag(readsubtrg[i], function (res, err) {
              //console.log(res);
              if (err) {
                console.error(new ServerError('false', 802, err));
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
              odk.writetag(ackname, ackvalue, function (response, err) {
                if (err) {
                  console.error(new ServerError('false', 800, err));
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
                  myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function (jsonvalue) { //transform value
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
                    let clientReq = https.request(clientOptions, function (res) {
                      res.setEncoding('utf-8');
                      res.on('data', function (chunk) {
                        try {
                          chunk = JSON.parse(chunk);
                        } catch {
                          console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                        }
                        console.log(chunk);
                        if (chunk.error) {
                          console.error(new ServerError(500, 700, chunk.error.message));
                        }
                      });
                    });
                    clientReq.write(curentbody);
                    clientReq.on('error', function (e) {
                      console.error(new ServerError(500, 702, e.message));
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
              myconvert.convertjsonviaxsl(jsonobj, winccXsltPathAll, function (jsonvalue) { //transform value
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
                let clientReq = https.request(clientOptions, function (res) {
                  res.setEncoding('utf-8');
                  res.on('data', function (chunk) {
                    try {
                      chunk = JSON.parse(chunk);
                    } catch {
                      console.error(new ServerError(500, 701, 'Opcenter_Server resource Not found'));
                    }
                    console.log(chunk);
                    if (chunk.error) {
                      console.error(new ServerError(500, 700, chunk.error.message));
                    }
                  });
                });
                clientReq.write(curentbody);
                clientReq.on('error', function (e) {
                  console.error(new ServerError(500, 702, e.message));
                });
                clientReq.end();
              });
            }
          }
        }//end for
      });//end odksuvscribe.on
    });//end readxml-wincc
  }//end if tagconfigflag==1
}//end else branch

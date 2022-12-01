/* eslint-disable padded-blocks */
/* eslint-disable no-multiple-empty-lines */
/* eslint-disable no-trailing-spaces */
/* eslint-disable eqeqeq */
/* eslint-disable brace-style */

const myconvert = require('./convertdata');

const openpipe = require('./Openpipe');
const winccXsltPathAll = __dirname + '/winccValueconfig.xslt';
let odk = null;

const ServerError = require('./ServerError');
let buFunc = require('./backUpFunc.js');

exports.readTag = function(req, res, next, Branchflag, clientAll) {

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
  let error = buFunc.readhandlejsonobj(jsondata)[1];
  if (error !== undefined) {
    errres = ServerError.failed(error.errcode, error.errmsg);
    return next(errres);
  }
  //get jsondata keys
  let jsonkeys = buFunc.readhandlejsonobj(jsondata)[0];
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
    if (!odk) {
      odk = require('./odk');
    }
  
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
};

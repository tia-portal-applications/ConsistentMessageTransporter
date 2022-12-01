/* eslint-disable eqeqeq */
const { json } = require('express');
const os = require('os');
let net = require('net');
let readline = require('readline');
const { rootCertificates } = require('tls');

// If the current system is windows
if (os.type() == 'Windows_NT') {
  PIPE_PATH = '\\\\.\\pipe\\HmiRuntime';
// if the current system is Linux
} else if (os.type() == 'Linux') {
  PIPE_PATH = '/tempcontainer/HmiRuntime';
} else {
  PIPE_PATH = '/tempcontainer/HmiRuntime';
}

//via openpipe to write tags
exports.writetag = function(data, callback) {
  let res = '';
  let err = '';
  let client = net.connect(PIPE_PATH, function() {
    console.log('************************************************************************\n*openpipe client: on connection');
    let tagwritecommand =  '{"Message":"WriteTag","Params":{"Tags":' + data + '},"ClientCookie":"CookiewriteTags"}\n';
    client.write(tagwritecommand);
    console.log('start writetag');
    const rl = readline.createInterface({
      input: client,
      crlfDelay: Infinity
    });
    rl.on('line', function(line) {
      let obj = JSON.parse(line);
      if (obj.Message == 'NotifyWriteTag') {
        res = obj.Params.Tags;
        err = res.filter((item)=>{
          if (item.ErrorDescription) {
            return item;
          }
        });
      }
      if (obj.Message == 'ErrorWriteTag') {
        err = obj;
      }
      client.end();

      return callback(err, res);
    });
  }).on('error', (error)=>{
    err = error.message;
    callback(err, res);
  });
  client.on('end', function() {
    console.log('*openpipe client writetag end\n************************************************************************\n');
  });
};
//via openpipe to read tags
//data formate：["<Tag>","<Tag>"]
//res formate：[
//    {"Name":"<Tag>","Quality":"<Value>","QualityCode":"<Value>","TimeStamp":"<Value>","Value":"<TagValue>","ErrorCode":<Value>,"ErrorDescription":"<ErrorText>"},
//    {"Name":"<Tag>","Quality":"<Value>","QualityCode":"<Value>","TimeStamp":"<Value>","Value":"<TagValue>","ErrorCode":<Value>,"ErrorDescription":"<ErrorText>"}]
exports.readtag = function(data, callback) {
  let res = '';
  let err = '';
  let client = net.connect(PIPE_PATH, function() {
    console.log('************************************************************************\n*openpipe client: on connection');
    let tagreadcommand = '{"Message":"ReadTag","Params":{"Tags":' + data + '},"ClientCookie":"CookiereadTags"}\n';
    console.log('connand text :' + tagreadcommand);
    client.write(tagreadcommand);
    console.log('openpipe client : start readtag');
    const rl = readline.createInterface({
      input: client,
      crlfDelay: Infinity
    });
    rl.on('line', function(line) {
      let obj = JSON.parse(line);
      if (obj.Message == 'NotifyReadTag') {
        res = obj.Params.Tags;
        err = res.filter((item)=>{
          if (item.ErrorDescription) {
            return item;
          }
        });
      }
      if (obj.Message == 'ErrorReadTag') {
        err = obj;
      }

      client.end();
      console.log('read tags :');
      console.log(res);
      return callback(err, res);
    });
  }).on('error', (error)=>{
    err = error.message;
    callback(err, res);
  });
  client.on('end', function() {
    console.log('*openpipe client readtag end\n************************************************************************');
  });
  //return {resdata:res,errdata:err};
};
//via openpipe subscribe tags
exports.subscribetag = function(data, callback) {
  let res = '';
  let err = '';

  let client = net.connect(PIPE_PATH, function() {
    console.log('************************************************************************\n*openpipe client: on connection');

    let subscribecommand = '{"Message":"SubscribeTag","Params":{"Tags":' + data + '},"ClientCookie":"CookieSubscription"}\n';
    console.log('connand text :' + subscribecommand);
    client.write(subscribecommand);
    console.log('openpipe client : start subscribe');
    const rl = readline.createInterface({
      input: client,
      crlfDelay: Infinity
    });
    rl.on('line', function(line) {
      let obj = JSON.parse(line);
      if (obj.Message == 'NotifySubscribeTag') {
        res = obj.Params.Tags;
        err = res.filter((item)=>{
          if (item.ErrorDescription) {
            return item;
          }
        });
      }
      if (obj.Message == 'ErrorSubscribeTag') {
        err = obj;
      }

      //client.end();
      return callback(err, res);
    });
  }).on('error', (error)=>{
    err = error.message;
    callback(err, res);
  });


  client.on('end', function() {
    console.log('*openpipe client subscribe end\n************************************************************************');
  });
};
//via openpipe cancel subscribe
exports.unsubscribetag = function(callback) {
  let res = '';
  let err = '';
  let client = net.connect(PIPE_PATH, function() {
    console.log('************************************************************************\n*openpipe client: on connection');

    let unsubscribecommand = '{"Message":"UnsubscribeTag","ClientCookie":"CookieunSubscription"}\n';
    console.log('connand text :' + unsubscribecommand);
    client.write(unsubscribecommand);
    console.log('openpipe client : start unsubscribe');
    const rl = readline.createInterface({
      input: client,
      crlfDelay: Infinity
    });
    rl.on('line', function(line) {
      let obj = JSON.parse(line);
      if (obj.Message == 'NotifyUnsubscribeTag') {
        res = obj.parses.Tags;
        err = res.filter((item)=>{
          if (item.ErrorDescription) {
            return item;
          }
        });
      }
      if (obj.Message == 'ErrorUnsubscribeTag') {
        err = obj;
      }

      client.end();
      return callback(err, res);
    });
  }).on('error', (error)=>{
    err = error.message;
    callback(err, res);
  });
  client.on('end', function() {
    console.log('*openpipe client unsubscribe end\n************************************************************************');
  });
};
//via openpipe browser tags
exports.BrowseTags = function(data, callback) {
  let res = '';
  let err = '';
  let client = net.connect(PIPE_PATH, function() {
    console.log('************************************************************************\n*openpipe client: on connection');
    /*
        var browseobjcommand = {
            "Message": "BrowseTags",
            "Params": {
                "LanguageId": languageid,   //optional 显示语言 2052
                "Filter": filter,           //optional 过滤字符串"*"
                                            // Available attributes:	Name, DisplayName, DataType, Connection, AcquisitionCycle
                                            //							AcquisitionMode, MaxLength, Persistent, InitialValue, SubstituteValue,
                                            //							InitialMaxValue, InitialMinValue, SubstituteValueUsage
                "Attributes": attributes,   //optional 返回变量属性 * 代表所有属性 ["DataType", "Connection", "AcquisitionCycle", "AcquisitionMode", "MaxLength", "Persistent", "InitialValue"],
                "PageSize": pagesize,       //optional 导出变量数量 100,
                "SystemNames": systemname   //optional 项目名称 ["HMI_RT_1"]
            },
            "ClientCookie": "BrowseRequesttag"
        };
        */
    let browseobjcommand = {'Message': 'BrowseTags', 'Params': data, 'ClientCookie': 'BrowseRequesttag'};
    let start = new Date();
    let browsestrcommand = JSON.stringify(browseobjcommand) + '\n';
    console.log('connand text :' + browsestrcommand);
    const rl = readline.createInterface({
      input: client,
      crlfDelay: Infinity
    });
    rl.on('line', function(line) {
      let end = new Date();
      let obj = JSON.parse(line);
      if (obj.Message == 'NotifyBrowseTags') {
        res = obj.Params.Tags;
        wtireTagstohtml(res);
      }
      if (obj.Message == 'ErrorBrowseTags') {
        err = obj;
        client.end();
      }
      if (obj.Message == 'NotifyBrowseTags') {
        if (obj.Params.Tags.length > 0) {
          browseCommandObjTags = {
            'Message': 'BrowseTags',
            'Params': 'Next',
            'ClientCookie': 'myBrowseTagRequest1'
          };
          jsonCommandTags = JSON.stringify(browseCommandObjTags) + '\n';
          client.write(jsonCommandTags);
        } else {
          let ms = end.getTime() - start.getTime();
          let min = ms / 1000.0 / 60.0;
          console.log('Dauer: ' + min + ' min.');

          client.end();
          return callback(err, res);
        }
      }
    });
    client.write(browsestrcommand);
    console.log('openpipe client : start browsetags');
  }).on('error', (error)=>{
    err = error.message;
    callback(err, res);
  });
  client.on('end', function() {
    console.log('*openpipe client browsetags end\n************************************************************************');
  });
};
//print had browsered tags
function wtireTagstohtml(data) {
  for (let i = 0; i < data.length; i++) {
    let tagName = data[i].Name;
    let displayName = data[i].DisplayName;
    let acquisitionMode = data[i].AcquisitionMode;
    let persistent = data[i].Persistent;
    let dataType = data[i].DataType;
    let connection = data[i].Connection;
    let acquisitionCycle = data[i].AcquisitionCycle;

    let maxLength = data[i].MaxLength;
    let initialValue = data[i].InitialValue;

    console.log('\nTagName: ' + tagName + '\nDisplayName: ' + displayName +
            '\nAcquisitionMode: ' + acquisitionMode + '\nPersistent: ' + persistent + '\nDataType: ' + dataType +
            '\nConnection: ' + connection + '\nAcquisitionCycle: ' + acquisitionCycle +
            '\nMaxLength: ' + maxLength + '\nInitialValue: ' + initialValue);
  }
}

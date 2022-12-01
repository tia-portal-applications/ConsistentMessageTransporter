let fs = require('fs');
let xml2js = require('xml2js');
let builder = new xml2js.Builder();
//const log4js = require('./log4jsconfig.js');
//const logger = log4js.getLogger();
//const logger2 = log4js.getLogger('Error');
//console.log = function() {
//  return logger.info.apply(logger, arguments);
//};
//console.error = function() {
//  return logger2.error.apply(logger2, arguments);
//};
//from put body data, convert to the openpipe write needed data formate
exports.convertw = function(name, value, callback) {
  let tem = [];
  let tems = Object;
  let temd = '';

  console.log('************************************************************************\n*start convert');

  for (let i in name) {
    //console.log(typeof data[i].variableName);
    //tems= {"Name": data[i].variableName,"Value":data[i].value};
    // tem.push(tems);

    //tems= {Name:data[i].variableName,Value:""+data[i].value};
    tems = {Name: name[i].name, Value: '' + value[i]};
    tem.push(tems);
  }


  temd = JSON.stringify(tem);
  console.log('converted data : \n' + temd);
  console.log('*finished convert\n************************************************************************\n');
  callback(temd);
};
//from put body data, convert to the openpipe read needed data formate
exports.convertr = function(data, callback) {
  let tems = Object;
  let temd = '';

  console.log('************************************************************************\n*start convert');

  tems = data.variableNames;

  temd = JSON.stringify(tems);
  console.log('converted data : \n' + temd);
  console.log('*finished convert\n************************************************************************\n');
  callback(temd);
};
//from put body data, convert to the openpipe subscribe needed data formate
exports.converts = function(data, callback) {
  let tems = Object;
  let temd = '';

  console.log('************************************************************************\n*start convert');

  tems = data.variableNames;

  temd = JSON.stringify(tems);
  console.log('converted data : \n' + temd);
  console.log('*finished convert\n************************************************************************\n');
  callback(temd);
};
//from put body data, convert to the openpipe broswer needed data formate
exports.convertb = function(data, callback) {
  let temd = '';

  console.log('************************************************************************\n*start convert');

  temd = {
    'LanguageId': data.LanguageId,
    'Filter': data.Filter,
    'Attributes': data.Attributes,
    'PageSize': data.PageSize,
    'SystemNames': data.SystemNames
  };
  console.log('converted data : \n');
  console.log(temd);
  console.log('*finished convert\n************************************************************************\n');
  callback(temd);
};
//read a xml file and convert to JSON string
exports.readxml = function(path, callback) {
  fs.readFile(path, 'utf-8', function(err, rdata) {
    if (!err) {
      //explicitArray : true parse arry，defult is true
      //ignoreAttrs: true ingnore parse attribut
      // eslint-disable-next-line no-shadow
      xml2js.parseString(rdata, {explicitArray: false, ignoreAttrs: false }, function(err, jdata) {
        if (!err) {
          console.log('************************************************************************\n*start converted xmldata');
          console.log('read xml file successed');
          //console.log(JSON.stringify(JSON.stringify(jdata)));
          console.log('*end converted xmldata\n************************************************************************\n');
          callback(JSON.stringify(jdata));
          //callback(util.inspect(jdata, false, null));
        } else {
          console.error(err);
          console.log('*parseString xmlfile error\n************************************************************************\n');
        }
      });
    } else {
      console.log('************************************************************************\n*read xmlfile error');
      console.error(err);
      console.log('*read xmlfile error\n************************************************************************\n');
    }
  });
};
//read a xml and a xsl file convert to a JSON formate
exports.convertxml = function(xmlpath, xslpath, callback) {
  let xmlbuf = fs.readFileSync(xmlpath, 'utf-8');
  let xslbuf = fs.readFileSync(xslpath, 'utf-8');
  console.log('read xml and xsl file successed');
  xml2js.parseString(xmlbuf, {explicitArray: false, ignoreAttrs: false }, function(err, jdata) {
    if (!err) {
      console.log('************************************************************************\n*start convert xmlfile');
      console.log('convert xml to string successed');
      console.log(JSON.stringify(JSON.stringify(jdata)));
      console.log('*end convert xmlfile\n************************************************************************\n');
      let xmldom = builder.buildObject(jdata);
      console.log(jdata);
      console.log(typeof jdata.Tags.tag[0]);
      console.log(jdata.Tags.tag[0]);
      console.log(xmldom);
      // eslint-disable-next-line no-shadow
      xml2js.parseString(xslbuf, {explicitArray: false, ignoreAttrs: false }, function(err, jdata) {
        if (!err) {
          console.log('************************************************************************\n*start convert xslfile');
          console.log('convert xsl to string successed');
          console.log(JSON.stringify(JSON.stringify(jdata)));
          console.log('*end converted xmlfile\n************************************************************************\n');
          let xsldom = builder.buildObject(jdata);

          let xsltProcess = require('xslt-processor');
          let outXmlString = xsltProcess.xsltProcess(
            xsltProcess.xmlParse(xmldom),
            xsltProcess.xmlParse(xsldom));
          // eslint-disable-next-line no-shadow
          xml2js.parseString(outXmlString, function(err, outjson) {
            if (!err) {
              console.log('************************************************************************\n*start convert xslfile to json');
              console.log('convert xsl to json successed');
              console.log(outjson);
              console.log('*end converted xmlfile to json\n************************************************************************\n');

              callback(outjson);
            } else {
              console.log(err);
            }
          });
        } else {
          console.log(err);
        }
      });
    } else {
      console.log(err);
    }
  });
};
//via a json and a xslt file convert to a JSON formate
//callback={ Tags: { tag: [ '1', '0', '300' ] } }
exports.convertjsonviaxsl = function(xmljson, xslpath, callback) {
  let xslbuf;
  try {
    xslbuf = fs.readFileSync(xslpath, 'utf-8');
  } catch (error) {
    console.error(error);
  }

  console.log('read xslt file successed');
  let xmldom = builder.buildObject(xmljson);
  console.log('************************************************************************\n*start convert json to xmlstring');
  console.log('convert json to xml successed');
  //console.log(xmldom);
  console.log('*end convert json to xmlstring\n************************************************************************\n');

  xml2js.parseString(xslbuf, {explicitArray: false, ignoreAttrs: false }, function(err, jdata) {
    if (!err) {
      console.log('************************************************************************\n*start convert xslfile');
      console.log('convert xsl to string successed');
      //console.log(JSON.stringify(JSON.stringify(jdata)));
      console.log('*end converted xmlfile\n************************************************************************\n');
      let xsldom = builder.buildObject(jdata);

      let xsltProcess = require('xslt-processor');
      let outXmlString = xsltProcess.xsltProcess(
        xsltProcess.xmlParse(xmldom),
        xsltProcess.xmlParse(xsldom));
      // eslint-disable-next-line no-shadow
      xml2js.parseString(outXmlString, function(err, outjson) {
        if (!err) {
          console.log('************************************************************************\n*start convert outxmlstr to json');
          console.log('convert json via xsl to json successed');
          //console.log(outjson);
          console.log('*end converted outxmlstr to json\n************************************************************************\n');

          callback(outjson.Tags.tag);
        } else {
          console.error(err);
        }
      });
    } else {
      console.log('*parseString xmlfile error\n************************************************************************\n');
      console.error(err);
    }
  });
};

//via a operator string convert to operator
exports.operator = (value, opstring)=>{
  let str = '';
  str = opstring;
  if (str.includes('<')) {
    return (value < parseFloat(str.substring(1)));
  } else if (str.includes('<=')) {
    return (value <= parseFloat(str.substring(2)));
  } else if (str.includes('>')) {
    return (value > parseFloat(str.substring(1)));
  } else if (str.includes('>=')) {
    return (value >= parseFloat(str.substring(2)));
  } else if (str.includes('=')) {
    // eslint-disable-next-line eqeqeq
    return (value == parseFloat(str.substring(2)));
  }
  return false;
};

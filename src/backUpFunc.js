/* eslint-disable brace-style */
/* eslint-disable eqeqeq */
/******************************************/
//All additional functions for  middleware
/*****************************************/

/**
 * readhandlejsonobj function specfy the jsonobj into app(rurl)
 * @param {Object} json {"variableName" : [ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ]}
 * @returns [value, err]; value:[ "Tag name 1", "Tag name 2", "Tag name 3", "Tag name 4" ],err:different err obj
 */
exports.readhandlejsonobj = function(json) {
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
        err = {errcode: 504, errmsg: 'unacceptable property: ' + keys[0] };
      }
    }
    //has more key is not acceptiable
    else {
      err = {errcode: 504, errmsg: 'unacceptable for more than one property'};
    }
  }
  return [value, err];
};

/**the function deal with different type of jsondata
 * @param {obj} jsondata different type of jsondata from client
 * @returns {array} [keys,value,err] handeljsondata(jsondata)[0] is key of jsondata,handeljsondata(jsondata)[1] is value of jsondata,handeljsondata(jsondata)[2] is err msg
 */
exports.handeljsondata = function(json) {
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
  } else {//case3:jsondata is just a object
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
  exports.handleobj = handleobj;
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
};

/**
 *function for get realtime
 * @param {string/number/null} timegap 1 or'1s'or null or 0.1
 * @returns ISO Date realtime or n sec before realtime
 */
exports.getRealTime = function(timegap) {
  let timestamp = new Date().getTime();
  let Rt;
  //if there is timegap
  if (timegap) {
    Rt = new Date(timestamp - 1000 * parseFloat(timegap, 10)).toISOString();
  } else { //get realtime
    Rt = new Date(timestamp).toISOString();
  }
  return Rt;
};

/**
 * the connetion and post function of IIH
 * @param {string} url  '/AssetService/Assets/'
 * @param {string} body '{ name: 'AssetName_1', parentId: '0' }'
 * @param {string} method 'POST' or 'GET'
 * @returns promise obj arrray variableIds['1','2','3']
 */
exports.connectIih = function(url, body, method) {
  if (method == 'POST') {
  //set client option
    let options = {
      hostname: '10.31.1.2',
      port: 4203,
      method: method,
      path: url,
      headers: {
        'content-Type': 'Application/json',
        'content-Length': body.length
      }
    };
    //use promise to synchronous
    return new Promise(function(resolve, reject) {
    //start to enable http request
      let req = http.request(options, function(res) {
        let data = '';
        //listen data
        res.on('data', function(chunk) {
          data += chunk;
        });
        //listen to the end and return data
        res.on('end', function() {
          data = JSON.parse(data);
          if (data.errorCode && data.errorParams.reason !== 'Name already in use') {
            reject(data);
          } else {
            resolve(data);
          }
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
  return new Promise(function(resolve, reject) {
    http.get(url, function(res) {
      let todo = '';
      res.on('data', function(chunk) {
        todo += chunk;
      });
      //listen to the end and return data
      res.on('end', function() {
        todo = JSON.parse(todo);
        if (todo.errorCode && todo.errorParams.reason !== 'Name already in use') {
          reject(todo);
        } else {
          resolve(todo);
        }
      });
    }).on('error', (error)=>{
      reject(error);
    });
  });
};

/**
 * the connetion and get fnction of IIh
 * @param {string} url 'http://10.31.1.2:4203/Dataservice/Data/?variableIds=["2523801ea9c64839b580172269c401ed"]&from=2022-04-16T02:52:24.904Z&to=2022-04-16T02:52:25.904Z'
 * @param {string} callback response data obj
 */
exports.getvariable = function(url, callback) {
  http.get(url, function(res) {
    let todo = '';
    res.on('data', function(chunk) {
      todo += chunk;
    });
    //listen to the end and return data
    res.on('end', function() {
      callback(todo);
    });
  }).on('error', (error)=>{
    console.error(new ServerError(500, 900, error.message));
  });
};

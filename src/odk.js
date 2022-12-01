const odk = require('./build/Release/odk.node');

//write function
exports.writetag = function(name, value, callback) {
  let arr = [];
  let err = '';
  let res = '';
  // call napi connection function
  odk.connectFunc();
  //first to check if name tag exist in wincc
  let result = odk.getValue(name);  //result=['tagvalue1','tagvalue2]
  //console.log(result);
  let arry = [];
  //if connection failed
  if (result[0]) {
    err = {error: result[0]};
  } else {  //else tags do not exist in wincc
    result.splice(0, 1);
    if (result.length === 0) {
      for (let i in name) {
        arry.push(name[i]);
      }
    } else {
      for (let i = 0; i < name.length; i++) {
        if (typeof(result[i]) === 'undefined' || typeof(result[i]) === undefined) {
          arry.push(name[i]);
        }
      }
    }
    if (arry.length !== 0) {
      err = {variables: arry, error: 'can not find variable names in wincc'};
    }
  }
  //no error(tags are all found in wincc) then can set value into wincc
  if (!err) {
  // call napi Setvalue function to set tag value into wincc v7.5
    result = odk.setValue(name, value);

    //deal with callback error function
    if (result.length !== 0) {
      if (result[0]) {
        err = JSON.parse(JSON.stringify(result[0]));
      } else {
        result = result.map((item, index) => {
          if (item.length !== 0) {
            item = { value: value[index - 1], error: item };
            return item;
          }
        });
        for (let i in result) {
          if (result[i].length !== 0) {
            arr.push(result[i]);
          }
        }
        err = arr;
      }
    } else {
      res = 'write into wincc successd';
    }
  }
  return callback(res, err);
};
//read function
exports.readtag = function(datar, callback) {
// call napi connection function
  odk.connectFunc();

  // call napi getvalue function to get tag value from wincc v7.5
  let result = odk.getValue(datar);  //result=['tagvalue1','tagvalue2]
  let arry = [];
  let err = '';
  if (result[0]) {
    err = {error: result[0]};
  } else {
    result.splice(0, 1);
    if (result.length === 0) {
      for (let i in datar) {
        arry.push(datar[i]);
      }
    } else {
      for (let i = 0; i < datar.length; i++) {
        if (typeof(result[i]) === 'undefined' || typeof(result[i]) === undefined) {
          arry.push(datar[i]);
        }
      }
    }
    if (arry.length !== 0) {
      err = {variables: arry, error: 'can not find variable names in wincc'};
    }
  }
  let res = {Name: datar, Value: result};

  //error need to do!
  return callback(res, err);
};


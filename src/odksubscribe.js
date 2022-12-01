const odk = require('./build/Release/odk.node');
process.on('message', function(subdata) {
  odk.connectFunc();
  // call napi getvalue function to get tag value from wincc v7.5
  let result = odk.getValue(subdata);  //result=['tagvalue1','tagvalue2]
  let arry = [];
  let err = '';
  if (result[0]) {
    err = {error: result[0]};
  } else {
    result.splice(0, 1);
    if (result.length === 0) {
      for (let i in subdata) {
        arry.push(subdata[i]);
      }
    } else {
      for (let i = 0; i < subdata.length; i++) {
        if (typeof(result[i]) === 'undefined' || typeof(result[i]) === undefined) {
          arry.push(subdata[i]);
        }
      }
    }
    if (arry.length !== 0) {
      err = {variables: arry, error: 'can not find variable names in wincc'};
    }
  }
  if (err) {
    return process.send(err);
  }

  odk.subscribeValue(subdata, cbvaluechange);

  function cbvaluechange(uptag, upvalue) {
    //first read the subscribed  tag and get value into a array
    result = odk.getValue(subdata);  //result=['tagvalue1','tagvalue2]
    result.splice(0, 1);
    let res = [];
    //res=[{Name:"tag1",Value:"value1"},{Name:"tag2",Value:"value2"},{Name:"tag3",Value:"value3"}]
    for (let i in result) {
      let newobj = {};
      newobj.Name = subdata[i];
      newobj.Value = result[i];
      res.push(newobj);
    }

    //replace updated value
    res.forEach(element => {
      if (element.Name === uptag) {
        element.Value = upvalue;
      }
    });
    //res=[{Name:"tag1",Value:"upvalue"},{Name:"tag2",Value:"value2"},{Name:"tag3",Value:"value3"}]
    return process.send(res);
  }
});

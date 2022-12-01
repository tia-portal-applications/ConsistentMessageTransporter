
const jsondata = require('./dataservice-backup-config.json');
//get variables json array
const data = jsondata.variables;
//get variableId
let variableids = data.map((item) => {
  return item.variableId;
});
//get variableName
let variablenames = data.map((item) => {
  return item.variableName;
});
console.log(variableids);
console.log(variablenames);


//Creat bearertoken from certificate
let fs = require('fs');
const util = require('util');
const exec = util.promisify(require('child_process').execFile);
const authcertificatepath = __dirname + '/certificate/server_cert.pfx';
const authcertificatePassword = fs.readFileSync(__dirname + '/certificate/servercertpassword.txt');
const authcreatetokenpath = __dirname + '/CreateTokenFromCertificate_V1.0/CreateTokenFromCertificate.exe';
let bearerTokenprivate;

exec(authcreatetokenpath, ['certificate_path=' + authcertificatepath, 'password=' + authcertificatePassword ], function(error, stdout, stderr) {
  if (error) {
    console.log('Creating a bearer token failed:' + stderr);
    return error.code;
  }
  bearerTokenprivate = stdout.trim();
  console.log(bearerTokenprivate);
});

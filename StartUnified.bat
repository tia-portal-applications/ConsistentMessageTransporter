REM title foobar

pushd %~dp0\src
node Server.js --southboundservice=unified --serverconfig=TagConfiguration.xml  --clientconfig=unified2opcenter --log=verbose
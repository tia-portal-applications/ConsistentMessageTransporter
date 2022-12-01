REM title foobar
@echo off

pushd %~dp0\src
start node Server.js --southboundservice=classic --serverconfig=WinccTagConfiguration.xml  --clientconfig=wincc2opcenter --log=verbose
popd
timeout 1
tasklist /v /fo csv | findstr /i "RestApp"
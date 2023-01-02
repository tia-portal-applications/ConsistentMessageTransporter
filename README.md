# ConsistentMessageTransporter

## Dependencies
1. Node.js 32-bit  
2. WinCC V7.5  and WinCC ODK RT  
3. WinCC Unified RT
4. A fresh install of Node need to run command `npm install -g node-gyp` in cmd.exe  
5. For rest of node model dependencies,  type `npm install -s express..` to install the node modules which `package.json` listed.  

## Build ODK.node Native Module
The building process was done based on this tutorial:[https://medium.com/jspoint/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32](https://medium.com/jspoint/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32)

The `binding.gyp` file contains information about the sources, includes and libraries.  
**Please install 32-bit version of Node.js!**  
***
**The building Process:**  
1. Install the node-addon-api: `npm install -S node-addon-api`
2. Install node-gyp `npm install -g node-gyp`
3. Configure and build it: `node-gyp clean configure build` (this gives you the ODK.node Native Module)  
**if you got error message:`gyp ERR! stack Error: EBUSY: resource busy or locked`,Please type`node-gyp configure build`instead,or close vscode and do step2 again in Windows cmd.**
*** 
 **Test ODK functions form C++ to JavaScript:**  
 1. Open `index.js`, type `node index.js` in Terminal or Windows cmd to see  whether ODK functions works in JavaScript or not.   
 **In `index.js`,ODK connetion,read,write and Subscribe functions can be tested one by one.**     
***   

 ## Generation of certificates
Before running the server, you needd to generate the certificates as it is explained in the file inside the "docs" folder.

 ## Run `Server.js` and connect to WinCC V7.5 
1. Run Wincc ODK Runtime and make sure the tagnames of  `Name`column in `WinccTagConfiguration.xml` are all existed in WinCC.  
2. type `node Server.js  --southboundservice=classic --serverconfig=WinccTagConfiguration.xml --clientconfig=wincc2opcenter --log=verbose` in Terminal or Windows cmd to start up the Server connect to WinCC V7.5.    

 ## Run `Server.js` and connect to WinCC Unified
1. Run Wincc Unified Runtime and make sure the tagnames of `Name`column in `TagConfiguration.xml` are all existed in WinCC Unified.  
2. type `node Server.js --southboundservice=unified --serverconfig=TagConfiguration.xml --clientconfig=unified2opcenter --log=verbose` in Terminal or Windows cmd to start up the Server connect to WinCC Unified.  




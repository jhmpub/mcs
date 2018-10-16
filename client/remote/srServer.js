// Abstract: This Smart Remote Server provides a browser client with remote 
//           control functionality.  This server controls a TV, DVD Player
//           and a Home Theater PC
//
// Note: This server uses the Global Cache WF2IR device that acts as a
//       wifi to IR transducer for remote control of a TV and DVD player:
//
//       http://www.globalcache.com/products/itach/wf2irspecs/
// 
// Software Installation:      
// 1. download and install node.js from https://nodejs.org/en/download/
// 2. populate express dependencies by running:
//
//    npm install --save express
//    npm install --save socket.io
//
//    from the parent directory where the package.json resides.
//
// Use the following command to run this server:
//
// node srServer.js
//
// 2016 Mar 26 jhm original creation
// 

var app = require('../node_modules/express')();
var http = require('http').Server(app);
var io = require('../node_modules/socket.io')(http);
var network = require('net');  
var irCmd = require('./irCmds.js');
var localhost = require('os').hostname();
var transducer = 'lvr-tdr';
var wf2ir = new network.Socket();

connectTransducer(wf2ir);

app.get('/', function(req, res) {
   console.log('srClient page requested');
   res.sendFile(__dirname + '/srClient.html');
});

app.get('/socket.io', function(req, res) {
   console.log('socket.io-client requested');
   res.sendFile(__dirname.replace(/[^\\]+$/,'') + 
      '/node_modules/socket.io/node_modules/socket.io-client/socket.io.js');
});

// initialize the smart remote server
io.on('connection', function(srClient) {
   console.log('srClient connected');

   srClient.on('request', function(msg) {
      console.log('request ' + msg);
      wf2ir.write(irCmd.power);

      // io emits to all srClients, srClient only emits to a single client
      io.emit('notification', 'power on');
   });
      
   srClient.on('close', function() {
      console.log('srClient disconnected');
      wf2ir.destroy();
   });
});
      
http.listen(3000, function() {
   console.log(localhost + ' listening on *:3000');
});

// create the wifi to ir transducer client connection
function connectTransducer(wf2ir) {
   if (localhost == 'den-mm') {
      transducer = 'den-tdr';
   }

   wf2ir.connect(4998, transducer, function() {
      console.log(transducer + ' connected');
      
      wf2ir.on('data', function(response) {
         console.log(transducer + ' ' + response);
      }); 

      wf2ir.on('close', function() {
         console.log(transducer + ' disconnected');
      });
   });
}   
         

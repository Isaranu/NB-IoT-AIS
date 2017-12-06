var mongojs = require("mongojs");
var db_udp = mongojs("db_udp");

/* For UDP connection */
var dgram = require("dgram");
var server = dgram.createSocket("udp4");
var PORT = 41234; // UDP opened port on GCP console.

server.on("listening", function () {
    var address = server.address();
    console.log("set listen udp on " + address.address + ":" + address.port);
});

server.on("message", function (message, remote) {
    console.log(remote.address + ":" + remote.port +' - ' + message);

    // Recording string from ESP32 UDP connection into MongoDB
    var db_udp_col = db_udp.collection('col_udp');

    var d = new Date();
    var utc = d.getTime();

    db_udp_col.insert({
      data: message,
      utc: Number(utc)

    }, function(err){
      if(err) console.log('record failed');
    });

});

server.bind(PORT);

/* Route get data to Dashboard */
const express = require('express');
const app = express();
var port = 4000;

app.get('/read/', function (req, res) {

  db_udp_read = db_udp.collection('col_udp');

  db_udp_read.find({}).limit(10).sort({utc:-1}, function(err, docs){
    if(err) return console.log('read failed');
    console.log(docs);
    res.send(docs[0].data);
  });
});

app.listen(port, function () {
  var nodeStartTime = new Date();
  console.log('-- [Write] Protocol start running on port ' + port + ' at ' + nodeStartTime + ' --');
});

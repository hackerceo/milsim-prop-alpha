const DATADIR = 'src/data';


var http = require('http');
var fs = require('fs');
var path = require('path');
var zlib = require('zlib');

var cmdlineArgs = process.argv;
cmdlineArgs.shift();
cmdlineArgs.shift();
var confProxyPath = cmdlineArgs.shift();
var confProxyIP = cmdlineArgs.shift();
console.log("PROXY http://127.0.0.1:8125/" + confProxyPath + "/* => http://" + confProxyIP + "/" + confProxyPath + "/*");

http.createServer(function (request, response) {
    console.log(request.url);
	var tempPath = request.url.split('/');
	if (tempPath[tempPath.length-1] === '') {
		tempPath[tempPath.length-1] = 'index.html';
	}
	var requestUrl = tempPath.join('/');
	
	if (requestUrl.indexOf("/" + confProxyPath + "/") === 0) {
		console.log("handle proxy request");
		response.end("PROXIED TEXT GOES HERE!", 'utf-8');
	} else {
		var filePath = './' + DATADIR + requestUrl;

		var extname = path.extname(filePath);
		var contentType = 'text/html';
		switch (extname) {
			case '.js':
				contentType = 'text/javascript';
				break;
			case '.css':
				contentType = 'text/css';
				break;
			case '.json':
				contentType = 'application/json';
				break;
			case '.png':
				contentType = 'image/png';
				break;
			case '.jpg':
				contentType = 'image/jpeg';
				break;
			case '.gif':
				contentType = 'image/gif';
				break;
			case '.wav':
				contentType = 'audio/wav';
				break;
		}

		fs.readFile(filePath, function(error, content) {
			if (error) {
				if(error.code == 'ENOENT'){
					fs.readFile('./' + DATADIR + '/404.html', function(error, content) {
						response.writeHead(200, { 'Content-Type': contentType });
						response.end(content, 'utf-8');
					});
				}
				else {
					response.writeHead(500);
					response.end('Sorry, check with the site admin for error: '+error.code+' ..\n');
					response.end(); 
				}
			}
			else {
				response.writeHead(200, { 'Content-Type': contentType, 'Content-Encoding': 'gzip'});
				zlib.gzip(content, (_, ctnt) => {
					response.end(ctnt, 'utf-8');
				});
			}
		});
	}
}).listen(8125);


const { exec } = require('child_process');
exec('start http://127.0.0.1:8125/');
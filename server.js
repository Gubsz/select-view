//server.js

//Base Setup
// =================================================================

//imports
var fs			= require('fs');
var express		= require('express');
var app			= express(); 			//Define app using express
var bodyParser	= require('body-parser');
var addon		= require('./build/Release/addon')

//configure app to use bodyParser(to get POST data)
app.use(bodyParser.urlencoded({extended : true}));
app.use(bodyParser.json());

var port = process.env.port || 8080;	//Port config


//API Routes
// =================================================================
var router = express.Router()			//instance of express Router

//Middleware used for all requests
router.use(function(req, res, next) {
	//console.log(req.body);
	next(); //Go to next routes and dont stop here
})

// /images routes
router.route('/images')
	//create bear - POST /api/images
	.post(function(req, res) {
		var image64 = req.body.image;	//set bear name(comes from req)
		var width 	= parseInt(req.body.width, 10);
		var height 	= parseInt(req.body.height, 10);

		var imageBuffer = new Buffer(image64, 'base64');
		var imageBuffer2 = new Buffer(image64);

		addon.saveimage(imageBuffer, width, height);
		
		fs.writeFile('test_js.jpg', imageBuffer, function(err) {
			if(err) res.send(err);

			res.json({message : 'Image received and saved!'});
		});
	});


//test route to make sure its working
router.get('/', function(req, res) {
	res.json({message : 'hooray! welcome to our api!'});
});

//TODO add more routes

//API Routes
// =================================================================
//all routes prefixed with /api
app.use('/api', router);


//Server start
// =================================================================
app.listen(port);
console.log('Magic happens on port ' + port)
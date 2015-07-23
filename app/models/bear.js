// app/modles/bear.js

var mongoose	= require('mongoose');
var Schema		= mongoose.Schema;

var BearSchema	= new Schema({
	name : String
});

module.exports = mongoose.model('Bear', BearSchema)


//Bear Routes 
// /api/bears 			- 	GET 	- 	Get all bears
// /api/bears 			- 	POST 	- 	Create a bear
// /api/bears/:bear_id 	- 	GET 	- 	Get a single bear
// /api/bears/:bear_id	- 	PUT 	- 	Update bear w/ new info
// /api/bears 			- 	DELETE 	- 	Delete a bear
	
var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/nodehello');

var Schema = mongoose.Schema
  , ObjectId = Schema.ObjectId;



var User = new Schema({
    name     : { type: String, index: true }
  , bio       : String
});

mongoose.model('User', User);

module.exports = mongoose;


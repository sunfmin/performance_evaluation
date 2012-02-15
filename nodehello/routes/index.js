var db = require('../db');
var mongoose = require('mongoose');
var User = mongoose.model('User');

/*
 * GET home page.
 */

exports.index = function(req, res){
  User.find().limit(100).run(function(error, docs){
    res.render('index', { title: 'Express', users: docs});
  });
};


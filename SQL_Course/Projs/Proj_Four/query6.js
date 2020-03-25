// query6 : Find the Average friend count per user for users
//
// Return a decimal variable as the average user friend count of all users
// in the users document.

function find_average_friendcount(dbname){
  db = db.getSiblingDB(dbname);
  var ret_val = 0;
  db.users.aggregate([{$group: {_id: null, avg_num_friends: {$avg: {$size: "$friends"}}}}, 
                      {$out: "avg_num_friends_collec"}]);
  db.avg_num_friends_collec.find().forEach(function(doc)
  {
  	ret_val = doc.avg_num_friends;
  });
  return ret_val;
}
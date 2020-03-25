
// query 4: find user pairs (A,B) that meet the following constraints:
// i) user A is male and user B is female
// ii) their Year_Of_Birth difference is less than year_diff
// iii) user A and B are not friends
// iv) user A and B are from the same hometown city
// The following is the schema for output pairs:
// [
//      [user_id1, user_id2],
//      [user_id1, user_id3],
//      [user_id4, user_id2],
//      ...
//  ]
// user_id is the field from the users collection. Do not use the _id field in users.
  
function suggest_friends(year_diff, dbname) {
    db = db.getSiblingDB(dbname);
    var pairs = [];
    db.users.find({gender: 'male'},{}).forEach(
    	function(friends_t) {
    		db.users.find({gender:'female'},{}).forEach(
    			function(friends_n_t) {
    				var yob_diff = 0;
    				if(friends_t.YOB > friends_n_t.YOB) {
    					yob_diff = friends_t.YOB - friends_n_t.YOB;
    				}
    				else {
    					yob_diff = friends_n_t.YOB - friends_t.YOB;
    				}
    				if(yob_diff < year_diff) {
    					if(friends_t.hometown.city == friends_n_t.hometown.city) {
    						var f_check = 0;
    						if(friends_t.user_id > friends_n_t.user_id) {
    							f_check = friends_n_t.friends.includes(friends_t.user_id);
    						}
    						else {
    							f_check = friends_t.friends.includes(friends_n_t.user_id);

    						}
    						if(f_check == false) {
    							var temp_pairs = [friends_t.user_id, friends_n_t.user_id];
    							pairs.push(temp_pairs);
    						}
    					}
    				}

    			}
    			)
    			
    		
    	});

    // TODO: implement suggest friends
    // Return an array of arrays.
    return pairs;
}

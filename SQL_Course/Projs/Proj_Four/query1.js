// query1 : find users whose hometown citys the specified city. 

function find_user(city, dbname){
    db = db.getSiblingDB(dbname);
    var results = [];
    ((db.users.find({},{hometown:1, user_id:1})).forEach(
		function(ht_test) {
    		if(ht_test.hometown.city == city) {
    			results.push(ht_test.user_id);
    		}
    	} 
    	));
    return results;
}

/* SQL QUERY 1*/
/*
	SELECT USER_ID 
	FROM jiaqni.Public_User_Hometown_City
	WHERE HOMETOWN_CITY_ID IN 
	(SELECT CITY_ID 
	FROM jiaqni.Public_Cities
	WHERE CITY_NAME = city)


*/
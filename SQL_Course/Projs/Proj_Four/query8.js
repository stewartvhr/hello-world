// query 8: Find the city average friend count per user using MapReduce
// Using the same terminology in query6, we are asking you to write the mapper,
// reducer and finalizer to find the average friend count for each city.


var city_average_friendcount_mapper = function() {
  // implement the Map function of average friend count
  // var friends_info = {num_friends: (this.friends).length, user_count: 1};
  emit(this.hometown.city, (this.friends).length);
};

var city_average_friendcount_reducer = function(key, values) {
  // implement the reduce function of average friend count

  //Get average of array
  var total_t = 0;
  var sum_t = 0;
  for(var i = 0; i < values.length; ++i) {
  	sum_t += values[i];
  	total_t += 1;
  }
  ret_val = {sum: sum_t, total: total_t};
  return ret_val;
};

var city_average_friendcount_finalizer = function(key, reduceVal) {
  // We've implemented a simple forwarding finalize function. This implementation 
  // is naive: it just forwards the reduceVal to the output collection.
  // Feel free to change it if needed.
  if(reduceVal.total == 0) {
  	return 0;
  }

  var ret = reduceVal.sum / reduceVal.total;

  return ret;
}
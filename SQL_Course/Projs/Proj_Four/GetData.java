import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.TreeSet;
import java.util.Vector;



//json.simple 1.1
// import org.json.simple.JSONObject;
// import org.json.simple.JSONArray;

// Alternate implementation of JSON modules.
import org.json.JSONObject;
import org.json.JSONArray;

public class GetData{
	
    static String prefix = "jiaqni.";
	
    // You must use the following variable as the JDBC connection
    Connection oracleConnection = null;
	
    // You must refer to the following variables for the corresponding 
    // tables in your database

    String cityTableName = null;
    String userTableName = null;
    String friendsTableName = null;
    String currentCityTableName = null;
    String hometownCityTableName = null;
    String programTableName = null;
    String educationTableName = null;
    String eventTableName = null;
    String participantTableName = null;
    String albumTableName = null;
    String photoTableName = null;
    String coverPhotoTableName = null;
    String tagTableName = null;

    // This is the data structure to store all users' information
    // DO NOT change the name
    JSONArray users_info = new JSONArray();		// declare a new JSONArray

	
    // DO NOT modify this constructor
    public GetData(String u, Connection c) {
	super();
	String dataType = u;
	oracleConnection = c;
	// You will use the following tables in your Java code
	cityTableName = prefix+dataType+"_CITIES";
	userTableName = prefix+dataType+"_USERS";
	friendsTableName = prefix+dataType+"_FRIENDS";
	currentCityTableName = prefix+dataType+"_USER_CURRENT_CITY";
	hometownCityTableName = prefix+dataType+"_USER_HOMETOWN_CITY";
	programTableName = prefix+dataType+"_PROGRAMS";
	educationTableName = prefix+dataType+"_EDUCATION";
	eventTableName = prefix+dataType+"_USER_EVENTS";
	albumTableName = prefix+dataType+"_ALBUMS";
	photoTableName = prefix+dataType+"_PHOTOS";
	tagTableName = prefix+dataType+"_TAGS";
    }
	
	
	
	
    //implement this function

    @SuppressWarnings("unchecked")
    public JSONArray toJSON() throws SQLException{ 

    	JSONArray users_info = new JSONArray();
		
	// Your implementation goes here...
    	final int AllScroll = ResultSet.TYPE_SCROLL_INSENSITIVE;
    	final int ReadOnly = ResultSet.CONCUR_READ_ONLY;		
    	try (Statement stmt = oracleConnection.createStatement(AllScroll, ReadOnly)) {
    		//Get all users in table
            ResultSet rst_user_info = stmt.executeQuery(
                "SELECT * " +     
                "FROM " + userTableName);    

            Statement stmt_friends = oracleConnection.createStatement(AllScroll, ReadOnly);
            Statement stmt_hometown = oracleConnection.createStatement(AllScroll, ReadOnly);
            Statement stmt_city = oracleConnection.createStatement(AllScroll, ReadOnly);
            Statement stmt_current_city = oracleConnection.createStatement(AllScroll, ReadOnly);

            //Iterate through all users
            while (rst_user_info.next()) {
            	
            	//Current user information from Users Table
            	int user_id_int = rst_user_info.getInt(1);
            	String first_name = rst_user_info.getString(2);
            	String last_name = rst_user_info.getString(3);
            	int year_of_birth = rst_user_info.getInt(4);
            	int month_of_birth = rst_user_info.getInt(5);
            	int date_of_birth = rst_user_info.getInt(6);
            	String gender = rst_user_info.getString(7);

            	//Get result set of all friends of current user
            	ResultSet rst_friends = stmt_friends.executeQuery(
            		"SELECT USER2_ID " +
            		"FROM " + friendsTableName + " " +
            		"WHERE USER1_ID = " + user_id_int);

            	//Create vector to hold all friends of current user
            	Vector<Integer> friendsVec = new Vector<Integer>(); 
            	while(rst_friends.next()){
            		friendsVec.addElement(rst_friends.getInt(1));
            	}


            	//Get result set of hometown of current user
            	ResultSet rst_hometown = stmt_hometown.executeQuery(
            		"SELECT HOMETOWN_CITY_ID " +
            		"FROM " + hometownCityTableName + " " +
            		"WHERE USER_ID = " + user_id_int);

            	JSONObject user_hometown = new JSONObject();

            	//TODO users should only have 1 hometown, could this loop cause problems?
            	while(rst_hometown.next()){
            		//Get city info for user hometown
            		ResultSet rst_city = stmt_city.executeQuery(
            			"SELECT CITY_NAME, STATE_NAME, COUNTRY_NAME " +
            			"FROM " + cityTableName + " " +
            			"WHERE CITY_ID = " + rst_hometown.getInt(1));
            		rst_city.next();
            		user_hometown.put("state", rst_city.getString(2));
            		user_hometown.put("country", rst_city.getString(3));
            		user_hometown.put("city", rst_city.getString(1));
            		rst_city.close();

            	}


            	ResultSet rst_current_city = stmt_current_city.executeQuery(
            		"SELECT CURRENT_CITY_ID " +
            		"FROM " + currentCityTableName + " " +
            		"WHERE USER_ID = " + user_id_int);

            	JSONObject user_cc = new JSONObject();
            	//TODO users should only have 1 current city, could this loop cause problems?
            	while(rst_current_city.next()) {
            		//Get current city info for user
            		ResultSet rst_cc = stmt_city.executeQuery(
            			"SELECT CITY_NAME, STATE_NAME, COUNTRY_NAME " +
            			"FROM " + cityTableName + " " +
            			"WHERE CITY_ID = " + rst_current_city.getInt(1));
            		rst_cc.next();
            		user_cc.put("state", rst_cc.getString(2));
            		user_cc.put("country", rst_cc.getString(3));
            		user_cc.put("city", rst_cc.getString(1));
            		rst_cc.close();
            	}

            	//Object to be added to JSONArray for current user
            	JSONObject user_string = new JSONObject();
            	user_string.put("user_id", user_id_int);
            	user_string.put("first_name", first_name);
            	user_string.put("last_name", last_name);
            	user_string.put("gender", gender);
            	user_string.put("YOB", year_of_birth);
            	user_string.put("MOB", month_of_birth);
            	user_string.put("DOB", date_of_birth);
            	user_string.put("friends", friendsVec);
            	user_string.put("hometown", user_hometown);
            	user_string.put("current", user_cc);
            	
            	users_info.put(user_string);

            	rst_hometown.close();
            	rst_friends.close();

            }   

            stmt_city.close();
            stmt_current_city.close();
            stmt_hometown.close();
            stmt_friends.close();
            rst_user_info.close();
            stmt.close();
		}
		return users_info;
    }

    // This outputs to a file "output.json"
    public void writeJSON(JSONArray users_info) {
	// DO NOT MODIFY this function
	try {
	    FileWriter file = new FileWriter(System.getProperty("user.dir")+"/output.json");
	    file.write(users_info.toString());
	    file.flush();
	    file.close();

	} catch (IOException e) {
	    e.printStackTrace();
	}
		
    }
}



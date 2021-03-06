INSERT INTO Users
SELECT DISTINCT USER_ID, FIRST_NAME, LAST_NAME, YEAR_OF_BIRTH, MONTH_OF_BIRTH, DAY_OF_BIRTH, GENDER FROM jsoren.PUBLIC_USER_INFORMATION;

INSERT INTO Cities (CITY_NAME, STATE_NAME, COUNTRY_NAME)
SELECT DISTINCT CURRENT_CITY, CURRENT_STATE, CURRENT_COUNTRY FROM jsoren.PUBLIC_USER_INFORMATION
WHERE CURRENT_CITY IS NOT NULL
UNION
SELECT DISTINCT HOMETOWN_CITY, HOMETOWN_STATE, HOMETOWN_COUNTRY FROM jsoren.PUBLIC_USER_INFORMATION
WHERE HOMETOWN_CITY IS NOT NULL
UNION
SELECT DISTINCT EVENT_CITY, EVENT_STATE, EVENT_COUNTRY FROM jsoren.PUBLIC_EVENT_INFORMATION;


INSERT INTO USER_CURRENT_CITIES
SELECT U.USER_ID, max(C.CITY_ID)
FROM jsoren.PUBLIC_USER_INFORMATION U
JOIN Cities C ON (U.CURRENT_CITY IS NOT NULL AND C.CITY_NAME = U.CURRENT_CITY AND C.STATE_NAME = U.CURRENT_STATE AND C.COUNTRY_NAME = U.CURRENT_COUNTRY)
GROUP BY U.USER_ID;


INSERT INTO USER_HOMETOWN_CITIES
SELECT U.USER_ID, max(C.CITY_ID)
FROM jsoren.PUBLIC_USER_INFORMATION U
JOIN Cities C ON (U.HOMETOWN_CITY IS NOT NULL AND C.CITY_NAME = U.HOMETOWN_CITY AND C.STATE_NAME = U.HOMETOWN_STATE AND C.COUNTRY_NAME = U.HOMETOWN_COUNTRY)
GROUP BY U.USER_ID;

INSERT INTO Programs (INSTITUTION, CONCENTRATION, DEGREE) 
SELECT DISTINCT INSTITUTION_NAME, PROGRAM_CONCENTRATION, PROGRAM_DEGREE FROM jsoren.PUBLIC_USER_INFORMATION
WHERE INSTITUTION_NAME IS NOT NULL;


INSERT INTO Education
SELECT DISTINCT U.USER_ID, P.PROGRAM_ID, U.PROGRAM_YEAR
FROM jsoren.PUBLIC_USER_INFORMATION U, Programs P
WHERE U.INSTITUTION_NAME IS NOT NULL 
AND U.INSTITUTION_NAME = P.INSTITUTION
AND U.PROGRAM_CONCENTRATION = P.CONCENTRATION 
AND U.PROGRAM_DEGREE = P.DEGREE;

INSERT INTO Friends (USER1_ID, USER2_ID)
SELECT DISTINCT LEAST(P.USER1_ID, P.USER2_ID), GREATEST(P.USER1_ID, P.USER2_ID)
FROM jsoren.PUBLIC_ARE_FRIENDS P
WHERE (P.USER1_ID <> P.USER2_ID);


INSERT INTO Albums
SELECT DISTINCT ALBUM_ID, OWNER_ID, ALBUM_NAME, ALBUM_CREATED_TIME, ALBUM_MODIFIED_TIME, ALBUM_LINK, ALBUM_VISIBILITY, COVER_PHOTO_ID FROM jsoren.PUBLIC_PHOTO_INFORMATION;

INSERT INTO Photos (PHOTO_ID, ALBUM_ID, PHOTO_CAPTION, PHOTO_CREATED_TIME, PHOTO_MODIFIED_TIME, PHOTO_LINK)
SELECT DISTINCT PHOTO_ID, ALBUM_ID, PHOTO_CAPTION, PHOTO_CREATED_TIME, PHOTO_MODIFIED_TIME, PHOTO_LINK FROM jsoren.PUBLIC_PHOTO_INFORMATION;

INSERT INTO Tags
SELECT U.PHOTO_ID, A.USER_ID, max(U.TAG_CREATED_TIME), max(U.TAG_X_COORDINATE), max(U.TAG_Y_COORDINATE)
FROM jsoren.PUBLIC_TAG_INFORMATION U
JOIN Users A ON (A.USER_ID = U.TAG_SUBJECT_ID)
GROUP BY U.PHOTO_ID, A.USER_ID;


INSERT INTO User_Events
SELECT P.EVENT_ID, max(P.EVENT_CREATOR_ID), max(P.EVENT_NAME), max(P.EVENT_TAGLINE), max(P.EVENT_DESCRIPTION), max(P.EVENT_HOST), max(P.EVENT_TYPE), max(P.EVENT_SUBTYPE), max(P.EVENT_ADDRESS), max(C.CITY_ID), max(P.EVENT_START_TIME), max(P.EVENT_END_TIME)
FROM jsoren.PUBLIC_EVENT_INFORMATION P
JOIN Cities C ON (C.CITY_NAME = P.EVENT_CITY AND C.STATE_NAME = P.EVENT_STATE AND C.COUNTRY_NAME = P.EVENT_COUNTRY)
GROUP BY P.EVENT_ID;
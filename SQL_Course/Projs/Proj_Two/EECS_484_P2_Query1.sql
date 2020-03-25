SELECT DISTINCT U.First_Name 
FROM jiaqni.PUBLIC_USERS U
WHERE LENGTH(U.First_Name) = (SELECT MAX(LENGTH(U.First_Name)) FROM jiaqni.PUBLIC_USERS U) 
        OR LENGTH(U.First_Name) = (SELECT MIN(LENGTH(U.First_Name)) FROM jiaqni.PUBLIC_USERS U) 
ORDER BY LENGTH(U.First_Name) ASC, U.First_Name ASC;

CREATE VIEW First_Name_Info AS
SELECT U.First_Name, COUNT(U.First_Name) AS Name_Count FROM jiaqni.PUBLIC_USERS U 
GROUP BY U.First_Name;

SELECT DISTINCT First_Name_Info.First_Name, First_Name_Info.Name_Count 
FROM First_Name_Info
WHERE First_Name_Info.Name_Count = (SELECT MAX(First_Name_Info.Name_Count) FROM First_Name_Info);

DROP VIEW First_Name_Info;

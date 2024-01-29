SELECT U.UserName, U.Password, R.Role FROM User AS U
INNER JOIN Roles AS R ON U.Role = R.ID
WHERE U.UserName = 'Chepush' AND U.Password = '1234'

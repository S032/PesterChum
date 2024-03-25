SELECT U.UserName, U.Password, S.Status FROM User AS U
INNER JOIN Status AS S ON U.Status = S.ID

SELECT US2.UserName
FROM Chats as CH
INNER JOIN User AS US1
ON CH.User1 = US1.ID
INNER JOIN User AS US2
ON CH.User2 = US2.ID
WHERE US1.UserName = 'test1' AND US2.UserName = 'test2'; 
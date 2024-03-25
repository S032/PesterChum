CREATE TABLE Status (
    ID INT PRIMARY KEY,
    Status VARCHAR(5)
);

CREATE TABLE User (
    ID INT PRIMARY KEY,
    UserName VARCHAR(30),
    Password VARCHAR(30),
    Status INT,
    FOREIGN KEY(Status) REFERENCES Status(ID),
    UNIQUE (UserName)
);

CREATE TABLE Chats (
    User1 INT,
    FOREIGN KEY(User1) REFERENCES User(ID),
    User2 INT,
    FOREIGN KEY(User2) REFERENCES User(ID)
);

CREATE TABLE Request (
    User1 INT,
    FOREIGN KEY(User1) REFERENCES User(ID),
    User2 INT,
    FOREIGN KEY(User2) REFERENCES User(ID),
    UNIQUE (User1, User2)   
);

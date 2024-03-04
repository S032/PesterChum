CREATE TABLE Roles (
    ID INT PRIMARY KEY,
    Role VARCHAR(5)
);

CREATE TABLE User (
    ID INT PRIMARY KEY,
    UserName VARCHAR(30),
    Password VARCHAR(30),
    Role INT,
    FOREIGN KEY(Role) REFERENCES Roles(ID)
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
    FOREIGN KEY(User2) REFERENCES User(ID)   
);

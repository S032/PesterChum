#include "ChatDatabase.h"

void ChatDatabase::print_err(sql::SQLException e) {
    std::cerr << "# ERR: SQLException in " << __FILE__;
    std::cerr << "# ERR: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
};

ChatDatabase::ChatDatabase() {
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        printf("DB: Connecting DataBase...\n");
        conn = driver->connect(db_uri, db_user, db_pass);
        conn->setSchema(db);
        printf("DB: Success\n");
    }
    catch(sql::SQLException& e)
    {
    print_err(e);
    return;
    }
}

int ChatDatabase::find_next_id(sql::SQLString Table) {
    try
    {
        sql::ResultSet *rs;

        stmt = conn->createStatement();
        rs = stmt->executeQuery("SELECT MAX(ID) FROM " + Table);
        rs->next();
        return rs->getInt("MAX(ID)") + 1;
    }
    catch(sql::SQLException &e)
    {
        printf("find_next_id:\n");
        print_err(e);
        return -1;
    }
}

bool ChatDatabase::check_username(user_t *user) {
    try
    {
        stmt = conn->createStatement();
        rs = stmt->executeQuery("SELECT UserName From User WHERE BINARY UserName ='" +user->login+"'");
        if(!rs->next())
            return false;
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("check_username:\n");
        print_err(e);
        return false;
    }
}

int ChatDatabase::UsernameToInt(std::string username) {
    try
    {
        sql::SQLString query = "SELECT ID FROM User";
        query += " WHERE BINARY UserName = '"+username+"'";
        stmt = conn->createStatement();
        rs= stmt->executeQuery(query);
        if(!rs->next()) return -1;
        return rs->getInt("ID");
    }
    catch(sql::SQLException &e)
    {
        printf("usernameToInt:\n");
        print_err(e);
        return -1;
    }
}

int ChatDatabase::statusToInt(std::string status) {
    try
    {
        sql::SQLString query = "SELECT ID FROM Status";
        query += " WHERE Status = '"+status+"'";
        stmt = conn->createStatement();
        rs= stmt->executeQuery(query);
        if(!rs->next()) return -1;
        return rs->getInt("ID");
    }
    catch(sql::SQLException &e)
    {
        printf("usernameToInt:\n");
        print_err(e);
        return -1;
    }
}

bool ChatDatabase::check_user_in_chattable(std::string sender_name, std::string recipient_name) {
    try
    {
        sql::SQLString query = "SELECT US2.UserName FROM Chats AS CH";
        query += " INNER JOIN User AS US1 ON CH.User1 = US1.ID";
        query += " INNER JOIN User AS US2 ON CH.User2 = US2.ID";
        query += " WHERE BINARY US1.UserName = '"+sender_name+"'";
        query += " AND US2.UserName = '"+recipient_name+"'";
        stmt = conn->createStatement();
        rs= stmt->executeQuery(query);
        if(!rs->next()) return false;
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("check_user:\n");
        print_err(e);
        return false;
    }
}

std::vector<std::string> ChatDatabase::getFriendList(std::string username) {
    try
    {
        std::vector<std::string> friendList;
        sql::SQLString query = "SELECT US2.UserName FROM Chats AS CH";
        query += " INNER JOIN User AS US1 ON CH.User1 = US1.ID";
        query += " INNER JOIN User AS US2 ON CH.User2 = US2.ID";
        query += " WHERE BINARY US1.UserName = '"+username+"'";
        stmt = conn->createStatement();
        rs= stmt->executeQuery(query);
        while(rs->next()) {
            friendList.push_back(rs->getString("UserName"));
        }
        return friendList;
    }
    catch(sql::SQLException &e)
    {
        printf("check_user:\n");
        print_err(e);
        return {};
    }
}

bool ChatDatabase::reg_user(user_t *user) {
    try
    {
        if(check_username(user))
            return false; //login exist

        std::string next_id = std::to_string(find_next_id("User"));
        stmt = conn->createStatement(); 

        stmt->execute("INSERT INTO User VALUE("+ next_id +", '"+user->login+"', '"+user->password+"', 1)");
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("reg_user:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::log_user(user_t *user) {
    try
    {
        sql::SQLString query = "SELECT U.UserName, U.Password, S.Status FROM User AS U";
        query += " INNER JOIN Status AS S ON U.Status = S.ID";
        query += " WHERE BINARY U.UserName = '"+user->login+"' AND U.Password = '"+user->password+"'";
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        if(!rs->next())
            return false;
        user->status = rs->getString("Status");
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("log_user:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::getListOfIcReq(std::string *answer, std::string username) {
    try
    {
        *(answer) = "/icreqlist/";
        sql::SQLString query = "SELECT US1.UserName FROM Request AS Req";
        query += " INNER JOIN User AS US1 ON Req.User1 = US1.ID";
        query += " INNER JOIN User AS US2 ON Req.User2 = US2.ID";
        query += " WHERE BINARY US2.UserName = '"+username+"'";
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        while(rs->next()) {
            *(answer) += rs->getString("UserName") + "/";
        }
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("getListOfIcReq:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::getListOfOgReq(std::string *answer, std::string username) {
    try
    {
        *(answer) = "/ogreqlist/";
        sql::SQLString query = "SELECT US2.UserName FROM Request AS Req";
        query += " INNER JOIN User AS US1 ON Req.User1 = US1.ID";
        query += " INNER JOIN User AS US2 ON Req.User2 = US2.ID";
        query += " WHERE BINARY US1.UserName = '"+username+"'";
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        while(rs->next()) {
            *(answer) += rs->getString("UserName") + "/";
        }
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("getListOfOgReq:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::addRequest(std::string sender_name, std::string recipient_name, std::string *answer) {
    try
    {
        std::string sender_id = std::to_string(UsernameToInt(sender_name));
        std::string recipient_id = std::to_string(UsernameToInt(recipient_name));
        if (recipient_id == "-1") {
            *(answer) = "/reqanswer/" + recipient_name + "/notexist";
            return false;
        }
        //check if recipient is already friend
        sql::SQLString query = "SELECT * FROM Chats";
        query += " WHERE BINARY User1 = '"+sender_id+"' AND User2 = "+recipient_id;
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        if (rs->next()) {
            *(answer) = "/reqanswer/" + recipient_name + "/already";
            return false;
        }
        //check if recipient has already sent req and make them friends :3
        
        //adding request
        query = "INSERT INTO Request VALUES("+sender_id+", "+recipient_id+")";
        stmt = conn->createStatement();
        stmt->execute(query);
        *(answer) = "/s";
        return true;
    }
    catch(sql::SQLException &e)
    {
        if (e.getErrorCode() == 1062) {
            *(answer) = "/reqanswer/" + recipient_name + "/exist";
            return false;
        }
        printf("addRequest:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::delRequest(std::string requester_name, std::string recipient_name, std::string *answer) {
    try
    {
        std::string requester_id = std::to_string(UsernameToInt(requester_name));
        std::string recipient_id = std::to_string(UsernameToInt(recipient_name));
        if (recipient_id == "-1") {
            *(answer) = "/reqanswer/" + recipient_name + "/notexist";
            return false;
        }

        sql::SQLString query = "DELETE FROM Request WHERE BINARY User1 = '"+requester_id+"' AND User2 = '"+recipient_id+"'";
        stmt = conn->createStatement();
        stmt->execute(query);
        *(answer) = "/s";
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("delRequest:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::makeChat(std::string user1, std::string user2) {
    try
    {
        std::string user1_id = std::to_string(UsernameToInt(user1));
        std::string user2_id = std::to_string(UsernameToInt(user2));
        if (user1 == "-1" or user2 == "-1") return false;
        sql::SQLString query = "INSERT INTO Chats VALUES("+user1_id+", "+user2_id+")";
        stmt = conn->createStatement();
        stmt->execute(query);
        if (user1_id != user2_id) {
            query = "INSERT INTO Chats VALUES("+user2_id+", "+user1_id+")";
            stmt = conn->createStatement();
            stmt->execute(query);
        }
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("makeChat:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::delChat(std::string sender_name, std::string friend_name) {
    try
    {
        std::string sender_id = std::to_string(UsernameToInt(sender_name));
        std::string friend_id = std::to_string(UsernameToInt(friend_name));
        if (friend_id == "-1") return false;
        sql::SQLString query = "DELETE FROM Chats WHERE BINARY User1 = "+sender_id+" AND User2 = "+friend_id;
        stmt = conn->createStatement();
        stmt->execute(query);
        query = "DELETE FROM Chats WHERE BINARY User2 = "+sender_id+" AND User1 = "+friend_id;
        stmt = conn->createStatement();
        stmt->execute(query);
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("delChat:\n");
        print_err(e);
        return false;
    }
}

bool ChatDatabase::writeLastStatus(std::string username, std::string status) {
    try
    {
        std::string status_id = std::to_string(statusToInt(status));
        if (status_id == "-1") return false;
        sql::SQLString query = "UPDATE User";
        query += " SET Status = "+status_id;
        query += " WHERE BINARY UserName = '"+username+"'";
        stmt = conn->createStatement();
        stmt->execute(query);
        return true;
    }
    catch(sql::SQLException &e)
    {
        printf("writeLastStatus:\n");
        print_err(e);
        return false;
    }
}
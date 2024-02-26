#include "ChatDatabase.h"

void ChatDatabase::print_err(sql::SQLException e) {
    std::cerr << "# ERR: SQLException in " << __FILE__;
    std::cerr << " on line " << __LINE__ << std::endl;
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
        print_err(e);
        return -1;
    }
}

bool ChatDatabase::check_username(user_t *user) {
    try
    {
        stmt = conn->createStatement();
        rs = stmt->executeQuery("SELECT UserName From User WHERE UserName ='" +user->login+"'");
        if(!rs->next())
            return false;
        return true;
    }
    catch(sql::SQLException &e)
    {
        print_err(e);
        return false;
    }
}

bool ChatDatabase::reg_user(user_t *user) {
    try
    {
        if(check_username(user))
            return false; //login exist

        std::string next_id = std::to_string(find_next_id("User"));
        stmt = conn->createStatement(); 

        stmt->execute("INSERT INTO User VALUE("+ next_id +", '"+user->login+"', '"+user->password+"', 3)");
        return true;
    }
    catch(sql::SQLException &e)
    {
        print_err(e);
        return false;
    }
}

bool ChatDatabase::log_user(user_t *user) {
    try
    {
        sql::SQLString query = "SELECT U.UserName, U.Password, R.Role FROM User AS U";
        query += " INNER JOIN Roles AS R ON U.Role = R.ID";
        query += " WHERE U.UserName = '"+user->login+"' AND U.Password = '"+user->password+"'";
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        if(!rs->next())
            return false;
        user->role = rs->getString("Role");
        return true;
    }
    catch(sql::SQLException &e)
    {
        print_err(e);
        return false;
    }
}

bool ChatDatabase::getListOfUsers(std::string *answer, user_t *user) {
    try
    {
        *(answer) = "/";
        sql::SQLString query = "SELECT US2.UserName FROM Chats AS CH";
        query += " INNER JOIN User AS US1 ON CH.User1 = US1.ID";
        query += " INNER JOIN User AS US2 ON CH.User2 = US2.ID";
        query += " WHERE US1.UserName = '"+user->login+"'";
        stmt = conn->createStatement();
        rs = stmt->executeQuery(query);
        while(rs->next()) {
            *(answer) += rs->getString("UserName") + "/";
        }
        return true;
    }
    catch(sql::SQLException &e)
    {
        print_err(e);
        return false;
    }
}
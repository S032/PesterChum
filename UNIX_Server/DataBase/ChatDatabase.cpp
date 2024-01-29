#include "ChatDatabase.h"

ChatDatabase::ChatDatabase() {
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        printf("Connecting DataBase...\n");
        conn = driver->connect(db_uri, db_user, db_pass);
        conn->setSchema(db);
        printf("Success\n");
    }
    catch(sql::SQLException& e)
    {
    std::cerr << "# ERR: SQLException in " << __FILE__;
    std::cerr << " on line " << __LINE__ << std::endl;
    std::cerr << "# ERR: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << " on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << " on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << " on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
}

int main() {
    user_t user = {"Chepush1", "1234", ""};
    ChatDatabase base;
    if(base.reg_user(&user))
        printf("%s registred\n", user.login.c_str());
    else
        printf("%s's already taken\n", user.login.c_str());


}
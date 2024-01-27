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
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << " on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return;
    }
}

int reg_user(user_t *user) {
    return 111;
}

int main() {
    ChatDatabase base;
}
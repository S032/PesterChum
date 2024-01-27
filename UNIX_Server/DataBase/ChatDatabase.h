#ifndef CHAT_DATABASE_H
#define CHAT_DATABASE_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

struct user_t {
    std::string login;
    std::string password;
    std::string role;
};

class ChatDatabase {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;
    user_t User;
    std::string db_user = "chat_admin";
    std::string db_pass = "5639";
    std::string db_uri = "tcp://127.0.0.1:3306";
    std::string db = "PesterChum";
public:
    ChatDatabase();
    int reg_user(user_t *user);
    int log_user(user_t *user);
private:
    bool check_user(user_t user);
};


#endif

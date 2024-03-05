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
    sql::Statement *stmt;
    sql::ResultSet *rs;
    user_t User;
    std::string db_user = "chat_admin";
    std::string db_pass = "5639";
    std::string db_uri = "tcp://127.0.0.1:3306";
    std::string db = "PesterChum";
public:
    ChatDatabase();
    bool reg_user(user_t *user);
    bool log_user(user_t *user);
    bool check_user_in_chattable(std::string sender_name, std::string recipient_name);
    bool getListOfUsers(std::string *answer, std::string username);
    bool getListOfIcReq(std::string *answer, std::string username);
    bool getListOfOgReq(std::string *answer, std::string username);
    bool addRequest(std::string sender_name, std::string recipient_name, std::string *answer);
    bool delRequest(std::string requester_name, std::string recipient_name, std::string *answer);
    bool makeChat(std::string user1, std::string user2);
    bool delChat(std::string sender_name, std::string friend_name);
private:
    int UsernameToInt(std::string username);
    int find_next_id(sql::SQLString Table);
    bool check_username(user_t *user);
    void print_err(sql::SQLException e);
};


#endif

#ifndef CHAT_DATABASE_H
#define CHAT_DATABASE_H

#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

struct user_t {
  std::string login;
  std::string password;
  std::string status;
};

class ChatDatabase {
private:
  sql::mysql::MySQL_Driver *driver;
  std::unique_ptr<sql::Connection> conn;
  sql::Statement *stmt;
  sql::ResultSet *rs;
  user_t User;
  std::string db_user = "chat_admin";
  std::string db_pass = "5639";
  std::string db_uri = "tcp://database:3306";
  std::string db = "PesterChum";

public:
  ChatDatabase();
  bool reg_user(user_t *user);
  bool log_user(user_t *user);
  bool check_user_in_chattable(std::string sender_name,
                               std::string recipient_name);
  std::vector<std::string> getFriendList(std::string username);
  bool getListOfIcReq(std::string *answer, std::string username);
  bool getListOfOgReq(std::string *answer, std::string username);
  bool addRequest(std::string sender_name, std::string recipient_name,
                  std::string *answer);
  bool delRequest(std::string requester_name, std::string recipient_name,
                  std::string *answer);
  bool makeChat(std::string user1, std::string user2);
  bool delChat(std::string sender_name, std::string friend_name);
  bool writeLastStatus(std::string username, std::string status);

private:
  void mysqlServerConnect(int timeoutSeconds, int retryIntervalMs);
  bool waitForMe();
  int UsernameToInt(std::string username);
  int statusToInt(std::string status);
  int find_next_id(sql::SQLString Table);
  bool check_username(user_t *user);
  void print_err(sql::SQLException e);
};

#endif

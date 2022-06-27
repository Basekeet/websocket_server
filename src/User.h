#ifndef WEBSOCKETSERVER_USER
#define WEBSOCKETSERVER_USER


//We need to define this when using the Asio library without Boost
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_

#include <string>

#include "utills.h"

using std::string;

// User object
class User {
private:
    ClientConnection conn;// Pointer to client connection object
    string username;      // Username of that user

public:
    // Constructor with client connection object and username
    // @param _conn - ClientConnection object
    // @param _username - username of that user
    User(ClientConnection _conn, string _username);

    ClientConnection get_conn();// Client connection getter

    string get_username();// username getter
};

#endif

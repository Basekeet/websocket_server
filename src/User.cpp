#include "User.h"


User::User(ClientConnection _conn, string _username) {
    conn = std::move(_conn);
    username = std::move(_username);
}

ClientConnection User::get_conn() {
    return conn;
}

string User::get_username() {
    return username;
}
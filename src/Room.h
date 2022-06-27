#ifndef WEBSOCKETSERVER_ROOM
#define WEBSOCKETSERVER_ROOM

#include <string>
#include <vector>

#include "User.h"
#include "WebsocketServer.h"

using std::string;
using std::vector;


// Chat room object
class Room {
private:
    string name;       // Room name property
    vector<User> users;// Users in that room

public:
    // Empty constructor
    Room() = default;

    // Constructor with name parameter
    // @param _name - name of that room
    explicit Room(const string &_name);

    // Add callback for writing to that room
    // @param s - WebsocketServer object
    // @param mainEventLoop - main thread for different operations(not network operations)
    void add_callback(WebsocketServer &s, asio::io_service &mainEventLoop);


    string get_name() const;            // Getter of name parameter
    void add_user(const User &new_user);// Adding user to this->users vector
};

#endif

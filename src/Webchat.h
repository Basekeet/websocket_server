#ifndef WEBSOCKETSERVER_WEBCHAT
#define WEBSOCKETSERVER_WEBCHAT

#include <map>

#include "Room.h"
#include "User.h"

using std::map;

// Object that stores and processes all rooms and users in web chat server
class Webchat {
public:
    map<string, Room> rooms;// All rooms
    vector<User> users;     // All users

    Room *get_room(const string &room_name);// Getter for unique room

    User *get_user(const string &username);// Getter for unique user


    bool contains_room(const string &room_name) const;// Checking whether room_name in rooms or not

    bool contains_user(const string &username);// Checking whether username in users or not


    void add_room(const Room &room);// Adding room to rooms vector

    void add_user(const User &u);// Adding user to users vector
};


#endif

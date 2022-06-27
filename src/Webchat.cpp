#include "Webchat.h"


Room *Webchat::get_room(const string &room_name) {
    if (contains_room(room_name))
        return &(*rooms.find(room_name)).second;
    else
        return nullptr;
}

User *Webchat::get_user(const string &username) {
    for (auto &user: users) {
        if (user.get_username() == username) {
            return &user;
        }
    }
    return nullptr;
}

bool Webchat::contains_room(const string &room_name) const {
    return rooms.count(room_name) > 0;
}

bool Webchat::contains_user(const string &username) {
    for (auto u: users) {
        if (u.get_username() == username) {
            return true;
        }
    }
    return false;
}

void Webchat::add_room(const Room &room) {
    rooms[room.get_name()] = room;
}

void Webchat::add_user(const User &u) {
    users.push_back(u);
}

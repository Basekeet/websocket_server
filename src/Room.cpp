#include "Room.h"


Room::Room(const string &_name) {
    name = _name;
}

void Room::add_callback(WebsocketServer &s, asio::io_service &mainEventLoop) {
    s.message(this->name,
              [&mainEventLoop, &s, this](const ClientConnection &conn,
                                         const Json::Value &args) {
                  mainEventLoop.post([&s, args, this]() {
                      vector<User> v = this->users;
                      for (auto u: v) {
                          s.sendMessage(u.get_conn(), args);
                      }
                  });
              });
}

string Room::get_name() const {
    return name;
}

void Room::add_user(const User &new_user) {
    users.push_back(new_user);
}

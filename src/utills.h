#ifndef WEBSOCKETSERVER_UTILLS
#define WEBSOCKETSERVER_UTILLS

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_

#include <json/json.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

using std::string;

// Useful type definitions
typedef websocketpp::server<websocketpp::config::asio> WebsocketEndpoint;
typedef websocketpp::connection_hdl ClientConnection;

// Parse string to json
// @param json - string which will be converted to json object
Json::Value parseJson(const string &json);
// Parse json to string
// @param val - json object which will be converted to string
string stringifyJson(const Json::Value &val);

#endif

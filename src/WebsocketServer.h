#ifndef _WEBSOCKET_SERVER
#define _WEBSOCKET_SERVER

//We need to define this when using the Asio library without Boost
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

#include "utills.h"

using std::map;
using std::string;
using std::vector;


// Web chat core object
class WebsocketServer {
public:
    // Empty constructor
    WebsocketServer();
    // Running network thread
    // @param port - which port do we use
    void run(int port);

    //Returns the number of currently connected clients
    size_t numConnections();

    //Registers a callback for when a client connects
    template<typename CallbackTy>
    void connect(CallbackTy handler) {
        //Make sure we only access the handlers list from the networking thread
        this->eventLoop.post([this, handler]() {
            this->connectHandlers.push_back(handler);
        });
    }

    //Registers a callback for when a client disconnects
    template<typename CallbackTy>
    void disconnect(CallbackTy handler) {
        //Make sure we only access the handlers list from the networking thread
        this->eventLoop.post([this, handler]() {
            this->disconnectHandlers.push_back(handler);
        });
    }

    //Registers a callback for when a particular type of message is received
    template<typename CallbackTy>
    void message(const string &messageType, CallbackTy handler) {
        //Make sure we only access the handlers list from the networking thread
        this->eventLoop.post([this, messageType, handler]() {
            this->messageHandlers[messageType].push_back(handler);
        });
    }

    //Sends a message to an individual client
    void sendMessage(ClientConnection conn, const Json::Value &arguments);

    //Sends a message to all connected clients
    void broadcastMessage(const Json::Value &arguments);

private:
    // On open connection callback for websocket library
    void onOpen(const ClientConnection &conn);
    // On close connection callback for websocket library
    void onClose(const ClientConnection &conn);
    // On message callback for websocket library
    void onMessage(const ClientConnection &conn, const WebsocketEndpoint::message_ptr &msg);

    asio::io_service eventLoop;              // Network event loop
    WebsocketEndpoint endpoint;              //
    vector<ClientConnection> openConnections;// List of all connections
    std::mutex connectionListMutex;          // Mutex for locking open connections

    vector<std::function<void(ClientConnection)>> connectHandlers;                                  // All handlers for connections
    vector<std::function<void(ClientConnection)>> disconnectHandlers;                               // All handlers for disconnecting
    map<string, vector<std::function<void(ClientConnection, const Json::Value &)>>> messageHandlers;// All message handlers
};

#endif

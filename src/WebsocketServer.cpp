#include "WebsocketServer.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

//The name of the special JSON field that holds the message type for messages
#define MESSAGE_FIELD "__MESSAGE__"

WebsocketServer::WebsocketServer() {
    //Wire up our event handlers
    this->endpoint.set_open_handler(std::bind(&WebsocketServer::onOpen, this, std::placeholders::_1));
    this->endpoint.set_close_handler(std::bind(&WebsocketServer::onClose, this, std::placeholders::_1));
    this->endpoint.set_message_handler(std::bind(&WebsocketServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));

    //Initialise the Asio library, using our own event loop object
    this->endpoint.init_asio(&(this->eventLoop));
}

void WebsocketServer::run(int port) {
    //Listen on the specified port number and start accepting connections
    this->endpoint.listen(port);
    this->endpoint.start_accept();

    //Start the Asio event loop
    this->endpoint.run();
}

size_t WebsocketServer::numConnections() {
    //Prevent concurrent access to the list of open connections from multiple threads
    std::lock_guard<std::mutex> lock(this->connectionListMutex);

    return this->openConnections.size();
}

void WebsocketServer::sendMessage(ClientConnection conn, const Json::Value &arguments) {
    //Copy the argument values, and bundle the message type into the object
    const Json::Value &messageData = arguments;

    //Send the JSON data to the client (will happen on the networking thread's event loop)
    this->endpoint.send(std::move(conn), stringifyJson(messageData), websocketpp::frame::opcode::text);
}

void WebsocketServer::broadcastMessage(const Json::Value &arguments) {
    //Prevent concurrent access to the list of open connections from multiple threads
    std::lock_guard<std::mutex> lock(this->connectionListMutex);

    for (const auto &conn: this->openConnections) {
        this->sendMessage(conn, arguments);
    }
}

void WebsocketServer::onOpen(const ClientConnection &conn) {
    {
        //Prevent concurrent access to the list of open connections from multiple threads
        std::lock_guard<std::mutex> lock(this->connectionListMutex);

        //Add the connection handle to our list of open connections
        this->openConnections.push_back(conn);
    }

    //Invoke any registered handlers
    for (const auto &handler: this->connectHandlers) {
        handler(conn);
    }
}

void WebsocketServer::onClose(const ClientConnection &conn) {
    {
        //Prevent concurrent access to the list of open connections from multiple threads
        std::lock_guard<std::mutex> lock(this->connectionListMutex);

        //Remove the connection handle from our list of open connections
        auto connVal = conn.lock();
        auto newEnd = std::remove_if(this->openConnections.begin(), this->openConnections.end(), [&connVal](const ClientConnection &elem) {
            //If the pointer has expired, remove it from the vector
            if (elem.expired()) {
                return true;
            }

            //If the pointer is still valid, compare it to the handle for the closed connection
            auto elemVal = elem.lock();
            if (elemVal.get() == connVal.get()) {
                return true;
            }

            return false;
        });

        //Truncate the connections vector to erase the removed elements
        this->openConnections.resize(std::distance(openConnections.begin(), newEnd));
    }

    //Invoke any registered handlers
    for (const auto &handler: this->disconnectHandlers) {
        handler(conn);
    }
}

void WebsocketServer::onMessage(const ClientConnection &conn, const WebsocketEndpoint::message_ptr &msg) {
    //Validate that the incoming message contains valid JSON
    Json::Value messageObject = parseJson(msg->get_payload());
    if (!messageObject.isNull()) {
        //Validate that the JSON object contains the message type field
        if (messageObject.isMember(MESSAGE_FIELD)) {
            //Extract the message type and remove it from the payload
            std::string messageType = messageObject[MESSAGE_FIELD].asString();
            messageObject.removeMember(MESSAGE_FIELD);

            //If any handlers are registered for the message type, invoke them
            auto &handlers = this->messageHandlers[messageType];
            for (const auto &handler: handlers) {
                handler(conn, messageObject);
            }
        }
    }
}

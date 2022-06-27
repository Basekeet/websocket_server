#include "src/Webchat.h"
#include "src/WebsocketServer.h"

#include <asio/io_service.hpp>
#include <iostream>
#include <thread>

//The port number the WebSocket server listens on
#define PORT_NUMBER 8080

int main() {
    //Create the event loop for the main thread, and the WebSocket server
    asio::io_service mainEventLoop;
    WebsocketServer server;

    Webchat wc;

    server.connect([&mainEventLoop, &server](const ClientConnection &conn) {
        mainEventLoop.post([conn, &server]() {
            std::clog << "Connection opened." << std::endl;
            std::clog << "There are now " << server.numConnections() << " open connections." << std::endl;

            Json::Value ans;
            ans["data"] = "Hello";
            server.sendMessage(conn, ans);
        });
    });
    server.disconnect([&mainEventLoop, &server](const ClientConnection &conn) {
        mainEventLoop.post([&server]() {
            std::clog << "Connection closed." << std::endl;
            std::clog << "There are now " << server.numConnections() << " open connections." << std::endl;
        });
    });

    server.message("connect", [&mainEventLoop, &server, &wc](const ClientConnection &conn, const Json::Value &args) {
        mainEventLoop.post([&wc, conn, args, &server, &mainEventLoop]() {
            if (args.isMember("room_name") && args.isMember("username")) {
                if (!wc.contains_user(args["username"].asString())) {
                    User u(conn, args["username"].asString());
                    wc.add_user(u);
                }
                User *u = wc.get_user(args["username"].asString());
                if (!wc.contains_room(args["room_name"].asString())) {
                    Room new_room(args["room_name"].asString());
                    wc.add_room(new_room);
                    wc.get_room(args["room_name"].asString())->add_callback(server, mainEventLoop);
                }

                wc.get_room(args["room_name"].asString())->add_user(*u);
                Json::Value res;
                res["data"] = "you are connected";
                server.sendMessage(conn, res);
            }
        });
    });

    server.disconnect([&mainEventLoop, &server](const ClientConnection &conn) {
        mainEventLoop.post([&server]() {
            std::clog << "Connection closed." << std::endl;
            std::clog << "There are now " << server.numConnections() << " open connections." << std::endl;
        });
    });


    //Start the networking thread
    std::thread serverThread([&server]() {
        server.run(PORT_NUMBER);
    });

    //Start a keyboard input thread that reads from stdin
    std::thread inputThread([&server, &mainEventLoop]() {
        string input;
        while (true) {
            //Read user input from stdin
            std::getline(std::cin, input);

            //Broadcast the input to all connected clients (is sent on the network thread)
            Json::Value payload;
            payload["input"] = input;
            server.broadcastMessage(payload);

            //Debug output on the main thread
            mainEventLoop.post([]() {
                std::clog << "User input debug output on the main thread" << std::endl;
            });
        }
    });

    //Start the event loop for the main thread
    asio::io_service::work work(mainEventLoop);
    mainEventLoop.run();

    return 0;
}

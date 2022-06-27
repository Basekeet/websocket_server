#include "doctest.h"

#include "../src/Room.h"
#include "../src/User.h"
#include "../src/Webchat.h"
#include "../src/utills.h"


TEST_CASE("Check contains room") {
    Webchat wc;
    Room r1("1");
    Room r2("2");

    wc.add_room(r1);
    REQUIRE(wc.contains_room("1"));
    REQUIRE(!wc.contains_room("2"));
    wc.add_room(r2);
    REQUIRE(wc.contains_room("2"));
}

TEST_CASE("Check contains user") {
    ClientConnection c;
    User u1(c, "1");
    User u2(c, "2");
    User u3(c, "3");

    Webchat wc;
    wc.add_user(u1);
    wc.add_user(u2);

    REQUIRE(wc.contains_user("1"));
    REQUIRE(wc.contains_user("2"));
    REQUIRE(!wc.contains_user("3"));

    wc.add_user(u3);
    REQUIRE(wc.contains_user("3"));
}

TEST_CASE("Check getting room from webchat") {
    Webchat wc;
    Room r1("1");
    Room r2("2");

    wc.add_room(r1);
    REQUIRE(wc.get_room("1")->get_name() == "1");
    REQUIRE(wc.get_room("2") == nullptr);

    wc.add_room(r2);
    REQUIRE(wc.get_room("2")->get_name() == "2");
}

TEST_CASE("Check getting user from webchat") {
    ClientConnection c;
    User u1(c, "1");
    User u2(c, "2");
    Webchat wc;

    wc.add_user(u1);
    REQUIRE(wc.get_user("1")->get_username() == "1");
    REQUIRE(wc.get_user("2") == nullptr);

    wc.add_user(u2);
    REQUIRE(wc.get_user("2")->get_username() == "2");
}
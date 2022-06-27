#include "../src/User.h"
#include "../src/utills.h"
#include "doctest.h"
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_


TEST_CASE("Create user and check fields") {
    ClientConnection c;
    User u(c, "Sam");
    REQUIRE("Sam" == u.get_username());
}

#include "../src/Room.h"
#include "doctest.h"

TEST_CASE("Check empty constructor") {
    Room r;
    REQUIRE(r.get_name() == "");
}

TEST_CASE("Check not empty constructor") {
    Room r1("First room");
    Room r2("");

    REQUIRE(r1.get_name() == "First room");
    REQUIRE(r2.get_name() == "");
}
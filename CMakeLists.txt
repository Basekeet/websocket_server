cmake_minimum_required (VERSION 3.5)
project(WebsocketServer)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable all compiler warnings
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Link against pthreads under Unix systems
if (NOT MSVC AND NOT MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# Under MinGW, statically link against the standard libraries
if (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

#add_link_options(-lws2_32)

# Force WebSocket++ to use 100% C++11 mode, so that it doesn't try to look for Boost
# (Note that under MinGW, Boost.Thread is still required, due to a bug in MinGW that prevents the C++11 version from being used)
add_definitions(-D_WEBSOCKETPP_CPP11_STRICT_)

# Add the third-party headers directory to the search path
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/external")

# Compile jsoncpp from source
add_library(jsoncpp STATIC ${PROJECT_SOURCE_DIR}/external/jsoncpp.cpp)

set(Boost_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/boost")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55.0 COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS})

# Build the server executable
add_executable(server server.cpp src/WebsocketServer.cpp src/User.cpp src/User.h src/Room.cpp src/Room.h src/utills.h src/Webchat.cpp src/Webchat.h src/utills.cpp)
target_link_libraries (server jsoncpp)

target_link_libraries(server ws2_32)
target_link_libraries(
		server
		${Boost_LIBRARIES}
)
target_link_libraries(
		server
		wsock32
)


add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_17)
enable_testing()

# Taken from homework files
add_library(doctest STATIC external/doctest.cpp)
target_include_directories(doctest PUBLIC external/ )
set_target_properties(doctest PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
add_library(doctest::doctest ALIAS doctest)
add_library(Catch2::Catch2 ALIAS doctest)

add_executable(project-tests tests/test_chat.cpp tests/test_room.cpp tests/test_user.cpp src/WebsocketServer.cpp src/User.cpp src/User.h src/Room.cpp src/Room.h src/utills.h src/Webchat.cpp src/Webchat.h src/utills.cpp)
target_link_libraries(project-tests PUBLIC doctest jsoncpp ws2_32 ${Boost_LIBRARIES}  wsock32 default)
add_test(NAME project-tests COMMAND project-tests --force-colors -d)
#target_link_libraries (project-tests jsoncpp)
#
#target_link_libraries(project-tests ws2_32)
#target_link_libraries(
#		project-tests
#		${Boost_LIBRARIES}
#)
#target_link_libraries(
#		project-tests
#		wsock32
#)



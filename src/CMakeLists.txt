set(SRC1 "myutility.cc" "socketcpp.cc" "chat_server.cc")
set(SRC2 "socketcpp.cc" "client.cc")
set(SRC3 "socketcpp.cc" "user.cc" "login_server.cc")

add_executable(chat ${SRC1})
add_executable(client ${SRC2})
add_executable(login ${SRC3})

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(jsoncpp REQUIRED)
target_link_libraries(client PRIVATE jsoncpp_lib)
cmake_minimum_required(VERSION 3.16)
project(cpp2web VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cpp2web INTERFACE)
target_include_directories(cpp2web INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(cpp2web_app src/main.cpp src/converter.cpp)
target_include_directories(cpp2web_app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(cpp2web_app PRIVATE cpp2web)

install(TARGETS cpp2web_app RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

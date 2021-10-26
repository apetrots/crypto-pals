cmake_minimum_required(VERSION 3.10)

# set the project name
project(s1c8)

# add_subdirectory(OpenSSL)

# add the executable
add_executable(s1c8 s1c8.cpp)

find_library(LIBRARIES protobuf openssl )

target_link_libraries(s1c8 PUBLIC ${LIBRARIES})

target_link_libraries(s1c8 PRIVATE -lssl -lcrypto)
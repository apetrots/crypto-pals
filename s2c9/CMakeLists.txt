cmake_minimum_required(VERSION 3.10)

# set the project name
project(s2c9)

# add_subdirectory(OpenSSL)

# add the executable
add_executable(s2c9 s2c9.cpp)

find_library(LIBRARIES protobuf openssl )

target_link_libraries(s2c9 PUBLIC ${LIBRARIES})

target_link_libraries(s2c9 PRIVATE -lssl -lcrypto)
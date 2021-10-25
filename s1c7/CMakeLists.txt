cmake_minimum_required(VERSION 3.10)

# set the project name
project(s1c7)

# add_subdirectory(OpenSSL)

# add the executable
add_executable(s1c7 s1c7.cpp)

find_library(LIBRARIES protobuf openssl )

target_link_libraries(s1c7 PUBLIC ${LIBRARIES})
# find_package(OpenSSL REQUIRED)

target_link_libraries(s1c7 PRIVATE -lssl -lcrypto)


# target_include_directories(s1c7 PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           "${PROJECT_SOURCE_DIR}/OpenSSL"
#                           )

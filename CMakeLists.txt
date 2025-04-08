cmake_minimum_required(VERSION 3.10)
project(ArtHobby)

# For drogon
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)

# For JWT
set(JWT_BUILD_EXAMPLES OFF)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

file(GLOB BACKEND_SOURCES "src/*.cpp" "src/*/*.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ArtHobby

    ${BACKEND_SOURCES}
)

target_link_libraries(ArtHobby drogon jwt-cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/)

target_include_directories(ArtHobby PUBLIC ${CMAKE_SOURCE_DIR}/include/)

add_subdirectory(libraries/drogon)
add_subdirectory(libraries/jwt-cpp)

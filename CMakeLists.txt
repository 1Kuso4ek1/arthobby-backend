cmake_minimum_required(VERSION 3.10)
project(ArtHobby)

# For drogon
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)

# For JWT
set(JWT_BUILD_EXAMPLES OFF)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

set(
    BACKEND_HEADERS

    include/Controllers/CartController.hpp
    include/Controllers/LoginController.hpp
    include/Controllers/ProductController.hpp
    include/Controllers/StaticController.hpp

    include/Filters/JwtFilter.hpp

    include/Models/Cart.hpp
    include/Models/Product.hpp
    include/Models/User.hpp

    include/Utils/Singleton.hpp
)

set(
    BACKEND_SOURCES

    src/Controllers/CartController.cpp
    src/Controllers/LoginController.cpp
    src/Controllers/ProductController.cpp
    src/Controllers/StaticController.cpp

    src/Filters/JwtFilter.cpp

    src/Models/Cart.cpp
    src/Models/Product.cpp
    src/Models/User.cpp

    src/main.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libraries/drogon)
add_subdirectory(libraries/jwt-cpp)

add_executable(
    ArtHobby

    ${BACKEND_HEADERS}
    ${BACKEND_SOURCES}
)

target_link_libraries(ArtHobby drogon jwt-cpp)

target_include_directories(ArtHobby PUBLIC ${CMAKE_SOURCE_DIR}/include/)


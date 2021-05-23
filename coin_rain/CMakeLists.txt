cmake_minimum_required(VERSION 3.10)

project(coin_rain)

set (CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2-cmake-modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(coin_rain
	src/main.cpp
	src/coin.hpp
	src/coin.cpp)
target_link_libraries(coin_rain PRIVATE
	SDL2::Main
	SDL2::Image
	SDL2::TTF)

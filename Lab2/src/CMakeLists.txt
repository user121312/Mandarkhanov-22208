cmake_minimum_required(VERSION 3.20)

project(game_of_life)

set(SOURCES main.cpp view/view.h view/view.cpp model/model.h model/model.cpp controller/controller.h controller/controller.cpp util/util.h util/util.cpp)

add_executable(game_of_life ${SOURCES})



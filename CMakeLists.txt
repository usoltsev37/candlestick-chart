cmake_minimum_required(VERSION 3.15)
project(Project_andlestick_hart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -v -Wall -Wextra")

set(CMAKE_AUTOMOC ON)

include_directories("include/")

add_executable(Project_andlestick_hart
        src/main.cpp
        src/controller.cpp
        src/load.cpp
        src/model.cpp
        src/view.cpp
        src/MainDialog.cpp
        include/controller.h
        include/load.h
        include/model.h
        include/view.h
        include/MainDialog.h
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SOURCE_FILES src/main.cpp include/MainDialog.h)
add_executable(HelloWorld ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
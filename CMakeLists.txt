cmake_minimum_required(VERSION 3.15)
project(Project_candlestick_chart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -v -Wall -Wextra")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories("include/")

add_executable(Project_candlestick_chart
        src/main.cpp
        src/load.cpp
        src/model.cpp
        src/view.cpp
        src/MainDialog.cpp
        src/chartwindow.cpp
        src/themewindow.cpp
        src/styleset.cpp

        include/load.h
        include/model.h
        include/view.h
        include/MainDialog.h
        include/chartwindow.h
        include/themewindow.h
        include/styleset.h
        include/candlesFwd.h
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Network REQUIRED)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Charts)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
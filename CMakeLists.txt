cmake_minimum_required(VERSION 3.15)
project(Project_candlestick_chart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -v -Wall -Wextra")

set(CMAKE_AUTOMOC ON)
set(Qt5_DIR "~/Qt/5.14.2/gcc_64/lib/cmake")
set(Qt5Core_DIR "~/Qt/5.14.2/gcc_64/lib/cmake/Qt5Core")
set(Qt5Gui_DIR "~/Qt/5.14.2/gcc_64/lib/cmake/Qt5Gui")
set(Qt5Widgets_DIR "~/Qt/5.14.2/gcc_64/lib/cmake/Qt5Widgets")
set(Qt5Charts_DIR "~/Qt/5.14.2/gcc_64/lib/cmake/Qt5Charts")
set(Qt5Network_DIR "~/Qt/5.14.2/gcc_64/lib/cmake/Qt5Network")

include_directories("include/")

add_executable(Project_candlestick_chart
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
find_package(Qt5Charts REQUIRED)
find_package(Qt5Network REQUIRED)

set(SOURCE_FILES src/main.cpp include/MainDialog.h)
add_executable(HelloWorld ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
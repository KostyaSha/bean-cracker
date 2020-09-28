cmake_minimum_required(VERSION 3.8)
project(bean-cracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_PREFIX_PATH /Users/integer/Qt/5.14.1/clang_64/lib/cmake/)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_FILES main.cpp
        beanpacket.cpp
        beanpacketsloggermodel.cpp
        beanpacketsloggerheaderview.cpp
        beanpacketsloggerwindow.cpp
        beanpacketsmonitor.cpp
        beanpacketsmonitormodel.cpp
        beanpacketsplayermodel.cpp
        beanpacketsplayerwindow.cpp
        beanpacketssender.cpp
        beanpacketssendermodel.cpp
        beanpacketwidget.cpp
        main.cpp
        mainwindow.cpp
        )

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5SerialBus REQUIRED)

add_executable(bean-cracker ${SOURCE_FILES} ${SRC} ${UI_HEADERS})

#qt5_use_modules(bean-cracker Core Widgets Gui)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::SerialPort)
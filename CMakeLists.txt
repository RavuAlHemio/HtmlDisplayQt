cmake_minimum_required(VERSION 2.8.11)
project(HtmlDisplayQt)

find_package(Qt5Quick)
find_package(Qt5Widgets)
find_package(Qt5WebEngine)
find_package(Qt5WebEngineWidgets)

set(CMAKE_AUTOMOC ON)

add_executable(
    HtmlDisplayQt
    WIN32
    src/main.cpp
    src/MainWindow.cpp
)
target_link_libraries(
    HtmlDisplayQt
    Qt5::Widgets
    Qt5::WebEngineWidgets
)

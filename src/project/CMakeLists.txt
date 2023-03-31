cmake_minimum_required(VERSION 3.5)

project(photolab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_NAME photolab)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(model view controller lib)

add_executable(${EXECUTABLE_NAME}
        view/main.cpp
        view/mainwindow.cpp
        view/mainwindow.h
        model/s21_matrix.cpp
        model/s21_matrix.h
        model/model.cpp
        controller/controller.cpp
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${EXECUTABLE_NAME})
endif()

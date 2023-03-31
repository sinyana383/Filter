cmake_minimum_required(VERSION 3.5)
project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_NAME tests)
set(SOURCE_DIR ../project)
set(SOURCE_LIST
	kernelTest.cpp
	${SOURCE_DIR}/model/s21_matrix.cpp
	${SOURCE_DIR}/model/model.cpp
)

add_subdirectory(googletest-main)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(GTest REQUIRED)

include_directories(
	${SOURCE_DIR}/model
	${SOURCE_DIR}/view
	${SOURCE_DIR}/controller
)

add_executable(${EXECUTABLE_NAME} ${SOURCE_LIST})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets gtest gtest_main)

add_test(NAME all COMMAND ${EXECUTABLE_NAME})

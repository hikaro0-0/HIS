cmake_minimum_required(VERSION 3.16)

project(HIS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

find_package(Qt6 COMPONENTS Widgets QUIET)
if(TARGET Qt6::Widgets)
    set(QT_WIDGETS Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_WIDGETS Qt5::Widgets)
endif()

set(SOURCES
    src/main.cpp
    src/WelcomeWindow.cpp
    src/MainWindow.cpp
    src/GuestDialog.cpp
    src/DateSelectionDialog.cpp
    src/HotelSystem.cpp
    src/Room.cpp
    src/Guest.cpp
    src/Standard.cpp
    src/DeluxeRoom.cpp
    src/SuiteRoom.cpp
)

set(HEADERS
    include/WelcomeWindow.h
    include/MainWindow.h
    include/GuestDialog.h
    include/DateSelectionDialog.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_DEPRECATED_WARNINGS)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    C:/Qt/6.10.1/msvc2022_64/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_WIDGETS})


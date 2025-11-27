QT += core gui widgets

CONFIG += c++17

TARGET = HIS
TEMPLATE = app

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    HotelSystem.cpp \
    Room.cpp \
    Standard.cpp \
    DeluxeRoom.cpp \
    SuiteRoom.cpp

HEADERS += \
    MainWindow.h \
    HotelSystem.h \
    Room.h \
    RoomConstants.h \
    StandardRoom.h \
    DeluxeRoom.h \
    SuiteRoom.h \
    exceptions.h

# Настройки компилятора
win32 {
    CONFIG += console
    QMAKE_CXXFLAGS += /std:c++17
}

# Отключение предупреждений для удобства (опционально)
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

# Настройки для отладки и релиза
CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.qrc
    UI_DIR = debug/.ui
}

CONFIG(release, debug|release) {
    DESTDIR = release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.qrc
    UI_DIR = release/.ui
}

# Кодировка исходных файлов
CODECFORTR = UTF-8
CODECFORSRC = UTF-8

# Дополнительные флаги
DEFINES += QT_DEPRECATED_WARNINGS
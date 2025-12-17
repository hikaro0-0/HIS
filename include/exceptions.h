#pragma once
#include <stdexcept>
#include <string>

class RoomException : public std::runtime_error {
private:
    int roomNumber;

public:
    RoomException(const std::string& message, int roomNum = 0)
        : std::runtime_error(message), roomNumber(roomNum) {}

    int getRoomNumber() const { return roomNumber; }
};

class BookingException : public RoomException {
public:
    BookingException(const std::string& message, int roomNum = 0)
        : RoomException(message, roomNum) {}
};

class FileException : public std::runtime_error {
public:
    FileException(const std::string& message)
        : std::runtime_error(message) {}
};



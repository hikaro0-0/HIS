#pragma once
#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/RoomConstants.h"

class StandardRoom : public Room {
private:
    static constexpr int roomCount = RoomConstants::STANDARD_ROOM_COUNT;

public:
    StandardRoom(int number = 0, double price = 0.0, int beds = 1, bool wifi = true);
    ~StandardRoom() override = default;

    double calculateTotalPrice(int days) const override;
    void displayInfo() const override;
    std::string getRoomType() const override;
    Room* clone() const override;

    static constexpr int getRoomCount() { return roomCount; }

    StandardRoom& operator=(const StandardRoom& other);
};



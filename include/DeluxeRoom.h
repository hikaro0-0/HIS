#pragma once
#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/RoomConstants.h"

class DeluxeRoom : public Room {
private:
    bool hasMiniBar;
    bool hasBalcony;
    static constexpr int roomCount = RoomConstants::DELUXE_ROOM_COUNT;

public:
    DeluxeRoom(int number = 0, double price = 0.0, int beds = 2,
        bool wifi = true, bool miniBar = true, bool balcony = true);
    ~DeluxeRoom() override = default;

    double calculateTotalPrice(int days) const override;
    void displayInfo() const override;
    std::string getRoomType() const override;
    Room* clone() const override;

    bool getHasMiniBar() const { return hasMiniBar; }
    bool getHasBalcony() const { return hasBalcony; }
    static constexpr int getRoomCount() { return roomCount; }

    void setHasMiniBar(bool miniBar);
    void setHasBalcony(bool balcony);

    DeluxeRoom& operator=(const DeluxeRoom& other);
};



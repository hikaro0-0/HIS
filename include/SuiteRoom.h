#pragma once
#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/RoomConstants.h"

class SuiteRoom : public Room {
private:
    bool hasMiniBar;
    bool hasBalcony;
    bool hasJacuzzi;
    static constexpr int roomCount = RoomConstants::SUITE_ROOM_COUNT;

public:
    SuiteRoom(int number = 0, double price = 0.0, int beds = 2,
        bool wifi = true, bool miniBar = true, bool balcony = true,
        bool jacuzzi = true);
    ~SuiteRoom() override = default;

    double calculateTotalPrice(int days) const override;
    void displayInfo() const override;
    std::string getRoomType() const override;
    Room* clone() const override;

    bool getHasMiniBar() const { return hasMiniBar; }
    bool getHasBalcony() const { return hasBalcony; }
    bool getHasJacuzzi() const { return hasJacuzzi; }
    static constexpr int getRoomCount() { return roomCount; }

    void setHasMiniBar(bool miniBar);
    void setHasBalcony(bool balcony);
    void setHasJacuzzi(bool jacuzzi);

    SuiteRoom& operator=(const SuiteRoom& other);
};



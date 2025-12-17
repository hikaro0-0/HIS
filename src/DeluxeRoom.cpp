#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/RoomUtils.h"
#include <iostream>
#include <iomanip>

DeluxeRoom::DeluxeRoom(int number, double price, int beds, bool wifi, bool miniBar, bool balcony)
    : Room(number, price, beds, wifi),
    hasMiniBar(miniBar), hasBalcony(balcony) {
}

double DeluxeRoom::calculateTotalPrice(int days) const {
    return RoomUtils::calculateBasicTotalPrice(this, days);
}

void DeluxeRoom::displayInfo() const {
    RoomUtils::displayRoomCommonInfo(this, "Deluxe Room", true, true, false);
}

std::string DeluxeRoom::getRoomType() const {
    return "Deluxe";
}

Room* DeluxeRoom::clone() const {
    return new DeluxeRoom(*this);
}

void DeluxeRoom::setHasMiniBar(bool miniBar) {
    hasMiniBar = miniBar;
}

void DeluxeRoom::setHasBalcony(bool balcony) {
    hasBalcony = balcony;
}

DeluxeRoom& DeluxeRoom::operator=(const DeluxeRoom& other) {
    if (this != &other) {
        Room::operator=(other);
        hasMiniBar = other.hasMiniBar;
        hasBalcony = other.hasBalcony;
    }
    return *this;
}

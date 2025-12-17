#include "C:/Users/User/Desktop/HIS/include/StandardRoom.h"
#include "C:/Users/User/Desktop/HIS/include/RoomUtils.h"
#include <iostream>
#include <iomanip>

StandardRoom::StandardRoom(int number, double price, int beds, bool wifi)
    : Room(number, price, beds, wifi) {
}

double StandardRoom::calculateTotalPrice(int days) const {
    return RoomUtils::calculateBasicTotalPrice(this, days);
}

void StandardRoom::displayInfo() const {
    RoomUtils::displayRoomCommonInfo(this, "Standard Room", false, false, false);
}

std::string StandardRoom::getRoomType() const {
    return "Standard";
}

Room* StandardRoom::clone() const {
    return new StandardRoom(*this);
}

StandardRoom& StandardRoom::operator=(const StandardRoom& other) {
    if (this != &other) {
        Room::operator=(other);
    }
    return *this;
}

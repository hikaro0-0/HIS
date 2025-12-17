#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include "C:/Users/User/Desktop/HIS/include/RoomUtils.h"
#include <iostream>
#include <iomanip>

SuiteRoom::SuiteRoom(int number, double price, int beds, bool wifi, bool miniBar, bool balcony, bool jacuzzi)
    : Room(number, price, beds, wifi),
    hasMiniBar(miniBar), hasBalcony(balcony), hasJacuzzi(jacuzzi) {
}

double SuiteRoom::calculateTotalPrice(int days) const {
    return RoomUtils::calculateBasicTotalPrice(this, days);
}

void SuiteRoom::displayInfo() const {
    RoomUtils::displayRoomCommonInfo(this, "Suite Room", true, true, true);
}

std::string SuiteRoom::getRoomType() const {
    return "Suite";
}

Room* SuiteRoom::clone() const {
    return new SuiteRoom(*this);
}

void SuiteRoom::setHasMiniBar(bool miniBar) {
    hasMiniBar = miniBar;
}

void SuiteRoom::setHasBalcony(bool balcony) {
    hasBalcony = balcony;
}

void SuiteRoom::setHasJacuzzi(bool jacuzzi) {
    hasJacuzzi = jacuzzi;
}

SuiteRoom& SuiteRoom::operator=(const SuiteRoom& other) {
    if (this != &other) {
        Room::operator=(other);
        hasMiniBar = other.hasMiniBar;
        hasBalcony = other.hasBalcony;
        hasJacuzzi = other.hasJacuzzi;
    }
    return *this;
}

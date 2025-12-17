#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include <iostream>
#include <iomanip>

SuiteRoom::SuiteRoom(int number, double price, int beds, bool wifi, bool miniBar, bool balcony, bool jacuzzi)
    : Room(number, price, beds, wifi),
    hasMiniBar(miniBar), hasBalcony(balcony), hasJacuzzi(jacuzzi) {
}

double SuiteRoom::calculateTotalPrice(int days) const {
    if (days < 1) {
        throw std::invalid_argument("Number of days must be positive");
    }
    return pricePerDay * days;
}

void SuiteRoom::displayInfo() const {
    std::cout << "\n=== Suite Room ===" << std::endl;
    std::cout << "Room number: " << roomNumber << std::endl;
    std::cout << "Price per day: $" << std::fixed << std::setprecision(2) << pricePerDay << std::endl;
    std::cout << "Number of beds: " << bedCount << std::endl;
    std::cout << "Number of rooms: " << roomCount << std::endl;
    std::cout << "Mini-bar: " << (hasMiniBar ? "Yes" : "No") << std::endl;
    std::cout << "Balcony: " << (hasBalcony ? "Yes" : "No") << std::endl;
    std::cout << "Jacuzzi: " << (hasJacuzzi ? "Yes" : "No") << std::endl;
    std::cout << "Status: " << getStatusString() << std::endl;
    if (status != RoomStatus::AVAILABLE) {
        if (const Guest* guest = getGuest()) {
            std::cout << "Guest: " << guest->toContactString() << std::endl;
        }
        if (bookingTime > 0) {
            std::cout << "Booked at: " << getBookingTimeString() << std::endl;
        }
        if (checkInTime > 0) {
            std::cout << "Checked in at: " << getCheckInTimeString() << std::endl;
        }
        if (checkOutTime > 0) {
            std::cout << "Checked out at: " << getCheckOutTimeString() << std::endl;
        }
    }
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

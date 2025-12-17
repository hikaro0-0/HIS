#include "C:/Users/User/Desktop/HIS/include/StandardRoom.h"
#include <iostream>
#include <iomanip>

StandardRoom::StandardRoom(int number, double price, int beds, bool wifi)
    : Room(number, price, beds, wifi) {
}

double StandardRoom::calculateTotalPrice(int days) const {
    if (days < 1) {
        throw std::invalid_argument("Number of days must be positive");
    }
    return pricePerDay * days;
}

void StandardRoom::displayInfo() const {
    std::cout << "\n=== Standard Room ===" << std::endl;
    std::cout << "Room number: " << roomNumber << std::endl;
    std::cout << "Price per day: $" << std::fixed << std::setprecision(2) << pricePerDay << std::endl;
    std::cout << "Number of beds: " << bedCount << std::endl;
    std::cout << "Number of rooms: " << roomCount << std::endl;
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

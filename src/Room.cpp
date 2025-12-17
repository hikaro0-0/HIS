#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>

Room::Room(int number, double price, int beds, bool wifi)
    : roomNumber(number), pricePerDay(price), status(RoomStatus::AVAILABLE),
    bedCount(beds), hasWiFi(wifi), bookingTime(0), checkInTime(0), checkOutTime(0), plannedCheckInTime(0), bookingDays(0),
    rateType(RateType::FlexibleStart) {
    if (number < 0) {
        throw std::invalid_argument("Room number cannot be negative");
    }
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
    if (beds < 1) {
        throw std::invalid_argument("Number of beds must be at least 1");
    }
}

std::string Room::getStatusString() const {
    switch (status) {
    case RoomStatus::AVAILABLE: return "Available";
    case RoomStatus::BOOKED: return "Booked";
    case RoomStatus::OCCUPIED: return "Occupied";
    default: return "Unknown";
    }
}

void Room::bookRoom(const Guest& guestInfo) {
    if (status != RoomStatus::AVAILABLE) {
        throw std::runtime_error("Room is not available for booking");
    }
    if (!guestInfo.hasName()) {
        throw std::invalid_argument("Guest name cannot be empty");
    }
    status = RoomStatus::BOOKED;
    assignGuest(guestInfo);
    bookingTime = std::time(nullptr);
    std::cout << "Room " << roomNumber << " booked for " << guestInfo.toShortString()
        << " at " << getBookingTimeString() << std::endl;
}

void Room::checkIn() {
    if (status != RoomStatus::BOOKED) {
        throw std::runtime_error("Room must be booked before check-in");
    }
    status = RoomStatus::OCCUPIED;
    checkInTime = std::time(nullptr);
    const std::string guestLabel = getGuestName().empty() ? "Unknown guest" : getGuestName();
    std::cout << "Guest " << guestLabel << " checked in to room " << roomNumber << " at " << getCheckInTimeString() << std::endl;
}

void Room::checkOut() {
    if (status != RoomStatus::OCCUPIED) {
        throw std::runtime_error("Room is not occupied");
    }
    status = RoomStatus::AVAILABLE;
    checkOutTime = std::time(nullptr);
    const std::string guestLabel = getGuestName().empty() ? "Unknown guest" : getGuestName();
    std::cout << "Guest " << guestLabel << " checked out from room " << roomNumber << " at " << getCheckOutTimeString() << std::endl;

    if (checkInTime > 0 && checkOutTime > checkInTime) {
        double hours = std::difftime(checkOutTime, checkInTime) / 3600.0;
        std::cout << "Stay duration: " << std::fixed << std::setprecision(1) << hours << " hours" << std::endl;
    }

    clearGuest();
    bookingTime = 0;
    checkInTime = 0;
}

void Room::cancelBooking() {
    if (status != RoomStatus::BOOKED) {
        throw std::runtime_error("Room is not booked");
    }
    status = RoomStatus::AVAILABLE;
    std::cout << "Booking for room " << roomNumber << " cancelled (was booked at " << getBookingTimeString() << ")" << std::endl;
    clearGuest();
    bookingTime = 0;
}

bool Room::checkAvailability() const {
    return status == RoomStatus::AVAILABLE;
}

void Room::setRoomNumber(int number) {
    if (number < 0) {
        throw std::invalid_argument("Room number cannot be negative");
    }
    roomNumber = number;
}

void Room::setPricePerDay(double price) {
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
    pricePerDay = price;
}

void Room::setBedCount(int beds) {
    if (beds < 1) {
        throw std::invalid_argument("Number of beds must be at least 1");
    }
    bedCount = beds;
}

void Room::setHasWiFi(bool wifi) {
    hasWiFi = wifi;
}

std::string Room::getGuestName() const {
    return guest ? guest->getName() : std::string();
}

void Room::assignGuest(const Guest& guestInfo) {
    guest = guestInfo;
}

void Room::clearGuest() {
    guest.reset();
}

void Room::setGuestName(const std::string& name) {
    if (!guest) {
        guest = Guest(name);
    }
    else {
        guest->setName(name);
    }
}

std::string Room::getBookingTimeString() const {
    return timeToString(bookingTime);
}

std::string Room::getCheckInTimeString() const {
    return timeToString(checkInTime);
}

std::string Room::getCheckOutTimeString() const {
    return timeToString(checkOutTime);
}

std::string Room::timeToString(std::time_t time) {
    if (time == 0) {
        return "Not set";
    }

    std::tm* timeinfo = std::localtime(&time);
    if (!timeinfo) {
        return "Invalid time";
    }

    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Room::operator==(const Room& other) const {
    return roomNumber == other.roomNumber;
}

bool Room::operator!=(const Room& other) const {
    return !(*this == other);
}

bool Room::operator<(const Room& other) const {
    return roomNumber < other.roomNumber;
}

Room& Room::operator=(const Room& other) {
    if (this != &other) {
        roomNumber = other.roomNumber;
        pricePerDay = other.pricePerDay;
        status = other.status;
        guest = other.guest;
        bedCount = other.bedCount;
        hasWiFi = other.hasWiFi;
        bookingTime = other.bookingTime;
        checkInTime = other.checkInTime;
        checkOutTime = other.checkOutTime;
    }
    return *this;
}

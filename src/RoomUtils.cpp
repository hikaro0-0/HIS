#include "C:/Users/User/Desktop/HIS/include/RoomUtils.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

std::string RoomUtils::getStatusString(RoomStatus status) {
    switch (status) {
    case RoomStatus::AVAILABLE:
        return "Доступен";
    case RoomStatus::BOOKED:
        return "Забронирован";
    case RoomStatus::OCCUPIED:
        return "Занят";
    default:
        return "Неизвестно";
    }
}

std::string RoomUtils::getRoomTypeString(const Room* room) {
    if (!room) return "Неизвестно";
    
    int beds = room->getBedCount();
    
    bool hasMiniBar = false;
    bool hasBalcony = false;
    bool hasJacuzzi = false;
    
    if (auto dr = dynamic_cast<const DeluxeRoom*>(room)) {
        hasMiniBar = dr->getHasMiniBar();
        hasBalcony = dr->getHasBalcony();
    }
    
    if (auto sr = dynamic_cast<const SuiteRoom*>(room)) {
        hasMiniBar = sr->getHasMiniBar();
        hasBalcony = sr->getHasBalcony();
        hasJacuzzi = sr->getHasJacuzzi();
    }
    
    if (beds == 4) {
        return "8-местный";
    } else if (beds == 3) {
        return "6-местный";
    } else if (beds == 2 && !hasMiniBar && !hasBalcony) {
        return "4-местный женский";
    } else if (beds == 1 && !hasMiniBar && !hasBalcony) {
        return "Твин Джуниор";
    } else if (beds == 1 && hasMiniBar && hasBalcony && !hasJacuzzi) {
        double price = room->getPricePerDay();
        if (price <= 25) {
            return "Двухместный Эконом";
        } else if (price <= 28) {
            return "Loft Double";
        } else {
            return "Стандартный Дабл";
        }
    } else if (beds == 2 && hasMiniBar && hasBalcony && hasJacuzzi) {
        return "Семейный";
    }
    
    return room->getRoomType();
}

double RoomUtils::calculateBasicTotalPrice(const Room* room, int days) {
    if (days < 1) {
        throw std::invalid_argument("Number of days must be positive");
    }
    return room->getPricePerDay() * days;
}

void RoomUtils::displayRoomCommonInfo(const Room* room, const std::string& roomTypeName,
                                     bool showMiniBar, bool showBalcony, bool showJacuzzi) {
    std::cout << "\n=== " << roomTypeName << " ===" << std::endl;
    std::cout << "Room number: " << room->getRoomNumber() << std::endl;
    std::cout << "Price per day: $" << std::fixed << std::setprecision(2) << room->getPricePerDay() << std::endl;
    std::cout << "Number of beds: " << room->getBedCount() << std::endl;
    
    // Get actual values from room if it's a DeluxeRoom or SuiteRoom
    bool actualMiniBar = false;
    bool actualBalcony = false;
    bool actualJacuzzi = false;
    
    if (auto dr = dynamic_cast<const DeluxeRoom*>(room)) {
        if (showMiniBar) actualMiniBar = dr->getHasMiniBar();
        if (showBalcony) actualBalcony = dr->getHasBalcony();
    }
    if (auto sr = dynamic_cast<const SuiteRoom*>(room)) {
        if (showMiniBar) actualMiniBar = sr->getHasMiniBar();
        if (showBalcony) actualBalcony = sr->getHasBalcony();
        if (showJacuzzi) actualJacuzzi = sr->getHasJacuzzi();
    }
    
    if (showMiniBar) {
        std::cout << "Mini-bar: " << (actualMiniBar ? "Yes" : "No") << std::endl;
    }
    if (showBalcony) {
        std::cout << "Balcony: " << (actualBalcony ? "Yes" : "No") << std::endl;
    }
    if (showJacuzzi) {
        std::cout << "Jacuzzi: " << (actualJacuzzi ? "Yes" : "No") << std::endl;
    }
    
    std::cout << "Status: " << room->getStatusString() << std::endl;
    if (room->getStatus() != RoomStatus::AVAILABLE) {
        if (const Guest* guest = room->getGuest()) {
            std::cout << "Guest: " << guest->toContactString() << std::endl;
        }
        if (room->getBookingTime() > 0) {
            std::cout << "Booked at: " << room->getBookingTimeString() << std::endl;
        }
        if (room->getCheckInTime() > 0) {
            std::cout << "Checked in at: " << room->getCheckInTimeString() << std::endl;
        }
        if (room->getCheckOutTime() > 0) {
            std::cout << "Checked out at: " << room->getCheckOutTimeString() << std::endl;
        }
    }
}


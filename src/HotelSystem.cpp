#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"
#include "C:/Users/User/Desktop/HIS/include/exceptions.h"
#include "C:/Users/User/Desktop/HIS/include/StandardRoom.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include "C:/Users/User/Desktop/HIS/include/RoomConstants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <locale>
#include <memory>
#include <iomanip>
#include <cmath>
#include <optional>
#ifdef _WIN32
#include <Windows.h>
#endif

HotelSystem::HotelSystem(const std::string& name) : hotelName(name), totalRevenue(0.0) {
}

int HotelSystem::bookRoom(const Guest& guest, int days, RoomType roomType, bool wifi, bool miniBar, bool balcony, bool jacuzzi, double price, RateType rateType, std::time_t plannedCheckInTime) {
    int roomNumber = getNextRoomNumber();
    
    double roomPrice = price;
    if (roomPrice <= 0.0) {
        switch (roomType) {
        case RoomType::TwinJunior:
            roomPrice = 20.0;
            break;
        case RoomType::DoubleEconomy:
            roomPrice = 25.0;
            break;
        case RoomType::LoftDouble:
            roomPrice = 28.0;
            break;
        case RoomType::Room4BedFemale:
            roomPrice = 32.0;
            break;
        case RoomType::StandardDouble:
            roomPrice = 35.0;
            break;
        case RoomType::FamilyRoom:
            roomPrice = 40.0;
            break;
        case RoomType::Room6Bed:
            roomPrice = 45.0;
            break;
        case RoomType::Room8Bed:
            roomPrice = 50.0;
            break;
        default:
            roomPrice = 25.0;
            break;
        }
        
        if (miniBar) roomPrice += 4.0;
        if (balcony) roomPrice += 5.0;
        if (jacuzzi) roomPrice += 7.0;
    }
    
    try {
        std::unique_ptr<Room> newRoom;
        int beds = 1;
        bool useMiniBar = miniBar;
        bool useBalcony = balcony;
        bool useJacuzzi = jacuzzi;
        
        switch (roomType) {
        case RoomType::Room8Bed:
            beds = 4;
            newRoom = std::make_unique<StandardRoom>(roomNumber, roomPrice, beds, wifi);
            break;
        case RoomType::Room6Bed:
            beds = 3;
            newRoom = std::make_unique<StandardRoom>(roomNumber, roomPrice, beds, wifi);
            break;
        case RoomType::Room4BedFemale:
            beds = 2;
            newRoom = std::make_unique<StandardRoom>(roomNumber, roomPrice, beds, wifi);
            break;
        case RoomType::TwinJunior:
            beds = 1;
            newRoom = std::make_unique<StandardRoom>(roomNumber, roomPrice, beds, wifi);
            break;
        case RoomType::DoubleEconomy:
        case RoomType::StandardDouble:
        case RoomType::LoftDouble:
            beds = 1;
            newRoom = std::make_unique<DeluxeRoom>(roomNumber, roomPrice, beds, wifi, miniBar, balcony);
            break;
        case RoomType::FamilyRoom:
            beds = 2;
            newRoom = std::make_unique<SuiteRoom>(roomNumber, roomPrice, beds, wifi, miniBar, balcony, jacuzzi);
            break;
        default:
            newRoom = std::make_unique<StandardRoom>(roomNumber, roomPrice, 1, wifi);
            break;
        }
        
        Room* room = newRoom.get();
        rooms.addRoom(std::move(newRoom));
        
        room->setBookingDays(days);
        room->setRateType(rateType);
        room->setPlannedCheckInTime(plannedCheckInTime);
        
        room->bookRoom(guest);
        
        double pricePerDay = room->getPricePerDay();
        totalRevenue += pricePerDay;
        
        return roomNumber;
    }
    catch (const std::exception& e) {
        throw BookingException("Failed to create and book room: " + std::string(e.what()), roomNumber);
    }
}

int HotelSystem::getNextRoomNumber() const {
    if (rooms.empty()) {
        return 1;
    }
    
    int maxNumber = 0;
    for (const auto& room : rooms) {
        if (room && room->getRoomNumber() > maxNumber) {
            maxNumber = room->getRoomNumber();
        }
    }
    return maxNumber + 1;
}

bool HotelSystem::checkIn(int roomNumber) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        throw BookingException("Room not found", roomNumber);
    }
    try {
        std::time_t plannedCheckInTime = room->getPlannedCheckInTime();
        if (plannedCheckInTime > 0) {
            std::time_t currentTime = std::time(nullptr);
            
            if (currentTime < plannedCheckInTime) {
                double hoursUntilCheckIn = std::difftime(plannedCheckInTime, currentTime) / 3600.0;
                throw BookingException(
                    "Ранний заезд невозможен. Заезд разрешен с 14:00 в день заезда. "
                    "До времени заезда осталось: " + std::to_string(static_cast<int>(hoursUntilCheckIn)) + " часов",
                    roomNumber
                );
            }
        }
        
        int days = room->getBookingDays();
        if (days <= 0) {
            days = 1;
        }
        
        double totalPrice = room->calculateTotalPrice(days);
        double pricePerDay = room->getPricePerDay();
        
        totalRevenue += (totalPrice - pricePerDay);
        
        room->checkIn();
        return true;
    }
    catch (const BookingException& e) {
        throw;
    }
    catch (const std::exception& e) {
        throw BookingException("Check-in error: " + std::string(e.what()), roomNumber);
    }
}

bool HotelSystem::checkOut(int roomNumber) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        return false;
    }
    try {
        room->checkOut();
        return true;
    }
    catch (const std::exception& e) {
        throw BookingException("Check-out error: " + std::string(e.what()), roomNumber);
    }
}

bool HotelSystem::cancelBooking(int roomNumber) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        return false;
    }
    try {
        if (room->getRateType() == RateType::FlexibleStart) {
            std::time_t bookingTime = room->getBookingTime();
            
            if (bookingTime > 0) {
                std::time_t currentTime = std::time(nullptr);
                std::time_t checkInTime = room->getCheckInTime();
                
                if (checkInTime > 0) {
                }
                else {
                    double hoursSinceBooking = std::difftime(currentTime, bookingTime) / 3600.0;
                    
                    if (hoursSinceBooking < 24.0) {
                        double prepayment = room->getPricePerDay();
                        totalRevenue -= prepayment;
                        if (totalRevenue < 0.0) {
                            totalRevenue = 0.0;
                        }
                    }
                }
            }
        }
        
        room->cancelBooking();
        return true;
    }
    catch (const std::exception& e) {
        throw BookingException("Cancel booking error: " + std::string(e.what()), roomNumber);
    }
}

Room* HotelSystem::findRoom(int roomNumber) {
    return rooms.findRoom(roomNumber);
}

const Room* HotelSystem::findRoom(int roomNumber) const {
    return rooms.findRoom(roomNumber);
}

bool HotelSystem::removeRoom(int roomNumber) {
    return rooms.removeRoom(roomNumber);
}

void HotelSystem::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);

    try {
        std::locale utf8Locale("en_US.UTF-8");
        file.imbue(utf8Locale);
    }
    catch (const std::exception&) {
        try {
            file.imbue(std::locale(""));
        }
        catch (const std::exception&) {
        }
    }

    if (!file.is_open()) {
        throw FileException("Failed to open file for writing: " + filename);
    }

    file << hotelName << "\n";
    file << totalRevenue << "\n";
    file << rooms.size() << "\n";

    for (const auto& room : rooms) {
        if (room) {
            file << room->getRoomType() << "\n";
            file << room->getRoomNumber() << "\n";
            file << room->getPricePerDay() << "\n";
            file << static_cast<int>(room->getStatus()) << "\n";
            if (room->getStatus() != RoomStatus::AVAILABLE && room->getGuest()) {
                const Guest* guest = room->getGuest();
                file << guest->getName() << "\n";
                file << guest->getPhone() << "\n";
                file << guest->getEmail() << "\n";
            }

            file << room->getBookingTime() << "\n";
            file << room->getCheckInTime() << "\n";
            file << room->getCheckOutTime() << "\n";
            file << room->getPlannedCheckInTime() << "\n";
            file << room->getBookingDays() << "\n";
            file << static_cast<int>(room->getRateType()) << "\n";

            if (auto sr = dynamic_cast<const StandardRoom*>(room.get())) {
                file << sr->getBedCount() << "\n";
                file << (sr->getHasWiFi() ? "1" : "0") << "\n";
            }
            else if (auto dr = dynamic_cast<const DeluxeRoom*>(room.get())) {
                file << dr->getBedCount() << "\n";
                file << (dr->getHasWiFi() ? "1" : "0") << "\n";
                file << (dr->getHasMiniBar() ? "1" : "0") << "\n";
                file << (dr->getHasBalcony() ? "1" : "0") << "\n";
            }
            else if (auto suite = dynamic_cast<const SuiteRoom*>(room.get())) {
                file << suite->getBedCount() << "\n";
                file << (suite->getHasWiFi() ? "1" : "0") << "\n";
                file << (suite->getHasMiniBar() ? "1" : "0") << "\n";
                file << (suite->getHasBalcony() ? "1" : "0") << "\n";
                file << (suite->getHasJacuzzi() ? "1" : "0") << "\n";
            }
        }
    }

    file.flush();
    if (!file.good()) {
        throw FileException("Error writing to file: " + filename);
    }
    file.close();
    std::cout << "Data saved to file: " << filename << " (rooms: " << rooms.size() << ")" << std::endl;
}

void HotelSystem::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    try {
        std::locale utf8Locale("en_US.UTF-8");
        file.imbue(utf8Locale);
    }
    catch (const std::exception&) {
        try {
            file.imbue(std::locale(""));
        }
        catch (const std::exception&) {

        }
    }

    if (!file.is_open()) {
        throw FileException("Failed to open file for reading: " + filename);
    }

    rooms.clear();

    std::string line;
    std::getline(file, line);
    hotelName = line;

    std::getline(file, line);
    if (!line.empty()) {
        try {
            totalRevenue = std::stod(line);
        } catch (const std::exception&) {
            totalRevenue = 0.0;
        }
    } else {
        totalRevenue = 0.0;
    }
    
    std::getline(file, line);
    size_t count = 0;
    if (!line.empty()) {
        try {
            count = std::stoull(line);
        } catch (const std::exception&) {
            count = 0;
        }
    }

    for (size_t i = 0; i < count; ++i) {
        std::string roomType;
        std::getline(file, roomType);

        int number;
        double price;
        int statusInt;
        std::string guestName;
        std::string guestPhone;
        std::string guestEmail;
        std::time_t bookingTime, checkInTime, checkOutTime, plannedCheckInTime;

        std::string line;
        std::getline(file, line);
        number = std::stoi(line);
        
        std::getline(file, line);
        price = std::stod(line);
        
        std::getline(file, line);
        statusInt = std::stoi(line);

        RoomStatus status = static_cast<RoomStatus>(statusInt);
        std::optional<Guest> guestDetails;
        if (status != RoomStatus::AVAILABLE) {
            std::getline(file, guestName);
            std::getline(file, guestPhone);
            std::getline(file, guestEmail);
            guestDetails.emplace(guestName, guestPhone, guestEmail);
        }

        std::getline(file, line);
        bookingTime = std::stoll(line);
        
        std::getline(file, line);
        checkInTime = std::stoll(line);
        
        std::getline(file, line);
        checkOutTime = std::stoll(line);
        
        plannedCheckInTime = 0;
        if (std::getline(file, line) && !line.empty()) {
            try {
                plannedCheckInTime = std::stoll(line);
            } catch (...) {
                plannedCheckInTime = 0;
            }
        }
        
        int bookingDays = 0;
        RateType rateType = RateType::FlexibleStart;
        if (std::getline(file, line) && !line.empty()) {
            try {
                bookingDays = std::stoi(line);
            } catch (...) {
                bookingDays = 0;
            }
        } else {
            bookingDays = 0;
        }
        if (std::getline(file, line) && !line.empty()) {
            try {
                int rateTypeInt = std::stoi(line);
                rateType = static_cast<RateType>(rateTypeInt);
            } catch (...) {
                rateType = RateType::FlexibleStart;
            }
        } else {
            rateType = RateType::FlexibleStart;
        }

        auto finalizeRoom = [&](std::unique_ptr<Room> roomPtr) {
            roomPtr->setStatus(status);
            roomPtr->setBookingTime(bookingTime);
            roomPtr->setCheckInTime(checkInTime);
            roomPtr->setCheckOutTime(checkOutTime);
            roomPtr->setPlannedCheckInTime(plannedCheckInTime);
            roomPtr->setBookingDays(bookingDays);
            roomPtr->setRateType(rateType);
            if (guestDetails) {
                roomPtr->assignGuest(*guestDetails);
            }

            rooms.addRoom(std::move(roomPtr));
            
            if (status == RoomStatus::OCCUPIED) {
                checkIn(number);
            }
        };

        auto readBool = [&](const std::string& fieldName, const std::string& roomTypeName) -> bool {
            if (!std::getline(file, line)) {
                throw FileException("Failed to read " + fieldName + " for " + roomTypeName + " room " + std::to_string(number));
            }
            return (line == "1" || line == "true");
        };

        auto readInt = [&](const std::string& fieldName, const std::string& roomTypeName) -> int {
            if (!std::getline(file, line) || line.empty()) {
                throw FileException("Failed to read " + fieldName + " for " + roomTypeName + " room " + std::to_string(number));
            }
            return std::stoi(line);
        };

        if (roomType == "Standard") {
            int beds = readInt("beds", "Standard");
            bool wifi = readBool("wifi", "Standard");
            
            try {
                auto room = std::make_unique<StandardRoom>(number, price, beds, wifi);
                finalizeRoom(std::move(room));
            }
            catch (const std::exception& e) {
                throw RoomException("Error loading standard room: " + std::string(e.what()), number);
            }
        }
        else if (roomType == "Deluxe") {
            int beds = readInt("beds", "Deluxe");
            bool wifi = readBool("wifi", "Deluxe");
            bool miniBar = readBool("miniBar", "Deluxe");
            bool balcony = readBool("balcony", "Deluxe");
            
            try {
                auto room = std::make_unique<DeluxeRoom>(number, price, beds, wifi, miniBar, balcony);
                finalizeRoom(std::move(room));
            }
            catch (const std::exception& e) {
                throw RoomException("Error loading deluxe room: " + std::string(e.what()), number);
            }
        }
        else if (roomType == "Suite") {
            int beds = readInt("beds", "Suite");
            bool wifi = readBool("wifi", "Suite");
            bool miniBar = readBool("miniBar", "Suite");
            bool balcony = readBool("balcony", "Suite");
            bool jacuzzi = readBool("jacuzzi", "Suite");
            
            try {
                auto room = std::make_unique<SuiteRoom>(number, price, beds, wifi, miniBar, balcony, jacuzzi);
                finalizeRoom(std::move(room));
            }
            catch (const std::exception& e) {
                throw RoomException("Error loading suite room: " + std::string(e.what()), number);
            }
        }
        else {
            throw FileException("Unknown room type: " + roomType + " for room " + std::to_string(number));
        }
    }

    file.close();
    std::cout << "Data loaded from file: " << filename << std::endl;
}

void HotelSystem::clearAllData() {
    rooms.clear();
    totalRevenue = 0.0;
}

size_t HotelSystem::getAvailableRoomsCount() const {
    size_t count = 0;
    for (const auto& room : rooms) {
        if (room && room->checkAvailability()) {
            count++;
        }
    }
    return count;
}

size_t HotelSystem::getOccupiedRoomsCount() const {
    size_t count = 0;
    for (const auto& room : rooms) {
        if (room && room->getIsOccupied()) {
            count++;
        }
    }
    return count;
}

size_t HotelSystem::getBookedRoomsCount() const {
    size_t count = 0;
    for (const auto& room : rooms) {
        if (room && room->getIsBooked()) {
            count++;
        }
    }
    return count;
}

double HotelSystem::getTotalRevenue() const {
    return totalRevenue;
}

double HotelSystem::getFutureRevenue() const {
    double future = 0.0;
    for (const auto& room : rooms) {
        if (room && room->getStatus() == RoomStatus::BOOKED && room->getBookingDays() > 0) {
            future += room->calculateTotalPrice(room->getBookingDays());
        }
    }
    return future;
}

std::vector<const Room*> HotelSystem::findRoomsByStatus(RoomStatus status) const {
    std::vector<const Room*> result;
    for (const auto& room : rooms) {
        if (room && room->getStatus() == status) {
            result.push_back(room.get());
        }
    }
    return result;
}

double HotelSystem::calculateActualStayCost(int roomNumber) const {
    const Room* room = findRoom(roomNumber);
    if (!room || room->getStatus() != RoomStatus::OCCUPIED) {
        throw std::runtime_error("Room is not occupied or not found");
    }

    std::time_t checkInTime = room->getCheckInTime();
    if (checkInTime == 0) {
        throw std::runtime_error("Check-in time not set");
    }

    std::time_t currentTime = std::time(nullptr);
    double hours = std::difftime(currentTime, checkInTime) / 3600.0;
    double days = hours / 24.0;

    if (days < 1.0) {
        days = 1.0;
    }

    return room->calculateTotalPrice(static_cast<int>(std::ceil(days)));
}


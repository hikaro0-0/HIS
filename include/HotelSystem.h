#pragma once
#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/Guest.h"
#include "C:/Users/User/Desktop/HIS/include/StandardRoom.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <ctime>

enum class RoomType {
    Standard = 0,
    Deluxe = 1,
    Suite = 2,
    Room8Bed = 3,
    Room6Bed = 4,
    Room4BedFemale = 5,
    TwinJunior = 6,
    DoubleEconomy = 7,
    StandardDouble = 8,
    LoftDouble = 9,
    FamilyRoom = 10
};

enum class RateType {
    DynamicRate = 0,
    NonRefundable = 1,
    FlexibleStart = 2
};

template<typename T>
class HotelCollection {
private:
    std::vector<std::unique_ptr<T>> rooms;

public:
    void addRoom(std::unique_ptr<T> room) {
        if (!room) {
            throw std::invalid_argument("??????? ???????? ?????? ????????? ?? ?????");
        }
        rooms.push_back(std::move(room));
    }

    bool removeRoom(int roomNumber) {
        auto it = std::remove_if(rooms.begin(), rooms.end(),
            [roomNumber](const std::unique_ptr<T>& room) {
                return room && room->getRoomNumber() == roomNumber;
            });

        if (it != rooms.end()) {
            rooms.erase(it, rooms.end());
            return true;
        }
        return false;
    }

    T* findRoom(int roomNumber) {
        for (auto& room : rooms) {
            if (room && room->getRoomNumber() == roomNumber) {
                return room.get();
            }
        }
        return nullptr;
    }

    const T* findRoom(int roomNumber) const {
        for (const auto& room : rooms) {
            if (room && room->getRoomNumber() == roomNumber) {
                return room.get();
            }
        }
        return nullptr;
    }

    size_t size() const {
        return rooms.size();
    }

    bool empty() const {
        return rooms.empty();
    }

    void clear() {
        rooms.clear();
    }

    auto begin() { return rooms.begin(); }
    auto end() { return rooms.end(); }
    auto begin() const { return rooms.begin(); }
    auto end() const { return rooms.end(); }
};

class HotelSystem {
private:
    HotelCollection<Room> rooms;
    std::string hotelName;
    double totalRevenue;

public:
    HotelSystem(const std::string& name = "?????????");
    ~HotelSystem() = default;

    int bookRoom(const Guest& guest, int days, RoomType roomType = RoomType::Standard, 
                 bool wifi = true, bool miniBar = true, bool balcony = true, bool jacuzzi = true,
                 double price = 0.0, RateType rateType = RateType::FlexibleStart, std::time_t plannedCheckInTime = 0);
    bool checkIn(int roomNumber);
    bool checkOut(int roomNumber);
    bool cancelBooking(int roomNumber);
    Room* findRoom(int roomNumber);
    const Room* findRoom(int roomNumber) const;
    bool removeRoom(int roomNumber);

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void clearAllData();

    size_t getTotalRooms() const { return rooms.size(); }
    size_t getAvailableRoomsCount() const;
    size_t getOccupiedRoomsCount() const;
    size_t getBookedRoomsCount() const;
    double getTotalRevenue() const;
    double getFutureRevenue() const;

    double calculateActualStayCost(int roomNumber) const;
    std::vector<const Room*> findRoomsByStatus(RoomStatus status) const;

    auto begin() const { return rooms.begin(); }
    auto end() const { return rooms.end(); }

    std::string getHotelName() const { return hotelName; }

    HotelCollection<Room>& getRooms() { return rooms; }

    int getNextRoomNumber() const;
};


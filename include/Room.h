#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <optional>
#include "C:/Users/User/Desktop/HIS/include/Guest.h"

enum class RateType;

enum class RoomStatus {
    AVAILABLE,
    BOOKED,
    OCCUPIED
};

class Room {
protected:
    int roomNumber;
    double pricePerDay;
    RoomStatus status;
    std::optional<Guest> guest;
    int bedCount;
    bool hasWiFi;
    std::time_t bookingTime;
    std::time_t checkInTime;
    std::time_t checkOutTime;
    std::time_t plannedCheckInTime;
    int bookingDays;
    RateType rateType;
public:
    Room(int number = 0, double price = 0.0, int beds = 1, bool wifi = true);
    virtual ~Room() = default;

    virtual double calculateTotalPrice(int days) const = 0;
    virtual void displayInfo() const = 0;
    virtual std::string getRoomType() const = 0;
    virtual Room* clone() const = 0;

    virtual void bookRoom(const Guest& guestInfo);
    virtual void checkIn();
    virtual void checkOut();
    virtual void cancelBooking();

    virtual bool checkAvailability() const;
    RoomStatus getStatus() const { return status; }
    std::string getStatusString() const;

    int getRoomNumber() const { return roomNumber; }
    double getPricePerDay() const { return pricePerDay; }
    bool getIsOccupied() const { return status == RoomStatus::OCCUPIED; }
    bool getIsBooked() const { return status == RoomStatus::BOOKED; }
    std::string getGuestName() const;
    const Guest* getGuest() const { return guest ? &*guest : nullptr; }
    Guest* getGuest() { return guest ? &*guest : nullptr; }
    void assignGuest(const Guest& guestInfo);
    void clearGuest();
    int getBedCount() const { return bedCount; }
    bool getHasWiFi() const { return hasWiFi; }
    std::time_t getBookingTime() const { return bookingTime; }
    std::time_t getCheckInTime() const { return checkInTime; }
    std::time_t getCheckOutTime() const { return checkOutTime; }
    std::time_t getPlannedCheckInTime() const { return plannedCheckInTime; }
    int getBookingDays() const { return bookingDays; }

    void setRoomNumber(int number);
    void setPricePerDay(double price);
    void setBedCount(int beds);
    void setHasWiFi(bool wifi);
    void setStatus(RoomStatus newStatus) { status = newStatus; }
    void setBookingTime(std::time_t time) { bookingTime = time; }
    void setCheckInTime(std::time_t time) { checkInTime = time; }
    void setCheckOutTime(std::time_t time) { checkOutTime = time; }
    void setPlannedCheckInTime(std::time_t time) { plannedCheckInTime = time; }
    void setBookingDays(int days) { bookingDays = days; }
    void setRateType(RateType rate) { rateType = rate; }
    RateType getRateType() const { return rateType; }
    void setGuestName(const std::string& name);

    std::string getBookingTimeString() const;
    std::string getCheckInTimeString() const;
    std::string getCheckOutTimeString() const;
    static std::string timeToString(std::time_t time);

    bool operator==(const Room& other) const;
    bool operator!=(const Room& other) const;
    bool operator<(const Room& other) const;
    Room& operator=(const Room& other);
};

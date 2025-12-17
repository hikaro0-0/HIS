#pragma once
#include "C:/Users/User/Desktop/HIS/include/Room.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include <string>

class RoomUtils {
public:
    static std::string getStatusString(RoomStatus status);
    static std::string getRoomTypeString(const Room* room);
    
    static double calculateBasicTotalPrice(const Room* room, int days);
    static void displayRoomCommonInfo(const Room* room, const std::string& roomTypeName, 
                                     bool showMiniBar = false, bool showBalcony = false, bool showJacuzzi = false);
};


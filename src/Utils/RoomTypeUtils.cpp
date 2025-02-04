//
// Created by ahmed Ibrahim on 26-Nov-24.
//

#include "RoomTypeUtils.h"

string RoomTypeUtils::parseRoomTypeToString(RoomType roomType) {
    string roomTypeStr;
    switch (roomType) {
        case RoomType::Living:
            roomTypeStr = "Living";
            break;
        case RoomType::Bedroom:
            roomTypeStr = "Bedroom";
            break;
        case RoomType::Kitchen:
            roomTypeStr = "Kitchen";
            break;
        case RoomType::Dining:
            roomTypeStr = "Dining";
            break;
        case RoomType::Bathroom:
            roomTypeStr = "Bath";
            break;
        case RoomType::GuestBathroom:
            roomTypeStr = "GB";
            break;
        case RoomType::FamilyRoom:
            roomTypeStr = "FamilyRoom";
            break;
        case RoomType::PlayRoom:
            roomTypeStr = "PlayRoom";
            break;
        case RoomType::Office:
            roomTypeStr = "Office";
            break;
        case RoomType::MultiPurpose:
            roomTypeStr = "MultiPurpose";
            break;
        case RoomType::SunRoom:
            roomTypeStr = "SunRoom";
            break;
        case RoomType::HomeTheater:
            roomTypeStr = "HomeTheater";
            break;
        case RoomType::Corridor:
            roomTypeStr = "Corridor";
            break;
        case RoomType::Other:
            roomTypeStr = "Other";
            break;
    }
    return roomTypeStr;
}

string RoomTypeUtils::normalizeRoomTypeString(const string &type)
{
    string normalizedRoomType = type;
    //convert normalizedRoomType to lower case
    transform(normalizedRoomType.begin() , normalizedRoomType.end() , normalizedRoomType.begin() , ::tolower);

    return normalizedRoomType;
}
RoomType RoomTypeUtils::parseStringToRoomType(const string &type) {
    //convert to Lower
    string normalizedRoomType = normalizeRoomTypeString(type);

    regex livingMatch(".*living.*" , regex_constants::icase);
    regex bedroomMatch(".*bedroom.*" , regex_constants::icase);
    regex diningMatch(".*dining.*" , regex_constants::icase);
    regex kitchenMatch(".*kitchen.*" , regex_constants::icase);
    regex guestBathMatch(".*guestBath.*" , regex_constants::icase);
    regex guestBathMatch2(".*gb.*" , regex_constants::icase);
    regex guestBathMatch3(".*gp.*" , regex_constants::icase);
    regex bathroomMatch(".*bath.*" , regex_constants::icase);
    regex familyMatch(".*family.*" , regex_constants::icase);
    regex officeMatch(".*office.*" , regex_constants::icase);
    regex multiPurposeMatch(".*multiPurpose.*" , regex_constants::icase);
    regex sunRoomMatch(".*sunRoom.*" , regex_constants::icase);
    regex theaterMatch(".*theater.*" , regex_constants::icase);
    regex playRoomMatch(".*play.*" , regex_constants::icase);
    regex corridorMatch(".*corridor.*" , regex_constants::icase);

    if(regex_match(normalizedRoomType , livingMatch))
    {
        return RoomType::Living;
    }
    else if(regex_match(normalizedRoomType , corridorMatch))
    {
        return RoomType::Corridor;
    }
    else if(regex_match(normalizedRoomType , bedroomMatch))
    {
        return RoomType::Bedroom;
    }
    else if(regex_match(normalizedRoomType , diningMatch))
    {
        return RoomType::Dining;
    }
    else if(regex_match(normalizedRoomType , kitchenMatch))
    {
        return RoomType::Kitchen;
    }
    else if(regex_match(normalizedRoomType , guestBathMatch) || regex_match(normalizedRoomType , guestBathMatch2) ||regex_match(normalizedRoomType , guestBathMatch3) )
    {
        return RoomType::GuestBathroom;
    }
    else if(regex_match(normalizedRoomType , bathroomMatch))
    {
        return RoomType::Bathroom;
    }
    else if(regex_match(normalizedRoomType , familyMatch))
    {
        return RoomType::FamilyRoom;
    }
    else if(regex_match(normalizedRoomType , officeMatch))
    {
        return RoomType::Office;
    }
    else if(regex_match(normalizedRoomType , multiPurposeMatch))
    {
        return RoomType::MultiPurpose;
    }
    else if(regex_match(normalizedRoomType , sunRoomMatch))
    {
        return RoomType::SunRoom;
    }
    else if(regex_match(normalizedRoomType , theaterMatch))
    {
        return RoomType::HomeTheater;
    }
    else if(regex_match(normalizedRoomType , playRoomMatch))
    {
        return RoomType::PlayRoom;
    }
    else{
        return RoomType::Other;
    }
}

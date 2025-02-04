//
// Created by ahmed Ibrahim on 25-Nov-24.
//
#ifndef DESIGNI_ROOMDEFAULTS_H
#define DESIGNI_ROOMDEFAULTS_H
#include "RoomTypes.h"

const double MaxCorridorWidth  = 12; //Max Width for corridor
const double MaxCorridorHeight  = 1.5; // Max Height for corridor

class RoomDefault{
public:
    static double getDefaultArea(RoomType type)
    {
        switch (type)
        {
            case RoomType::Living:
                return 16;
            case RoomType::Bedroom:
                return 20;
            case RoomType::Kitchen:
                return 9;
            case RoomType::Dining:
                return 12;
            case RoomType::Bathroom:
                return 6;
            case RoomType::GuestBathroom:
                return 4;
            case RoomType::FamilyRoom:
                return 24;
            case RoomType::PlayRoom:
                return 20;
            case RoomType::Office:
                return 20;
            case RoomType::MultiPurpose:
                return 20;
            case RoomType::SunRoom:
                return 16;
            case RoomType::HomeTheater:
                return 20;
            case RoomType::Corridor:
                return 18;
            case RoomType::Other:
                return 20;
        }
    }

    static double getDefaultAspectRatio(RoomType type)
    {
        switch (type) {
            case RoomType::Living:
                return 2;
            case RoomType::Bedroom:
                return 1.5;
            case RoomType::Kitchen:
                return 1.5;
            case RoomType::Dining:
                return 1.5;
            case RoomType::Bathroom:
                return 1.5;
            case RoomType::GuestBathroom:
                return 1.5;
            case RoomType::FamilyRoom:
                return 1.5;
            case RoomType::PlayRoom:
                return 1.5;
            case RoomType::Office:
                return 1.5;
            case RoomType::MultiPurpose:
                return 1.5;
            case RoomType::SunRoom:
                return 1.5;
            case RoomType::HomeTheater:
                return 1.5;
            case RoomType::Corridor:
                return 8;
            case RoomType::Other:
                return 1.5;
        }
    }
};



#endif //DESIGNI_ROOMDEFAULTS_H

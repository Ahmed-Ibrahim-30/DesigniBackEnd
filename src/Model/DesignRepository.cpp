//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#include "DesignRepository.h"
#include "algorithm"
#include <bits/stdc++.h>

void DesignRepository::addDesign(const Design &design)
{
    int designIndex = (int)designs.size();

    int spaces = design.getSpacesCount();
    int rooms = design.getRoomsCount();
    int bedrooms = design.getBedroomsCount();
    double area = design.getArea();
    Design newDesign = design;

    newDesign.addWindows();
    newDesign.addDoors();
    newDesign.addOpenings();

    designs.push_back(newDesign);

    mapDesignIndexToSpaces[spaces] .push_back( designIndex);
    mapDesignIndexToBedrooms[bedrooms] .push_back( designIndex);
    mapDesignIndexToRooms[rooms] .push_back( designIndex);
    mapDesignIndexToArea[area].push_back( designIndex);
}

void DesignRepository::scaleDesigns(double scaleUnit)
{
    for(auto &design : designs)
    {
        design.scaleDesign(scaleUnit);
    }
}

Design DesignRepository::getDesignBySpaces(int numberOfSpaces)
{

    if(!mapDesignIndexToSpaces.count(numberOfSpaces))
    {
        LOG(LogLevel::Error , "NOT FOUND Design with Spaces = "+ to_string(numberOfSpaces));

        for (int i = numberOfSpaces; i <= 19; ++i) {
            if(!mapDesignIndexToSpaces.count(i) || mapDesignIndexToSpaces[i].empty()) continue;

            return designs[mapDesignIndexToSpaces[i][0]];
        }
    }

    int curIndex = mapSpacesToSolIndex[numberOfSpaces];
    int size = mapDesignIndexToSpaces[numberOfSpaces].size();
    (curIndex += 1 ) %= size ;
    mapSpacesToSolIndex[numberOfSpaces] = curIndex;

    return designs[mapDesignIndexToSpaces[numberOfSpaces][curIndex]];
}

Design DesignRepository::getDesignByBedrooms(int numberOfBedrooms)
{
    if(!mapDesignIndexToBedrooms.count(numberOfBedrooms))
    {
        LOG(LogLevel::Error , "NOT FOUND Design with Bedrooms = "+ to_string(numberOfBedrooms));

        return {};
    }

    int curIndex = mapBedroomsToSolIndex[numberOfBedrooms];
    int size = mapDesignIndexToBedrooms[numberOfBedrooms].size();
    (curIndex += 1 ) %= size ;
    mapBedroomsToSolIndex[numberOfBedrooms] = curIndex;

    return designs[mapDesignIndexToBedrooms[numberOfBedrooms][curIndex]];
}

Design DesignRepository::getDesignByRooms(int numberOfRooms)
{
    if(!mapDesignIndexToRooms.count(numberOfRooms))
    {
        LOG(LogLevel::Error , "NOT FOUND Design with Rooms = "+ to_string(numberOfRooms));

        return {};
    }

    int curIndex = mapRoomsToSolIndex[numberOfRooms];
    int size = mapDesignIndexToRooms[numberOfRooms].size();
    (curIndex += 1 ) %= size ;
    mapRoomsToSolIndex[numberOfRooms] = curIndex;

    return designs[mapDesignIndexToRooms[numberOfRooms][curIndex]];
}

Design DesignRepository::getDesignByArea(double area)
{

    auto curArea = mapDesignIndexToArea.lower_bound(area);

    if(curArea == mapDesignIndexToArea.end())
    {
        LOG(LogLevel::Error , "No Found Design With Area = "+ to_string(area));
        curArea--;
    }

    double newArea = curArea->first;

    int curIndex = mapAreaToSolIndex[newArea];
    int size = mapDesignIndexToArea[newArea].size();
    (curIndex += 1 ) %= size ;
    mapAreaToSolIndex[newArea] = curIndex;

    return designs[mapDesignIndexToArea[newArea][curIndex]];
}

const vector<Design> &DesignRepository::getDesigns() const {
    return designs;
}





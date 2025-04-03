//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#include "BuildingDesigner.h"
#include "queue"
BuildingDesigner::BuildingDesigner(const vector<string> &zone1Ids,
                                   const vector<string> &zone2Ids,
                                   const vector<pair<string, string>> &_connections,
                                   map<string, pair<double, double>> &_dimensionsLimits
                                   ){
    dimensionsLimits = _dimensionsLimits;

    map<string , set<string>> roomMapConnections;
    for(auto &room :_connections )
    {
        roomMapConnections[room.first].insert(room.second);
        roomMapConnections[room.second].insert(room.first);
    }
    connections = roomMapConnections;
    map<string , vector<string>> roomVecConnections;
    for(auto &room :roomMapConnections )
    {
        for(auto &conn : room.second)
        {
            roomVecConnections[room.first].push_back(conn);
        }
    }

    for(const string &roomId : zone1Ids)
    {
        RoomEntity roomEntity(roomId , roomVecConnections[roomId] , dimensionsLimits[roomId]);
        zone1.push_back(roomEntity);
    }

    for(const string &roomId : zone2Ids)
    {
        RoomEntity roomEntity(roomId , roomVecConnections[roomId] , dimensionsLimits[roomId]);
        zone2.push_back(roomEntity);
    }
}

Design BuildingDesigner::generateDesign() {
    zone1 = sortZone(zone1);
    zone2 = sortZone(zone2);

    reverse(zone1.begin(), zone1.end());

    Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
    vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);

    rooms[rooms.size()-1].addDoor(0,0,0,0);
    rooms[rooms.size()-1].addWindow(0,0,0,0);
    rooms[rooms.size()-1].addOpening(0,0,0,0);

    Design design("" , rooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);

    return design;
}

vector<RoomEntity> BuildingDesigner::sortZone(vector<RoomEntity> &zone)
{
    if (zone.size()<2)return zone;

    vector<RoomEntity> newZone;
    set<string> zoneIds;
    for(auto &room : zone)zoneIds.insert(room.getRoomId());

    map<string  ,int> mapRoomIndex;
    for (int i = 0; i <zone.size() ; ++i)
    {
        mapRoomIndex[zone[i].getRoomId()] = i;
    }

    RoomEntity startRoom = zone[0];

    for(auto &room : zoneIds)
    {
        bool flag = false;
        for(auto &conn : connections[room])
        {
            if (!zoneIds.count(conn))
            {
                startRoom = zone[mapRoomIndex[room]];
                flag= true;
                break;
            }
        }
        if (flag) break;
    }

    vector<RoomEntity> roomSorted; set<string> visited;
    sortZoneRoomsDFS(startRoom , roomSorted , visited , zone , mapRoomIndex);

    return roomSorted;
}

void BuildingDesigner::sortZoneRoomsDFS(RoomEntity &curRoom , vector<RoomEntity> &ans , set<string> &visited , vector<RoomEntity> &zone , map<string  ,int> &mapRoomIndex)
{
    ans.push_back(curRoom);
    const string& id = curRoom.getRoomId();
    visited.insert(id);
    bool found = false;
    for(auto &connRoom : connections[id])
    {
        if (!visited.count(connRoom))
        {
            found = true;
            RoomEntity newRoom = zone[mapRoomIndex[connRoom]];
            sortZoneRoomsDFS(newRoom , ans , visited , zone , mapRoomIndex);
        }
    }
    if (!found)
    {
        for(auto &room : zone)
        {
            if (!visited.count(room.getRoomId()))
            {
                RoomEntity newRoom = zone[mapRoomIndex[room.getRoomId()]];
                sortZoneRoomsDFS(newRoom , ans , visited , zone , mapRoomIndex);
                break;
            }
        }
    }
}

vector<Room> BuildingDesigner::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
{
    vector<Room> ans;
    double width = mainRoom.getWidth();
    double height = mainRoom.getHeight();

    int n = (int)roomE.size();

    int topRoomsSize = (n-1)/2;
    int bottomRoomsSize = n/2;

    vector<double>topRoomsLimitDiff; double sumTopRoomsLimitDiff = 0 , sumWidths = 0;
    for (int i = 0; i < topRoomsSize; ++i)
    {
        topRoomsLimitDiff.push_back(roomE[i].getDimensionLimit().second - roomE[i].getDimensionLimit().first);
        sumTopRoomsLimitDiff+=topRoomsLimitDiff.back();

        sumWidths+=roomE[i].getDimensionLimit().first;
    }

    double newCorridorWidth = corridorWidth;
    double curX = 0.0;

    for (int i = 0; i < topRoomsSize; ++i)
    {
        double curWidth = roomE[i].getDimensionLimit().first;
        double curDiff = topRoomsLimitDiff[i] + 1;
        double increaseRatio = ((width - sumWidths) * curDiff) / sumTopRoomsLimitDiff;
        if (width > sumWidths && increaseRatio< 5)curWidth += increaseRatio;
        newCorridorWidth+=curWidth;

        string id = roomE[i].getRoomId();
        Room newRoom(id , curX , -5 , curX + curWidth , 0);
        curX = newRoom.getX2();
        ans.push_back(newRoom);
    }

    Room corridor("" , 0 , 0 , newCorridorWidth , 2);
    ans.push_back(corridor);

    Room rightRoom (roomE[topRoomsSize+1].getRoomId() , corridor.getX2() , corridor.getY2() - roomE[topRoomsSize+1].getDimensionLimit().second , corridor.getX2() + roomE[topRoomsSize+1].getDimensionLimit().second , corridor.getY2());

    ans.push_back(rightRoom);
    corridorWidth = newCorridorWidth;

    curX = 0.0;

    for (int i = 0; i < topRoomsSize; ++i)
    {
        double curWidth = roomE[i].getDimensionLimit().first;
        double curDiff = topRoomsLimitDiff[i] + 1;
        double increaseRatio = ((width - sumWidths) * curDiff) / sumTopRoomsLimitDiff;
        curWidth += increaseRatio;

        string id = roomE[i].getRoomId();
        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + 5);
        curX = newRoom.getX2();
        ans.push_back(newRoom);
    }

    return ans;
}

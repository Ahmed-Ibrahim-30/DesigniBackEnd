//
// Created by ahmed Ibrahim on 16-Apr-25.
//

#include "BuildingSecondFloorDesign.h"
BuildingSecondFloorDesign::BuildingSecondFloorDesign(const vector<string> &zone1Ids, const vector<string> &zone2Ids, const vector<pair<string, string>> &_connections, map<string, double> &_roomsAreas)
{
    for(auto &room : _roomsAreas)
    {
        double area = room.second;
        string id = room.first;
        double minRange = sqrt(area / dimensionDiffFactor);
        double maxRange = area / minRange;
        dimensionsLimits[id] = {minRange , maxRange};
    }

    roomsArea = _roomsAreas;

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

BuildingSecondFloorDesign::BuildingSecondFloorDesign(const vector<string> &zone1Ids,
                                   const vector<string> &zone2Ids,
                                   const vector<pair<string, string>> &_connections,
                                   map<string, pair<double, double>> &_dimensionsLimits
){
    dimensionsLimits = _dimensionsLimits;

    for(auto &room : dimensionsLimits)
    {
        double a = room.second.first;
        double b = room.second.second;
        roomsArea[room.first] = a*b;
    }

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

vector <Design> BuildingSecondFloorDesign::generateDesign(Design &firstFloorDesigns)
{
    vector<Design>secondFloorDesigns;
    vector<Room> rooms = firstFloorDesigns.getRooms();

    //1- remove guestBath Room
    Room guestBath , living;
    int guestIndex = -1;

    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getRoomType() == RoomType::GuestBathroom)
        {
            guestBath = rooms[i];
            guestIndex = i;
        }
        if (rooms[i].getRoomType() == RoomType::Living)
        {
            living = rooms[i];
        }
    }
    auto it = rooms.begin();
    if (~guestIndex)rooms.erase(it + guestIndex);

    for (int i = 0; i < rooms.size(); ++i)
    {
        if (rooms[i].getRoomType() == RoomType::Living)continue;

        if (guestBath.getY1() == rooms[i].getY2() && ((guestBath.getX1() == living.getX2() && rooms[i].getX1() == living.getX2()) ||
                (guestBath.getX2() == living.getX1() && rooms[i].getX2() == living.getX1())))
        {
            rooms[i].setY2(guestBath.getY2() );
            break;
        }
        else if (guestBath.getY2() == rooms[i].getY1() && ((guestBath.getX1() == living.getX2() && rooms[i].getX1() == living.getX2()) ||
                                                           (guestBath.getX2() == living.getX1() && rooms[i].getX2() == living.getX1())))
        {
            rooms[i].setY1(guestBath.getY1());
            break;
        }
        else if (guestBath.getX1() == rooms[i].getX2() && ((guestBath.getY1() == living.getY2() && rooms[i].getY1() == living.getY2()) ||
                                                           (guestBath.getY2() == living.getY1() && rooms[i].getY2() == living.getY1())))
        {
            rooms[i].setX2(guestBath.getX2());
            break;
        }
        else if (guestBath.getX2() == rooms[i].getX1()&& ((guestBath.getY1() == living.getY2() && rooms[i].getY1() == living.getY2()) ||
                                                          (guestBath.getY2() == living.getY1() && rooms[i].getY2() == living.getY1())))
        {
            rooms[i].setX1(guestBath.getX1());
            break;
        }
    }

    Design newDesign("",rooms);
    secondFloorDesigns.push_back(newDesign);
    return secondFloorDesigns;
}

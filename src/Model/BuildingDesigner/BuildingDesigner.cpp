//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#include "BuildingDesigner.h"
#include "queue"

BuildingDesigner::BuildingDesigner(const vector<string> &zone1Ids, const vector<string> &zone2Ids, const vector<pair<string, string>> &_connections, map<string, double> &_roomsAreas)
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

BuildingDesigner::BuildingDesigner(const vector<string> &zone1Ids,
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

Design BuildingDesigner::generateDesign()
{

//    cout<<"ZONE1 = "<<"\n";
//    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
//    cout<<"\n";
//
//    cout<<"ZONE2 = "<<"\n";
//    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
//    cout<<"\n";

    zone1 = sortZone(zone1);
    zone2 = sortZone(zone2);

    reverse(zone1.begin(), zone1.end());

//    cout<<"ZONE1 = "<<"\n";
//    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
//    cout<<"\n";
//
//    cout<<"ZONE2 = "<<"\n";
//    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
//    cout<<"\n";

    Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
    vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);
    Room Living ("Living" , -5 , -2.5 , 0 , 4.5);
    rooms.push_back(Living);

    vector<Room> newRooms = generateLivingLayout(zone2 , Living);
    rooms.insert(rooms.end() , newRooms.begin() , newRooms.end());

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
    for (int i = 0; i < zone.size() ; ++i)
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

    vector<RoomEntity> roomSorted;
    visited.clear();
//    cout<<"StartRoom = "<<startRoom.getRoomId()<<"\n";
    sortZoneRoomsDFS(startRoom , roomSorted ,zoneIds, zone , mapRoomIndex);

    return roomSorted;
}

void BuildingDesigner::sortZoneRoomsDFS(RoomEntity &curRoom , vector<RoomEntity> &ans ,set<string>&zonesIds, vector<RoomEntity> &zone , map<string  ,int> &mapRoomIndex)
{
    ans.push_back(curRoom);
    const string& id = curRoom.getRoomId();
    zonesIds.erase(id);
    bool found = false;
    for(auto &connRoom : connections[id])
    {
        if (zonesIds.count(connRoom))
        {
            found = true;
            RoomEntity newRoom = zone[mapRoomIndex[connRoom]];
            sortZoneRoomsDFS(newRoom , ans ,zonesIds, zone , mapRoomIndex);
        }
    }
    if (!found)
    {
        for(auto &room : zone)
        {
            if (zonesIds.count(room.getRoomId()))
            {
                RoomEntity newRoom = room;
                sortZoneRoomsDFS(newRoom , ans ,zonesIds, zone , mapRoomIndex);
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

    int n = (int)roomE.size() , index = -1;

    vector<vector<double>> values;

    for(auto &room : roomE)
    {
        double firstL = room.getDimensionLimit().first;
        double secL = room.getDimensionLimit().second;

        double counter = (secL -firstL) / 5;

        vector<double> val;
        for (double i = firstL; i <= secL; i+= counter)
        {
            val.push_back(i);
        }
        values.push_back(val);
    }

    double newCorridorWidth = 0.0;
    double curX = 0.0;
    vector<vector<double>>tempV;

    Room lastTopRoom;

    for (int i = 0; i < n; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width , tempV);
        if (res.first >= width)
        {
            index = i + 1;
            vector<double> out = res.second;
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[j].getRoomId();
                double curWidth = out[j];
                double curHeight = roomsArea[id] / curWidth;
                newCorridorWidth+=curWidth;
                Room newRoom(id , curX , -curHeight , curX + curWidth , 0);
                lastTopRoom = newRoom;
                curX = newRoom.getX2();
                ans.push_back(newRoom);
            }
            break;
        }
    }

    cout<<"newCorridorWidth = "<<newCorridorWidth<<"\n";

    Room corridor("" , 0 , 0 , newCorridorWidth , height);
    corridor.addDoor(0 , 0.4 , 0 , 1.6);
    corridor.addDoor(newCorridorWidth , 0.4 , newCorridorWidth , 1.6);

    ans.push_back(corridor);

    string rightRoomId = roomE[index].getRoomId();
    double minLimit = dimensionsLimits[rightRoomId].first;
    double maxLimit = dimensionsLimits[rightRoomId].second;

    double lastTopRoomHeight = lastTopRoom.getHeight();

    double preferHeight = lastTopRoomHeight + height;

    if (preferHeight < minLimit) preferHeight = minLimit;
    else if (preferHeight > maxLimit) preferHeight = maxLimit;

    double rightRoomWidth = roomsArea[rightRoomId] / preferHeight;

    Room rightRoom (roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - lastTopRoomHeight , corridor.getX2() + rightRoomWidth , corridor.getY2());


    ans.push_back(rightRoom);
    corridorWidth = newCorridorWidth;
    width = newCorridorWidth;

    curX = 0.0;
    tempV.clear();

    for (int i = index+1; i < n; ++i)
    {
        tempV.push_back(values[i]);
    }

    pair<double , vector<double>> res = findClosestSum(width , tempV);
    vector<double> out = res.second;
    index = n-1;
    for (int j = 0; j < out.size(); ++j)
    {
        string id = roomE[j].getRoomId();
        double curWidth = out[j];
        double curHeight = roomsArea[id] / curWidth;
        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
        curX = newRoom.getX2();
        ans.push_back(newRoom);
        index--;
    }
    return ans;
}


//vector<Room> BuildingDesigner::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
//{
//    vector<Room> ans;
//    double width = mainRoom.getWidth();
//    double height = mainRoom.getHeight();
//
//    int n = (int)roomE.size() , index = -1;
//
//    map<string , double> appropriateValue;
//
//    for(auto &room : roomE)
//    {
//        appropriateValue[room.getRoomId()] = room.getDimensionLimit().first;
//    }
//
//    double newCorridorWidth = 0.0;
//    double curX = 0.0;
//
//    for (int i = 0; i < n; ++i)
//    {
//        double curWidth = roomE[i].getDimensionLimit().first;
//
//        if (newCorridorWidth >= corridorWidth)
//        {
//            index = i;
//            break;
//        }
//
//        newCorridorWidth+=curWidth;
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , -5 , curX + curWidth , 0);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//    cout<<"newCorridorWidth = "<<newCorridorWidth<<"\n";
//
//    Room corridor("" , 0 , 0 , newCorridorWidth , height);
//    corridor.addDoor(0 , 0.4 , 0 , 1.6);
//    corridor.addDoor(newCorridorWidth , 0.4 , newCorridorWidth , 1.6);
//
//    ans.push_back(corridor);
//
//    Room rightRoom (roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - roomE[index].getDimensionLimit().second , corridor.getX2() + roomE[index].getDimensionLimit().second , corridor.getY2());
//
//    ans.push_back(rightRoom);
//    corridorWidth = newCorridorWidth;
//
//    curX = 0.0;
//
//    double remaining = 0.0; newCorridorWidth = 0.0;
//
//    for (int i = index+1; i < n; ++i)
//    {
//        double curWidth = roomE[i].getDimensionLimit().first;
//        newCorridorWidth+=curWidth;
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + 5);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//    remaining = corridorWidth - newCorridorWidth;
//
//    vector<double> remLimits; double sumRem = 0.0;
//    for(auto &room : roomE)
//    {
//        remLimits.push_back(room.getDimensionLimit().second - room.getDimensionLimit().first);
//        sumRem += remLimits.back();
//    }
//
//    for (int i = 0; i < n; ++i)
//    {
//        double diffLimit = remLimits[i];
//        double limit = min(diffLimit , (remaining * diffLimit) / sumRem);
//        appropriateValue[roomE[i].getRoomId()] = limit + roomE[i].getDimensionLimit().first;
//    }
//
//    ans.clear();
//
//    index = -1;
//
//    newCorridorWidth = 0.0;
//
//    curX = 0.0;
//
//    for (int i = 0; i < n; ++i)
//    {
//        double curWidth = appropriateValue[roomE[i].getRoomId()];
//
//        if (newCorridorWidth >= corridorWidth)
//        {
//            index = i;
//            break;
//        }
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , -5 , curX + curWidth , 0);
//        newCorridorWidth = curX + curWidth;
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//    cout<<"newCorridorWidth = "<<newCorridorWidth<<"\n";
//
//    corridor = Room("" , 0 , 0 , newCorridorWidth , height);
//    corridor.addDoor(0 , 0.5 , 0 , 1.5);
//    corridor.addDoor(newCorridorWidth , 0.5 , newCorridorWidth , 1.5);
//
//    ans.push_back(corridor);
//
//    rightRoom  = Room(roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - roomE[index].getDimensionLimit().second , corridor.getX2() + roomE[index].getDimensionLimit().second , corridor.getY2());
//
//    ans.push_back(rightRoom);
//    corridorWidth = newCorridorWidth;
//
//    curX = 0.0;
//
//    for (int i = index+1; i < n; ++i)
//    {
//        double curWidth = appropriateValue[roomE[i].getRoomId()];
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + 5);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//
//
//    return ans;
//}


//vector<Room> BuildingDesigner::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
//{
//    vector<Room> ans;
//    double width = mainRoom.getWidth();
//    double height = mainRoom.getHeight();
//
//    int n = (int)roomE.size();
//
//    int topRoomsSize = (n-1)/2;
//    int bottomRoomsSize = n/2;
//
//    vector<double>topRoomsLimitDiff; double sumTopRoomsLimitDiff = 0 , sumWidths = 0;
//    for (int i = 0; i < topRoomsSize; ++i)
//    {
//        topRoomsLimitDiff.push_back(roomE[i].getDimensionLimit().second - roomE[i].getDimensionLimit().first);
//        sumTopRoomsLimitDiff+=topRoomsLimitDiff.back();
//
//        sumWidths += roomE[i].getDimensionLimit().first;
//    }
//
//    double newCorridorWidth = 0.0;
//    double curX = 0.0;
//
//    for (int i = 0; i < topRoomsSize; ++i)
//    {
//        double curWidth = roomE[i].getDimensionLimit().first;
//        double curDiff = topRoomsLimitDiff[i];
//        double increaseRatio = ((width - sumWidths) * curDiff) / sumTopRoomsLimitDiff;
//        curWidth += increaseRatio;
//        newCorridorWidth+=curWidth;
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , -5 , curX + curWidth , 0);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//    cout<<"newCorridorWidth = "<<newCorridorWidth<<"\n";
//
//    Room corridor("" , 0 , 0 , newCorridorWidth , height);
//    corridor.addDoor(0 , 0.4 , 0 , 1.6);
//    corridor.addDoor(newCorridorWidth , 0.4 , newCorridorWidth , 1.6);
//
//    ans.push_back(corridor);
//
//    Room rightRoom (roomE[topRoomsSize].getRoomId() , corridor.getX2() , corridor.getY2() - roomE[topRoomsSize].getDimensionLimit().second , corridor.getX2() + roomE[topRoomsSize+1].getDimensionLimit().second , corridor.getY2());
//
//    ans.push_back(rightRoom);
//    corridorWidth = newCorridorWidth;
//
//    curX = 0.0;
//
//    vector<double>bottomRoomsLimitDiff; double sumBottomRoomsLimitDiff = 0 ;
//    sumWidths = 0;
//    for (int i = roomE.size()-1; i >= roomE.size() - bottomRoomsSize; --i)
//    {
//        bottomRoomsLimitDiff.push_back(roomE[i].getDimensionLimit().second - roomE[i].getDimensionLimit().first);
//        sumBottomRoomsLimitDiff+=bottomRoomsLimitDiff.back();
//
//        sumWidths += roomE[i].getDimensionLimit().first;
//    }
//
//    for (int i = roomE.size()-1; i >= roomE.size() - bottomRoomsSize; --i)
//    {
//        double curWidth = roomE[i].getDimensionLimit().first;
//        double curDiff = bottomRoomsLimitDiff[roomE.size() - 1 - i] ;
//        double diff = (newCorridorWidth - sumWidths);
//        double increaseRatio = (diff * curDiff) / sumBottomRoomsLimitDiff;
//        curWidth += increaseRatio;
//
//        string id = roomE[i].getRoomId();
//        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + 5);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//    }
//
//    return ans;
//}


vector<Room> BuildingDesigner::generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom)
{
    vector<Room> ans;
    double width = mainRoom.getWidth();
    double height = mainRoom.getHeight();

    int n = (int)roomE.size();

    double curX = mainRoom.getX2();
    int index = 0;

    for (int i = index; i < roomE.size(); ++i)
    {
        double curWidth = roomE[i].getDimensionLimit().first;

        string id = roomE[i].getRoomId();
        Room newRoom(id , curX - curWidth , mainRoom.getY2() , curX , mainRoom.getY2() + 5);
        curX = newRoom.getX1();
        ans.push_back(newRoom);

        if (curX - mainRoom.getX1() < 1)
        {
            index = i+1;
            break;
        }
    }

    double curY = mainRoom.getY2();

    for (int i = index; i < roomE.size(); ++i)
    {
        double curHeight = roomE[i].getDimensionLimit().first;

        string id = roomE[i].getRoomId();
        Room newRoom(id , mainRoom.getX1() - 5 , curY - curHeight , mainRoom.getX1() , curY);
        curY = newRoom.getY1();
        ans.push_back(newRoom);

        if (curY - mainRoom.getY1() < 1)
        {
            index = i + 1;
            break;
        }
    }

    curX = mainRoom.getX1();

    for (int i = index; i < n; ++i)
    {
        double curWidth = roomE[i].getDimensionLimit().first;

        string id = roomE[i].getRoomId();
        Room newRoom(id , curX , mainRoom.getY1() -5, curX+curWidth, mainRoom.getY1() );
        curX = newRoom.getX2();
        ans.push_back(newRoom);

        if (mainRoom.getX2() - curX  < 1)
        {
            break;
        }
    }

    return ans;
}

pair<double , vector<double>> BuildingDesigner::findClosestSum(double x, const vector<vector<double>> &values)
{
    vector<double> output;

    set<pair<double, vector<double>>> dp;
    dp.emplace(0 , vector<double>());

    for(auto &value : values)
    {
        set<pair<double,vector<double>>> newDp;
        for(auto &sum : dp)
        {
            vector<double> curArr = sum.second;

            for(auto &v : value)
            {
                curArr.push_back(v);
                newDp.emplace(v + sum.first , curArr);
                curArr.pop_back();
            }
        }
        dp = newDp;
    }

    auto it = dp.upper_bound({x , vector<double>()});

    if (it == dp.end()) it --;

    return *it;
}



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

    cout<<"ZONE1 = "<<"\n";
    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    cout<<"ZONE2 = "<<"\n";
    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
    vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);
    Room Living ("Living" , -5 , -2.5 , 0 , 4.5);
    rooms.push_back(Living);

    vector<Room> newRooms = generateLivingLayout(zone2 , Living);
    rooms.insert(rooms.end() , newRooms.begin() , newRooms.end());

    Design design("" , rooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);

//    vector<Room> newRooms;
//    Design design("" , newRooms , 1 , 0 , 28 , 0 ,36);

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


//vector<Room> BuildingDesigner::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
//{
//    vector<Room> ans;
//    double width = mainRoom.getWidth();
//    double height = mainRoom.getHeight();
//
//    int n = (int)roomE.size() , index = -1;
//
//    vector<vector<double>> values;
//    vector<double> minLimits;
//    double minLimitsSum = 0.0;
//
//    for(auto &room : roomE)
//    {
//        double firstL = room.getDimensionLimit().first;
//        double secL = room.getDimensionLimit().second;
//        minLimitsSum += firstL;
//        minLimits.push_back(firstL);
//
//        double counter = (secL -firstL) / 5;
//
//        vector<double> val;
//        for (double i = firstL; i <= secL; i+= counter)
//        {
//            val.push_back(i);
//        }
//        values.push_back(val);
//    }
//
//    double curTopLimits = 0.0 , diffLimits = 0.0;
//    for (int i = 0; i < n - 2; ++i)
//    {
//        double firstL = roomE[i].getDimensionLimit().first;
//        double secL = roomE[i].getDimensionLimit().second;
//
//        diffLimits+= (secL - firstL);
//        double firstL2 = roomE[i+1].getDimensionLimit().first;
//        curTopLimits += firstL;
//        double remaining = (minLimitsSum - curTopLimits - firstL2);
//
//        remaining -= roomE.back().getDimensionLimit().first;
//        remaining += 1.5;
//
//        if (curTopLimits >= remaining)
//        {
//            width = curTopLimits;
//            break;
//        }
//    }
//
//    double newCorridorWidth = 0.0;
//    double curX = 0.0;
//    vector<vector<double>>tempV;
//
//    Room lastTopRoom;
//
//    for (int i = 0; i < n; ++i)
//    {
//        string id = roomE[i].getRoomId();
//        double curWidth = roomE[i].getDimensionLimit().first;
//        double curHeight = roomsArea[id] / curWidth;
//        Room newRoom(id , curX , -curHeight , curX + curWidth , 0);
//        lastTopRoom = newRoom;
//        curX = newRoom.getX2();
//        newCorridorWidth =curX;
//        ans.push_back(newRoom);
//
//        if (newCorridorWidth >= width)
//        {
//            index = i + 1;
//            break;
//        }
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
//    string rightRoomId = roomE[index].getRoomId();
//    double minLimit = dimensionsLimits[rightRoomId].first;
//    double maxLimit = dimensionsLimits[rightRoomId].second;
//
//    double lastTopRoomHeight = lastTopRoom.getHeight();
//
//    double preferHeight = lastTopRoomHeight + height;
//
//    if (preferHeight < minLimit) preferHeight = minLimit;
//    else if (preferHeight > maxLimit) preferHeight = maxLimit;
//
//    double rightRoomWidth = roomsArea[rightRoomId] / preferHeight;
//
//    Room rightRoom (roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - preferHeight , corridor.getX2() + rightRoomWidth , corridor.getY2());
//
//
//    ans.push_back(rightRoom);
//    corridorWidth = newCorridorWidth;
//    width = newCorridorWidth;
//
////    curX = 0.0;
////
////    for (int i = index+1; i < n; ++i)
////    {
////        string id = roomE[i].getRoomId();
////        double curWidth = roomE[i].getDimensionLimit().first;
////        double curHeight = roomsArea[id] / curWidth;
////
////        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
////        curX = newRoom.getX2();
////        ans.push_back(newRoom);
////    }
//
//
//    curX = 0.0;
//    tempV.clear();
//
//    vector<RoomEntity> newRoomsE;
//
//    for (int i = index+1; i < n; ++i)
//    {
//        newRoomsE.push_back(roomE[i]);
//        cout<<"id = "<<roomE[i].getRoomId()<<" ---> ";
//        tempV.push_back(values[i]);
//        for(auto &value : values[i])
//        {
//            cout<<"TEMP = "<<value<<" ";
//        }
//
//        cout<<"\n";
//    }
//
//
//    pair<double , vector<double>> res = findClosestSum(width , tempV , newRoomsE);
//    vector<double> out = res.second;
//    std::reverse(out.begin(), out.end());
//    index = n-1;
//    for (int j = 0; j < out.size(); ++j)
//    {
//        string id = roomE[index].getRoomId();
//        double curWidth = out[j];
//        double curHeight = roomsArea[id] / curWidth;
//
//        cout<<"ID = "<<id <<" "<<curWidth <<" "<<curHeight<<"\n";
//
//        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
//        curX = newRoom.getX2();
//        ans.push_back(newRoom);
//        index--;
//    }
//    return ans;
//}


vector<Room> BuildingDesigner::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
{
    vector<Room> ans;
    double width = mainRoom.getWidth();
    double height = mainRoom.getHeight();

    int n = (int)roomE.size() , index = -1;

    vector<vector<double>> values;
    vector<double> minLimits;
    double minLimitsSum = 0.0;

    for(auto &room : roomE)
    {
        double firstL = room.getDimensionLimit().first;
        double secL = room.getDimensionLimit().second;
        minLimitsSum += firstL;
        minLimits.push_back(firstL);

        double counter = (secL -firstL) / 10;

        vector<double> val;
        for (double i = firstL; i <= secL; i+= counter)
        {
            val.push_back(i);
        }
        values.push_back(val);
    }

    double curTopLimits = 0.0 , diffLimits = 0.0;
    for (int i = 0; i < n - 2; ++i)
    {
        double firstL = roomE[i].getDimensionLimit().first;
        double secL = roomE[i].getDimensionLimit().second;

        diffLimits+= (secL - firstL);
        double firstL2 = roomE[i+1].getDimensionLimit().first;
        curTopLimits += firstL;
        double remaining = (minLimitsSum - curTopLimits - firstL2);

        remaining -= roomE.back().getDimensionLimit().first;
        remaining += 1.5;

        if (curTopLimits >= remaining)
        {
            width = curTopLimits + (diffLimits * 0.5);
            break;
        }
    }

    double newCorridorWidth = 0.0;
    double curX = 0.0;
    vector<vector<double>>tempV;

    Room lastTopRoom;

    for (int i = 0; i < n; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width , tempV , roomE);
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

    Room rightRoom (roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - preferHeight , corridor.getX2() + rightRoomWidth , corridor.getY2());


    ans.push_back(rightRoom);
    corridorWidth = newCorridorWidth;
    width = newCorridorWidth;

    curX = 0.0;
    tempV.clear();
    vector<RoomEntity> newRoomsE;

    for (int i = index+1; i < n; ++i)
    {
        newRoomsE.push_back(roomE[i]);
        cout<<"id = "<<roomE[i].getRoomId()<<" ---> ";
        tempV.push_back(values[i]);
        for(auto &value : values[i])
        {
            cout<<"TEMP = "<<value<<" ";
        }

        cout<<"\n";
    }

    pair<double , vector<double>> res = findClosestSum(width +1.5, tempV , newRoomsE);
    vector<double> out = res.second;
    std::reverse(out.begin(), out.end());
    index = n-1;
    for (int j = 0; j < out.size(); ++j)
    {
        string id = roomE[index].getRoomId();
        double curWidth = out[j];
        double curHeight = roomsArea[id] / curWidth;

        cout<<"ID = "<<id <<" "<<curWidth <<" "<<curHeight<<"\n";

        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
        curX = newRoom.getX2();
        ans.push_back(newRoom);
        index--;
    }
    return ans;
}

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
        string id = roomE[i].getRoomId();
        double curWidth = roomE[i].getDimensionLimit().first;
        double curHeight = roomsArea[id] / curWidth;

        Room newRoom(id , curX - curWidth , mainRoom.getY2() , curX , mainRoom.getY2() + curHeight);
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
        string id = roomE[i].getRoomId();
        double curHeight = roomE[i].getDimensionLimit().first;
        double curWidth = roomsArea[id] / curHeight;

        Room newRoom(id , mainRoom.getX1() - curWidth , curY - curHeight , mainRoom.getX1() , curY);
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
        string id = roomE[i].getRoomId();
        double curWidth = roomE[i].getDimensionLimit().first;
        double curHeight = roomsArea[id] / curWidth;

        Room newRoom(id , curX , mainRoom.getY1() -curHeight, curX+curWidth, mainRoom.getY1() );
        curX = newRoom.getX2();
        ans.push_back(newRoom);

        if (mainRoom.getX2() - curX  < 1)
        {
            break;
        }
    }

    return ans;
}

pair<double , vector<double>> BuildingDesigner::findClosestSum(double x, const vector<vector<double>> &values , vector<RoomEntity> &roomE)
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

    vector<pair<int , vector<double>>> validSolutions;

    for(auto &sol : dp)
    {
        if (sol.first < x)continue;
        set<double> repetitions;
        double sum = sol.first - sol.second.back() + 1.5;
        if (sum >x) continue;
        for (int i = 0; i < sol.second.size(); ++i)
        {
            string id = roomE[i].getRoomId();
            double width = sol.second[i];
            double height = roomsArea[id] / width;
            repetitions.insert(height);
        }

        //3 5 7
        double diff = 0.0 , lastValue = *repetitions.begin();
        for(auto &val : repetitions)
        {
            diff += (val - lastValue);
            lastValue = val;
        }
        validSolutions.emplace_back(diff , sol.second);
    }
    sort(validSolutions.begin(), validSolutions.end());

    if (!validSolutions.empty())
    {
        auto valid = validSolutions[0];
        cout<<"Valid = "<<valid.first<<"\n";
        vector<double> ans = valid.second;
        double sum = 0.0; int index = 0;
        for(auto &v : ans) {
            sum += v;
            string id = roomE[index++].getRoomId();
            double height = roomsArea[id] / v;
            cout << "V = " << height << "\n";
        }
        return {sum , ans};
    }
    cout<<"Not Valid Ans for repetitions\n";


    auto it = dp.upper_bound({x , vector<double>()});

    if (it == dp.end() && !dp.empty()) it --;
    else if (it == dp.end())
    {
        return {INT_MAX , vector<double>()};
    }

    return *it;
}



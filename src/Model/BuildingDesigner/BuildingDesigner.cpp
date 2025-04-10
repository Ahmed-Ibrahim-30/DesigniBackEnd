//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#include "BuildingDesigner.h"
#include "queue"
#include "src/Model/DesignCompactnessRefactor.h"

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

    vector<pair<double , vector<double>>> validSolutions;

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

        map<double , double> mapLimitValues;

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
        vector<double> ans = valid.second;
        double sum = 0.0; int index = 0;

        for (int i = 1; i < ans.size(); ++i)
        {
            double width = ans[i];
            double prevWidth = ans[i-1];

            string id = roomE[i].getRoomId();
            string prevId = roomE[i-1].getRoomId();

            double height = roomsArea[id] / width;
            double height2 = roomsArea[prevId] / prevWidth;

            double diff = MathUtils::roundingToDecimal(abs(height2 - height));

            if (diff <= 0.5)
            {
                ans[i]  = prevWidth * (roomsArea[id] / roomsArea[prevId]);
            }
        }
        for(auto &v : ans)
        {
            sum += v;
        }
        if(x == 5)
        {
            cout<<"Sum = "<<sum <<" "<<ans.size()<<"   "<<values.size()<<"\n";
        }
        return {sum , ans};
    }
    cout<<"Not Valid Ans for repetitions -- "<<x<<" \n";


    auto it = dp.upper_bound({x , vector<double>()});

    if (it == dp.end() && !dp.empty()) it --;
    else if (it == dp.end())
    {
        return {INT_MAX , vector<double>()};
    }

    return *it;
}



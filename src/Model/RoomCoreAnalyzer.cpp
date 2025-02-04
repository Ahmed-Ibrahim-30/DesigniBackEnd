//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "RoomCoreAnalyzer.h"

vector<string> RoomCoreAnalyzer::calculateRoomCores(const Design &design) {
    set<string> cores;
    int roomsSize = design.getRooms().size();
    vector<string> roomIds;
    for(auto &room : design.getRooms())
    {
        roomIds.push_back(room.getRoomId());
    }

    map<string , set<string>> roomsConnections = design.getConnections();

    int maxConnections = 0;
    if(roomsSize < 3){
        if(!roomsSize)cores.insert(roomIds[0]);
        if(roomsSize > 1)cores.insert(roomIds[1]);
    }
    else
    {
        for (int r1 = 0; r1 < roomsSize; ++r1)
        {
            string r1Id = roomIds[r1];
            int conn1 = roomsConnections[r1Id].size();
            for (int r2 = r1+1; r2 < roomsSize; ++r2)
            {
                string r2Id = roomIds[r2];
                int conn2 = roomsConnections[r2Id].size();
                if(!roomsConnections[r1Id].count(r2Id))continue;
                for (int r3 = r2+1; r3 < roomsSize; ++r3)
                {
                    string r3Id = roomIds[r3];
                    if(!roomsConnections[r1Id].count(r3Id) || !roomsConnections[r2Id].count(r3Id))continue;
                    int conn3 = roomsConnections[r3Id].size();
                    int connCount = conn1 + conn2 + conn3;
                    if(connCount > maxConnections){
                        cores.clear();
                        cores.insert(r1Id);
                        cores.insert(r2Id);
                        cores.insert(r3Id);
                        maxConnections = connCount;
                    }
                }
            }
        }
    }

    vector<string> curCores;
    for(auto &core : cores)
    {
        curCores.push_back(core);
    }
    return curCores;
}

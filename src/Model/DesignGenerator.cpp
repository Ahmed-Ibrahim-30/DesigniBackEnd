//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "DesignGenerator.h"

vector<Design> DesignGenerator::generateDesign(const Design &design) {
     vector<Design> designs;
     vector<string> cores;

     // LOG(LogLevel::Error , "Design Error");
     // for(auto &room : design.getRooms())room.printDetails();

     vector<Design> corridorDesigns = corridorConnectionManager.connectCorridors(design);


     if(corridorDesigns.empty())
     {
         LOG(LogLevel::Error , "corridorDesigns Empty");
         cores = RoomCoreAnalyzer::calculateRoomCores(design);

         if(cores.size() < 3)
         {
            LOG(LogLevel::Error , "Cores is less than 3");
            return designs;
         }

         designs = roomConnectionManager.connectRooms(design , cores);
     }

     else {
        designs = corridorDesigns;
        for(auto &room : corridorDesigns[0].getRooms())
        {
            cores.push_back(room.getRoomId());
        }
        designs = roomConnectionManager.connectRooms(design , cores , designs);
        if(designs.empty()) designs = corridorDesigns;
     }

     designRefactor = new DesignCompactnessRefactor();
     for(auto &dn : designs)
     {
         designRefactor->refactorDesign(dn);
     }

     designsSorting = new DesignCompactnessSorter();
     designsSorting->sortDesigns(designs);

     for(auto &newD : designs)
     {
         newD.setConnections(design.getConnections());
     }
     return designs;
}

vector<Design> DesignGenerator::generateDesign1(const Design &design) {
     vector<Design> designs;
     vector<string> cores;



     vector<Design> corridorDesigns = corridorConnectionManager.connectCorridors(design);




     if(corridorDesigns.empty())
     {
         LOG(LogLevel::Error , "corridorDesigns Empty");
         cores = RoomCoreAnalyzer::calculateRoomCores(design);

         if(cores.size() < 3)
         {
            LOG(LogLevel::Error , "Cores is less than 3");
            return designs;
         }

         designs = roomConnectionManager.connectRooms(design , cores);
     }

     else {
        designs = corridorDesigns;
        for(auto &room : corridorDesigns[0].getRooms())
        {
            cores.push_back(room.getRoomId());
        }
        designs = roomConnectionManager.connectRooms(design , cores , designs);
     }
     designRefactor = new DesignCompactnessRefactor();
     for(auto &dn : designs)
     {
         designRefactor->refactorDesign(dn);
     }

     designsSorting = new DesignCompactnessSorter();
     designsSorting->sortDesigns(designs);

     for(auto &newD : designs)
     {
         newD.setConnections(design.getConnections());
     }
     return designs;
}

vector<Design> DesignGenerator::generateDesign(const map<string, double> &livingCore, const map<string, double> &bedroomCore, const map<string, double> &familyCore)
{
    Design newDesign;
    RoomBuilder roomBuilder;

    vector<Room> livingRooms;


    string corridor1 = "Corridor1" , corridor2 = "Corridor2" , livingID;
    bool isFoundCorridor1 = bedroomCore.size() > 2 , isFoundCorridor2 = familyCore.size() > 2;

    for(auto &room : livingCore)
    {
        livingRooms.push_back(roomBuilder.createRoom(room.first , room.second));
        newDesign.addRoom(roomBuilder.createRoom(room.first , room.second));
    }

    for(auto &room : livingRooms)
    {
        if(room.getRoomType() == RoomType::Living)
        {
            livingID = room.getRoomId();
            if(isFoundCorridor1)newDesign.addConnection(room.getRoomId() , corridor1);
            if(isFoundCorridor2)newDesign.addConnection(room.getRoomId() , corridor2);
            for(auto &room2 : livingRooms)
            {
                if(room2.getRoomType() == RoomType::Dining)
                {
                    newDesign.addConnection(room.getRoomId() , room2.getRoomId());
                }
            }
        }
        else if(room.getRoomType() == RoomType::Dining)
        {
            for(auto &room2 : livingRooms)
            {
                if(room.getRoomId() == room2.getRoomId()) continue;
                newDesign.addConnection(room.getRoomId() , room2.getRoomId());
            }
        }
    }

    vector<Room> bedroomRooms;

    for(auto &room : bedroomCore)
    {
        bedroomRooms.push_back(roomBuilder.createRoom(room.first , room.second));
        newDesign.addRoom(roomBuilder.createRoom(room.first , room.second));
    }

    if(bedroomRooms.size() > 2)
    {
        newDesign.addRoom(roomBuilder.createRoom(corridor1 , 18));

        for(int i = 0; i < bedroomRooms.size() ; i++)
        {
            string room1Id = bedroomRooms[i].getRoomId();
            newDesign.addConnection(room1Id , corridor1);
        }
    }
    else
    {
        for(int i = 0; i < bedroomRooms.size() ; i++)
        {
            string room1Id = bedroomRooms[i].getRoomId();
            newDesign.addConnection(livingID , room1Id);
            for(int j = i+1; j < bedroomRooms.size() ; j++)
            {
                string room2Id = bedroomRooms[j].getRoomId();
                newDesign.addConnection(room1Id , room2Id);
            }
        }
    }


    //Family Core
    vector<Room> familyRooms;

    for(auto &room : familyCore)
    {
        familyRooms.push_back(roomBuilder.createRoom(room.first , room.second));
        newDesign.addRoom(roomBuilder.createRoom(room.first , room.second));
    }

    if(familyRooms.size() > 2)
    {
        newDesign.addRoom(roomBuilder.createRoom(corridor2 , 18));

        for(int i = 0; i < familyRooms.size() ; i++)
        {
            string room1Id = familyRooms[i].getRoomId();
            newDesign.addConnection(room1Id , corridor2);
        }
    }
    else
    {
        for(int i = 0; i < familyRooms.size() ; i++)
        {
            string room1Id = familyRooms[i].getRoomId();
            newDesign.addConnection(livingID , room1Id);
            for(int j = i+1; j < familyRooms.size() ; j++)
            {
                string room2Id = familyRooms[j].getRoomId();
                newDesign.addConnection(room1Id , room2Id);
            }
        }
    }

    for(auto &conn : newDesign.getConnections())
    {
        for(auto c : conn.second)
        {
            LOG(LogLevel::Error , "First = "+conn.first + " Second = "+c);
        }
    }

    vector<Design>des =  generateDesign(newDesign);

    for(auto &d : des)
    {
        d.setConnections(newDesign.getConnections());
    }
    return des;
}

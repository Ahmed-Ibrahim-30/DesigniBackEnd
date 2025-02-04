//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "RoomConnectionManager.h"
#include "AreaToDimensionsCalculator.h"

vector<Design> RoomConnectionManager::connectRooms(const Design &design, const vector<string> &cores) {
    vector<Design> designs;
    currentDesign = design;

    roomConnection = design.getConnections();

    for(auto &core : cores)coreRooms.insert(core);

    for(auto &room : design.getRooms())
        roomsIDSArea[room.getRoomId()] = room.getArea();

    calculateRoomLevel(cores [0]);

    int maxLevel = 0;
    for(auto &room : roomLevel)
    {
        maxLevel = max(maxLevel , room.second);
    }

    maxLevel++;
    levels.resize(maxLevel);
    for(auto &room : roomLevel)
    {
        levels[room.second].push_back(room.first);
    }

    for (int var = 0; var < levels.size(); ++var)
    {
        cout<<"Level "<<var<<" --> ";
        for(auto &id : levels[var])
        {
            cout<<id<<" ";
            roomsIDSLayer[id] = var;
        }
        cout<<"\n";
    }

    if(levels.size() < 1 )
    {
        LOG(LogLevel::Warning , "Levels Empty !!");
        return designs;
    }

    for (int i = 1; i < levels.size(); ++i)
    {
        map<string , vector<string>> level1Conn;
        for (int l = 0; l < levels[i].size(); ++l)
        {
            string id = levels[i][l];
            if(!level1Conn.count(id))level1Conn[id] = vector<string>();
            for (int l2 = l+1; l2 < levels[i].size(); ++l2)
            {
                if(roomConnection[id].count(levels[i][l2]))
                {
                    level1Conn[id].push_back(levels[i][l2]);
                    level1Conn[levels[i][l2]].push_back(id);
                }
            }
        }

        int groupSize = 1;
        set<string>visited;
        vector<vector<string>> groups;
        for (int l = 0; l < levels[i].size(); ++l)
        {
            string id = levels[i][l];
            if(visited.count(id))continue;
            int conn = level1Conn[id].size();
            if(conn>1)continue;
            vector<string> group;
            group.push_back(id);

            if(conn == 1)
            {
                visited.insert(id);
                while(true)
                {
                    string newId = level1Conn[id][0];
                    if(visited.count(newId) && level1Conn[id].size()>1)
                    {
                        newId = level1Conn[id][1];
                    }
                    group.push_back(newId);
                    visited.insert(newId);
                    id = newId;
                    if(level1Conn[newId] .size() == 1)break;
                }
            }
            groupSize++;
            groups.push_back(group);
        }
        vector<string> newLevel;
        for(auto &group : groups)
        {
            for(auto &room : group)newLevel.push_back(room);
        }
        levels[i] = newLevel;
    }

    //print groups
    int groupSize = 0;
    for(auto &g : levels)
    {
        cout<<"Level "<<groupSize++<<" -->";
        for(auto &g1 : g)cout<<" "<<g1;
        cout<<"\n";
    }


    map<string , vector<string>> levelRoomConnections;

    for (int var = 1; var < levels.size(); ++var)
    {
        for(auto &roomId : levels[var])
        {
            for(auto &roomParent : levels[var-1])
            {
                if(roomConnection[roomId].count(roomParent))levelRoomConnections[roomId].push_back(roomParent);
            }
        }
    }
    cout<<"all Positions : \n";
    for(auto &roomId : levelRoomConnections)
    {
        cout<<roomId.first <<" --> ";
        for(auto &rooms : roomId.second)cout<<rooms<<" ";
        cout<<"\n";
    }

    vector<Design> coreTemplates = storeCoreTemplateShapes();

    for(auto &temp : coreTemplates)
    {
        connectLayersToCore(1 ,0, temp , levelRoomConnections , designs);
    }

    return designs;
}

vector<Design> RoomConnectionManager::connectRooms(const Design &design, const vector<string> &cores,
                                                   vector<Design> &corridorDesigns) {
    vector<Design> designs;
    currentDesign = design;

    roomConnection = design.getConnections();

    for(auto &core : cores)coreRooms.insert(core);

    for(auto &room : design.getRooms())
        roomsIDSArea[room.getRoomId()] = room.getArea();

    calculateRoomLevel(cores [0]);

    int maxLevel = 0;
    for(auto &room : roomLevel)
    {
        maxLevel = max(maxLevel , room.second);
    }

    maxLevel++;
    levels.resize(maxLevel);
    for(auto &room : roomLevel)
    {
        levels[room.second].push_back(room.first);
    }

    if(levels.size() < 1 )
    {
        LOG(LogLevel::Warning , "Levels Empty !!");
        return designs;
    }

    for (int i = 1; i < levels.size(); ++i)
    {
        map<string , vector<string>> level1Conn;
        for (int l = 0; l < levels[i].size(); ++l)
        {
            string id = levels[i][l];
            if(!level1Conn.count(id))level1Conn[id] = vector<string>();
            for (int l2 = l+1; l2 < levels[i].size(); ++l2)
            {
                if(roomConnection[id].count(levels[i][l2]))
                {
                    level1Conn[id].push_back(levels[i][l2]);
                    level1Conn[levels[i][l2]].push_back(id);
                }
            }
        }

        int groupSize = 1;
        set<string>visited;
        vector<vector<string>> groups;
        for (int l = 0; l < levels[i].size(); ++l)
        {
            string id = levels[i][l];
            if(visited.count(id))continue;
            int conn = level1Conn[id].size();
            if(conn>1)continue;
            vector<string> group;
            group.push_back(id);

            if(conn == 1)
            {
                visited.insert(id);
                while(true)
                {
                    string newId = level1Conn[id][0];
                    if(visited.count(newId) && level1Conn[id].size()>1)
                    {
                        newId = level1Conn[id][1];
                    }
                    group.push_back(newId);
                    visited.insert(newId);
                    id = newId;
                    if(level1Conn[newId] .size() == 1)break;
                }
            }
            groupSize++;
            groups.push_back(group);
        }
        vector<string> newLevel;
        for(auto &group : groups)
        {
            for(auto &room : group)newLevel.push_back(room);
        }
        levels[i] = newLevel;
    }

    //print groups
    int groupSize = 0;
    for(auto &g : levels)
    {
        cout<<"Level "<<groupSize++<<" -->";
        for(auto &g1 : g)cout<<" "<<g1;
        cout<<"\n";
    }


    map<string , vector<string>> levelRoomConnections;

    for (int var = 1; var < levels.size(); ++var)
    {
        for(auto &roomId : levels[var])
        {
            for(auto &roomParent : levels[var-1])
            {
                if(roomConnection[roomId].count(roomParent))levelRoomConnections[roomId].push_back(roomParent);
            }
        }
    }
    cout<<"all Positions : \n";
    for(auto &roomId : levelRoomConnections)
    {
        cout<<roomId.first <<" --> ";
        for(auto &rooms : roomId.second)cout<<rooms<<" ";
        cout<<"\n";
    }

    for(auto &temp : corridorDesigns)
    {
        connectLayersToCore(1 ,0, temp , levelRoomConnections , designs);
    }

    LOG(LogLevel::Info , "Designs Size = "+ to_string(designs.size()));

    return designs;
}


void RoomConnectionManager::connectLayersToCore(int level ,int roomIndex, Design &coreShape , const map<string , vector<string>> &levelRoomConnections , vector<Design> &roomsCollections)
{
    if(roomsCollections.size() >40) return;
    if(level == levels.size()  )//|| coreShape.getRooms() .size() >8
    {
        Design R1 = coreShape;
        roomsCollections.push_back(R1);
        return;
    }

    map<string , Room>allRoomsVisited;

    string roomId = levels[level][roomIndex];
    RoomType roomType = RoomTypeUtils::parseStringToRoomType(roomId);
    double roomArea = roomsIDSArea[roomId];

    vector<Room>requiredRoomConnections;
    for(auto &room : coreShape.getRooms())
    {
        allRoomsVisited[room.getRoomId()] = room;

        if(roomConnection[roomId].count(room.getRoomId()))
            requiredRoomConnections.push_back(room);
    }

    Room newRoom  = roomBuilder.createRoom(roomId , 0 , 0 , 0 , 0);

    vector<Room>curnsRooms;
    set<string>roomCursVisisted;

    bool curRoomSelected = false;

    for(auto &reqRoom : requiredRoomConnections)
    {
        if(reqRoom.getRoomType() == RoomType::Corridor)
        {
            roomCursVisisted.insert(reqRoom.getRoomId());
            curRoomSelected = true;
            curnsRooms.push_back(reqRoom);
            break;
        }
    }

    if( (roomIndex == 0 || !roomConnection[roomId].count(levels[level][roomIndex-1])))
    {
        string curId = levelRoomConnections.at(roomId)[0];
        if(!roomCursVisisted.count(allRoomsVisited[curId].getRoomId()))
            curnsRooms.push_back(allRoomsVisited[curId]);
    }
    else
    {
        if(!roomCursVisisted.count(coreShape.getRooms().back().getRoomId()))
            curnsRooms.push_back(coreShape.getRooms().back());
    }

    vector<Room>cornerRooms;//store Points and side for it

    for(auto &cur : curnsRooms)
    {
        vector<Room>cRooms = setRoomInCorners(cur , newRoom , coreShape.getRooms() );
        cornerRooms.insert(cornerRooms.end() , cRooms.begin() , cRooms.end());
    }

    int pointsIndex = 0;

    for(auto &room : cornerRooms)
    {
        vector<Room>testsRoom ;

        testsRoom.push_back(room);
        for(auto &r1 : testsRoom)
        {
            bool recRequiredConnections = true;

            for(auto &room : requiredRoomConnections)
            {
                Room checkRoom = room;

                for(auto &room2 : roomConnection[room.getRoomId()])
                {
                    string roomId2 = room2;
                    RoomType roomType2 = RoomTypeUtils::parseStringToRoomType(roomId2);
                    if(roomType2 == RoomType::Corridor && allRoomsVisited.count(room2))
                    {
                        checkRoom = allRoomsVisited[room2];
                        break;
                    }
                }


                if( !DesignValidator::isTwoRoomsConnected(r1 , checkRoom) && !DesignValidator::isTwoRoomsConnected(r1 , room))
                {
                    recRequiredConnections = false;
                    break;
                }
            }
            if(recRequiredConnections)
            {
                vector<Room>updatedRooms = coreShape.getRooms();
                updatedRooms.push_back(r1);
                Design newDesign;
                for(auto &r : updatedRooms)newDesign.addRoom(r);

                connectLayersToCore(roomIndex+1 == levels[level].size() ? level+1 : level , (roomIndex+1) % levels[level].size() ,newDesign ,  levelRoomConnections , roomsCollections);
                break;
            }
        }

        if(pointsIndex == cornerRooms.size() - 1)
        {
            return;
        }
        pointsIndex++;
    }

}

void RoomConnectionManager::calculateRoomLevel(const string &id )
{
    //roomLevel
    int curLevel = roomLevel[id];
    for(auto &connId : roomConnection[id])
    {
        if(!roomLevel.count(connId) && coreRooms.count(connId))
        {
            roomLevel[connId] = 0;
        }
        else if(roomLevel.count(connId) && roomLevel[connId] > curLevel + 1)
        {
            roomLevel[connId] = curLevel + 1;
        }else if(!roomLevel.count(connId))
        {
            roomLevel[connId] = curLevel + 1;
        }
        else continue;
        calculateRoomLevel(connId);
    }
    return;
}

vector<Room> RoomConnectionManager::setRoomInCorners(Room mainRoom,Room requiredRoom,const vector<Room> &rooms)
{
    vector<Room> finalRooms;
    string roomId = requiredRoom.getRoomId();
    double height , width , area = roomsIDSArea[roomId];

    RoomType roomType = requiredRoom.getRoomType();

    double sharedWallY = 0;
    double sharedWallX = 0;

    for(auto &room : rooms)
    {
        if(room.getRoomId() == mainRoom.getRoomId()) continue;
        if(room.getRoomId() == requiredRoom.getRoomId()) continue;

        //left
        if(mainRoom.getX1() == room.getX2() && room.getY2() >= mainRoom.getY1() && mainRoom.getY2() >= room.getY1())
        {
            vector<Room>newRooms;
            //other room in left with top
            if(room.getY1() < mainRoom.getY1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - mainRoom.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY1() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);
                int connectFormId = 1;

                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX1() , mainRoom.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY1() > mainRoom.getY1() )
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - room.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY1() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX1() - X, room.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY2() < mainRoom.getY2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - room.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - room.getY2();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX1() - X , room.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }

            }

            if(room.getY2() > mainRoom.getY2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - mainRoom.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - mainRoom.getY2();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;

                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX1() , mainRoom.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY1() == mainRoom.getY1())
            {
                //design 1
                width = (mainRoom.getX2() - mainRoom.getX1()) + (room.getX2() - room.getX1());
                height = round ( (area / width) * 10) / 10;
                Point p(room.getX1() , room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX1() , room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(room.getX1() , room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - width, room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 4
                width = room.getX2() - room.getX1();
                width = round(width * 10)/10;
                height = round ( (area / width) * 10) / 10;
                p = Point(room.getX2() - (width / 2), room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 5
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - (width / 2) - width, room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            if(room.getY2() == mainRoom.getY2())
            {
                //design 1
                width = (mainRoom.getX2() - mainRoom.getX1()) + (room.getX2() - room.getX1());
                height = round ( (area / width) * 10) / 10;
                Point p(room.getX1() , room.getY2());
                if(room.getX1() + width > mainRoom.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX1() , room.getY2());
                if(room.getX1() + width > mainRoom.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(room.getX1() , room.getY2());

                if(room.getX1() + width > mainRoom.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - width, room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 4
                width = room.getX2() - room.getX1();
                width = round(width * 10)/10;
                height = round ( (area / width) * 10) / 10;
                p = Point(room.getX2() - (width / 2), room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 5
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - (width / 2) - width, room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }
        }
        else if(mainRoom.getX1() == room.getX2() && mainRoom.getY2() == room.getY1())
        {
            vector<Room>newRooms;
            vector<pair<double,double>>allXY;

            //design 1
            width = room.getX2() - room.getX1();
            height = round ( (area / width) * 10) / 10;
            allXY.emplace_back(width , height);

            //design 2
            height = mainRoom.getY2() - mainRoom.getY1();
            width = round ( (area / height) * 10) / 10;
            allXY.emplace_back(width , height);

            //design 3
            width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
            height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
            allXY.emplace_back(width , height);

            //design 4
            width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
            height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
            allXY.emplace_back(width , height);
            int connectFormId = 1;
            for(const auto &[X , Y] : allXY)
            {
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                Point newPoint(mainRoom.getX1(), mainRoom.getY2());

                newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }
        }

        //right
        if(mainRoom.getX2() == room.getX1() && room.getY2() >= mainRoom.getY1() && mainRoom.getY2() >= room.getY1())
        {
            vector<Room>newRooms;
            //other room in left with top
            if(room.getY1() < mainRoom.getY1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - mainRoom.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY1() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX2() - X, mainRoom.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY1() > mainRoom.getY1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - room.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY1() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);
                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX2() , room.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY2() < mainRoom.getY2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - room.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - room.getY2();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);
                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX2() , room.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }

            }

            if(room.getY2() > mainRoom.getY2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - mainRoom.getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - mainRoom.getY2();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint(mainRoom.getX2() - X , mainRoom.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room.getY1() == mainRoom.getY1())
            {
                //design 1
                width = (mainRoom.getX2() - mainRoom.getX1()) + (room.getX2() - room.getX1());
                height = round ( (area / width) * 10) / 10;
                Point p(mainRoom.getX1() , room.getY1() - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() , room.getY1() - height);
                if(mainRoom.getX1() + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() , room.getY1() - height);
                if(mainRoom.getX1() + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX2() - width, room.getY1() - height);
                if(room.getX2() - width < mainRoom.getX2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 4
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - (width / 2), room.getY1() - height);
                if(mainRoom.getX2() - (width / 2) + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 5
                width = room.getX2() - room.getX1();
                width = round(width * 10)/10;
                height = round ( (area / width) * 10) / 10;
                p = Point(room.getX2() - (width / 2) - width, room.getY1() - height);
                if(room.getX2() - (width / 2) - width < mainRoom.getX2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }
            }

            if(room.getY2() == mainRoom.getY2())
            {
                //design 1
                width = (mainRoom.getX2() - mainRoom.getX1()) + (room.getX2() - room.getX1());
                height = round ( (area / width) * 10) / 10;
                Point p(mainRoom.getX1() , room.getY2());
                if(mainRoom.getX1() + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() , room.getY2());
                if(mainRoom.getX1() + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() , room.getY2());
                if(mainRoom.getX1() + width > room.getX1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX2() - width, room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 4
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() - (width / 2), room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 5
                width = room.getX2() - room.getX1();
                width = round(width * 10)/10;
                height = round ( (area / width) * 10) / 10;
                p = Point(room.getX2() - (width / 2) - width, room.getY2());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }

        }
        else if(mainRoom.getX2() == room.getX1() && mainRoom.getY2() == room.getY1())
        {
            vector<Room>newRooms;
            vector<pair<double,double>>allXY;

            //design 1
            width = mainRoom.getX2() - mainRoom.getX1();
            height = round ( (area / width) * 10) / 10;
            allXY.emplace_back(width , height);

            //design 2
            height = room.getY2() - room.getY1();
            width = round ( (area / height) * 10) / 10;
            allXY.emplace_back(width , height);

            //design 3
            width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
            height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
            allXY.emplace_back(width , height);

            //design 4
            width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
            height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
            allXY.emplace_back(width , height);
            int connectFormId = 1;
            for(const auto &[X , Y] : allXY)
            {
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                Point newPoint(mainRoom.getX2() - X, mainRoom.getY2());

                newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }
        }

        //top
        if(mainRoom.getY1() == room.getY2() && room.getX2() > mainRoom.getX1() && mainRoom.getX2() > room.getX1())
        {
            vector<Room>newRooms;
            if(room .getX1() < mainRoom.getX1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX1() - room .getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (mainRoom.getX1() - X , mainRoom.getY1());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX1() > mainRoom.getX1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX1() - mainRoom .getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (room.getX1() - X , mainRoom.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() < mainRoom.getX2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - room .getX2();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (room.getX2() , mainRoom.getY1() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() > mainRoom.getX2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - mainRoom .getX2();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (mainRoom.getX2() , mainRoom.getY1());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() == mainRoom.getX2())
            {
                //design 1
                height = (mainRoom.getY2() - mainRoom.getY1()) + (room.getY2() - room.getY1());
                width = round ( (area / height) * 10) / 10;
                Point p(room.getX2() , room.getY1());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX2() , room.getY1());
                if(height + room.getY1() > mainRoom.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX2() , room.getY1());
                if(height + room.getY1() > mainRoom.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX2(), mainRoom.getY2() - height);
                if(mainRoom.getY2() - height < room.getY2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 4
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX2() , room.getY2() - (height / 2));
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 5
                height = mainRoom.getY2() - mainRoom.getY1();
                height = round(height * 10)/10;
                width = round ( (area / height) * 10) / 10;
                p = Point(room.getX2() , mainRoom.getY2() - (height / 2) - height);
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            if(room .getX1() == mainRoom.getX1())
            {
                //design 1
                height = (mainRoom.getY2() - mainRoom.getY1()) + (room.getY2() - room.getY1());
                width = round ( (area / height) * 10) / 10;
                Point p(room.getX1() - width , room.getY1());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(room.getX1() - width , room.getY1());
                if(height + room.getY1() > mainRoom.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                if(height + room.getY1() > mainRoom.getY1())
                {
                    p = Point(room.getX1() - width , room.getY1());
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 3
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX1() - width, mainRoom.getY2() - height);
                if(mainRoom.getY2() - height < room.getY2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                //design 4
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(room.getX1() - width , room.getY2() - (height / 2));
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }
        }

        //bottom
        if(mainRoom.getY2() == room.getY1() && room.getX2() > mainRoom.getX1() && mainRoom.getX2() > room.getX1())
        {
            vector<Room>newRooms;
            if(room .getX1() < mainRoom.getX1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX1() - room .getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (mainRoom.getX1() - X , mainRoom.getY2() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX1() > mainRoom.getX1())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX1() - mainRoom .getX1();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (room.getX1() - X , mainRoom.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() < mainRoom.getX2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = mainRoom.getX2() - room .getX2();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = room.getY2() - room.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (room.getX2() , mainRoom.getY2());

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() > mainRoom.getX2())
            {
                vector<pair<double,double>>allXY;

                //design 1
                width = room.getX2() - mainRoom .getX2();
                height = round ( (area / width) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 2
                height = mainRoom.getY2() - mainRoom.getY1();
                width = round ( (area / height) * 10) / 10;
                allXY.emplace_back(width , height);

                //design 3
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                //design 4
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                allXY.emplace_back(width , height);

                int connectFormId = 1;
                for(const auto &[X , Y] : allXY)
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , 0 , 0 , X , Y));

                    Point newPoint (mainRoom.getX2() , mainRoom.getY2() - Y);

                    newRooms.back().moveRoom(newPoint.getX() , newPoint.getY());
                }
            }

            if(room .getX2() == mainRoom.getX2())
            {
                //design 1
                height = (mainRoom.getY2() - mainRoom.getY1()) + (room.getY2() - room.getY1());
                width = round ( (area / height) * 10) / 10;
                Point p(mainRoom.getX2() , mainRoom.getY1());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() , mainRoom.getY1());
                if(mainRoom.getY1() + height > room.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }


                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() , mainRoom.getY1());
                if(mainRoom.getY1() + height > room.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX2(), room.getY2() - height);
                if(room.getY2() - height < mainRoom.getY2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 4
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX2() , mainRoom.getY2() - (height / 2));
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            if(room .getX1() == mainRoom.getX1())
            {
                //design 1
                height = (mainRoom.getY2() - mainRoom.getY1()) + (room.getY2() - room.getY1());
                width = round ( (area / height) * 10) / 10;
                Point p(mainRoom.getX1() - width , mainRoom.getY1());
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

                //design 2
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() - width , mainRoom.getY1());
                if(mainRoom.getY1() + height > room.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 2
                width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() - width , mainRoom.getY1());
                if(mainRoom.getY1() + height > room.getY1())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 3
                width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() - width, room.getY2() - height);
                if(room.getY2() - height < mainRoom.getY2())
                {
                    newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
                }

                //design 4
                height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
                width  = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
                p = Point(mainRoom.getX1() - width , mainRoom.getY2() - (height / 2));
                newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));
            }

            for(auto &newRoom : newRooms)
            {
                if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
                {
                    sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
                else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                    sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                    if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                    {
                        finalRooms.push_back(newRoom);
                    }
                }
            }
        }
    }


    vector<Room> copy;
    for(auto &room : finalRooms)
    {
        double aspectRatio = max(room.getWidth(), room.getHeight()) / min(room.getWidth(), room.getHeight());

        if (aspectRatio > RoomDefault::getDefaultAspectRatio(room.getRoomType()))
        {
            break;
        }
        copy.push_back(room);
    }

    if(copy.empty())
    {
        vector<Room>newRooms;
        //top
        width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        Point p(mainRoom.getX1() , mainRoom.getY1() - height);
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        //top2
        width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        p = Point(mainRoom.getX2() - width, mainRoom.getY1() - height);
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        //bottom
        width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        p = Point(mainRoom.getX1() , mainRoom.getY2() );
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        //bottom
        width = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        p = Point(mainRoom.getX2() - width, mainRoom.getY2() );
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        //left
        width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        p = Point(mainRoom.getX1() - width , mainRoom.getY1() );
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        //right
        width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomType);
        height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomType);
        p = Point(mainRoom.getX2()  , mainRoom.getY2() );
        newRooms.emplace_back(roomBuilder.createRoom(requiredRoom.getRoomId() , p.getX() , p.getY() , p.getX() + width , p.getY()+height));

        for(auto &newRoom : newRooms)
        {
            if(newRoom.getX1() == mainRoom.getX2() || newRoom.getX2() == mainRoom.getX1())
            {
                sharedWallY = min(mainRoom.getY2() , newRoom.getY2()) - max(mainRoom.getY1() , newRoom.getY1());

                if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallY >= 1.5 || roomType == RoomType::Corridor ))
                {
                    copy.push_back(newRoom);
                }
            }
            else if(newRoom.getY1() == mainRoom.getY2() || newRoom.getY2() == mainRoom.getY1()){
                sharedWallX = min(mainRoom.getX2() , newRoom.getX2()) - max(mainRoom.getX1() , newRoom.getX1());

                if(DesignValidator::isRoomOutsideRooms(newRoom , rooms) && (sharedWallX >= 1.5 || roomType == RoomType::Corridor ))
                {
                    copy.push_back(newRoom);
                }
            }
        }
    }

    return copy;
}

vector<Design> RoomConnectionManager::storeCoreTemplateShapes() {
    vector< Design > coreTemplateShapes;
    if(coreRooms.size() < 3)
    {
        LOG(LogLevel::Error, " cores size < 3");
        return coreTemplateShapes;
    }
    auto it = coreRooms.begin();

    double precentage = 1.5;

    string core1 = *it , core2 = *(++it) , core3 = *(++it);

    if(roomsIDSArea[core1] > roomsIDSArea[core2]) swap(core1 , core2);
    if(roomsIDSArea[core1] > roomsIDSArea[core3]) swap(core1 , core3);
    if(roomsIDSArea[core2] > roomsIDSArea[core3]) swap(core2 , core3);

    bool isCorridorFound = false;

    if(core1.find("corridor") != string::npos || core1.find("Corridor") != string::npos) swap(core1 , core3) , isCorridorFound=true;

    if(core2.find("corridor") != string::npos || core2.find("Corridor") != string::npos) swap(core2 , core3) , isCorridorFound=true;

    bool isThreeConnected = roomConnection [core1] .count(core2) && roomConnection [core2] .count(core3) && roomConnection [core1] .count(core3);

    Room room1 = roomBuilder.createRoom(core1 , 0 , 0 , 0 , 0);
    Room room2 = roomBuilder.createRoom(core2 , 0 , 0 , 0 , 0);
    Room room3 = roomBuilder.createRoom(core3 , 0 , 0 , 0 , 0);
    double core1Area = roomsIDSArea[core1] , core1X = round (sqrt(core1Area) * 10) / 10 , core1Y = round ((core1Area / core1X) * 10) / 10;
    double core2Area = roomsIDSArea[core2] , core2X = round (sqrt(core2Area) * 10) / 10 , core2Y = round ((core2Area / core2X) * 10) / 10;
    double core3Area = roomsIDSArea[core3] , core3X = round (sqrt(core3Area) * 10) / 10 , core3Y = round ((core3Area / core3X) * 10) / 10;

    if(isThreeConnected && !isCorridorFound)
    {
        core3X = round ( (sqrt((core3Area/precentage)) * precentage) * 10 ) / 10;
        core3Y = round ((core3Area / core3X) * 10) / 10;

        core2X = core3X/2 , core2Y = round ((core2Area / core2X) * 10) / 10;
        core1X = core3X/2 , core1Y = round ((core1Area / core1X) * 10) / 10;

        room3.setRoomDimensions(core3X , core3Y , 0             ,        0      );
        room2.setRoomDimensions(core2X , core2Y , 0             , room3.getY2() );
        room1.setRoomDimensions(core1X , core1Y , room2.getX2() , room3.getY2() );

        //to fix round issue
        room1.setX2(room3.getX2());

        Design design;
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);

        //shape 2
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = round (sqrt(core2Area) * 10) / 10;
        core2Y = round ((core2Area / core2X) * 10) / 10;

        core3X = core1X/2 +  core2X / 2 , core3Y = round ((core3Area / core3X) * 10) / 10;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0   );
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() , room1.getY2() -  core2Y);
        room3.setRoomDimensions(core3X , core3Y , ((room1.getX2() -room1.getX1()) /2) +room1.getX1(), room1.getY2());

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);

        //shape 3
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = core1X , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3X = round (sqrt(core3Area) * 10) / 10 , core3Y = round ((core3Area / core3X) * 10) / 10;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0      );
        room2.setRoomDimensions(core2X , core2Y , 0             , room1.getY2() );
        room3.setRoomDimensions(core3X , core3Y , room2.getX2() , room1.getY2() );

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 4
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = core1X , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3Y = core2Y + core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0      );
        room2.setRoomDimensions(core2X , core2Y , room1.getX1() , room1.getY2() );
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , room2.getY2() - core3Y);

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 5
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = max(core1X/2 , round (sqrt(core2Area/2) * 10) / 10) , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3Y = core2Y + core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0 );
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() - core2X , room1.getY2());
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , room2.getY2() - core3Y );

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 6
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = max(core1X/2 , round (sqrt(core2Area/2) * 10) / 10) , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3Y = core2Y/2 + core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0);
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() - core2X , room1.getY2() );
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , ((room2.getY2()-room2.getY1())/2)+ room2.getY1() - core3Y );

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
    }
    else if(isThreeConnected && isCorridorFound)
    {
        core3X = 12;
        core3Y = 1.5;

        core2X =  round ((sqrt(core2Area / 2) * 2) * 10) / 10 , core2Y = round ((core2Area / core2X) * 10) / 10;
        core1X =  round ((sqrt(core1Area / 2) * 2) * 10) / 10 , core1Y = round ((core1Area / core1X) * 10) / 10;

        room3.setRoomDimensions(core3X , core3Y , 0             ,        0      );
        room2.setRoomDimensions(core2X , core2Y , 0             , room3.getY2() );
        room1.setRoomDimensions(core1X , core1Y , room2.getX2() , room3.getY2() );

        //to fix round issue
        room3.setX2(room1.getX2());

        Design design ;
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 2
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = round (sqrt(core2Area) * 10) / 10;
        core2Y = round ((core2Area / core2X) * 10) / 10;

        core3X = 12 , core3Y = 1.5;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0  );
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() , room1.getY2() -  core2Y);

        if(core3X >= core2X + core1X)room3.setRoomDimensions(core3X , core3Y , 0 , room1.getY2() );
        else if(core3X < core2X + core1X){
            double sum = core2X + core1X;
            double diff = (sum - core3X)/2;
            double startX = diff;
            room3.setRoomDimensions(core3X , core3Y , startX , room1.getY2() );
        }

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 3
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = core1X , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3X = 1.5 , core3Y = 12;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0 );
        room2.setRoomDimensions(core2X , core2Y , 0             , room1.getY2());
        room3.setRoomDimensions(core3X , core3Y , room2.getX2() , room2.getY2() - core3Y);

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 4
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = core1X , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3X = 1.5 , core3Y = 12;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0  );
        room2.setRoomDimensions(core2X , core2Y , room1.getX1() , room1.getY2());
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , room2.getY2() - core3Y);

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 5
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = max(core1X/2 , round (sqrt(core2Area/2) * 10) / 10) , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3X = 1.5 , core3Y = 12;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0 );
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() - core2X , room1.getY2());
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , room2.getY2() - core3Y );

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 6
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2X = max(core1X/2 , round (sqrt(core2Area/2) * 10) / 10) , core2Y = round ((core2Area / core2X) * 10) / 10;
        core3X = 1.5 , core3Y = 12;

        room1.setRoomDimensions(core1X , core1Y , 0             ,        0);
        room2.setRoomDimensions(core2X , core2Y , room1.getX2() - core2X , room1.getY2() );
        room3.setRoomDimensions(core3X , core3Y , room1.getX2() , ((room2.getY2()-room2.getY1())/2)+ room2.getY1() - core3Y);

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
    }
    else
    {
        core1X = round (sqrt(core1Area) * 10) / 10;
        core1Y = round ((core1Area / core1X) * 10) / 10;

        core2Y = core1Y , core2X = round ((core2Area / core2Y) * 10) / 10;
        core3Y = core1Y , core3X = round ((core3Area / core3Y) * 10) / 10;

        if(roomConnection [core1].count(core2) && roomConnection [core1].count(core3))
        {
            room2.setRoomDimensions(core2X , core2Y , 0             , 0 );
            room1.setRoomDimensions(core1X , core1Y , room2.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
        }
        else if(roomConnection [core2].count(core1) && roomConnection [core2].count(core3))
        {
            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room2.setRoomDimensions(core2X , core2Y , room1.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room2.getX2() , 0 );
        }
        else if(roomConnection [core3].count(core1) && roomConnection [core3].count(core2))
        {
            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
            room2.setRoomDimensions(core2X , core2Y , room3.getX2() , 0 );
        }

        Design design;
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);
        //shape 2

        if(roomConnection [core1].count(core2) && roomConnection [core1].count(core3))
        {
            core2X = round (sqrt(core2Area) * 10) / 10;
            core2Y = round ((core1Area / core2X) * 10) / 10;

            core1Y = core2Y/2 , core1X = round ((core1Area / core1Y) * 10) / 10;
            core3Y = core2Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room2.setRoomDimensions(core2X , core2Y , 0             , 0 );
            room1.setRoomDimensions(core1X , core1Y , room2.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
        }
        else if(roomConnection [core2].count(core1) && roomConnection [core2].count(core3))
        {
            core1X = round (sqrt(core1Area) * 10) / 10;
            core1Y = round ((core1Area / core1X) * 10) / 10;

            core2Y = core1Y/2 , core2X = round ((core2Area / core2Y) * 10) / 10;
            core3Y = core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room2.setRoomDimensions(core2X , core2Y , room1.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room2.getX2() , 0 );
        }
        else if(roomConnection [core3].count(core1) && roomConnection [core3].count(core2))
        {
            core1X = round (sqrt(core1Area) * 10) / 10;
            core1Y = round ((core1Area / core1X) * 10) / 10;

            core2Y = core1Y/2 , core2X = round ((core2Area / core2Y) * 10) / 10;
            core3Y = core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
            room2.setRoomDimensions(core2X , core2Y , room3.getX2() , 0 );
        }

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);

        //shape 3
        if(roomConnection [core1].count(core2) && roomConnection [core1].count(core3))
        {
            core2X = round (sqrt(core2Area) * 10) / 10;
            core2Y = round ((core1Area / core2X) * 10) / 10;

            core1Y = core2Y/2 , core1X = round ((core1Area / core1Y) * 10) / 10;
            core3Y = core1Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room2.setRoomDimensions(core2X , core2Y , 0             , 0 );
            room1.setRoomDimensions(core1X , core1Y , room2.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
        }
        else if(roomConnection [core2].count(core1) && roomConnection [core2].count(core3))
        {
            core1X = round (sqrt(core1Area) * 10) / 10;
            core1Y = round ((core1Area / core1X) * 10) / 10;

            core2Y = core1Y/2 , core2X = round ((core2Area / core2Y) * 10) / 10;
            core3Y = core2Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room2.setRoomDimensions(core2X , core2Y , room1.getX2() , 0 );
            room3.setRoomDimensions(core3X , core3Y , room2.getX2() , 0 );
        }
        else if(roomConnection [core3].count(core1) && roomConnection [core3].count(core2))
        {
            core1X = round (sqrt(core1Area) * 10) / 10;
            core1Y = round ((core1Area / core1X) * 10) / 10;

            core2Y = core1Y/2 , core2X = round ((core2Area / core2Y) * 10) / 10;
            core3Y = core2Y/2 , core3X = round ((core3Area / core3Y) * 10) / 10;

            room1.setRoomDimensions(core1X , core1Y , 0             , 0 );
            room3.setRoomDimensions(core3X , core3Y , room1.getX2() , 0 );
            room2.setRoomDimensions(core2X , core2Y , room3.getX2() , 0 );
        }

        design = Design();
        design.addRoom(room1);
        design.addRoom(room2);
        design.addRoom(room3);
        design.addConnection(room1.getRoomId() , room2.getRoomId());
        design.addConnection(room1.getRoomId() , room3.getRoomId());
        design.addConnection(room3.getRoomId() , room2.getRoomId());

        coreTemplateShapes.emplace_back(design);    }

    return coreTemplateShapes;

}



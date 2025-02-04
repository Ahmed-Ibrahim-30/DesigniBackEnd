//
// Created by ahmed Ibrahim on 15-Dec-24.
//

#include "TopAlignedRoomCorridorDimensionsCalculator.h"

TopAlignedRoomCorridorDimensionsCalculator::TopAlignedRoomCorridorDimensionsCalculator(const Design &inputDesign,
                                                                                       const string &corridorName,
                                                                                       Side blockSide)
        : CorridorDimensionsCalculator(inputDesign, corridorName, blockSide) {

    maxCorridorWidth = 14;
    maxCorridorHeight = 1.5;

    if(blockSide == Side::TOP)
    {
        blockSide = Side::BOTTOM;
        rotatedDegree = 180;
    }
}

vector<Design> TopAlignedRoomCorridorDimensionsCalculator::ConnectCorridorRooms(Design &outputDesign) {
    vector<Design> designs;

    if(rotatedDegree != 0.0)
    {
        outputDesign.rotate(rotatedDegree);
        // designs.push_back(outputDesign);
        // return designs;
    }

    int numberOfCorridorConnections = (int)roomsConnections[corridorName].size();
    int groupCounter = numberOfCorridorConnections<3 ? numberOfCorridorConnections : 3;


    vector <Group> groups (groupCounter); // Right , Top , Left

    vector<vector<Group>> groupsDivisions  = getGroupDivisions(0 , groups);

     // cout<<"Divisions Size = "<<groupsDivisions.size()<<"\n";
     // for(auto &grs : groupsDivisions)
     // {
     //     cout<<"Group Solution --> \n";
     //     for(auto &g : grs) g.print();
     //     cout<<"\n";
     // }


   for(auto &groupDiv : groupsDivisions)
   {
       Design newDesign;
       vector<Design> shapes = getGroupsShapes(0 , groupDiv , outputDesign , newDesign);
       designs.insert(designs.end() , shapes.begin() , shapes.end());
       break;
   }
    return designs;
}

vector<Design> TopAlignedRoomCorridorDimensionsCalculator::getGroupsShapes(int groupNum, vector<Group> &groups, Design &mainDesign , Design &newDesign) {
    vector<Design> shapes;

    bool isLastGroup  = groupNum == groups.size();

    if(isLastGroup)
    {
        Design newMainDesign = mainDesign;

        Room corridor , sharedRoom;

        double corridorX = 0.0 , corridorY = 0.0;

        //set corridor
        for(auto &room : mainDesign.getRooms())
        {
            if(room.getRoomId() == corridorName)
            {
                corridorX = room.getWidth();
                corridorY = room.getHeight();
                corridor = room;
            }
        }
        //set Shared Room
        for(auto &room : mainDesign.getRooms())
        {
            if(room.getRoomType() != RoomType::Corridor && room.getY1() == corridor.getY2())
            {
                sharedRoom = room;
                break;
            }
        }


        //reposition Corridor in the center of SharedRoom
        double centerSharedRoom = sharedRoom.getX1() + (sharedRoom.getWidth()/2.0);

        map<Side , vector<Room>> groupRooms;
        for(auto &group : groups)
        {
            Side side = group.groupSide;
            vector<Room> newGroupRooms;
            vector<string> rooms = group.rooms;
            set<string> roomSet;
            for(auto &r :rooms)roomSet.insert(r);

            for(auto &room : newDesign.getRooms())
            {
                if(roomSet.count(room.getRoomId()))
                {
                    newGroupRooms.push_back(room);
                }
            }
            groupRooms[side]= newGroupRooms;
        }

        //template1

        vector<Room>template2Rooms;

        RoomBuilder roomBuilder;
        double xValue;
        bool isShapeValid ;

        //Template1
        newMainDesign.updateRoomDimensions(corridorName , centerSharedRoom - (corridorX/2) , corridor.getY1() , centerSharedRoom + (corridorX/2),corridor.getY2());
        corridor = newMainDesign.fetchRoomById(corridorName);
        cout<<"corridorX2 = "<<corridor.getX2()<<"\n";


        //template 1
        if(groupRooms.count(Side::LEFT)){
            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[Side::LEFT][0].getRoomId() , corridor.getX1()  - groupRooms[Side::LEFT][0].getWidth(), corridor.getY2() - groupRooms[Side::LEFT][0].getHeight() , corridor.getX1() , corridor.getY2()));
        }

        if(groupRooms.count(Side::RIGHT)){
            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[Side::RIGHT][0].getRoomId() , corridor.getX2()  , corridor.getY2() - groupRooms[Side::RIGHT][0].getHeight() , corridor.getX2() + groupRooms[Side::RIGHT][0].getWidth(), corridor.getY2()));
        }

        xValue = corridor.getX1();
        //top rooms
        for(auto &room : groupRooms[Side::TOP] )
        {
            RoomBuilder roomBuilder2;
            template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() ,corridor.getY1()));
            xValue += room.getWidth();
        }

        for(auto &room : template2Rooms)
        {
            newMainDesign.addRoom(room);
        }
        isShapeValid = DesignValidator::validateDesign(newMainDesign.getRooms());
        if(isShapeValid)
        {
            shapes.push_back(newMainDesign);
        }


        //template2
        template2Rooms.clear();
        newMainDesign = mainDesign;

        newMainDesign.updateRoomDimensions(corridorName , sharedRoom.getX1() , corridor.getY1() , sharedRoom.getX1() + corridorX ,corridor.getY2());
        corridor = newMainDesign.fetchRoomById(corridorName);


        if(groupRooms.count(Side::LEFT)){
            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[Side::LEFT][0].getRoomId() , corridor.getX1()  - groupRooms[Side::LEFT][0].getWidth(), corridor.getY2() - groupRooms[Side::LEFT][0].getHeight() , corridor.getX1() , corridor.getY2()));
        }

        if(groupRooms.count(Side::RIGHT)){
            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[Side::RIGHT][0].getRoomId() , corridor.getX2()  , corridor.getY2() - groupRooms[Side::RIGHT][0].getHeight() , corridor.getX2() + groupRooms[Side::RIGHT][0].getWidth(), corridor.getY2()));
        }

        xValue = corridor.getX1();
        //top rooms
        for(auto &room : groupRooms[Side::TOP] )
        {
            RoomBuilder roomBuilder2;
            template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() ,corridor.getY1()));
            xValue += room.getWidth();
        }

        for(auto &room : template2Rooms)
        {
            newMainDesign.addRoom(room);
        }
        isShapeValid = DesignValidator::validateDesign(newMainDesign.getRooms());
        if(isShapeValid)
        {
            shapes.push_back(newMainDesign);
        }

        return shapes;
    }

    Group group = groups[groupNum];
    RoomBuilder roomBuilder;

    vector<Room> mainRooms = newDesign.getRooms();


    //Group 1 is Left
    if(group.groupSide == Side::LEFT || group.groupSide == Side::RIGHT)
    {
        string roomId = group.rooms[0];
        double area = roomsIDSArea[roomId];
        double height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
        double width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

        newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  width , height));

        vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign ,newDesign);
        if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());

        newDesign.setRooms(mainRooms);
    }

    //Group 2 is Top
    else if(group.groupSide == Side::TOP)
    {
        string roomId = group.rooms[0];
        double area = roomsIDSArea[roomId];
        double MaxY = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
        double MinY = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

        for(auto &room : group.rooms)
        {
            area   = roomsIDSArea[room];
            MinY   = max(MinY , AreaToDimensionsCalculator::getMinValueFromArea(area , roomId));
            MaxY   = min(MaxY , AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId));
        }
        double corridorWidth = 0.0 , minCorridorWidth = 0.0;
        if(MinY <= MaxY) //then there are shared Y between Rooms
        {
            double minCorridorX = 0 , maxCorridorX = maxCorridorWidth;
            for (double Y = MinY; Y <= MaxY; ++Y)
            {
                corridorWidth = 0.0 , minCorridorWidth = 0.0;
                for(auto &room : group.rooms)
                {
                    roomId = room;
                    area   = roomsIDSArea[roomId];
                    double X = round((area / Y) * 10) / 10;
                    newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));
                    corridorWidth = min(corridorWidth + X , maxCorridorWidth);
                    if(room == group.rooms.back())minCorridorWidth +=1.5;
                    else minCorridorWidth+=X;
                }
                minCorridorX = min ( minCorridorWidth , maxCorridorWidth );
                maxCorridorX = corridorWidth;

//                if(minCorridorX == minCorridorWidth)
//                {
//                    corridorWidth = minCorridorWidth;
//                }

                mainDesign.updateRoomDimensions(corridorName , corridorWidth , maxCorridorHeight);

                vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
                if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
                newDesign.setRooms( mainRooms);
            }
        }
        else
        {
            corridorWidth = 0.0 , minCorridorWidth = 0.0;
            for(auto &room : group.rooms)
            {
                roomId = room;
                area   = roomsIDSArea[roomId];
                double X = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
                double Y = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);
                corridorWidth = corridorWidth + X ;
                newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));
            }
            mainDesign.updateRoomDimensions(corridorName , corridorWidth , maxCorridorHeight);

            vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
            newDesign.setRooms( mainRooms );
        }
    }

    return shapes;
}

vector<vector<TopAlignedRoomCorridorDimensionsCalculator::Group>>
TopAlignedRoomCorridorDimensionsCalculator::getGroupDivisions(int roomNum, vector<Group> &groups) {
    vector<vector<Group>> groupsDivisions;

    if(groups.empty())return groupsDivisions;

    int groupSize = (int)groups.size();

    if(groupSize >=3)
    {
        for(auto &roomIdLeft : corridorRoomsIds)
        {
            groups[0].rooms.clear();

            groups[0].rooms.push_back(roomIdLeft);
            groups[0].groupSide = Side::LEFT;


            for(auto &roomIdRight : corridorRoomsIds)
            {
                if(roomIdLeft == roomIdRight)continue;
                groups[2].rooms.clear();

                groups[2].rooms.push_back(roomIdRight);
                groups[2].groupSide = Side::RIGHT;

                groups[1].rooms.clear();
                for(auto &roomIdTop : corridorRoomsIds)
                {
                    if(roomIdTop == roomIdLeft || roomIdTop == roomIdRight)continue;
                    groups[1].rooms.push_back(roomIdTop);
                    groups[1].groupSide = Side::TOP;
                }
                groupsDivisions.push_back(groups);
            }
        }
    }

    else if(groupSize == 2)
    {
        for(auto &roomId : corridorRoomsIds)
        {
            //0 --> Top
            vector<Side> corridorSides = {Side::LEFT};
            for (auto &side : corridorSides)
            {
                if(side == blockSide) continue;

                groups[0].rooms.push_back(roomId);
                groups[0].groupSide = side;

                for(auto &roomIdTop : corridorRoomsIds)
                {
                    if(roomIdTop == roomId)continue;
                    groups[1].rooms.push_back(roomIdTop);
                    groups[1].groupSide = Side::TOP;
                }

                groupsDivisions.push_back(groups);

                groups[0].rooms.clear();
                groups[1].rooms.clear();
            }
        }
    }

    else if(groupSize == 1)
    {
        for(auto &roomId : corridorRoomsIds)
        {
            //0 --> Top
            vector<Side> corridorSides = {Side::TOP };
            int index = 0;
            for (auto &side : corridorSides)
            {
                if(side == blockSide) continue;

                groups[index].rooms.push_back(roomId);
                groups[index].groupSide = side;
                groupsDivisions.push_back(groups);
                groups[index].rooms.pop_back();
            }
        }
    }

    return groupsDivisions;
}

vector<Design>
TopAlignedRoomCorridorDimensionsCalculator::getGroupsShapes(int index, int room, vector<Group> &groups, Design &design,
                                                            Design &newDesign) {
    return vector<Design>();
}

vector<Design>
TopAlignedRoomCorridorDimensionsCalculator::getGroupsShapesTwoSide(int index, int room, vector<Group> &groups,
                                                                   Design &design, Design &newDesign) {
    return vector<Design>();
}

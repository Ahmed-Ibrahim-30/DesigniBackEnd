//
// Created by ahmed Ibrahim on 02-Dec-24.
//

#include "HorizontalCorridorDimensionsCalculator.h"

HorizontalCorridorDimensionsCalculator::HorizontalCorridorDimensionsCalculator(const Design &inputDesign,
                                                                               const string &corridorName,
                                                                               Side _blockSide)
        : CorridorDimensionsCalculator(inputDesign, corridorName, _blockSide) {

    maxCorridorWidth = 14;
    maxCorridorHeight = 1.5;

    if(blockSide == Side::RIGHT)
    {
        blockSide = Side::LEFT;
        rotatedDegree = 180;
    }
    else if(blockSide == Side::TOP)
    {
        blockSide = Side::LEFT;
        rotatedDegree = 90;
    }
    else if(blockSide == Side::BOTTOM)
    {
        blockSide = Side::LEFT;
        rotatedDegree = 90;
    }

}

vector<Design>HorizontalCorridorDimensionsCalculator::ConnectCorridorRooms(Design &outputDesign) {
    vector<Design> designs;

    if(rotatedDegree != 0.0)
    {
        outputDesign.rotate(rotatedDegree);
        // designs.push_back(outputDesign);
        // return designs;
    }

    vector <Group> groups (3);

    vector<vector<Group>> groupsDivisions  = getGroupDivisions(0 , groups);

    groupsDivisions = sortGroupDivisions(groupsDivisions);

    // cout<<"Divisions Size = "<<groupsDivisions.size()<<"\n";
    // for(auto &grs : groupsDivisions)
    // {
    //     cout<<"Group Solution --> \n";
    //     for(auto &g : grs) g.print();
    //     cout<<"\n";
    // }

     for(auto &div : groupsDivisions)
     {
        Design newDesign;
        vector<Design> shapes = getGroupsShapes(0 , div , outputDesign , newDesign);
        designs.insert(designs.end() , shapes.begin() , shapes.end());
        break;
     }

     if(designs.empty())
     {
         groups = vector<Group>(2);
         groupsDivisions  = getTwoGroupDivisions(0 , groups);
         groupsDivisions = sortGroupDivisions(groupsDivisions);


        for(auto &div : groupsDivisions)
        {
            Design newDesign;
            vector<Design> shapes = getGroupsShapesTwoSide(0 ,0, div , outputDesign , newDesign);
            designs.insert(designs.end() , shapes.begin() , shapes.end());
            break;
        }
     }

    return designs;
}

vector<vector<HorizontalCorridorDimensionsCalculator::Group>>
HorizontalCorridorDimensionsCalculator::getGroupDivisions(int roomNum, vector<Group> &groups) {
    vector<vector<Group>> groupsDivisions;
    bool isGroupFoundedEmpty = groups[0].rooms.empty() || groups[1].rooms.empty() || groups[2].rooms.empty() ;

    if(roomNum == corridorRoomsIds.size())
    {
        if(!isGroupFoundedEmpty)groupsDivisions.push_back(groups);
        return groupsDivisions;
    }
    string roomId = corridorRoomsIds[roomNum];

    //0 --> left
    //1 -->Top
    //2 --> Bottom
    vector<Side> corridorSides = {Side::LEFT , Side::TOP , Side::BOTTOM , Side::RIGHT};
    int index = 0;
    for (auto &side : corridorSides)
    {
        if(side == blockSide) continue;

        //Right and Left Side must include one Room Only
        if((side == Side::LEFT || side == Side::RIGHT) && !groups[index].rooms.empty())
        {
            index ++ ;
            continue;
        }

        groups[index].rooms.push_back(roomId);
        groups[index].groupSide = side;

        vector<vector<Group>>gs = getGroupDivisions(roomNum + 1 , groups);
        groupsDivisions.insert(groupsDivisions.begin() , gs.begin() , gs.end());

        groups[index].rooms.pop_back();

        index++;
    }
    return groupsDivisions;
}

vector<vector<HorizontalCorridorDimensionsCalculator::Group>>
HorizontalCorridorDimensionsCalculator::getTwoGroupDivisions(int roomNum, vector<Group> &groups) {
    vector<vector<Group>> groupsDivisions;
    bool isGroupFoundedEmpty = groups[0].rooms.empty() || groups[1].rooms.empty()  ;

    if(roomNum == corridorRoomsIds.size())
    {
        if(!isGroupFoundedEmpty)groupsDivisions.push_back(groups);
        return groupsDivisions;
    }
    string roomId = corridorRoomsIds[roomNum];

    //0 -->Top
    //1 --> Bottom
    vector<Side> corridorSides = { Side::TOP , Side::BOTTOM};
    int index = 0;
    for (auto &side : corridorSides)
    {
        if(side == blockSide) continue;

        groups[index].rooms.push_back(roomId);
        groups[index].groupSide = side;

        vector<vector<Group>>gs = getTwoGroupDivisions(roomNum + 1 , groups);
        groupsDivisions.insert(groupsDivisions.begin() , gs.begin() , gs.end());

        groups[index].rooms.pop_back();

        index++;
    }
    return groupsDivisions;
}

vector<Design> HorizontalCorridorDimensionsCalculator::getGroupsShapes(int groupNum, vector<Group> &groups, Design &mainDesign , Design &newDesign) {
    vector<Design> shapes;

    if(groupNum == 3)
    {
       Design fixedShape = newDesign;
       Design newMainDesign = mainDesign;
       Room corridor;
        
       double corridorX = 0.0 , corridorY = 0.0;
       for(auto &room : mainDesign.getRooms())
       {
           if(room.getRoomId() == corridorName)
           {
               corridorX = room.getWidth();
               corridorY = room.getHeight();
               corridor = room;
               break;
           }
       }

       vector<vector<Room>> groupRooms (3);
       int sizeGroup = 0 , group1Size = (int)groups[0].rooms.size() ;
       for (int i = 0; i < newDesign.getRooms().size(); ++i)
       {
           if(newDesign.getRooms()[i].getRoomId() == corridorName)continue;

           if(i == newDesign.getRooms().size() - 1 )
           {
               groupRooms[2].push_back(newDesign.getRooms()[i]);
           }
           else
           {
               if(sizeGroup < group1Size)
               {
                   groupRooms[0].push_back(newDesign.getRooms()[i]);
               }

               else
               {
                   groupRooms[1].push_back(newDesign.getRooms()[i]);
               }
               sizeGroup++;
           }
       }

       vector<Room>template2Rooms;
       double topRoomsX = 0.0 , bottomRoomsX = 0.0;
       for(auto &room : groupRooms[0] )
       {
           topRoomsX += room.getWidth();
       }
       for(auto &room : groupRooms[1] )
       {
           bottomRoomsX += room.getWidth();
       }

        //Template1
       double xValue=0.0;
       RoomBuilder roomBuilder;
       bool isShapeValid = false;
        //template 1
        template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));

        xValue = corridor.getX1();
        //top rooms
        for(auto &room : groupRooms[0] )
        {
            RoomBuilder roomBuilder2;
            template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() ,corridor.getY1()));
            xValue += room.getWidth();
        }

        xValue = ((corridor.getWidth()/2) + corridor.getX1()) - bottomRoomsX/2;
        for(auto &room : groupRooms[1] )
        {
            RoomBuilder roomBuilder2;
            template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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

        //Template 2
        template2Rooms.clear();
        newMainDesign = mainDesign;

        template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));

        //top Rooms
        xValue = corridor.getX2();
        for(auto &room : groupRooms[0] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY1()-room.getHeight() , xValue , corridor.getY1()));
            xValue -= room.getWidth();
        }

        //Bottom Rooms
        xValue = corridor.getX1();
        for(auto &room : groupRooms[1] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
        // Template 3
        template2Rooms.clear();
        newMainDesign = mainDesign;

        template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));


        //top rooms
        xValue = corridor.getX1();
        for(auto &room : groupRooms[0] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
            xValue += room.getWidth();
        }

        //bottom rooms
        xValue = corridor.getX1();
        for(auto &room : groupRooms[1] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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


        // Template 4
        template2Rooms.clear();
        newMainDesign = mainDesign;

        template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));


        xValue = corridor.getX1();
        for(auto &room : groupRooms[0] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
            xValue += room.getWidth();
        }

        xValue = corridor.getX2();
        for(auto &room : groupRooms[1] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY2() , xValue , corridor.getY2() + room.getHeight()));
            xValue -= room.getWidth();
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

        //template5
        if(topRoomsX > bottomRoomsX)
        {
            template2Rooms.clear();
            newMainDesign = mainDesign;

            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));

            xValue = ((corridor.getWidth()/2) + corridor.getX1()) - topRoomsX/2;
            //top rooms
            for(auto &room : groupRooms[0] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
                xValue += room.getWidth();
            }

            //bottom rooms
            xValue = corridor.getX1();
            for(auto &room : groupRooms[1] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
        }

        //template6
        if(topRoomsX > bottomRoomsX)
        {
            template2Rooms.clear();
            newMainDesign = mainDesign;

            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));

            xValue = corridor.getX1();
            for(auto &room : groupRooms[0] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
                xValue += room.getWidth();
            }

            //bottom rooms
            xValue = corridor.getX1();
            for(auto &room : groupRooms[1] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
        }

        //template7
        if(bottomRoomsX > topRoomsX)
        {
            template2Rooms.clear();
            newMainDesign = mainDesign;

            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));

            //top rooms
            xValue = corridor.getX1();
            for(auto &room : groupRooms[0] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
                xValue += room.getWidth();
            }

            xValue = corridor.getX2();
            for(auto &room : groupRooms[1] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY2() , xValue , corridor.getY2() + room.getHeight()));
                xValue -= room.getWidth();
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
        }

        //template8
        if(topRoomsX > bottomRoomsX)
        {
            template2Rooms.clear();
            newMainDesign = mainDesign;

            template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));

            //top rooms
            xValue = corridor.getX2();
            for(auto &room : groupRooms[0] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY1()-room.getHeight() , xValue , corridor.getY1()));
                xValue -= room.getWidth();
            }

            //bottoms rooms
            xValue = corridor.getX1();
            for(auto &room : groupRooms[1] )
            {
                template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
        }

       //template9
       if(topRoomsX == bottomRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;

           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));

           //top Items
           xValue = corridor.getX1();
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
               xValue += room.getWidth();
           }

           xValue = corridor.getX1();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       }

        return shapes;
    }

    Group group = groups[groupNum];

    vector<Room> mainRooms = newDesign.getRooms();

    //Group 1 is Top
    if(groupNum == 0)
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
        if(MinY <= MaxY) //then there are shared Y between Rooms
        {
            for (double Y = MinY; Y <= MaxY; ++Y)
            {
                double startX = 0.0;
                vector<Room> mainRooms = newDesign.getRooms();
                for(auto &room : group.rooms)
                {
                    roomId = room;
                    area   = roomsIDSArea[roomId];
                    double X = round((area / Y) * 10) / 10;
                    RoomBuilder roomBuilder;
                    newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));
                    startX = 0 + X;
                }

                vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
                if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
                newDesign.setRooms( mainRooms);
            }
        }
        else
        {
            double startX = 0.0;
            vector<Room> mainRooms = newDesign.getRooms();
            for(auto &room : group.rooms)
            {
                roomId = room;
                area   = roomsIDSArea[roomId];
                double X = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
                double Y = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

                RoomBuilder roomBuilder;
                newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));

                startX = 0 + X;
            }

            vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
            newDesign.setRooms( mainRooms );
        }
    }

    //Group 1 is Bottom
    else if(groupNum == 1)
    {
        string roomId = group.rooms[0];
        double area = roomsIDSArea[roomId];
        double MaxY = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
        double MinY = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

        for(auto &room : group.rooms)
        {
            area   = roomsIDSArea[room];
            MinY   = max(MinY , AreaToDimensionsCalculator::getMinValueFromArea(area , roomId));
            MaxY   = min(MaxY , AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId));
        }

        if(MinY <= MaxY) //then there are shared Y between Rooms
        {
            for (double Y = MinY; Y <= MaxY; ++Y)
            {
                double startX = 0.0;
                vector<Room> mainRooms = newDesign.getRooms();
                for(auto &room : group.rooms)
                {
                    roomId = room;
                    area   = roomsIDSArea[roomId];
                    double X = round((area / Y) * 10) / 10;
                    RoomBuilder roomBuilder;
                    newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));
                    startX = 0 + X;
                }
                vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
                if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
                newDesign.setRooms(mainRooms);
            }
        }
        else
        {
            double startX = 0.0;
            vector<Room> mainRooms = newDesign.getRooms();
            for(auto &room : group.rooms)
            {
                roomId = room;
                area   = roomsIDSArea[roomId];
                double X = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
                double Y = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);
                RoomBuilder roomBuilder;
                newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));
                startX = 0 + X;
            }

            vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign , newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
            newDesign.setRooms(mainRooms);
        }
    }

    //Group 1 is Right
    else if(groupNum == 2)
    {
        //before set Right Rooms We must calculate corridor Width First
        string firstRoomInGroupTwo = groups[1].rooms[0];
        double minCorridorX = 0 , maxCorridorX = maxCorridorWidth;
        double group2MinX = 0 , group2MaxX = 0.0;
        double group3MinX = 0, group3MaxX = 0.0;
        for (int i =  0 ; i < newDesign.getRooms().size(); ++i)
        {
            double width = newDesign.getRooms()[i].getWidth();
            if(newDesign.getRooms()[i].getRoomId() == firstRoomInGroupTwo || group3MinX != 0.0)
            {
                group3MaxX = min(group3MaxX + width , maxCorridorWidth);
                if(i + 1 != newDesign.getRooms().size() || groups[2].rooms.size()==1)group3MinX +=width;
                else group3MinX += 1.5;
            }
            else
            {
                group2MaxX = min(group2MaxX + width , maxCorridorWidth);
                if(newDesign.getRooms()[i+1].getRoomId() != firstRoomInGroupTwo)group2MinX +=width;
                else group2MinX += 1.5;
            }
        }
        minCorridorX = min ( max(group2MinX , group3MinX) , maxCorridorWidth );
        maxCorridorX = min(group2MaxX , group3MaxX);

        if(group2MaxX < minCorridorX ||  group3MaxX < minCorridorX)return shapes;

        vector<double> corridorXs = {minCorridorX};
        if(group2MaxX >= maxCorridorX &&  group3MaxX >= maxCorridorX)corridorXs.push_back(maxCorridorX);

        for(auto &X : corridorXs)
        {
            mainDesign.updateRoomDimensions(corridorName , X , maxCorridorHeight);

            string roomId = group.rooms[0];
            double area = roomsIDSArea[roomId];
            double height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
            double width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

            RoomBuilder roomBuilder2;
            newDesign.addRoom(roomBuilder2.createRoom(roomId , 0 , 0 ,  width , height));
            vector<Design> newShapes = getGroupsShapes(groupNum + 1 , groups , mainDesign ,newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());

            newDesign.setRooms(mainRooms);
        }
    }

    return shapes;
}


vector<Design> HorizontalCorridorDimensionsCalculator::getGroupsShapes(int groupNum, int roomNum, vector<Group> &groups, Design &mainDesign , Design &newDesign) {
    vector<Design> shapes;

    if(groupNum == 3)
    {
       Design newMainDesign = mainDesign;
       Room corridor;

       double corridorX = 0.0 , corridorY = 0.0;
       for(auto &room : mainDesign.getRooms())
       {
           if(room.getRoomId() == corridorName)
           {
               corridorX = room.getWidth();
               corridorY = room.getHeight();
               corridor = room;
               break;
           }
       }

       vector<vector<Room>> groupRooms (3);
       int sizeGroup = 0 , group1Size = (int)groups[0].rooms.size() ;
       for (int i = 0; i < newDesign.getRooms().size(); ++i)
       {
           if(newDesign.getRooms()[i].getRoomId() == corridorName)continue;

           if(i == newDesign.getRooms().size() - 1 )
           {
               groupRooms[2].push_back(newDesign.getRooms()[i]);
           }
           else
           {
               if(sizeGroup < group1Size)
               {
                   groupRooms[0].push_back(newDesign.getRooms()[i]);
               }

               else
               {
                   groupRooms[1].push_back(newDesign.getRooms()[i]);
               }
               sizeGroup++;
           }
       }

       vector<Room>template2Rooms;
       double topRoomsX = 0.0 , bottomRoomsX = 0.0;
       for(auto &room : groupRooms[0] )
       {
           topRoomsX += room.getWidth();
       }
       for(auto &room : groupRooms[1] )
       {
           bottomRoomsX += room.getWidth();
       }

       double xValue=0.0;
       RoomBuilder roomBuilder;
       bool isShapeValid = false;

       //template 1
       template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));

       xValue = corridor.getX1();
       //top rooms
       for(auto &room : groupRooms[0] )
       {
           RoomBuilder roomBuilder2;
           template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() ,corridor.getY1()));
           xValue += room.getWidth();
       }

       xValue = ((corridor.getWidth()/2) + corridor.getX1()) - bottomRoomsX/2;
       for(auto &room : groupRooms[1] )
       {
           RoomBuilder roomBuilder2;
           template2Rooms.emplace_back(roomBuilder2.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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


       //Template 2
       template2Rooms.clear();
       newMainDesign = mainDesign;
       template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));
       //top Rooms
       xValue = corridor.getX2();
       for(auto &room : groupRooms[0] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY1()-room.getHeight() , xValue , corridor.getY1()));
           xValue -= room.getWidth();
       }
       //Bottom Rooms
       xValue = corridor.getX1();
       for(auto &room : groupRooms[1] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       // Template 3
       template2Rooms.clear();
       newMainDesign = mainDesign;
       template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY2() - groupRooms[2][0].getHeight() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY2()));
       //top rooms
       xValue = corridor.getX1();
       for(auto &room : groupRooms[0] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
           xValue += room.getWidth();
       }
       //bottom rooms
       xValue = corridor.getX1();
       for(auto &room : groupRooms[1] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       // Template 4
       template2Rooms.clear();
       newMainDesign = mainDesign;
       template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));
       xValue = corridor.getX1();
       for(auto &room : groupRooms[0] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
           xValue += room.getWidth();
       }
       xValue = corridor.getX2();
       for(auto &room : groupRooms[1] )
       {
           template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY2() , xValue , corridor.getY2() + room.getHeight()));
           xValue -= room.getWidth();
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
       //template5
       if(topRoomsX > bottomRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;
           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));
           xValue = ((corridor.getWidth()/2) + corridor.getX1()) - topRoomsX/2;
           //top rooms
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
               xValue += room.getWidth();
           }
           //bottom rooms
           xValue = corridor.getX1();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       }
       //template6
       if(topRoomsX > bottomRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;
           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , corridor.getY1() , corridor.getX2() + groupRooms[2][0].getWidth() , corridor.getY1() + groupRooms[2][0].getHeight()));
           xValue = corridor.getX1();
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
               xValue += room.getWidth();
           }
           //bottom rooms
           xValue = corridor.getX1();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       }
       //template7
       if(bottomRoomsX > topRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;
           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));
           //top rooms
           xValue = corridor.getX1();
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
               xValue += room.getWidth();
           }
           xValue = corridor.getX2();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY2() , xValue , corridor.getY2() + room.getHeight()));
               xValue -= room.getWidth();
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
       }
       //template8
       if(topRoomsX > bottomRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;
           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));
           //top rooms
           xValue = corridor.getX2();
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue - room.getWidth() , corridor.getY1()-room.getHeight() , xValue , corridor.getY1()));
               xValue -= room.getWidth();
           }
           //bottoms rooms
           xValue = corridor.getX1();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       }
       //template9
       if(topRoomsX == bottomRoomsX)
       {
           template2Rooms.clear();
           newMainDesign = mainDesign;

           template2Rooms.emplace_back(roomBuilder.createRoom(groupRooms[2][0].getRoomId() , corridor.getX2() , ((corridor.getHeight()/2) + corridor.getY1()) -  (groupRooms[2][0].getHeight() / 2), corridor.getX2() + groupRooms[2][0].getWidth() , ((corridor.getHeight()/2) + corridor.getY1()) +  (groupRooms[2][0].getHeight() / 2)));

           //top Items
           xValue = corridor.getX1();
           for(auto &room : groupRooms[0] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
               xValue += room.getWidth();
           }

           xValue = corridor.getX1();
           for(auto &room : groupRooms[1] )
           {
               template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
       }

        return shapes;
    }

    Group group = groups[groupNum];
    string roomId = group.rooms[roomNum];
    double area = roomsIDSArea[roomId];

    double maxV = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
    double minV = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

    vector<Room> mainRooms = newDesign.getRooms();

    if(groupNum == 0 || groupNum == 1)
    {
        for (double X = minV; X <= maxV; ++X)
        {
            double Y = round((area / X) * 10) / 10;
            RoomBuilder roomBuilder;
            newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));

            int newRoomIndex = (roomNum + 1)% (int)groups[groupNum].rooms.size();
            int newGroupNum = roomNum +1 == groups[groupNum].rooms.size() ? groupNum+1 : groupNum;

            vector<Design> newShapes = getGroupsShapes(newGroupNum , newRoomIndex , groups , mainDesign , newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
            newDesign.setRooms(mainRooms);
        }
    }

    //Group 1 is Right
    else if(groupNum == 2)
    {
        //before set Right Rooms We must calculate corridor Width First
        string firstRoomInGroupTwo = groups[1].rooms[0];
        double minCorridorX = 0 , maxCorridorX = maxCorridorWidth;
        double group2MinX = 0 , group2MaxX = 0.0;
        double group3MinX = 0, group3MaxX = 0.0;
        for (int i =  0 ; i < newDesign.getRooms().size(); ++i)
        {
            double width = newDesign.getRooms()[i].getWidth();
            if(newDesign.getRooms()[i].getRoomId() == firstRoomInGroupTwo || group3MinX != 0.0)
            {
                group3MaxX = min(group3MaxX + width , maxCorridorWidth);
                if(i + 1 != newDesign.getRooms().size() || groups[2].rooms.size()==1)group3MinX +=width;
                else group3MinX += 1.5;
            }
            else
            {
                group2MaxX = min(group2MaxX + width , maxCorridorWidth);
                if(newDesign.getRooms()[i+1].getRoomId() != firstRoomInGroupTwo)group2MinX +=width;
                else group2MinX += 1.5;
            }
        }
        minCorridorX = min ( max(group2MinX , group3MinX) , maxCorridorWidth );
        maxCorridorX = min(group2MaxX , group3MaxX);

        if(group2MaxX < minCorridorX ||  group3MaxX < minCorridorX)return shapes;

        vector<double> corridorXs = {minCorridorX};
        if(group2MaxX >= maxCorridorX &&  group3MaxX >= maxCorridorX)corridorXs.push_back(maxCorridorX);

        for(auto &X : corridorXs)
        {
            mainDesign.updateRoomDimensions(corridorName , X , maxCorridorHeight);

            double height = AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
            double width = AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

            int newRoomIndex = (roomNum + 1)% (int)groups[groupNum].rooms.size();
            int newGroupNum = roomNum +1 == groups[groupNum].rooms.size() ? groupNum+1 : groupNum;

            RoomBuilder roomBuilder2;
            newDesign.addRoom(roomBuilder2.createRoom(roomId , 0 , 0 ,  width , height));
            vector<Design> newShapes = getGroupsShapes(newGroupNum , newRoomIndex , groups , mainDesign ,newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());

            newDesign.setRooms(mainRooms);
        }
    }

    return shapes;
}

vector<vector<HorizontalCorridorDimensionsCalculator::Group>> HorizontalCorridorDimensionsCalculator::sortGroupDivisions(vector<vector<Group>> groupDivisions) {
    vector<pair<double , vector<Group>>> sortGroups;

    for(auto &group : groupDivisions)
    {
        double sumAreaG1 = 0.0 , sumAreaG2 = 0.0;
        for(auto &room : group[0].rooms)sumAreaG1+=roomsIDSArea[room];
        for(auto &room : group[1].rooms)sumAreaG2+=roomsIDSArea[room];

        double diffArray = max(sumAreaG1 , sumAreaG2) - min(sumAreaG1 , sumAreaG2);
        sortGroups.emplace_back(diffArray , group);
    }
    sort(sortGroups.begin(), sortGroups.end());
    vector<vector<Group>> output;
    for(auto &g : sortGroups)
    {
        output.push_back(g.second);
    }
    return output;
}

vector<Design>
HorizontalCorridorDimensionsCalculator::getGroupsShapesTwoSide(int groupNum, int roomNum, vector<Group> &groups,
                                                               Design &mainDesign, Design &newDesign) {
    vector<Design> shapes;

    //Group 1 is Right
    if(groupNum == 2)
    {
        //before set Right Rooms We must calculate corridor Width First
        string firstRoomInGroupTwo = groups[1].rooms[0];
        double minCorridorX = 0 , maxCorridorX = maxCorridorWidth;
        double group2MinX = 0 , group2MaxX = 0.0;
        double group3MinX = 0, group3MaxX = 0.0;
        for (int i =  0 ; i < newDesign.getRooms().size(); ++i)
        {
            double width = newDesign.getRooms()[i].getWidth();
            if(newDesign.getRooms()[i].getRoomId() == firstRoomInGroupTwo || group3MinX != 0.0)
            {
                // group3MaxX = min(group3MaxX + width , maxCorridorWidth);
                group3MaxX += width;
                if(i + 1 != newDesign.getRooms().size() || groups[1].rooms.size()==1)group3MinX +=width;
                else group3MinX += 1.5;
            }
            else
            {
                group2MaxX += width;
                if(newDesign.getRooms()[i+1].getRoomId() != firstRoomInGroupTwo)group2MinX +=width;
                else group2MinX += 1.5;
            }
        }
        minCorridorX = min ( max(group2MinX , group3MinX) , maxCorridorWidth );
        maxCorridorX = min(group2MaxX , group3MaxX);

        vector<double> corridorXs = {group2MaxX , group3MaxX};

        for(auto &X : corridorXs)
        {
            mainDesign.updateRoomDimensions(corridorName , X , maxCorridorHeight);

            vector<Design> newShapes = getGroupsShapesTwoSide(3 , 0 , groups , mainDesign ,newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
        }
        return shapes;
    }

    if(groupNum == 3)
    {
        Design newMainDesign = mainDesign;
        Room corridor;

        double corridorX = 0.0 , corridorY = 0.0;
        for(auto &room : mainDesign.getRooms())
        {
            if(room.getRoomId() == corridorName)
            {
                corridorX = room.getWidth();
                corridorY = room.getHeight();
                corridor = room;
                break;
            }
        }

        vector<vector<Room>> groupRooms (2);
        int sizeGroup = 0 , group1Size = (int)groups[0].rooms.size() ;
        for (int i = 0; i < newDesign.getRooms().size(); ++i)
        {
            if(newDesign.getRooms()[i].getRoomId() == corridorName)continue;

            if(sizeGroup < group1Size)
            {
                groupRooms[0].push_back(newDesign.getRooms()[i]);
            }

            else
            {
                groupRooms[1].push_back(newDesign.getRooms()[i]);
            }
            sizeGroup++;
        }

        vector<Room>template2Rooms;
        double topRoomsX = 0.0 , bottomRoomsX = 0.0;
        for(auto &room : groupRooms[0] )
        {
            topRoomsX += room.getWidth();
        }
        for(auto &room : groupRooms[1] )
        {
            bottomRoomsX += room.getWidth();
        }

        double xValue=0.0;
        RoomBuilder roomBuilder;
        bool isShapeValid = false;

        //top Items
        xValue = corridor.getX1();
        for(auto &room : groupRooms[0] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY1()-room.getHeight() , xValue + room.getWidth() , corridor.getY1()));
            xValue += room.getWidth();
        }

        xValue = corridor.getX1();
        for(auto &room : groupRooms[1] )
        {
            template2Rooms.emplace_back(roomBuilder.createRoom(room.getRoomId() , xValue  , corridor.getY2() , xValue + room.getWidth() , corridor.getY2() + room.getHeight()));
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
    string roomId = group.rooms[roomNum];
    double area = roomsIDSArea[roomId];
    // LOG(LogLevel::Error , "Room Id = " + roomId + " -- Area = "+to_string(area));
    vector<Room> mainRooms = newDesign.getRooms();

    double maxV = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMaxValueFromArea(area , roomId);
    double minV = AreaToDimensionsCalculator::AreaToDimensionsCalculator::getMinValueFromArea(area , roomId);

    if(groupNum == 0 || groupNum == 1)
    {
        for (double X = minV; X <= maxV; ++X)
        {
            double Y = round((area / X) * 10) / 10;
            RoomBuilder roomBuilder;
            newDesign.addRoom(roomBuilder.createRoom(roomId , 0 , 0 ,  X , Y));

            int newRoomIndex = (roomNum + 1)% (int)groups[groupNum].rooms.size();
            int newGroupNum = roomNum +1 == groups[groupNum].rooms.size() ? groupNum+1 : groupNum;

            vector<Design> newShapes = getGroupsShapesTwoSide(newGroupNum , newRoomIndex , groups , mainDesign , newDesign);
            if(!newShapes.empty())shapes.insert(shapes.begin() , newShapes.begin() , newShapes.end());
            newDesign.setRooms(mainRooms);
            if(shapes.size()>0)return shapes;
        }
    }

    return shapes;
}



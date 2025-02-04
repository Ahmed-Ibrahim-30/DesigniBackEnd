//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "CorridorConnectionManager.h"

vector<Design> CorridorConnectionManager::connectCorridors(const Design &design)
{
    vector<Design> designs;

    set<string> corridorsIds;

    Design copyDesign = design;

    //1 - gather all Corridors Ids
    for(auto &room : copyDesign.getRooms())
    {
        if(room.getRoomType() == RoomType::Corridor)
        {
            corridorsIds.insert(room.getRoomId());
        }
    }
    if(corridorsIds.empty())
    {
        LOG(LogLevel::Debug , "No Corridor In This Design");
        return designs;
    }

    //2 - fetch One Room Connected to all Corridors
    string sharedRoomId = "";
    for(auto &room : copyDesign.getConnections())
    {
        string id = room.first;
        set<string> connectionsId = room.second;

        if(corridorsIds.count(id)) continue;

        bool flag = true;
        for(auto &corridor : corridorsIds)
        {
            if(!connectionsId.count(corridor))
            {
                flag = false;
                break;
            }
        }

        if(flag)
        {
            sharedRoomId = id;
            break;
        }
    }

    if(corridorsIds.size()  == 1)
    {
        for(auto &room : copyDesign.getRooms())
        {
            if(room.getRoomType() == RoomType::Living)
            {
                sharedRoomId = room.getRoomId();
                break;
            }
        }
    }


    Room sharedRoom = copyDesign.fetchRoomById(sharedRoomId);
    copyDesign.deleteRoom(sharedRoom.getRoomId());


    //3 - Set Shared Room Dimension
    auto [width , height] = AreaToDimensionsCalculator::setWidthHeightNarrowing(sharedRoom.getArea());
    sharedRoom.setRoomDimensions(width , height , 0 , 0);

    //4 - set All Possible solution for each Corridor
    //If user has one corridor then corridor set in the right of shared Room
    //if User has other corridor then it set in the Top of shared Room
    // vector<Side> orderSides = {Side ::RIGHT , Side ::LEFT, Side ::TOP  , Side::BOTTOM};


    vector<Side> orderSides = {Side ::RIGHT , Side ::LEFT, Side ::TOP  , Side::BOTTOM};
    int index = 0;
    for(const auto &corridorId : corridorsIds)
    {
        Room corridor = copyDesign.fetchRoomById(corridorId);
        designs = connectSharedRoomByCorridor(sharedRoom , corridor , orderSides[index++] , designs);
    }

    //5 - connect associated room to each Corridor

    index = -1;

    for(auto &corridorId : corridorsIds)
    {
        vector<Design> newCorridorDesigns;
        index++;
        // if(index == 0) continue;
        Side blockSide = orderSides[index];
        Side oppositeSide = blockSide == Side::RIGHT ? Side::LEFT : blockSide ==Side::LEFT ?Side::RIGHT : blockSide == Side::TOP ? Side::BOTTOM :Side::TOP;
        corridorDimensionsCalculator = new HorizontalCorridorDimensionsCalculator(copyDesign , corridorId , oppositeSide);
        for(auto &newDesign : designs)
        {
            Design d = newDesign;
            vector<Design> newDesigns = corridorDimensionsCalculator->ConnectCorridorRooms(d);
            // newCorridorDesigns.insert(newCorridorDesigns.end() , newDesigns.begin() , newDesigns.end());

            if(!newDesigns.empty()) newCorridorDesigns.push_back(newDesigns[0]);
        }

        designs = newCorridorDesigns;
    }



    //for Top Down Designs Corridors

    vector<Design> newDesignsTopDowns;
    index = 0;
    orderSides = {Side ::TOP  , Side::BOTTOM};
    for(const auto &corridorId : corridorsIds)
    {
        Room corridor = copyDesign.fetchRoomById(corridorId);
        newDesignsTopDowns = connectSharedRoomByCorridorTop(sharedRoom , corridor , orderSides[index++] , newDesignsTopDowns);
        if(index >=2) break;
    }


    index = 0;
    for(auto &corridorId : corridorsIds)
    {
        vector<Design> newCorridorDesigns;
        Side blockSide = orderSides[index++];
        Side oppositeSide = blockSide == Side::TOP ? Side::BOTTOM :Side::TOP;
        corridorDimensionsCalculator = new TopAlignedRoomCorridorDimensionsCalculator(copyDesign , corridorId , oppositeSide);
        for(auto &newDesign : newDesignsTopDowns)
        {
            Design d = newDesign;
            vector<Design> newDesigns = corridorDimensionsCalculator->ConnectCorridorRooms(d);

            newCorridorDesigns.insert(newCorridorDesigns.end() , newDesigns.begin() , newDesigns.end());
        }

        newDesignsTopDowns = newCorridorDesigns;
    }

    // designs.insert(designs.end() , newDesignsTopDowns.begin() , newDesignsTopDowns.end());

    return designs;

    vector<Design> rightTop ;
    if(!designs.empty())rightTop = designs;

    designs.clear();

    orderSides = {Side ::RIGHT , Side ::LEFT, Side ::TOP  , Side::BOTTOM};


    index = 0;
    for(const auto &corridorId : corridorsIds)
    {
        Room corridor = copyDesign.fetchRoomById(corridorId);
        designs = connectSharedRoomByCorridor(sharedRoom , corridor , orderSides[index++] , designs);
    }

    //5 - connect associated room to each Corridor

    index = -1;

    for(auto &corridorId : corridorsIds)
    {
        vector<Design> newCorridorDesigns;
        index++;
        // if(index == 0) continue;
        Side blockSide = orderSides[index];
        Side oppositeSide = blockSide == Side::RIGHT ? Side::LEFT : blockSide ==Side::LEFT ?Side::RIGHT : blockSide ==Side::TOP ? Side::BOTTOM :Side::BOTTOM;
        corridorDimensionsCalculator = new HorizontalCorridorDimensionsCalculator(copyDesign , corridorId , oppositeSide);
        for(auto &newDesign : designs)
        {
            Design d = newDesign;
            vector<Design> newDesigns = corridorDimensionsCalculator->ConnectCorridorRooms(d);
            // newCorridorDesigns.insert(newCorridorDesigns.end() , newDesigns.begin() , newDesigns.end());
            if(!newDesigns.empty()) newCorridorDesigns.push_back(newDesigns[0]);
        }

        designs = newCorridorDesigns;
    }

    vector<Design> rightLeft ;
    if(!designs.empty())rightLeft = designs;

    designs.clear();

//    for(auto &design : rightTop)
//    {
//        Design d = design;
//        d.mirrorRecX();
//        designs.push_back(d);
//    }

    if(!rightLeft.empty())designs.insert(designs.end() , rightLeft.begin() , rightLeft.end());
//    if(!rightTop.empty())designs.insert(designs.end() , rightTop.begin() , rightTop.end());

    return designs;
}

vector<Design> CorridorConnectionManager::connectSharedRoomByCorridor(const Room &sharedRoom, const Room &corridor, Side &side, vector<Design> &designs)
{
    if(designs.empty())
    {
        Design design; design.addRoom(sharedRoom);
        designs.push_back(design);
    }

    double corridorWidth = corridor.getWidth();
    double corridorHeight = corridor.getHeight();

    if(side == Side::TOP || side == Side::BOTTOM)
    {
        swap(corridorWidth , corridorHeight);
    }

    double sharedX1 = sharedRoom.getX1() , sharedY1 = sharedRoom.getY1();
    double sharedX2 = sharedRoom.getX2() , sharedY2 = sharedRoom.getY2();
    Room newCorridor = corridor;

    vector<Design> newDesigns;

    for(auto &design : designs)
    {
        if(side == Side::RIGHT)
        {
            //Top Right
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX2 , sharedY1);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Center Right
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX2 , (sharedY2-sharedY1)/2 - corridorHeight/2);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Bottom Right
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX2 , sharedY2 - corridorHeight);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);
        }

        else if(side == Side::LEFT)
        {
            //Top LEFT
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX1 - corridorWidth , sharedY1);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Center LEFT
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX1 - corridorWidth , (sharedY2-sharedY1)/2 - corridorHeight/2);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Bottom LEFT
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX1 - corridorWidth , sharedY2 - corridorHeight);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);
        }

        else if (side == Side::TOP)
        {
            //Right Top
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX1 , sharedY1 - corridorHeight);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Center Top
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , ((sharedX1 + sharedX2)/2) - corridorWidth/2 , sharedY1 - corridorHeight);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Left Top
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX2 - corridorWidth , sharedY1 - corridorHeight);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);
        }

        else if(side == Side::BOTTOM)
        {
            //Right BOTTOM
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX1 , sharedY2 );
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Center BOTTOM
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , ((sharedX1 + sharedX2)/2) - corridorWidth/2 , sharedY2);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);

            //Left BOTTOM
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight , sharedX2 - corridorWidth , sharedY2);
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
            design.deleteRoom(newCorridor);
        }

    }

    return newDesigns;
}

vector<Design> CorridorConnectionManager::connectSharedRoomByCorridorTop(const Room &sharedRoom, const Room &corridor, Side &side, vector<Design> &designs)
{
    if(designs.empty())
    {
        Design design; design.addRoom(sharedRoom);
        designs.push_back(design);
    }

    double corridorWidth = corridor.getWidth();
    double corridorHeight = corridor.getHeight();

    double sharedX1 = sharedRoom.getX1() , sharedY1 = sharedRoom.getY1();
    double sharedX2 = sharedRoom.getX2() , sharedY2 = sharedRoom.getY2();
    Room newCorridor = corridor;

    vector<Design> newDesigns;

    for(auto &design : designs)
    {
        if (side == Side::TOP)
        {
            //Right Top
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight,0 , sharedY1 - corridorHeight );
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
        }

        else if(side == Side::BOTTOM)
        {
            //Right Top
            newCorridor.setRoomDimensions(corridorWidth , corridorHeight,0 , sharedY2 );
            design.addRoom(newCorridor);
            newDesigns.push_back(design);
        }
    }
    return newDesigns;
}

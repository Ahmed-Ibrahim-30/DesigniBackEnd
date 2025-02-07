//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_LIVINGROOMSHAPESTRATEGY_H
#define DESIGNI_BACKEND_LIVINGROOMSHAPESTRATEGY_H

#include "RoomShapeStrategy.h"

class LivingRoomShapeStrategy :public RoomShapeStrategy{
public:
    vector<Design> createShapes() override
    {
        vector< Design > shapes;
        vector<Room>l1,l2,l3,l4,l5;
        string imagePath = "Images/LZone/";

        //l1 rectangles 4*4
        l1.emplace_back(RoomType::Living, 0 , 0 , 4 , 4 , 1 , imagePath+"L1/L1/L1-Living4x4.jpg");

        //l2 rectangles 7*4
        l2.emplace_back(RoomType::Kitchen , 0 , 0 , 2 , 4 , 1 , imagePath + "L2/L2/L2-Kitchen2x4.jpg");
        l2.emplace_back(RoomType::Living  , 2 , 0 , 7 , 4 , 1 , imagePath + "L2/L2/L2-Living5x4.jpg");

        //l3 rectangles 9*6
        l3.emplace_back(RoomType::Living   , 0 , 0 , 5 , 4 , 1 , imagePath + "L3/L3/L3-Lving5x4.jpg");
        l3.emplace_back(RoomType::Kitchen  , 5 , 0 , 9 , 3 , 1 , imagePath + "L3/L3/L3-Kitchen4x3.jpg");
        l3.emplace_back(RoomType::Dining   , 5 , 3 , 9 , 6 , 1 , imagePath + "L3/L3/L3-Dining4x3.jpg");
        l3.emplace_back(RoomType::Foyer    , 0 , 4 , 3 , 6 , 1 , imagePath + "L3/L3/L3-Foyer3x2.jpg");
        l3.emplace_back(RoomType::Bathroom , 3 , 4 , 5 , 6 , 1 , imagePath + "L3/L3/L3-Guestbathroom2x2.jpg");


        //l4 rectangles 9*8
        l4.emplace_back(RoomType::Living   , 0.0 , 0.0 , 5.0 , 8.0 , 1 ,imagePath + "L4/L4/L4-Living5x8.jpg");
        l4.emplace_back(RoomType::Kitchen  , 5.0 , 0.0 , 9.0 , 3.0 , 1 ,imagePath + "L4/L4/L4-Kitchen4x3.jpg");
        l4.emplace_back(RoomType::Bathroom , 6.5 , 3.0 , 9.0 , 4.5 , 1 ,imagePath + "L4/L4/L4-Guestbathroom2.5x1.5.jpg");
        l4.emplace_back(RoomType::Corridor , 5.0 , 3.0 , 6.5 , 4.5 , 1 , "");
        l4.emplace_back(RoomType::Dining   , 5.0 , 4.5 , 9.0 , 8.0 , 1 ,imagePath + "L4/L4/L4-Living4x3.5.jpg");

        //l5 rectangles 9 * 9
        l5.emplace_back(RoomType::Living   , 0.0 , 0.0 , 9.0 , 5.0 , 1 , imagePath + "L5/L5/L5-Living9x5.jpg");
        l5.emplace_back(RoomType::Kitchen  , 0.0 , 5.0 , 3.5 , 9.0 , 1 , imagePath + "L5/L5/L5-Kitchen3.5x4.jpg");
        l5.emplace_back(RoomType::Dining   , 3.5 , 5.0 , 7.0 , 9.0 , 1 , imagePath + "L5/L5/L5-Living3.5x4.jpg");
        l5.emplace_back(RoomType::Corridor , 7.0 , 5.0 , 9.0 , 6.0 , 1 ,  "");
        l5.emplace_back(RoomType::Bathroom , 7.0 , 6.0 , 9.0 , 9.0 , 1 , imagePath + "L5/L5/L5-Guestbathroom2x3.jpg");

        shapes.emplace_back("L1",l1);
        shapes.emplace_back("L2",l2);
        shapes.emplace_back("L3",l3);
        shapes.emplace_back("L4",l4);
        shapes.emplace_back("L5",l5);
        return shapes;
    }
};

#endif //DESIGNI_BACKEND_LIVINGROOMSHAPESTRATEGY_H

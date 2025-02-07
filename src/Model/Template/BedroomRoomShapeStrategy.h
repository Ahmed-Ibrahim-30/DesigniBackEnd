//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_BEDROOMROOMSHAPESTRATEGY_H
#define DESIGNI_BACKEND_BEDROOMROOMSHAPESTRATEGY_H
#include "RoomShapeStrategy.h"

class BedroomRoomShapeStrategy : public RoomShapeStrategy{
public:
    vector<Design> createShapes() override
    {
        vector< Design > shapes;
        vector<Room>b1,b2,b3,b4,b5,b6;
        string imagePath = "Images/BZone/";

        //b1 rectangles 7*4
        b1.emplace_back(RoomType::Bedroom   , 0 , 0 , 5 , 4 , 1 , imagePath + "B1/B1-Bedroom5x4.jpg");
        b1.emplace_back(RoomType::Kitchen  , 5 , 0 , 7 , 2 , 1 , imagePath + "B1/B1-Kitchen2x2.jpg");
        b1.emplace_back(RoomType::Bathroom , 5 , 2 , 7 , 4 , 1 , imagePath + "B1/B1-Bathroom2x2.jpg");

        //b2 rectangles 7*4
        b2.emplace_back(RoomType::Bedroom  , 0 , 0 , 5 , 4 , 1 , imagePath +"B2/B2-Bedroom5x4.jpg");
        b2.emplace_back(RoomType::Corridor , 5 , 0 , 7 , 2 , 1 , "");
        b2.emplace_back(RoomType::Bathroom , 5 , 2 , 7 , 4 , 1 , imagePath +"B2/B2-Bathroom2x2.jpg");

        //b3 rectangles 7*4
        b3.emplace_back(RoomType::Bedroom  , 0.0 , 0.0 , 4.0   , 4.0 , 1 , imagePath +"B3/B3-Bedroom4x4.jpg");
        b3.emplace_back(RoomType::Bathroom , 4.0 , 0.0 , 7.0   , 2.0 , 1 , imagePath +"B3/B3-Bathroom3x2.jpg");
        b3.emplace_back(RoomType::Bathroom , 5.5 , 2.0 , 7.0   , 4.0 , 2 , imagePath +"B3/B3-Guestbathroom1.5x2.jpg");
        b3.emplace_back(RoomType::Corridor , 4.0 , 2.0 , 5.5   , 4.0 , 1 , "");

        //b4 rectangles 9*6
        b4.emplace_back(RoomType::Bedroom   , 0   , 0 ,  5  , 4 , 1 , imagePath +"B4/B4-Bedroom5x4.jpg");
        b4.emplace_back(RoomType::Bedroom   , 5   , 0 ,  9  , 6 , 2 , imagePath +"B4/B4-Bedroom4x6.jpg");
        b4.emplace_back(RoomType::Bathroom  , 0   , 4 ,  3  , 6 , 1 , imagePath +"B4/B4-Bathroom3x2.jpg");
        b4.emplace_back(RoomType::Corridor  , 3   , 4 ,  5  , 6 , 1 , "");

        //b5 rectangles 9*8
        b5.emplace_back(RoomType::Bedroom  , 0 , 0 , 4 , 4  , 1 , imagePath +"B5/B5-Bedroom4x4.jpg");
        b5.emplace_back(RoomType::Bedroom  , 4 , 0 , 9 , 4  , 2 , imagePath +"B5/B5-Bedroom5x4.jpg");
        b5.emplace_back(RoomType::Bedroom  , 5 , 4 , 9 , 8  , 3 , imagePath +"B5/B5-Bedroom4x4-right-bottom.jpg");
        b5.emplace_back(RoomType::Bathroom , 0 , 4 , 3 , 6  , 1 , imagePath +"B5/B5-Bathroom3x2.jpg");
        b5.emplace_back(RoomType::Bathroom , 0 , 6 , 4 , 8  , 2 , imagePath +"B5/B5-Bathroom4x2.jpg");
        b5.emplace_back(RoomType::Corridor , 3 , 4 , 5 , 6  , 1 , "");
        b5.emplace_back(RoomType::Corridor , 4 , 6 , 5 , 8  , 2 , "");

        //b6 rectangles 9*11
        b6.emplace_back(RoomType::Bedroom  , 0   , 0   , 4.5 , 4   , 1 , imagePath +"B6/B6-Bedroom4.5x4-top-left.jpg");
        b6.emplace_back(RoomType::Bedroom  , 0   , 6.5 , 4   , 11  , 2 , imagePath +"B6/B6-Bedroom4x4.5-bottom-left.jpg");
        b6.emplace_back(RoomType::Bedroom  , 4.5 , 0   , 9   , 4   , 3 , imagePath +"B6/B6-Bedroom4.5x5-top-right.jpg");
        b6.emplace_back(RoomType::Bedroom  , 5   , 6.5 , 9   , 11  , 4 , imagePath +"B6/B6-Bedroom4x4.5-bottom-right.jpg");
        b6.emplace_back(RoomType::Bathroom , 0   , 4   , 3   , 6.5 , 1 , imagePath +"B6/B6-Bathroom3x2.5-middle-left.jpg");
        b6.emplace_back(RoomType::Bathroom , 6   , 4   , 9   , 6.5 , 2 , imagePath +"B6/B6-Bathroom3x2.5-middle-right.jpg");
        b6.emplace_back(RoomType::Corridor , 3   , 4   , 6   , 6.5 , 1 , "");
        b6.emplace_back(RoomType::Corridor , 4   , 6.5 , 5   , 11  , 2 , "");

        shapes.emplace_back("B1",b1);
        shapes.emplace_back("B2",b2);
        shapes.emplace_back("B3",b3);
        shapes.emplace_back("B4",b4);
        shapes.emplace_back("B5",b5);
        shapes.emplace_back("B6",b6);
        return shapes;
    }
};

#endif //DESIGNI_BACKEND_BEDROOMROOMSHAPESTRATEGY_H

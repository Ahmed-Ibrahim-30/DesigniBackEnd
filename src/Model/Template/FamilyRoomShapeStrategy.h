//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_FAMILYROOMSHAPESTRATEGY_H
#define DESIGNI_BACKEND_FAMILYROOMSHAPESTRATEGY_H

#include "RoomShapeStrategy.h"
class FamilyRoomShapeStrategy : public RoomShapeStrategy{
public:
    vector<Design> createShapes() override{
        vector< Design > shapes;
        vector<Room>a1,a2,a3,a4,a5,a6;
        string imagePath = "Images/AZone/";

        //a1 rectangles 6*4
        a1.emplace_back(RoomType::FamilyRoom      , 0.0 , 0.0 , 6.0 , 4.0 , 1 , imagePath + "A1/A1-Familyroom6x4.jpg");

        //a2 rectangles 6*8
        a2.emplace_back(RoomType::Office      , 0 , 0   , 4.0 , 4.0 , 1 , imagePath + "A2/A2-Office4x4.jpg");
        a2.emplace_back(RoomType::FamilyRoom      , 0 , 4   , 6 , 8.0 , 1 , imagePath + "A2/A2-Familyroom6x4.jpg");
        a2.emplace_back(RoomType::Bathroom    , 4 , 0   , 6 , 2.5 , 1 , imagePath + "A2/A2-Bathroom2x2.5.jpg");
        a2.emplace_back(RoomType::Corridor    , 4 , 2.5 , 6 , 4.0 , 1 , "");

        //a3 rectangles 7 * 9
        a3.emplace_back(RoomType::PlayRoom    , 0 , 0 , 3 , 5 , 1 , imagePath + "A3/A3-Playroom3x5.jpg");
        a3.emplace_back(RoomType::Office      , 3 , 0 , 7 , 4 , 1 , imagePath + "A3/A3-Office4x4.jpg");
        a3.emplace_back(RoomType::Bathroom    , 5 , 5 , 7 , 9 , 1 , imagePath + "A3/A3-Bathroom2x4.jpg");
        a3.emplace_back(RoomType::Corridor    , 3 , 4 , 7 , 5 , 1 ,  "");
        a3.emplace_back(RoomType::FamilyRoom      , 0 , 5 , 5 , 9 , 1 , imagePath + "A3/A3-Familyroom5x4.jpg");

        //a4 rectangles 11 *9
        a4.emplace_back(RoomType::Bathroom    , 4 , 6 , 7  , 9 , 1 , imagePath + "A4/A4-Bathroom3x3.jpg");
        a4.emplace_back(RoomType::Office      , 7 , 5 , 11 , 9 , 1 , imagePath + "A4/A4-Office4x4.jpg");
        a4.emplace_back(RoomType::Corridor    , 4 , 4 , 7  , 6 , 1 , "");
        a4.emplace_back(RoomType::Corridor    , 7 , 4 , 11 , 5 , 2 , "");
        a4.emplace_back(RoomType::MultiPurpose , 0 , 4 , 4  , 9 , 1 , imagePath + "A4/A4-Multipurposeroom4x5.jpg");
        a4.emplace_back(RoomType::PlayRoom    , 0 , 0 , 5  , 4 , 1 , imagePath + "A4/A4-Playroom5x4.jpg");
        a4.emplace_back(RoomType::FamilyRoom      , 5 , 0 , 11 , 4 , 1 , imagePath + "A4/A4-Familyroom6x4.jpg");

        //a5 rectangles 12 * 9
        a5.emplace_back(RoomType::Office      , 0 , 0 , 4  , 4.0 , 1 , imagePath + "A5/A5-Office4x4.jpg");
        a5.emplace_back(RoomType::Bathroom    , 4 , 0 , 6  , 3.0 , 1 , imagePath + "A5/A5-Bathroom2x3.jpg");
        a5.emplace_back(RoomType::HomeTheater     , 0 , 4 , 4  , 9.0 , 1 , imagePath + "A5/A5-Hometheater4x5.jpg");
        a5.emplace_back(RoomType::FamilyRoom      , 6 , 0 , 12 , 4.0 , 1 , imagePath + "A5/A5-Familyroom6x4.jpg");
        a5.emplace_back(RoomType::MultiPurpose , 4 , 5 , 8  , 9.0 , 1 , imagePath + "A5/A5-Multipurposeroom4x4.jpg");
        a5.emplace_back(RoomType::PlayRoom    , 8 , 5 , 12 , 9.0 , 1 , imagePath + "A5/A5-Playroom4x4.jpg");
        a5.emplace_back(RoomType::Corridor    , 4 , 3 , 6  , 5.0 , 1 , "");
        a5.emplace_back(RoomType::Corridor    , 6 , 4 , 12  , 5  , 2 , "");

        //a6 rectangles 14 * 9
        a6.emplace_back(RoomType::SunRoom     , 0   , 0 , 5   , 3 , 1 , imagePath + "A6/A6-Sunroom6x3.jpg");
        a6.emplace_back(RoomType::Bathroom    , 0   , 3 , 3.5 , 5 , 1 , imagePath + "A6/A6-Bathroom3.5x2.jpg");
        a6.emplace_back(RoomType::FamilyRoom      , 5   , 0 , 10  , 5 , 1 , imagePath + "A6/A6-Familyroom5x5.jpg");
        a6.emplace_back(RoomType::Office      , 10  , 0 , 14  , 4 , 1 , imagePath + "A6/A6-Office4x4.jpg");
        a6.emplace_back(RoomType::MultiPurpose , 0   , 5 , 5   , 9 , 1 , imagePath + "A6/A6-Multipurposeroom5x4.jpg");
        a6.emplace_back(RoomType::PlayRoom    , 5   , 5 , 10  , 9 , 1 , imagePath + "A6/A6-Playroom5x4.jpg");
        a6.emplace_back(RoomType::HomeTheater     , 10  , 5 , 14  , 9 , 1 , imagePath + "A6/A6-Hometheater4x4.jpg");
        a6.emplace_back(RoomType::Corridor    , 3.5 , 3 , 5   , 5 , 1 ,  "");
        a6.emplace_back(RoomType::Corridor    , 10  , 4 , 14  , 5 , 2 ,  "");

        shapes.emplace_back("A1",a1);
        shapes.emplace_back("A2",a2);
        shapes.emplace_back("A3",a3);
        shapes.emplace_back("A4",a4);
        shapes.emplace_back("A5",a5);
        shapes.emplace_back("A6",a6);
        return shapes;
    }
};

#endif //DESIGNI_BACKEND_FAMILYROOMSHAPESTRATEGY_H

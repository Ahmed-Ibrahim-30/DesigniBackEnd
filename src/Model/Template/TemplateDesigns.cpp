//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#include "TemplateDesigns.h"


void TemplateDesigns::attachRooms()
{
    RoomShapeStrategy *livingZone  = new LivingRoomShapeStrategy();
    RoomShapeStrategy *bedroomZone = new BedroomRoomShapeStrategy();
    RoomShapeStrategy *familyZone  = new FamilyRoomShapeStrategy();
    vector<Design>livingShapes = livingZone->createShapes();
    vector<Design>bedroomShapes = bedroomZone->createShapes();
    vector<Design>familyShapes = familyZone->createShapes();
    designs.clear();

    B(livingShapes,bedroomShapes,familyShapes);
    B2L1(livingShapes,bedroomShapes,familyShapes);
    B3L2(livingShapes,bedroomShapes,familyShapes);
    B3L3(livingShapes,bedroomShapes,familyShapes);
    B4L3(livingShapes,bedroomShapes,familyShapes);
    B5L4(livingShapes,bedroomShapes,familyShapes);
    B6L4(livingShapes,bedroomShapes,familyShapes);
    B6L4A1(livingShapes,bedroomShapes,familyShapes);
    B6L4A2(livingShapes,bedroomShapes,familyShapes);
    B6L4A3(livingShapes,bedroomShapes,familyShapes);
    B6L5A4(livingShapes,bedroomShapes,familyShapes);
    B6L5A5(livingShapes,bedroomShapes,familyShapes);
    B6L5A6(livingShapes,bedroomShapes,familyShapes);

    cout<<"designs = = "<<designs.size()<<"\n";
    for ( auto &collection : designs){
        double X = collection.getMainDesign().getX2() - collection.getMainDesign().getX1();
        double Y = collection.getMainDesign().getY2() - collection.getMainDesign().getY1();
        if(Y > X) collection.rotate(90) ,collection.setRotatedDegree(90);
    }
}

Design TemplateDesigns::templateInput(int chapterNum, int modelNum, vector<Design>&solutions){
    int cnt = 1;
    for(auto &shape: solutions ){
        if(shape.getId() == to_string(chapterNum)){
            if(modelNum == cnt)return shape;
            cnt++;
        }
    }
    Design rect;
    return rect;
}

vector<Design> TemplateDesigns::templateInput(int chapterNum, vector<Design> &solutions)
{
    vector<Design> ans;

    if(chapterNum < 1 || chapterNum > 45)
    {
        LOG(LogLevel::Debug,"TemplateDesigns::templateInput -- chapterNum out o Bounds");
        return solutions;
    }

    LOG(LogLevel::Debug,"TemplateDesigns::templateInput -- chapterNum = "+to_string(chapterNum));

    for(auto &shape: solutions )
    {
        if(shape.getId() == to_string(chapterNum)){
            ans.push_back(shape);
        }
    }
    return ans;
}

vector<Design> TemplateDesigns::areaInput(double area, vector<Design>&solutions)
{
    vector<Design>ans;

    if(area < 18 || area > 420)
    {
        LOG(LogLevel::Debug,"TemplateDesigns::areaInput -- area out of Bounds");
        return solutions;
    }

    for(auto &shape: solutions )
    {
        if(area >= shape.getMinArea() && area <= shape.getMaxArea())
        {
            Design myShape = shape;
            //myShape.resizeRoom(area);
            ans.push_back(myShape);
        }
    }
    // is ans is empty then take same input ans scaling based on area
    if(ans.empty())
    {
        LOG(LogLevel::Debug,"TemplateDesigns::areaInput -- array is empty");
        for(auto &shape: solutions )
        {
            Design myShape = shape;
            //myShape.resizeRoom(area);
            ans.push_back(myShape);
        }
    }
    return ans;
}

vector<Design> TemplateDesigns::bedroomsInput(int numberOfBedroom, vector<Design> &solutions)
{
    //numberOfRooms from 1 to 9
    vector<Design>ans;

    if(numberOfBedroom < 1 || numberOfBedroom > 9)
    {
        LOG(LogLevel::Debug,"TemplateDesigns::bedroomsInput -- numberOfBedroom out of Bounds");
        return solutions;
    }

    if(numberOfBedroom <= 4)
    {
        for(auto &shape: solutions )
        {
            if(numberOfBedroom == shape.getBedroomsCount() || (numberOfBedroom == 4 && shape.getBedroomsCount() >= 4))
            {
                ans.push_back(shape);
            }
        }
    }
    else
    {
        for(auto &shape: solutions )
        {
            int roomCnt = numberOfBedroom - 4;
            if(shape.getBedroomsCount() == numberOfBedroom)
            {
                if(roomCnt > 0)
                {
                    shape.changeRoomName(RoomType::Office , RoomType::Bedroom , 1 , 5);
                    roomCnt--;
                }
                if(roomCnt > 0)
                {
                    shape.changeRoomName(RoomType::PlayRoom , RoomType::Bedroom , 1 , 6);
                    roomCnt--;
                }
                if(roomCnt > 0)
                {
                    shape.changeRoomName(RoomType::MultiPurpose , RoomType::Bedroom , 1 , 7);
                    roomCnt--;
                }
                if(roomCnt > 0)
                {
                    shape.changeRoomName(RoomType::HomeTheater , RoomType::Bedroom , 1 , 8);
                    roomCnt--;
                }
                if(roomCnt > 0)
                {
                    shape.changeRoomName(RoomType::SunRoom , RoomType::Bedroom , 1 , 9);
                    roomCnt--;
                }
                ans.push_back(shape);
            }
        }
    }

    if(ans.empty())return solutions;
    return ans;
}
vector<Design> TemplateDesigns::spacesInput(int numberOfSpaces, vector<Design>&solutions)
{
    //numberOfRooms from 1 to 12
    vector<Design>ans;

    if(numberOfSpaces == -1)
    {
        return solutions;
    }

    for(auto &shape: solutions )
    {

        int spaces = 0;
        for(auto &room : shape.getRooms())
        {
            if(room.getRoomId().find("corridor")!= string::npos || room.getRoomId().find("Corridor")!= string::npos)
            {
                continue;
            }
            spaces++;
        }

        if(numberOfSpaces == spaces){
            ans.push_back(shape);
        }
    }

    if(ans.empty())
    {
        vector<pair<int,int>> mapIndexToSpaces;
        int index = 0;
        for(auto &shape: solutions )
        {
            int spaces = 0;
            for(auto &room : shape.getRooms())
            {
                if(room.getRoomId().find("corridor")!= string::npos || room.getRoomId().find("Corridor")!= string::npos)
                {
                    continue;
                }
                spaces++;
            }
            if(numberOfSpaces < spaces){
                mapIndexToSpaces.emplace_back(shape.getRooms().size() , index);
            }
            index++;
        }
        if(!mapIndexToSpaces.empty())
        {
            sort(mapIndexToSpaces.begin() , mapIndexToSpaces.end());
            ans.push_back(solutions[mapIndexToSpaces[0].second]);
        }

        if(ans.empty())return solutions;
    }
    return ans;
}

vector<Design> TemplateDesigns::roomsInput(int numberOfRooms, vector<Design>&solutions)
{
    //numberOfRooms from 1 to 12
    vector<Design>ans;

    if(numberOfRooms < 1 || numberOfRooms > 12)
    {
        LOG(LogLevel::Debug,"TemplateDesigns::roomsInput -- numberOfRooms out of Bounds");
        return solutions;
    }


    for(auto &shape: solutions )
    {
        if(numberOfRooms == shape.getRoomsCount()){
            ans.push_back(shape);
        }
    }
    return ans;
}

Design TemplateDesigns::singleRoomsInput(int numberOfRooms, vector<Design>&solutions)
{
    //numberOfRooms from 1 to 12
    vector<Design> ans = roomsInput (numberOfRooms , solutions);
    int ansSize = (int)ans.size();
    srand(time(0));
    Design output = ans [rand() % ansSize];

    return output;
}


void TemplateDesigns::B(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes)
{
    Design bRoom ("1" , bedroomShapes[0].getRooms() , 1 , 0 , 28 , 0 ,36);
    //doors
    bRoom.addDoor    (RoomType::Bedroom      , 3.25 , 4    , 4.25 , 4   );

    bRoom.addDoor    (RoomType::Bathroom    , 5    , 2.5  , 5    , 3.5 );

    //windows
    bRoom.addWindow  (RoomType::Bedroom   , 0 , 1.5  , 0 , 2.5 );
    bRoom.addWindow  (RoomType::Bathroom , 7 , 2.5  , 7 , 3.5 );
    bRoom.addWindow  (RoomType::Kitchen  , 7 , 0.5  , 7 , 1.5 );

    //Opening
    bRoom.addOpening(RoomType::Kitchen , 5    , 0.25 , 5 , 1.75);
    bRoom.addOpening(RoomType::Bedroom  , 5    , 0.25 , 5 , 1.75);

    //bRoom.resizeRoom(36);
    designs.push_back(bRoom);
}

void TemplateDesigns::B2L1(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes)
{
    //B2 L1 --> 3 shapes
    Design B2 = bedroomShapes[1] , L1 = livingShapes[0] ;
    L1.moveDesign({7,0});
    L1.addFurnitureImage(RoomType::Living , 1 , livingImagePath+"L1/LivingAlternatives/L1-M2-2/L1-Living4x4-M2-2.jpg");
    Design result ("2" , B2.getRooms() , 2 , 1 , 44 , 37 ,50);
    for(auto &rec:L1.getRooms())result.addRoom(rec);
    //doors
    result.addDoor(RoomType::Bedroom  , 5   , 1 , 5   , 1.8 );
    //result.addDoor(RoomType::Corridor  , 5   , 1 , 5   , 1.8 );//same previous

    result.addDoor(RoomType::Bathroom , 5.5 , 2 , 6.5 , 2   );
    //result.addDoor(RoomType::Corridor , 5.5 , 2 , 6.5 , 2   );//same previous

    result.addDoor(RoomType::Living   , 8.5 , 0 , 9.5 , 0   );

    //Windows
    result.addWindow(RoomType::Bedroom  , 0   , 1.5 , 0   , 2.5 );
    result.addWindow(RoomType::Bathroom , 5.5 , 4   , 6.5 , 4   );
    result.addWindow(RoomType::Living   , 8.5 , 4   , 9.5 , 4   );

    //Opening
    result.addOpening(RoomType::Living , 7 , 1 , 7 , 1.8 );
    result.addOpening(RoomType::Corridor , 7 , 1 , 7 , 1.8 );

    designs.push_back(result);


    //shape 2
    result.setRooms( B2.getRooms()); L1 = livingShapes[0];
    //update Min and Max area for final two shape
    result.setMinArea( 51)  , result.setMaxArea( 69 ) ;
    result.setId("3");
    L1.moveDesign({3,-4});
    L1.addFurnitureImage(RoomType::Living , 1 , livingImagePath+"L1/LivingAlternatives/L1-M3-2/L1-Living4x4-M3-2.jpg");

    for(auto &rec:L1.getRooms())result.addRoom(rec);

    //replace Corridor by Bathroom 2
    result.changeRoomName(RoomType::Corridor,RoomType::Bathroom , 1 , 2);

    result.addFurnitureImage(RoomType::Bedroom  , 1 , bedroomimagePath + "B2/BedroomAlternatives/B2-Bedroom5x4-1.jpg");
    result.addFurnitureImage(RoomType::Bathroom , 2 , bedroomimagePath + "B2/B2-Bathroom2x2.jpg");


    //doors
    result.addDoor(RoomType::Bedroom  , 3.5   , 0 , 4.5   , 0 );

    result.addDoor(RoomType::Bathroom , 5.5 , 0 , 6.5 , 0 , 2 );

    result.addDoor(RoomType::Bathroom , 5 , 2.5 , 5 , 3.5 , 1 );

    result.addDoor(RoomType::Living   , 3 , -1.5 , 3 , -0.5   );

    //Windows
    result.addWindow(RoomType::Bedroom  , 2   , 4   , 3   , 4       );
    result.addWindow(RoomType::Bathroom , 7   , 0.5 , 7   , 1.5 , 2 );
    result.addWindow(RoomType::Bathroom , 7   , 2.5 , 7   , 3.5 , 1 );
    result.addWindow(RoomType::Living   , 4.5 , -4  , 5.5 , -4      );

    designs.push_back(result);

    //shape3
    result.setRooms( B2.getRooms()); L1 = livingShapes[0];
    result.setId( "4");
    L1.moveDesign({2,-4});
    L1.addFurnitureImage(RoomType::Living , 1 , livingImagePath+"L1/LivingAlternatives/L1-M3-2/L1-Living4x4-M3-2.jpg");
    for(auto &rec:L1.getRooms())result.addRoom(rec);

    //replace Corridor by Bathroom 2
    result.changeRoomName(RoomType::Corridor,RoomType::Bathroom , 1 , 2);

    result.addFurnitureImage(RoomType::Bedroom  , 1 , bedroomimagePath + "B2/BedroomAlternatives/B2-Bedroom5x4-1.jpg");
    result.addFurnitureImage(RoomType::Bathroom , 2 , bedroomimagePath + "B2/B2-Bathroom2x2.jpg");

    //doors
    result.addDoor(RoomType::Bedroom  , 2.5   , 0 , 3.5   , 0 );

    result.addDoor(RoomType::Bathroom , 5.1 , 0 , 5.9 , 0 , 2 );

    result.addDoor(RoomType::Bathroom , 5 , 2.5 , 5 , 3.5 , 1 );

    result.addDoor(RoomType::Living   , 2 , -1.5 , 2 , -0.5   );

    //Windows
    result.addWindow(RoomType::Bedroom  , 2   , 4   , 3   , 4       );
    result.addWindow(RoomType::Bathroom , 7   , 0.5 , 7   , 1.5 , 2 );
    result.addWindow(RoomType::Bathroom , 7   , 2.5 , 7   , 3.5 , 1 );
    result.addWindow(RoomType::Living   , 3.5 , -4  , 4.5 , -4      );

    designs.push_back(result);
}

void TemplateDesigns::B3L2(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex = 2 ,livingIndex = 1 ;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ;
    //Bedroom Doors
    Bedroom.addDoor(RoomType::Bathroom , 4.25 , 2 , 5.25 , 2 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4 , 2.5 , 4 , 3.5 , 1);

    //Bedroom windows
    Bedroom.addWindow(RoomType::Bedroom  , 0 , 1.5 , 0 , 2.5 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 7 , 0.5 , 7 , 1.5 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 7 , 2.5 , 7 , 3.5 , 2);

    //Living
    Living.addDoor  (RoomType::Living   , 2.5 , 4 , 3.5 , 4 , 1);
    //Living windows
    Living.addWindow(RoomType::Living   , 7 , 1.5 , 7 , 2.5 , 1);
    Living.addWindow(RoomType::Kitchen  , 0 , 1.5 , 0 , 2.5 , 1);
    //Living openings
    Living.addOpening(RoomType::Kitchen , 2 , 0.25 , 2 , 3.75);
    Living.addOpening(RoomType::Living  , 2 , 0.25 , 2 , 3.75);

    //shape 1
    Design result ("5" , Bedroom.getRooms() , 2 , 1 , 56 , 51 ,74), LivingTemp = Living;

    LivingTemp.moveDesign({0,4});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    //doors
    result.addDoor(RoomType::Bathroom , 5.7 , 4 , 6.7 , 4 , 2);

    //Opening
    result.addOpening(RoomType::Corridor , 4.2 , 4 , 5.3 , 4);
    result.addOpening(RoomType::Living   , 4.2 , 4 , 5.3 , 4);


    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms());  LivingTemp = Living;
    result.setId( "6");
    LivingTemp.moveDesign({2,4});
    for(auto &rec : LivingTemp.getRooms())result.addRoom(rec);

    //doors
    result.addDoor(RoomType::Bathroom , 5.7 , 4 , 6.7 , 4 , 2);

    //Openings
    result.addOpening(RoomType::Corridor , 4.2 , 4 , 5.3 , 4);
    result.addOpening(RoomType::Living   , 4.2 , 4 , 5.3 , 4);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("7");
    LivingTemp.moveDesign({ -1.5 ,4});
    for(auto &rec : LivingTemp.getRooms())result.addRoom(rec);

    //doors
    result.addDoor(RoomType::Bathroom , 5.5 , 2.5 , 5.5 , 3.5 , 2);

    //Openings
    result.addOpening(RoomType::Corridor , 4.2 , 4 , 5.3 , 4);
    result.addOpening(RoomType::Living   , 4.2 , 4 , 5.3 , 4);


    designs.push_back(result);
}

void TemplateDesigns::B3L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex = 2 , livingIndex = 2 ;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ;
    //Bedroom Doors
    Bedroom.addDoor(RoomType::Bathroom , 4.2 , 2 , 5.0 , 2 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 5.7 , 4.0 , 6.7 , 4.0 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 4 , 2.5 , 4 , 3.5 , 1);

    //Bedroom windows
    Bedroom.addWindow(RoomType::Bedroom  , 0 , 1.5 , 0 , 2.5 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 7 , 0.5 , 7 , 1.5 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 7 , 2.5 , 7 , 3.5 , 2);

    //Living
    //doors
    Living.addDoor  (RoomType::Living , 0.0 , 2.8 , 0.0 , 3.8 , 1);

    Living.addDoor(RoomType::Bathroom , 5.0 , 4.5 , 5.0 , 5.5 , 1);

    //windows
    Living.addWindow(RoomType::Living   , 0.0 , 1.0 , 0.0 , 2.0 , 1);
    Living.addWindow(RoomType::Foyer    , 1.0 , 6.0 , 2.0 , 6.0 , 1);
    Living.addWindow(RoomType::Bathroom , 3.5 , 6.0 , 4.5 , 6.0 , 1);
    Living.addWindow(RoomType::Dining   , 6.5 , 6.0 , 7.5 , 6.0 , 1);

    //Opening
    Living.addOpening(RoomType::Kitchen  , 5.2 , 3.0 , 8.8 , 3.0 , 1);
    Living.addOpening(RoomType::Dining   , 5.2 , 3.0 , 8.8 , 3.0 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.2 , 5.0 , 3.8 , 1);
    Living.addOpening(RoomType::Dining   , 5.0 , 3.2 , 5.0 , 3.8 , 1);

    Living.addOpening(RoomType::Living   , 0.2 , 4.0 , 2.8 , 4.0 , 1);
    Living.addOpening(RoomType::Foyer    , 0.2 , 4.0 , 2.8 , 4.0 , 1);


    Design result ("8" , Bedroom.getRooms() , 3 , 1 , 92 , 75 ,100) , LivingTemp = Living;
    LivingTemp.moveDesign({0,4});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //windows
    result.addWindow(RoomType::Kitchen , 9 , 5.0 , 9 , 6.0 , 1);

    //opening
    result.addOpening(RoomType::Living   , 4.1 , 4.0 , 4.9 , 4.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 4.0 , 4.9 , 4.0 , 1);

    designs.push_back(result);

    //shape 2
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("9");
    LivingTemp.moveDesign({2,4});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //windows
    result.addWindow(RoomType::Kitchen , 11 , 5.0 , 11 , 6.0 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 4.0 , 4.9 , 4.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 4.0 , 4.9 , 4.0 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("10");
    LivingTemp.moveDesign({4,4});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //windows
    result.addWindow(RoomType::Kitchen , 13 , 5.0 , 13 , 6.0 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 4.0 , 4.9 , 4.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 4.0 , 4.9 , 4.0 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B4L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex = 3 , livingIndex = 2;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ;
    //Bedroom
    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.5 , 4.0 , 4.5 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 4.5 , 5.0 , 5.5 , 2);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 4.5 , 3.0 , 5.5 , 1);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 2.0 , 0.0 , 3.0 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 2.5 , 9.0 , 3.5 , 2);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.5 , 0.0 , 5.5 , 1);

    //Living
    //doors
    Living.addDoor  (RoomType::Living , 0.0 , 2.8 , 0.0 , 3.8 , 1);

    Living.addDoor(RoomType::Bathroom , 5.0 , 4.5 , 5.0 , 5.5 , 1);

    //windows
    Living.addWindow(RoomType::Living   , 0.0 , 1.0 , 0.0 , 2.0 , 1);
    Living.addWindow(RoomType::Foyer    , 1.0 , 6.0 , 2.0 , 6.0 , 1);
    Living.addWindow(RoomType::Bathroom , 3.5 , 6.0 , 4.5 , 6.0 , 1);
    Living.addWindow(RoomType::Dining   , 6.5 , 6.0 , 7.5 , 6.0 , 1);
    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0 , 9.0 , 2.0 , 1);

    //Opening
    Living.addOpening(RoomType::Kitchen  , 5.2 , 3.0 , 8.8 , 3.0 , 1);
    Living.addOpening(RoomType::Dining   , 5.2 , 3.0 , 8.8 , 3.0 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.2 , 5.0 , 3.8 , 1);
    Living.addOpening(RoomType::Dining   , 5.0 , 3.2 , 5.0 , 3.8 , 1);

    Living.addOpening(RoomType::Living   , 0.2 , 4.0 , 2.8 , 4.0 , 1);
    Living.addOpening(RoomType::Foyer    , 0.2 , 4.0 , 2.8 , 4.0 , 1);

    Design result ("11" , Bedroom.getRooms() , 4 , 2 , 108 , 101 ,126) , LivingTemp = Living;
    LivingTemp.moveDesign({0,6});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //Opening
    result.addOpening(RoomType::Living   , 3.1 , 6.0 , 4.9 , 6.0 , 1);
    result.addOpening(RoomType::Corridor , 3.1 , 6.0 , 4.9 , 6.0 , 1);

    designs.push_back(result);
    //shape 2
    result.setRooms(Bedroom.getRooms()); LivingTemp = Living;
    result.setId( "12");
    LivingTemp.moveDesign({3,6});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //Opening
    result.addOpening(RoomType::Living   , 3.1 , 6.0 , 4.9 , 6.0 , 1);
    result.addOpening(RoomType::Corridor , 3.1 , 6.0 , 4.9 , 6.0 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("13");
    LivingTemp.moveDesign({1.5,6});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //Opening
    result.addOpening(RoomType::Living   , 3.1 , 6.0 , 4.9 , 6.0 , 1);
    result.addOpening(RoomType::Corridor , 3.1 , 6.0 , 4.9 , 6.0 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B5L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex=4,livingIndex=3;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ;

    //                      Bedroom
    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.1 , 4.0 , 3.9 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.1 , 4.0 , 4.9 , 4.0 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 4.5 , 5.0 , 5.5 , 3);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 4.5 , 3.0 , 5.5 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 3.1 , 6.0 , 3.9 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 5.5 , 9.0 , 6.5 , 3);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.5 , 0.0 , 5.5 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 6.5 , 0.0 , 7.5 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.0 , 5.0 , 6.0 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.0 , 5.0 , 6.0 , 2);

    //                      Living
    //Doors
    Living.addDoor(RoomType::Living  , 0.0 , 4.5 , 0.0 , 5.5 , 1);

    Living.addDoor(RoomType::Bathroom , 6.5 , 3.25 , 6.5 , 4.25 , 1);
    //Windows
    Living.addWindow(RoomType::Living   , 2.0 , 8.0  , 3.0 , 8.0  , 1);
    Living.addWindow(RoomType::Dining   , 6.5 , 8.0  , 7.5 , 8.0  , 1);
    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0  , 9.0 , 2.0  , 1);
    Living.addWindow(RoomType::Bathroom , 9.0 , 3.25 , 9.0 , 4.25 , 1);

    //                      Opening
    Living.addOpening(RoomType::Kitchen  , 5.1 , 3.0 , 6.4 , 3.0 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 3.0 , 6.4 , 3.0 , 1);

    Living.addOpening(RoomType::Dining   , 5.1 , 4.5, 6.4 , 4.5 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 4.5, 6.4 , 4.5 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.3 , 5.0 , 4.2 , 1);
    Living.addOpening(RoomType::Corridor , 5.0 , 3.3 , 5.0 , 4.2 , 1);

    Living.addOpening(RoomType::Living  , 5.0 , 4.7 , 5.0 , 7.7 , 1);
    Living.addOpening(RoomType::Dining  , 5.0 , 4.7 , 5.0 , 7.7 , 1);

    Design result ("14" , Bedroom.getRooms() , 5 , 3 , 144 , 127 ,158) , LivingTemp = Living;
    LivingTemp.moveDesign({0,8});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //Openings
    result.addOpening(RoomType::Living   , 4.1 , 8.0 , 4.9 , 8.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 8.0 , 4.9 , 8.0 , 2);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("15");
    LivingTemp.moveDesign({1,8});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //openings
    result.addOpening(RoomType::Living   , 4.1 , 8.0 , 4.9 , 8.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 8.0 , 4.9 , 8.0 , 2);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId("16");
    LivingTemp.moveDesign({4,8});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //openings
    result.addOpening(RoomType::Living   , 4.1 , 8.0 , 4.9 , 8.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 8.0 , 4.9 , 8.0 , 2);

    designs.push_back(result);
}

void TemplateDesigns::B6L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes){
    //B3 L2 --> 3 shapes
    int bedroomIndex = 5, livingIndex = 3;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ;

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 5.0 , 0.0 , 6.0 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 5.0 , 9.0 , 6.0 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);


    //                      Living
    //Doors
    Living.addDoor(RoomType::Living  , 0.0 , 4.5 , 0.0 , 5.5 , 1);

    Living.addDoor(RoomType::Bathroom , 6.5 , 3.25 , 6.5 , 4.25 , 1);
    //Windows
    Living.addWindow(RoomType::Living   , 2.0 , 8.0  , 3.0 , 8.0  , 1);
    Living.addWindow(RoomType::Dining   , 6.5 , 8.0  , 7.5 , 8.0  , 1);
    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0  , 9.0 , 2.0  , 1);
    Living.addWindow(RoomType::Bathroom , 9.0 , 3.25 , 9.0 , 4.25 , 1);

    //                      Opening
    Living.addOpening(RoomType::Kitchen  , 5.1 , 3.0 , 6.4 , 3.0 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 3.0 , 6.4 , 3.0 , 1);

    Living.addOpening(RoomType::Dining   , 5.1 , 4.5, 6.4 , 4.5 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 4.5, 6.4 , 4.5 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.3 , 5.0 , 4.2 , 1);
    Living.addOpening(RoomType::Corridor , 5.0 , 3.3 , 5.0 , 4.2 , 1);

    Living.addOpening(RoomType::Living  , 5.0 , 4.7 , 5.0 , 7.7 , 1);
    Living.addOpening(RoomType::Dining  , 5.0 , 4.7 , 5.0 , 7.7 , 1);

    Design result ("17" , Bedroom.getRooms() , 6 , 4 , 171 , 159 ,183) , LivingTemp = Living;
    LivingTemp.moveDesign({0,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //Openings
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId( "18");
    LivingTemp.moveDesign({2,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //openings
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living;
    result.setId( "19");
    LivingTemp.moveDesign({4,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);

    //openings
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);
}

void TemplateDesigns::B6L4A1(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes){
    //B3 L2 --> 3 shapes
    int bedroomIndex=5,livingIndex=3,familyIndex=0;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);


    //                      Living

    //Doors

    Living.addDoor(RoomType::Bathroom , 6.5 , 3.25 , 6.5 , 4.25 , 1);
    //Windows

    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0  , 9.0 , 2.0  , 1);
    Living.addWindow(RoomType::Bathroom , 9.0 , 3.25 , 9.0 , 4.25 , 1);

    //                      Opening
    Living.addOpening(RoomType::Kitchen  , 5.1 , 3.0 , 6.4 , 3.0 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 3.0 , 6.4 , 3.0 , 1);

    Living.addOpening(RoomType::Dining   , 5.1 , 4.5, 6.4 , 4.5 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 4.5, 6.4 , 4.5 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.3 , 5.0 , 4.2 , 1);
    Living.addOpening(RoomType::Corridor , 5.0 , 3.3 , 5.0 , 4.2 , 1);

    Living.addOpening(RoomType::Living  , 5.0 , 4.7 , 5.0 , 7.7 , 1);
    Living.addOpening(RoomType::Dining  , 5.0 , 4.7 , 5.0 , 7.7 , 1);

    Design result ("20" , Bedroom.getRooms() , 7 , 4 , 195 , 184 ,207) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-6,15});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);


    //doors
    result.addDoor(RoomType::Living  , 0.0 , 12.5 , 0.0 , 13.5 , 1);
    //windows
    result.addWindow(RoomType::Living  , 2.0 , 19.0 , 3.0 , 19.0 , 1);
    result.addWindow(RoomType::FamilyRoom  , -3.5 , 19.0 , -2.5 , 19.0 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19.0  , 7.5 , 19.0  , 1);

    //Opening
    result.addOpening(RoomType::Living  , 0.0 , 15.2 , 0.0 , 18.8 , 1);
    result.addOpening(RoomType::FamilyRoom  , 0.0 , 15.2 , 0.0 , 18.8 , 1);

    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("21") ;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({0,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //windows
    result.addWindow(RoomType::Living  , 0.0 , 12.5 , 0.0 , 13.5 , 1);
    result.addWindow(RoomType::FamilyRoom  , 2.5 , 23.0 , 3.5 , 23.0 , 1);
    result.addWindow(RoomType::Dining  , 7.0 , 19.0  , 8.0 , 19.0  , 1);

    //doors
    result.addDoor(RoomType::Living  , 0.0 , 15.5 , 0.0 , 16.5 , 1);

    //Opening
    result.addOpening(RoomType::Living  , 0.8 , 19 , 4.2 , 19 , 1);
    result.addOpening(RoomType::FamilyRoom  , 0.8 , 19 , 4.2 , 19 , 1);

    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("22") ;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({2,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //windows
    result.addWindow(RoomType::Living  , 0.0 , 12.5 , 0.0 , 13.5 , 1);
    result.addWindow(RoomType::FamilyRoom  , 4.5 , 23.0 , 5.5 , 23.0 , 1);
    result.addWindow(RoomType::Dining  , 9.0 , 16.75  , 9.0 , 17.75  , 1);

    //doors
    result.addDoor(RoomType::Living  , 0.0 , 15.5 , 0.0 , 16.5 , 1);

    //Opening
    result.addOpening(RoomType::Living  , 2.2 , 19 , 4.8 , 19 , 1);
    result.addOpening(RoomType::FamilyRoom  , 2.2 , 19 , 4.8 , 19 , 1);

    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    designs.push_back(result);
}

void TemplateDesigns::B6L4A2(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex=5,livingIndex=3,familyIndex=1;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);

    //                      Living
    //Doors

    Living.addDoor(RoomType::Bathroom , 6.5 , 3.5 , 6.5 , 4.5 , 1);

    //Windows
    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0  , 9.0 , 2.0  , 1);
    Living.addWindow(RoomType::Bathroom , 9.0 , 3.25 , 9.0 , 4.25 , 1);

    //                      Opening
    Living.addOpening(RoomType::Kitchen  , 5.1 , 3.0 , 6.4 , 3.0 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 3.0 , 6.4 , 3.0 , 1);

    Living.addOpening(RoomType::Dining   , 5.1 , 4.5, 6.4 , 4.5 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 4.5, 6.4 , 4.5 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.3 , 5.0 , 4.2 , 1);
    Living.addOpening(RoomType::Corridor , 5.0 , 3.3 , 5.0 , 4.2 , 1);

    Living.addOpening(RoomType::Living  , 5.0 , 4.7 , 5.0 , 7.7 , 1);
    Living.addOpening(RoomType::Dining  , 5.0 , 4.7 , 5.0 , 7.7 , 1);

    //          FamilyRoom
    //doors
    FamilyRoom.addDoor(RoomType::Bathroom , 4.5 , 2.5 , 5.5 , 2.5 , 1);

    FamilyRoom.addDoor(RoomType::Office   , 4.0 , 2.75 , 4.0 , 3.75 , 1);

    //Windows
    FamilyRoom.addWindow(RoomType::FamilyRoom   , 2.5 , 8.0  , 3.5 , 8.0  , 1);
    FamilyRoom.addWindow(RoomType::Office   , 1.5 , 0.0  , 2.5 , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::Bathroom , 4.5 , 0.0  , 5.5 , 0.0  , 1);

    //                      Opening
    FamilyRoom.addOpening(RoomType::FamilyRoom   , 4.2 , 4.0 , 5.8 , 4.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 4.2 , 4.0 , 5.8 , 4.0 , 1);

    //TODO : you must delete it after Modify IDS
    FamilyRoom.addOpening(RoomType::Corridor , 6.0 , 2.75 , 6.0 , 3.75 , 1);

    Design result ("23" , Bedroom.getRooms() , 8 , 5 , 219 , 208 ,226) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-6,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living  , 1 , 19 , 2 , 19 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19  , 7.5 , 19  , 1);

    result.addDoor  (RoomType::Living  , 3.0 , 19 , 4.0 , 19 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.0 , 13.7 , 0.0 , 14.8 , 1);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("24");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-6,13});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living  , 1 , 19 , 2 , 19 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19  , 7.5 , 19  , 1);

    result.addDoor  (RoomType::Living  , 3.0 , 19 , 4.0 , 19 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.0 , 15.7 , 0.0 , 17.8 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("25");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-6,15});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living  , 1 , 19 , 2 , 19 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19  , 7.5 , 19  , 1);

    result.addDoor  (RoomType::Living  , 3.0 , 19 , 4.0 , 19 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.0 , 17.7 , 0.0 , 19.8 , 1);

    designs.push_back(result);

    //shape4
    result.setRooms( Bedroom.getRooms()); LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("26");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(270);
    FamilyTemp.moveDesign({1,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living   , 0.0 , 12.5 , 0.0   , 13.5  , 1);
    result.addWindow(RoomType::Dining   , 9.0 , 16.75  , 9.0 , 17.75 , 1);

    result.addDoor  (RoomType::Living   , 0.0 , 16.5  , 0.0 , 17.5 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 3.7 , 19 , 4.8 , 19 , 1);

    designs.push_back(result);

    //shape5
    result.setRooms( Bedroom.getRooms());LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("27");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(270);
    FamilyTemp.moveDesign({-2.5,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living   , 0.0 , 12.5 , 0.0   , 13.5  , 1);
    result.addWindow(RoomType::Dining   , 9.0 , 16.75  , 9.0 , 17.75 , 1);

    result.addDoor  (RoomType::Living   , 0.0 , 16.5  , 0.0 , 17.5 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.2 , 19 , 1.3 , 19 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B6L4A3(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes){
    //B3 L2 --> 3 shapes
    int bedroomIndex=5,livingIndex=3,familyIndex=2;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);

    //                      Living
    //Doors

    Living.addDoor(RoomType::Bathroom , 6.5 , 3.25 , 6.5 , 4.25 , 1);

    //Windows
    Living.addWindow(RoomType::Kitchen  , 9.0 , 1.0  , 9.0 , 2.0  , 1);
    Living.addWindow(RoomType::Bathroom , 9.0 , 3.25 , 9.0 , 4.25 , 1);

    //                      Opening
    Living.addOpening(RoomType::Kitchen  , 5.1 , 3.0 , 6.4 , 3.0 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 3.0 , 6.4 , 3.0 , 1);

    Living.addOpening(RoomType::Dining   , 5.1 , 4.5, 6.4 , 4.5 , 1);
    Living.addOpening(RoomType::Corridor , 5.1 , 4.5, 6.4 , 4.5 , 1);

    Living.addOpening(RoomType::Living   , 5.0 , 3.3 , 5.0 , 4.2 , 1);
    Living.addOpening(RoomType::Corridor , 5.0 , 3.3 , 5.0 , 4.2 , 1);

    Living.addOpening(RoomType::Living  , 5.0 , 4.7 , 5.0 , 7.7 , 1);
    Living.addOpening(RoomType::Dining  , 5.0 , 4.7 , 5.0 , 7.7 , 1);

    //          FamilyRoom
    //doors
    FamilyRoom.addDoor(RoomType::PlayRoom , 3.0 , 4.1 , 3.0 , 4.9 , 1);

    FamilyRoom.addDoor(RoomType::Office   , 4.5 , 4.0 , 5.5 , 4.0 , 1);

    FamilyRoom.addDoor(RoomType::Bathroom , 5.5 , 5.0 , 6.5 , 5.0 , 1);

    //Windows
    FamilyRoom.addWindow(RoomType::PlayRoom , 1.0 , 0.0 , 2.0 , 0.0 , 1);
    FamilyRoom.addWindow(RoomType::Office   , 4.5 , 0.0 , 5.5 , 0.0 , 1);
    FamilyRoom.addWindow(RoomType::FamilyRoom   , 2.0 , 9.0 , 3.0 , 9.0 , 1);
    FamilyRoom.addWindow(RoomType::Bathroom , 5.5 , 9.0 , 6.5 , 9.0 , 1);

    //                      Opening
    FamilyRoom.addOpening(RoomType::FamilyRoom   , 3.2 , 5.0 , 4.8 , 5.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 3.2 , 5.0 , 4.8 , 5.0 , 1);

    //TODO you must delete it after Modify Ids
    FamilyRoom.addOpening(RoomType::Corridor , 7.0 , 4.2 , 7.0 , 4.8 , 1);


    Design result ("28" , Bedroom.getRooms() , 9 , 6 , 234 , 227 ,256)  , LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-7,10});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living  , 1 , 19 , 2 , 19 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19  , 7.5 , 19  , 1);

    result.addDoor  (RoomType::Living  , 3.5 , 19 , 4.5 , 19 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.0 , 14.2 , 0.0 , 14.8 , 1);

    designs.push_back(result);

    //shape2
    result.setRooms( Bedroom.getRooms());  LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("29");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-7,14});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living  , 1 , 19 , 2 , 19 , 1);
    result.addWindow(RoomType::Dining   , 6.5 , 19  , 7.5 , 19  , 1);

    result.addDoor  (RoomType::Living  , 3.5 , 19 , 4.5 , 19 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.0 , 18.2 , 0.0 , 18.8 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms());  LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("30");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(270);
    FamilyTemp.moveDesign({0,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living   , 0.0 , 12.5 , 0.0   , 13.5  , 1);
    result.addWindow(RoomType::Dining   , 9.0 , 16.75  , 9.0 , 17.75 , 1);

    result.addDoor  (RoomType::Living   , 0.0 , 16.5  , 0.0 , 17.5 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 4.2 , 19 , 4.8 , 19 , 1);

    designs.push_back(result);

    //shape4
    result.setRooms( Bedroom.getRooms());  LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("31");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(270);
    FamilyTemp.moveDesign({-4,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living   , 0.0 , 12.5 , 0.0   , 13.5  , 1);
    result.addWindow(RoomType::Dining   , 9.0 , 16.75  , 9.0 , 17.75 , 1);

    result.addDoor  (RoomType::Living   , 0.0 , 16.5  , 0.0 , 17.5 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 0.2 , 19 , 0.8 , 19 , 1);

    designs.push_back(result);

    //shape5
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("32");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(270);
    FamilyTemp.moveDesign({-2,19});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    result.addWindow(RoomType::Living   , 0.0 , 12.5 , 0.0   , 13.5  , 1);
    result.addWindow(RoomType::Dining   , 9.0 , 16.75  , 9.0 , 17.75 , 1);

    result.addDoor  (RoomType::Living   , 0.0 , 16.5  , 0.0 , 17.5 , 1);

    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    //Opening with FamilyRoom
    result.addOpening(RoomType::Living   , 2.2 , 19 , 2.8 , 19 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B6L5A4(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex = 5 , livingIndex = 4 , familyIndex = 3;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);

    //                      Living
    //Doors

    Living.addDoor(RoomType::Kitchen  , 1.5 , 5.0 , 2.5 , 5.0 , 1);

    Living.addDoor(RoomType::Dining   , 5.5 , 5.0 , 6.5 , 5.0 , 1);

    Living.addDoor(RoomType::Bathroom , 7.5 , 6.0 , 8.5 , 6.0 , 1);

    //Windows
    Living.addWindow(RoomType::Kitchen  , 1.5 , 9.0 , 2.5 , 9.0 , 1);
    Living.addWindow(RoomType::Dining   , 4.5 , 9.0 , 5.5 , 9.0 , 1);
    Living.addWindow(RoomType::Bathroom , 7.5 , 9.0 , 8.5 , 9.0 , 1);

    //                      Opening
    Living.addOpening(RoomType::Living   , 7.2 , 5.0 , 8.8 , 5.0 , 1);
    Living.addOpening(RoomType::Corridor , 7.2 , 5.0 , 8.8 , 5.0 , 1);

    //          FamilyRoom
    //doors
    FamilyRoom.addDoor(RoomType::PlayRoom , 4.1 , 4.0 , 4.9 , 4.0 , 1);

    FamilyRoom.addDoor(RoomType::MultiPurpose , 4.0 , 4.5 , 4.0 , 5.5 , 1);

    FamilyRoom.addDoor(RoomType::Bathroom , 5.0 , 6.0  , 6.0 , 6.0  , 1);

    FamilyRoom.addDoor(RoomType::Office   , 8.5 , 5.0  , 9.5 , 5.0  , 1);

    //Windows
    FamilyRoom.addWindow(RoomType::PlayRoom    , 2.0 , 0.0  , 3.0 , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::FamilyRoom      , 7.5 , 0.0  , 8.5 , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::MultiPurpose , 1.5 , 9.0  , 2.5 , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::Bathroom    , 5.0 , 9.0  , 6.0 , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::Office      , 8.5 , 9.0  , 9.5 , 9.0  , 1);

    //                      Opening
    FamilyRoom.addOpening(RoomType::FamilyRoom   , 7.2 , 4.0 , 10.8 , 4.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 7.2 , 4.0 , 10.8 , 4.0 , 2);

    FamilyRoom.addOpening(RoomType::Corridor , 7.0 , 4.0 , 7.0 , 5.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 7.0 , 4.0 , 7.0 , 5.0 , 2);

    //TODO : you must delete it after Modify IDS
    FamilyRoom.addOpening(RoomType::Corridor , 11.0 , 4.2 , 11.0 , 4.8 , 2);


    Design result ("33" , Bedroom.getRooms() , 10 , 7 , 279 , 257 ,284), LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-11,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //Opening
    result.addWindow  (RoomType::Living , 9.0 , 12 , 9.0 , 13 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);

    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("34");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({-11,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 9.0 , 12 , 9.0 , 13 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("35");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

    designs.push_back(result);

    //shape4
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("36");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B6L5A5(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes)
{
    //B3 L2 --> 3 shapes
    int bedroomIndex = 5 , livingIndex = 4 , familyIndex = 4;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);

    //                      Living
    //Doors

    Living.addDoor(RoomType::Kitchen  , 1.5 , 5.0 , 2.5 , 5.0 , 1);

    Living.addDoor(RoomType::Dining   , 4.5 , 5.0 , 5.5 , 5.0 , 1);

    Living.addDoor(RoomType::Bathroom , 7.5 , 6.0 , 8.5 , 6.0 , 1);

    //Windows
    Living.addWindow(RoomType::Kitchen  , 1.5 , 9.0 , 2.5 , 9.0 , 1);
    Living.addWindow(RoomType::Dining   , 4.7 , 9.0 , 5.7 , 9.0 , 1);
    Living.addWindow(RoomType::Bathroom , 7.5 , 9.0 , 8.5 , 9.0 , 1);

    //                      Opening
    Living.addOpening(RoomType::Living   , 7.2 , 5.0 , 8.8 , 5.0 , 1);
    Living.addOpening(RoomType::Corridor , 7.2 , 5.0 , 8.8 , 5.0 , 1);

    //          FamilyRoom
    //doors
    FamilyRoom.addDoor(RoomType::PlayRoom , 9.5 , 5.0 , 10.5 , 5.0 , 1);

    FamilyRoom.addDoor(RoomType::MultiPurpose , 6.5 , 5.0 , 7.5 , 5.0 , 1);

    FamilyRoom.addDoor(RoomType::Bathroom ,  4.5 , 3.0  , 5.5  , 3.0 , 1);

    FamilyRoom.addDoor(RoomType::Office   , 4.0 , 3.1 , 4.0  , 3.9  , 1);

    FamilyRoom.addDoor(RoomType::HomeTheater  , 4.0 , 4.1 , 4.0  , 4.9 , 1);

    //Windows
    FamilyRoom.addWindow(RoomType::Office      , 1.5 , 0.0  , 2.5  , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::Bathroom    , 4.5 , 0.0  , 5.5  , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::FamilyRoom      , 8.5 , 0.0  , 9.5  , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::HomeTheater     , 1.5 , 9.0  , 2.5  , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::MultiPurpose , 5.5 , 9.0  , 6.5  , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::PlayRoom    , 9.5 , 9.0  , 10.5 , 9.0  , 1);


    //                      Opening
    FamilyRoom.addOpening(RoomType::FamilyRoom   , 6.4 , 4.0 , 11.6 , 4.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 6.4 , 4.0 , 11.6 , 4.0 , 2);

    FamilyRoom.addOpening(RoomType::Corridor , 6.0 , 4.0 , 6.0 , 5.0 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 6.0 , 4.0 , 6.0 , 5.0 , 2);

    //TODO : you must delete it after Modify IDS
    FamilyRoom.addOpening(RoomType::Corridor , 12.0 , 4.2 , 12.0 , 4.8 , 2);


    Design result ("37" , Bedroom.getRooms() , 11 , 8 , 288 , 285 ,297) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-12,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);
    //doors and Windows
    result.addWindow  (RoomType::Living , 9.0 , 12.0 , 9.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);
    //shape 2
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("38");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({-12,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 9.0 , 12.0 , 9.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("39");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

    designs.push_back(result);

    //shape4
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("40");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

    designs.push_back(result);
}

void TemplateDesigns::B6L5A6(const vector<Design> &livingShapes, const vector<Design> &bedroomShapes, const vector<Design> &familyShapes){
    //B3 L2 --> 3 shapes
    int bedroomIndex = 5 , livingIndex = 4 , familyIndex = 5;
    Design Bedroom = bedroomShapes[bedroomIndex] , Living = livingShapes[livingIndex] ,
            FamilyRoom=familyShapes[familyIndex];

    //doors
    Bedroom.addDoor(RoomType::Bedroom  , 3.25 , 4.0 , 4.25 , 4.0 , 1);

    Bedroom.addDoor(RoomType::Bedroom  , 4.75 , 4.0 , 5.75 , 4.0 , 3);

    Bedroom.addDoor(RoomType::Bedroom  , 4.0 , 8.5 , 4.0 , 9.5 , 2);

    Bedroom.addDoor(RoomType::Bedroom  , 5.0 , 8.5 , 5.0 , 9.5 , 4);

    Bedroom.addDoor(RoomType::Bathroom , 3.0 , 5.0 , 3.0 , 6.0 , 1);

    Bedroom.addDoor(RoomType::Bathroom , 6.0 , 5.0 , 6.0 , 6.0 , 2);

    //Windows
    Bedroom.addWindow(RoomType::Bedroom  , 1.5 , 0.0 , 2.5 , 0.0 , 1);
    Bedroom.addWindow(RoomType::Bedroom  , 6.0 , 0.0 , 7.0 , 0.0 , 3);
    Bedroom.addWindow(RoomType::Bedroom  , 0.0 , 8.5 , 0.0 , 9.5 , 2);
    Bedroom.addWindow(RoomType::Bedroom  , 9.0 , 8.5 , 9.0 , 9.5 , 4);
    Bedroom.addWindow(RoomType::Bathroom , 0.0 , 4.75 , 0.0 , 5.75 , 1);
    Bedroom.addWindow(RoomType::Bathroom , 9.0 , 4.75 , 9.0 , 5.75 , 2);

    //Opening
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 1);
    Bedroom.addOpening(RoomType::Corridor , 4.0 , 6.5 , 5.0 , 6.5 , 2);

    //                      Living
    //Doors

    Living.addDoor(RoomType::Kitchen  , 1.5 , 5.0 , 2.5 , 5.0 , 1);

    Living.addDoor(RoomType::Dining   , 5.5 , 5.0 , 6.5 , 5.0 , 1);

    Living.addDoor(RoomType::Bathroom , 7.5 , 6.0 , 8.5 , 6.0 , 1);

    //Windows
    Living.addWindow(RoomType::Kitchen  , 1.5 , 9.0 , 2.5 , 9.0 , 1);
    Living.addWindow(RoomType::Dining   , 4.7 , 9.0 , 5.7 , 9.0 , 1);
    Living.addWindow(RoomType::Bathroom , 7.5 , 9.0 , 8.5 , 9.0 , 1);

    //                      Opening
    Living.addOpening(RoomType::Living   , 7.2 , 5.0 , 8.8 , 5.0 , 1);
    Living.addOpening(RoomType::Corridor , 7.2 , 5.0 , 8.8 , 5.0 , 1);

    //          FamilyRoom
    //doors
    FamilyRoom.addDoor(RoomType::PlayRoom , 7.0 , 5.0 , 8.0 , 5.0 , 1);

    FamilyRoom.addDoor(RoomType::MultiPurpose , 3.5 , 5.0 , 4.5 , 5.0 , 1);

    FamilyRoom.addDoor(RoomType::SunRoom     , 3.5 , 3.0 , 4.5 , 3.0 , 1);

    FamilyRoom.addDoor(RoomType::Bathroom ,  3.5 , 3.5  , 3.5  , 4.5  , 1);

    FamilyRoom.addDoor(RoomType::Office   , 11.5 , 4.0 , 12.5  , 4.0  , 1);

    FamilyRoom.addDoor(RoomType::HomeTheater  , 11.5 , 5.0 , 12.5  , 5.0 , 1);

    //Windows
    FamilyRoom.addWindow(RoomType::SunRoom     , 0.2  , 0.0  , 4.8  , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::SunRoom     , 0.0  , 0.2  , 0.0  , 2.8  , 1);
    FamilyRoom.addWindow(RoomType::FamilyRoom      , 7.0  , 0.0  , 8.0  , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::Office      , 11.5 , 0.0  , 12.5 , 0.0  , 1);
    FamilyRoom.addWindow(RoomType::Bathroom    , 0.0 , 3.5  , 0.0  , 4.5   , 1);
    FamilyRoom.addWindow(RoomType::MultiPurpose , 2.0  , 9.0  , 3.0  , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::PlayRoom    , 7.0  , 9.0  , 8.0  , 9.0  , 1);
    FamilyRoom.addWindow(RoomType::HomeTheater     , 11.5 , 9.0  , 12.5 , 9.0  , 1);


    //                      Opening
    FamilyRoom.addOpening(RoomType::FamilyRoom   , 5.0 , 4.2 , 5.0 , 4.8 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 5.0 , 4.2 , 5.0 , 4.8 , 1);

    FamilyRoom.addOpening(RoomType::FamilyRoom   , 10.0 , 4.2 , 10.0 , 4.8 , 1);
    FamilyRoom.addOpening(RoomType::Corridor , 10.0 , 4.2 , 10.0 , 4.8 , 2);

    //TODO : you must delete it after Modify IDS
    FamilyRoom.addOpening(RoomType::Corridor , 14.0 , 4.2 , 14.0 , 4.8 , 2);


    Design result ("41" , Bedroom.getRooms() , 12 , 9 , 306 , 298 ,420) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    LivingTemp.moveDesign({0,11});
    FamilyTemp.moveDesign({-14,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 9.0 , 12.0 , 9.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);

    //shape2
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("42");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({-14,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 9.0 , 12.0 , 9.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 9.0 , 14.2 , 9.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living , 0.0 , 15.2 , 0.0 , 15.8 , 1);

    designs.push_back(result);

    //shape3
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("43");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.mirrorRecX();
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

    designs.push_back(result);

    //shape4
    result.setRooms( Bedroom.getRooms()) , LivingTemp = Living , FamilyTemp = FamilyRoom;
    result.setId("44");
    LivingTemp.moveDesign({0,11});
    FamilyTemp.rotate(180);
    FamilyTemp.moveDesign({9,11});
    for(auto &rec:LivingTemp.getRooms())result.addRoom(rec);
    for(auto &rec:FamilyTemp.getRooms())result.addRoom(rec);

    //doors and Windows
    result.addWindow  (RoomType::Living , 0.0 , 12.0 , 0.0 , 13.0 , 1);
    result.addDoor    (RoomType::Living , 0.0 , 14.2 , 0.0 , 15.2 , 1);
    //Opening
    result.addOpening(RoomType::Living   , 4.1 , 11.0 , 4.9 , 11.0 , 1);
    result.addOpening(RoomType::Corridor , 4.1 , 11.0 , 4.9 , 11.0 , 2);

    result.addOpening(RoomType::Living   , 9.0 , 15.2 , 9.0 , 15.8 , 1);

//    result.res(400);
    designs.push_back(result);
}


void TemplateDesigns::rePositionBasedOnScreen(double screenWidth, double screenHeight, vector<Design> &solutions)
{
    for(auto &design : solutions)
    {
        for(int unit = 100 ; unit >= 10 ; unit -=5)
        {
            Design copyDesign = design;
            copyDesign.scaleDesign(unit);
            double width = copyDesign.getMainDesign().getX2() - copyDesign.getMainDesign().getX1();
            double height = copyDesign.getMainDesign().getY2() - copyDesign.getMainDesign().getY1();
            if(width > screenWidth || height > screenHeight)continue;
            design.scaleDesign(unit);
            double centerScreenX = screenWidth/2;
            double centerScreenY = screenHeight/2;
            design.moveDesign({centerScreenX - width/2 , centerScreenY - height/2});
            break;
        }
    }
}
void TemplateDesigns::rePositionBasedOnScreen(double unit,double screenWidth, double screenHeight, vector<Design> &solutions)
{
    for(auto &design : solutions)
    {
        Design copyDesign = design;
        copyDesign.scaleDesign(unit);
        double width = copyDesign.getMainDesign().getX2() - copyDesign.getMainDesign().getX1();
        double height = copyDesign.getMainDesign().getY2() - copyDesign.getMainDesign().getY1();
        design.scaleDesign(unit);
        double centerScreenX = screenWidth/2;
        double centerScreenY = screenHeight/2;
        design.moveDesign({centerScreenX - width/2 , centerScreenY - height/2});
    }
}

map < string , vector<double> > TemplateDesigns::calculateHomeDimensions(const Design &home)
{

    map < string , vector<double> > dimensions;

    set<double>visitedX; vector<double>X;
    set<double>visitedY; vector<double>Y;

    //calculate North Dimensions
    for(auto &shape : home.getRooms() )
    {
        double x1 = shape.getX1() , y1 = shape.getY1() , x2 = shape.getX2() ;

        bool flag1 = true;
        for(auto &shape2 : home.getRooms() )
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;

            if(x1 > shape2.getX1() && x1 < shape2.getX2() && shape2.getY2() <= y1)
            {
                flag1=false;
                break;
            }
        }

        double valueX = x1;
        if(flag1 && !visitedX.count(valueX))visitedX.emplace(valueX),X.push_back(valueX);
        flag1=true;
        for(auto &shape2: home.getRooms())
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;

            if(x2>shape2.getX1() && x2<shape2.getX2()&& shape2.getY2()<=y1)
            {
                flag1=false;
                break;
            }
        }
        valueX = x2;
        if(flag1 && !visitedX.count(valueX))visitedX.emplace(valueX),X.push_back(valueX);
    }
    sort(X.begin(),X.end());
    vector<double>finalX ;
    for (int var = 1; var < X.size(); ++var) {
        finalX.push_back(X[var] - X[var-1]);
    }
    dimensions["North"] = finalX;

    X.clear(); visitedX.clear();

    //calculate South Dimensions
    for(auto &shape : home.getRooms())
    {
        double x1 = shape.getX1() , y2 = shape.getY2() , x2 = shape.getX2();

        Point a1(x1,y2) , a2(x2,y2);
        bool flag1 = true;
        for(auto &shape2: home.getRooms())
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;
            if(a1.getX()>shape2.getX1() && a1.getX()<shape2.getX2() && shape2.getY1()>=a1.getY()){

                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedX.count(a1.getX()))visitedX.emplace(a1.getX()),X.push_back(a1.getX());

        flag1=true;
        for(auto &shape2: home.getRooms())
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;
            if(a2.getX()>shape2.getX1() && a2.getX()<shape2.getX2() && shape2.getY1()>=a2.getY())
            {
                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedX.count(a2.getX()))visitedX.emplace(a2.getX()),X.push_back(a2.getX());
    }
    sort(X.begin(),X.end());
    finalX.clear();
    for (int var = 1; var < X.size(); ++var) {
        finalX.push_back(X[var] - X[var-1]);
    }
    dimensions["South"] = finalX;

    //calculate East Dimensions

    for(auto &shape : home.getRooms())
    {
        double x2 = shape.getX2() , y1 = shape.getY1() ,y2 = shape.getY2();
        Point a1(x2,y1) , a2(x2,y2);
        bool flag1 = true;
        for(auto &shape2: home.getRooms())
        {
            if(shape.getRoomId() == shape2.getRoomId())continue;
            if(a1.getY()>shape2.getY1() && a1.getY()<shape2.getY2() && shape2.getX1()>=a1.getX())
            {
                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedY.count(a1.getY()))visitedY.emplace(a1.getY()),Y.push_back(a1.getY());
        flag1=true;
        for(auto &shape2 : home.getRooms()){
            if(shape.getRoomId()==shape2.getRoomId())continue;
            if(a2.getY()>shape2.getY1() && a2.getY()<shape2.getY2() && shape2.getX1()>=a1.getX())
            {
                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedY.count(a2.getY()))visitedY.emplace(a2.getY()),Y.push_back(a2.getY());
    }
    sort(Y.begin(),Y.end());
    vector<double>finalY ;
    for (int var = 1; var < Y.size(); ++var) {
        finalY.push_back(Y[var] - Y[var-1]);
    }
    dimensions["East"] = finalY;

    Y.clear(); visitedY.clear();
    //calculate West Dimensions
    for(auto &shape : home.getRooms())
    {
        double x1 = shape.getX1() , y1 = shape.getY1() ,y2 = shape.getY2();
        Point a1(x1,y1) , a2(x1,y2);

        bool flag1 = true;
        for(auto &shape2 : home.getRooms())
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;

            if(a1.getY()>shape2.getY1() && a1.getY()<shape2.getY2() && shape2.getX2()<=a1.getX())
            {
                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedY.count(a1.getY()))visitedY.emplace(a1.getY()),Y.push_back(a1.getY());
        flag1=true;
        for(auto &shape2 : home.getRooms())
        {
            if(shape.getRoomId()==shape2.getRoomId())continue;
            if(a2.getY()>shape2.getY1() && a2.getY()<shape2.getY2() && shape2.getX2()<=a2.getX())
            {
                flag1=false;
                break;
            }
        }
        if(flag1 && !visitedY.count(a2.getY()))visitedY.emplace(a2.getY()),Y.push_back(a2.getY());
    }
    sort(Y.begin(),Y.end());
    finalY.clear();
    for (int var = 1; var < Y.size(); ++var) {
        finalY.push_back(Y[var] - Y[var-1]);
    }
    dimensions["West"] = finalY;

    //divide on Unit = 100
    double unit = 100;
    for(auto &dim : dimensions)
    {
        for(auto &val : dim.second)val/=unit;
    }

    return dimensions;
}

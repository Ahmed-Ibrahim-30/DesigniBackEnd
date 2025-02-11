//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_TEMPLATEDESIGNS_H
#define DESIGNI_BACKEND_TEMPLATEDESIGNS_H
#include "LivingRoomShapeStrategy.h"
#include "BedroomRoomShapeStrategy.h"
#include "FamilyRoomShapeStrategy.h"

class TemplateDesigns {
public:
    vector<Design> designs;
    void attachRooms();

    Design templateInput(int chapterNum , int modelNum , vector<Design>&solutions);
    vector<Design> templateInput(int chapterNum , vector<Design>&solutions);

    /**
     * Get all solution match number of rooms
     * @param numberOfRooms
     * @param solutions
     * @return
     */
    vector<Design> roomsInput(int numberOfRooms, vector<Design>&solutions);

    /**
     * @brief spacesInput
     * @param numberOfSpaces
     * @param solutions
     * @return
     */
    vector<Design> spacesInput(int numberOfSpaces, vector<Design>&solutions);

    /**
     * Get random single Rooms Input
     * @param numberOfRooms
     * @param solutions
     * @return random single solution
     */
    Design singleRoomsInput(int numberOfRooms, vector<Design>&solutions);

    vector<Design> bedroomsInput(int numberOfBedroom, vector<Design>&solutions);
    vector<Design> areaInput(double area, vector<Design>&solutions);


    void rePositionBasedOnScreen(double screenWidth , double screenHeight , vector<Design>&solutions);

    void rePositionBasedOnScreen(double unit ,double screenWidth, double screenHeight, vector<Design>&solutions);

private:
    void B(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B2L1(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B3L2(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B3L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B4L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B5L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A1(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A2(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A5(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A6(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);

    const string familyImagePath = "Images/AZone/";
    const string livingImagePath = "Images/LZone/";
    const string bedroomimagePath= "Images/BZone/";

    /**
     * Calculate Home Dimensions for North and South and East and West
     * @param home
     */
    map < string , vector<double> > calculateHomeDimensions(const Design &home);
};


#endif //DESIGNI_BACKEND_TEMPLATEDESIGNS_H

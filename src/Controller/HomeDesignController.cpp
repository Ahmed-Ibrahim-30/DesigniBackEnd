//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "HomeDesignController.h"
#include "src/View/JsonExtraction.h"
#include "src/Model/Polygon.h"
#include "src/Model/DesignWallGenerator.h"
#include "src/Model/DesignGeometryManager.h"
#include "src/Model/Land/Land.h"
#include "src/Model/PolygonLibrary/PolygonsSubtraction.h"
#include "src/Model/Land/Green Area/UniformGreenDistributor.h"
#include "src/Model/Land/Green Area/ClusteredGreenSelector.h"
#include "src/Model/Land/Green Area/CentralLandGreenSelector.h"

HomeDesignController::HomeDesignController()
{

    designProcess();

//    showDesignResult();

    //test Roof
    // vector<Point> polygonP2 {
    //     {100, 300},
    //     {200, 200},
    //     {400, 200},
    //     {500, 300}
    // };

     vector<Point> polygonP5 {
         {500, 200},
         {1000, 200},
         {1000, 500},
         {500, 500}
     };

     vector<Point> polygonP {
         {800, 500},
         {1000, 700},
         {800, 1000},
         {500, 700}
     };

    // vector<Point> polygonP2 {
    //     {100, 700},
    //     {300, 400},
    //     {400, 300},
    //     {500, 300},
    //     {600, 500},
    //     {700, 500},
    //     {700, 700}
    // };

    // vector<Point> polygonP2 {
    //     {24, 30},
    //     {54, 60},
    //     {84, 100},
    //     {54, 120},
    //     {6 , 120}
    // };

    // vector<Point> polygonP3 {
    //     {20, 20},
    //     {50, 20},
    //     {50, 50},
    //     {20, 50},
    // };

   vector<Point> polygonP3 {
           {20, 20},
           {50, 20},
           {50, 60},
           {100, 60},
           {100, 80},
           {20, 80},
   };

    // vector<Point> polygonP2 {
    //     {80, 20},
    //     {300, 20},
    //     {430, 200},
    //     {300, 300},
    //     {40, 300},
    //     {0, 200},
    // };

   vector<Point> polygonP2 {
       {300, 20},
       {1250, 20},
       {1530, 580},
       {1250, 1040},
       {300, 1040},
       {20, 580},
   };

    // vector<Point> polygonP2 {
    //     {10, 200},
    //     {60, 100},
    //     {160, 100},
    //     {110, 200},
    //     {300, 300},
    //     {300, 500},
    //     {180 , 600}
    // };

   vector<Point> polygonP4 {
       {200, 200},
       {600, 200},
       {600, 900},
       {200, 900},
   };

//    Design curr = templatesDesigns.getDesigns()[3];
//    curr.scaleDesign(10);
//    curr.shiftDesignX(80);

//    Polygon1 p (polygonP) , p2 (polygonP2) , p3(polygonP3) , p4(polygonP2) , p5 (polygonP5) , p6(polygonP4);
    // DesignWallGenerator::constructRoof(p , 15);

    // PolygonsSubtraction PolygonsSubtraction1(p4 , p5);

    // vector<Polygon1> pols = PolygonsSubtraction1.getNewPolygons();


//    double area = p4.getArea();

//    cout<<"Area = "<<area<<"\n";


//    Land land(p6);
//    land.SplitLand(22 , 1 , 1);

    // land.SplitLand(curr);
}

void HomeDesignController::initiateDesign() {
    userInputView = new ConstInputView();
    design = userInputView->getUserInput();
    designs.push_back(design);
}

void HomeDesignController::designProcess()
{
    //store 18 model2 design shapes
    templatesDesigns = extractRealTemplateDesign();

//    Design curr = templatesDesigns.getDesigns()[0];
//    vector<Wall> walls = curr.getWallFraming();

    initiateDesign();
}

DesignRepository HomeDesignController::extractRealTemplateDesign()
{
    vector<Design> temDesignsAll;
    DesignRepository temDesigns;

    vector<Design> templatesDesignsData = storeTemplateDesigns();

    int index = 0;

    for(auto &template1 : templatesDesignsData)
    {
        index++;
        if(index > 8)continue;
        DesignGenerator designGenerator1;
        vector<Design>ds = designGenerator1.generateDesign(template1);

        /**
         * For only bedrooms >4
         */
        vector<Design> designsBedrooms;

        for(auto &temD : ds)
        {
            temD.setSpacesCount(template1.getSpacesCount());
            temD.setRoomsCount(template1.getRoomsCount());
            temD.setBedroomsCount(template1.getBedroomsCount());
            temD.setArea(template1.getArea());
            temD.setConnections(template1.getConnections());
        }

        //generate new Designs to number of bedrooms >4
        int size = (int)ds.size();
        for(int i = 0; i < size ; i++)
        {
            Design temp = ds[i];
            if(temp.getBedroomsCount() <= 4) continue;

            int bedrooms = temp.getBedroomsCount();

            if(bedrooms > 4) temp.changeRoomName(RoomType::Office , "Bedroom5") , bedrooms--;

            if(bedrooms > 4) temp.changeRoomName(RoomType::PlayRoom , "Bedroom6") , bedrooms--;

            if(bedrooms > 4) temp.changeRoomName(RoomType::MultiPurpose, "Bedroom7") , bedrooms--;

            if(bedrooms > 4) temp.changeRoomName(RoomType::SunRoom , "Bedroom8") , bedrooms--;

            if(bedrooms > 4) temp.changeRoomName(RoomType::HomeTheater , "Bedroom9") , bedrooms--;

            designsBedrooms.push_back(temp);
            ds[i].setBedroomsCount(4);
        }

        for(auto &temD : ds)temDesigns.addDesign(temD);

        if(!designsBedrooms.empty())
        {
            for(auto &temD : designsBedrooms)temDesigns.addDesign(temD);
        }

        // if(index != 15) continue;
        if(!ds.empty())temDesignsAll.insert(temDesignsAll.end() , ds.begin() , ds.end());
        if(!designsBedrooms.empty())temDesignsAll.insert(temDesignsAll.end() , designsBedrooms.begin() , designsBedrooms.end());
    }

    designs = temDesignsAll;

    return temDesigns;
}


void HomeDesignController::showDesignResult() {
//    designResultView = new ConsoleResultView();


    designResultView->displayDesigns(designs);
}

vector<Design> HomeDesignController::storeTemplateDesigns() {
    return TemplateRepository::templatesCreations();
}








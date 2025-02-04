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

HomeDesignController::HomeDesignController()
{

    designProcess();

    showDesignResult();

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

    Design curr = templatesDesigns.getDesigns()[3];
    curr.scaleDesign(10);
    curr.shiftDesignX(80);

    Polygon1 p (polygonP) , p2 (polygonP2) , p3(polygonP3) , p4(polygonP2) , p5 (polygonP5) , p6(polygonP4);
    // DesignWallGenerator::constructRoof(p , 15);

    // PolygonsSubtraction PolygonsSubtraction1(p4 , p5);

    // vector<Polygon1> pols = PolygonsSubtraction1.getNewPolygons();


    double area = p4.getArea();

    cout<<"Area = "<<area<<"\n";


    Land land(p6);
    land.SplitLand(22 , 1 , 1);

    // land.SplitLand(curr);
}

void HomeDesignController::initiateDesign() {
    userInputView = new ConstInputView();
    design = userInputView->getUserInput();
    designs.push_back(design);
    getUserInput();
}

void HomeDesignController::designProcess()
{
    //store 18 model2 design shapes
    templatesDesigns = extractRealTemplateDesign();

    Design curr = templatesDesigns.getDesigns()[0];
    vector<Wall> walls = curr.getWallFraming();

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



    // Design two;
    // Cluster cluster(designs[0] , 32);
    // for(auto &d : cluster.getFinalCluster())
    // {
    //     for(auto &room : d.getRooms())
    //     {
    //         two.addRoom(room);
    //     }
    //     // break;
    // }
    // designs.clear();
    // designs.push_back(two);

    return temDesigns;
}


void HomeDesignController::showDesignResult() {
//    designResultView = new ConsoleResultView();


    designResultView->displayDesigns(designs);
}

vector<Design> HomeDesignController::storeTemplateDesigns() {
    return TemplateRepository::templatesCreations();
}

void HomeDesignController::getUserInput() {
    SimpleApp app;


    CROW_ROUTE(app , "/RequestDesign").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        int bedrooms = -1 , rooms = -1 , spaces = -1  , home = 0; double area = - 1 ;

        Design design1 ;
        map<string , double> mapRoomsWidthAreaLivingCore;
        map<string , double> mapRoomsWidthAreaBedroomCore;
        map<string , double> mapRoomsWidthAreaFamilyCore;

        string type = jsonData["type"].s();
        cout<<"Request Type = "<<type<<"\n";

        if(type.find("indirect") == string::npos)
        {
            cout<<"Direct = "<<"\n";
            if(jsonData["direct"].has("Model"))
            {
                home = jsonData["direct"]["Home"].i();
            }

            if(jsonData["direct"].has("Bedrooms"))
            {
                bedrooms = jsonData["direct"]["Bedrooms"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
            }
            if(jsonData["direct"].has("Rooms"))
            {
                rooms = jsonData["direct"]["Rooms"].i();
                if(rooms != -1)design1 = templatesDesigns.getDesignByRooms(rooms) , cout<<"Direct33 = "<<"\n";;
            }
            if(jsonData["direct"].has("Area"))
            {
                area = jsonData["direct"]["Area"].d();
                if(area != -1)design1 = templatesDesigns.getDesignByArea(area);
            }

            if(jsonData["direct"].has("Spaces"))
            {
                spaces = jsonData["direct"]["Spaces"].d();
                if(spaces != -1)design1 = templatesDesigns.getDesignBySpaces(spaces);
            }

            if(spaces == -1 && bedrooms == -1 && area == -1 && rooms == -1)
            {
                cout<<"NO SPACES AND ROOMS AND BEDROOMS AND AREA\n";
                design1 = templatesDesigns.getDesigns()[0];
            }
        }

        else
        {
            for(auto &x : jsonData["Spaces"]["List1"])
            {
                string roomName = x.key();

                double area = x.d();
                mapRoomsWidthAreaLivingCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List2"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaBedroomCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List3"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaFamilyCore[roomName] = area;
            }

            DesignGenerator designG;
            vector<Design> newDesigns = designG.generateDesign(mapRoomsWidthAreaLivingCore , mapRoomsWidthAreaBedroomCore ,mapRoomsWidthAreaFamilyCore );

            DesignRepository newRepo;

            if(newDesigns.empty())cout<<"NO SOLUTION FOUND FOR INDIRECT\n";

            if(!newDesigns.empty())
            {
                newRepo.addDesign(newDesigns[0]);

                newDesigns = newRepo.getDesigns();
            }



            if(newDesigns.empty())design1 = templatesDesigns.getDesigns()[0];
            else design1 = newDesigns[0];
        }

        design1.scaleDesign(100);

        DesignToDoublyLines drawing(design1);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(design1);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
                response,
                design1,
                0,
                oldRectangle,
                newRectangle,
                oldLines,
                drawingLines,
                drawing.getThickness(),
                drawing.getMapTopLine(),
                drawing.getMapBottomLine(),
                drawing.getMapLeftLine(),
                drawing.getMapRightLine(),
                surface3D,
                sortedRectangles,
                dimensions,
                innerLines,
                area,
                room3DPrinting
                );
        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

    CROW_ROUTE(app , "/RequestDesign2").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        int bedrooms = -1 , rooms = -1 , spaces = -1  , home = 0; double area = - 1 ;

        Design design1 ;
        map<string , double> mapRoomsWidthAreaLivingCore;
        map<string , double> mapRoomsWidthAreaBedroomCore;
        map<string , double> mapRoomsWidthAreaFamilyCore;

        string type = jsonData["type"].s();
        cout<<"Request Type = "<<type<<"\n";

        if(type.find("indirect") == string::npos)
        {
            cout<<"Direct = "<<"\n";
            if(jsonData["direct"].has("Model"))
            {
                home = jsonData["direct"]["Home"].i();
            }

            if(jsonData["direct"].has("Bedrooms"))
            {
                bedrooms = jsonData["direct"]["Bedrooms"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
            }
            if(jsonData["direct"].has("Rooms"))
            {
                rooms = jsonData["direct"]["Rooms"].i();
                if(rooms != -1)design1 = templatesDesigns.getDesignByRooms(rooms) , cout<<"Direct33 = "<<"\n";;
            }
            if(jsonData["direct"].has("Area"))
            {
                area = jsonData["direct"]["Area"].d();
                if(area != -1)design1 = templatesDesigns.getDesignByArea(area);
            }

            if(jsonData["direct"].has("Spaces"))
            {
                spaces = jsonData["direct"]["Spaces"].d();
                if(spaces != -1)design1 = templatesDesigns.getDesignBySpaces(spaces);
            }

            if(spaces == -1 && bedrooms == -1 && area == -1 && rooms == -1)
            {
                cout<<"NO SPACES AND ROOMS AND BEDROOMS AND AREA\n";
                design1 = templatesDesigns.getDesigns()[0];
            }
        }

        else
        {
            for(auto &x : jsonData["Spaces"]["List1"])
            {
                string roomName = x.key();

                double area = x.d();
                mapRoomsWidthAreaLivingCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List2"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaBedroomCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List3"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaFamilyCore[roomName] = area;
            }

            DesignGenerator designG;
            vector<Design> newDesigns = designG.generateDesign(mapRoomsWidthAreaLivingCore , mapRoomsWidthAreaBedroomCore ,mapRoomsWidthAreaFamilyCore );

            DesignRepository newRepo;

            if(newDesigns.empty())cout<<"NO SOLUTION FOUND FOR INDIRECT\n";

            if(!newDesigns.empty())
            {
                newRepo.addDesign(newDesigns[0]);

                newDesigns = newRepo.getDesigns();
            }



            if(newDesigns.empty())design1 = templatesDesigns.getDesigns()[0];
            else design1 = newDesigns[0];
        }


        // Cluster cluster(design1 , 8);
        // design1.clearRoom();
        // for(auto &d : cluster.getFinalCluster())
        // {
        //     for(auto &room : d.getRooms())
        //     {
        //         design1.addRoom(room);
        //     }
        //     // break;
        // }

        design1.scaleDesign(100);

        DesignToDoublyLines drawing(design1);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(design1);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
                response,
                design1,
                0,
                oldRectangle,
                newRectangle,
                oldLines,
                drawingLines,
                drawing.getThickness(),
                drawing.getMapTopLine(),
                drawing.getMapBottomLine(),
                drawing.getMapLeftLine(),
                drawing.getMapRightLine(),
                surface3D,
                sortedRectangles,
                dimensions,
                innerLines,
                area,
                room3DPrinting
                );
        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });


    CROW_ROUTE(app , "/ModifiyDesign").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;

        Design outputDesign ;

        string action = jsonData["Action"].s();
        auto designJson =  jsonData["Design"];
        Design mainDesign;
        RoomBuilder roomBuilder;



        for(auto &room : designJson["Rooms"])
        {
            string id = room["id"].s();
            double x1 = room["x1"].d();
            double y1 = room["y1"].d();
            double x2 = room["x2"].d();
            double y2 = room["y2"].d();

            double width = (x2 - x1) / 100;
            double height = (y2 - y1) / 100;

            Room newRoom(roomBuilder.createRoom(id , x1/100 , y1/100 , x2/100 , y2/100));

            for(auto &door : room["Door"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;

                newRoom.addDoor(_x1 , _y1 , _x2 , _y2);
            }

            for(auto &door : room["Window"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;
                newRoom.addWindow(_x1 , _y1 , _x2 , _y2);
            }

            for(auto &door : room["Opening"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;

                newRoom.addOpening(_x1 , _y1 , _x2 , _y2);
            }

            mainDesign.addRoom(newRoom);
            // mainDesign.addRoom(roomBuilder.createRoom(id , x1 , y1 , x2 , y2));
        }

        for(auto &room : designJson["connections"])
        {
            string first = room["first"].s();
            string second = room["second"].s();

            mainDesign.addConnection(first , second);
        }

        if(action == "Delete")
        {
            string space = jsonData["Space"].s();
            mainDesign.deleteRoom(space);

            DesignGenerator designGenerator1;
            vector<Design>ds = designGenerator1.generateDesign1(mainDesign);

            if(!ds.empty())
            {
                outputDesign = ds[0];
                cout<<"Found Designs\n";
            }
            else outputDesign = mainDesign;
        }

        outputDesign.setConnections(mainDesign.getConnections());

        DesignRepository designRepo;
        designRepo.addDesign(outputDesign);
        outputDesign = designRepo.getDesigns()[0];



        outputDesign.scaleDesign(100);
        cout<<"Final Design\n";

        for(auto &room : outputDesign.getRooms())room.printDetails();

        for(auto &conn : outputDesign.getConnections())
        {
            cout<<"Room -- > "<<conn.first<<"\n";
            for(auto &c : conn.second)
            {
                cout<<c<<" ";
            }
            cout<<"\n";
        }

        DesignToDoublyLines drawing(outputDesign);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(outputDesign);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
                response,
                outputDesign,
                0,
                oldRectangle,
                newRectangle,
                oldLines,
                drawingLines,
                drawing.getThickness(),
                drawing.getMapTopLine(),
                drawing.getMapBottomLine(),
                drawing.getMapLeftLine(),
                drawing.getMapRightLine(),
                surface3D,
                sortedRectangles,
                dimensions,
                innerLines,
                -1,
                room3DPrinting
                );
        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

    CROW_ROUTE(app , "/RotateDesign").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        Design outputDesign ;
        double degree = jsonData["rotation"].d();
        auto designJson =  jsonData["Design"];
        Design mainDesign;
        RoomBuilder roomBuilder;
        for(auto &room : designJson["Rooms"])
        {
            string id = room["id"].s();
            double x1 = room["x1"].d();
            double y1 = room["y1"].d();
            double x2 = room["x2"].d();
            double y2 = room["y2"].d();
            double width = (x2 - x1) / 100;
            double height = (y2 - y1) / 100;
            mainDesign.addRoom(roomBuilder.createRoom(id , x1/100 , y1/100 , x2/100 , y2/100));
        }
        for(auto &room : designJson["connections"])
        {
            string first = room["first"].s();
            string second = room["second"].s();
            mainDesign.addConnection(first , second);
        }

        if(degree < 360)mainDesign.rotate(degree);
        else if(degree == 450)mainDesign.mirrorRecX();
        else if(degree == 540)mainDesign.mirrorRecY();

        DesignRepository designRepo;
        designRepo.addDesign(mainDesign);
        outputDesign = designRepo.getDesigns()[0];
        outputDesign.scaleDesign(100);
//        cout<<"Final Design\n";
//        for(auto &room : outputDesign.getRooms())room.printDetails();
//        for(auto &conn : outputDesign.getConnections())
//        {
//            cout<<"Room -- > "<<conn.first<<"\n";
//            for(auto &c : conn.second)
//            {
//                cout<<c<<" ";
//            }
//            cout<<"\n";
//        }
        DesignToDoublyLines drawing(outputDesign);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(outputDesign);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
                response,
                outputDesign,
                0,
                oldRectangle,
                newRectangle,
                oldLines,
                drawingLines,
                drawing.getThickness(),
                drawing.getMapTopLine(),
                drawing.getMapBottomLine(),
                drawing.getMapLeftLine(),
                drawing.getMapRightLine(),
                surface3D,
                sortedRectangles,
                dimensions,
                innerLines,
                -1,
                room3DPrinting
                );
        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });


    CROW_ROUTE(app , "/RequestFraming").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;

        Design outputDesign ;

        auto designJson =  jsonData["floor1"]["design"];
        Design mainDesign;
        RoomBuilder roomBuilder;



        for(auto &room : designJson["Rooms"])
        {
            string id = room["id"].s();
            double x1 = room["x1"].d();
            double y1 = room["y1"].d();
            double x2 = room["x2"].d();
            double y2 = room["y2"].d();

            double width = (x2 - x1) / 100;
            double height = (y2 - y1) / 100;

            Room newRoom(roomBuilder.createRoom(id , x1/100 , y1/100 , x2/100 , y2/100));

            for(auto &door : room["Door"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;

                newRoom.addDoor(_x1 , _y1 , _x2 , _y2);
            }

            for(auto &door : room["Window"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;
                newRoom.addWindow(_x1 , _y1 , _x2 , _y2);
            }

            for(auto &door : room["Opening"])
            {
                double _x1 = door["x1"].d();
                double _y1 = door["y1"].d();
                double _x2 = door["x2"].d();
                double _y2 = door["y2"].d();

                _x1/=100;
                _x1 = (std::round(_x1*10) / 10) * 1.0;

                _y1/=100;
                _y1 = (std::round(_y1*10) / 10) * 1.0;

                _x2/=100;
                _x2 = (std::round(_x2*10) / 10) * 1.0;

                _y2/=100;
                _y2 = (std::round(_y2*10) / 10) * 1.0;

                newRoom.addOpening(_x1 , _y1 , _x2 , _y2);
            }

            mainDesign.addRoom(newRoom);
        }

        vector<Polygon1> polygons;

        for(auto &pol : jsonData["floor1"]["roof"])
        {
            auto vecs = pol["vecs"];
            vector<Point> points;
            for(auto & ppo : vecs)
            {
                double x = ppo["x"].d() * 100;
                double y = ppo["y"].d() * 100;
                double z = ppo["z"].d() * 100;

                cout<<"X = "<<x <<" Y = "<<y<<" Z = "<<z<<"\n";
                points.emplace_back(x,z,y);
            }
            polygons.emplace_back(points);
        }

        vector<Wall> walls = mainDesign.getWallFraming();



        for(auto &pol : polygons)
        {
            vector<Wall> roofWall = DesignWallGenerator::constructRoof( pol);
            cout<< "roofWall = " << roofWall.size()<<"\n";

            walls.insert(walls.end() , roofWall.begin() , roofWall.end());
        }

        for(auto &wall : walls)
        {
            wall.x1*=100;
            wall.x2*=100;
            wall.y1*=100;
            wall.y2*=100;
            wall.z1*=100;
            wall.z2*=100;
        }
        cout<<"Walls Size = "<<walls.size()<<"\n";

        for(int i = 0 ; i< walls.size() ; i++)
        {
            Wall w = walls[i];
            response["floor1"][i] = {
                {"x1" , w.x1},
                {"y1" , w.y1},
                {"x2" , w.x2},
                {"y2" , w.y2},
                {"z1" , w.z1},
                {"z2" , w.z2},
            };
        }

        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });


    CROW_ROUTE(app , "/RequestLand").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        int bedrooms = -1 , rooms = -1 , spaces = -1  , home = 0; double area = - 1 ;

        Design design1 ;
        map<string , double> mapRoomsWidthAreaLivingCore;
        map<string , double> mapRoomsWidthAreaBedroomCore;
        map<string , double> mapRoomsWidthAreaFamilyCore;

        string type = jsonData["type"].s();
        cout<<"Request Type = "<<type<<"\n";

        if(type.find("indirect") == string::npos)
        {
            cout<<"Direct = "<<"\n";
            if(jsonData["direct"].has("Model"))
            {
                home = jsonData["direct"]["Home"].i();
            }

            if(jsonData["direct"].has("Bedrooms"))
            {
                bedrooms = jsonData["direct"]["Bedrooms"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
            }
            if(jsonData["direct"].has("Rooms"))
            {
                rooms = jsonData["direct"]["Rooms"].i();
                if(rooms != -1)design1 = templatesDesigns.getDesignByRooms(rooms) , cout<<"Direct33 = "<<"\n";;
            }
            if(jsonData["direct"].has("Area"))
            {
                area = jsonData["direct"]["Area"].d();
                if(area != -1)design1 = templatesDesigns.getDesignByArea(area);
            }

            if(jsonData["direct"].has("Spaces"))
            {
                spaces = jsonData["direct"]["Spaces"].d();
                if(spaces != -1)design1 = templatesDesigns.getDesignBySpaces(spaces);
            }

            if(spaces == -1 && bedrooms == -1 && area == -1 && rooms == -1)
            {
                cout<<"NO SPACES AND ROOMS AND BEDROOMS AND AREA\n";
                design1 = templatesDesigns.getDesigns()[0];
            }
        }

        else
        {
            for(auto &x : jsonData["Spaces"]["List1"])
            {
                string roomName = x.key();

                double area = x.d();
                mapRoomsWidthAreaLivingCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List2"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaBedroomCore[roomName] = area;
            }

            for(auto &x : jsonData["Spaces"]["List3"])
            {
                string roomName = x.key();
                double area = x.d();
                mapRoomsWidthAreaFamilyCore[roomName] = area;
            }

            DesignGenerator designG;
            vector<Design> newDesigns = designG.generateDesign(mapRoomsWidthAreaLivingCore , mapRoomsWidthAreaBedroomCore ,mapRoomsWidthAreaFamilyCore );

            DesignRepository newRepo;

            if(newDesigns.empty())cout<<"NO SOLUTION FOUND FOR INDIRECT\n";

            if(!newDesigns.empty())
            {
                newRepo.addDesign(newDesigns[0]);

                newDesigns = newRepo.getDesigns();
            }



            if(newDesigns.empty())design1 = templatesDesigns.getDesigns()[0];
            else design1 = newDesigns[0];
        }

        auto polygon = jsonData["polygon"];
        vector<Point> points;
        for(auto &point : polygon)
        {
            double x = point["x"].d();
            double y = point["y"].d();
            points.emplace_back(x , y);
        }

        Polygon1 polygon1(points);
        // polygon1.shiftX(300);
        // polygon1.shiftY(300);
        polygon1.print();

        cout<<"Area = "<<polygon1.getArea() <<" \n";

        Land land(polygon1);
        vector<Polygon1> ans = land.SplitLand(design1);
        // land.SplitLand(200);

        for(int i = 0 ; i< ans.size() ; i++)
        {
            auto pol = ans[i];
            int index = 0;
            for(auto &p : pol.getPoints())
            {
                response[i][index++] = {
                    {"x" , p.getX()} ,
                    {"y" , p.getY()}
                };
            }
        }



        crow::response finalRes (200 , response);

        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

    CROW_ROUTE(app , "/LandDivision").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;

        Design design1 ;

        auto polygon = jsonData["polygon"];
        vector<Point> points;
        for(auto &point : polygon)
        {
            double x = point["x"].d();
            double y = point["y"].d();
            points.emplace_back(x , y);
        }

        Polygon1 polygon1(points);
        // polygon1.shiftX(300);
        // polygon1.shiftY(300);
        // polygon1.print();

        cout<<"Area = "<<polygon1.getArea() <<" \n";

        vector<Polygon1> ans;

        Land land(polygon1);

        if(jsonData.count("green_area") && !jsonData.count("length"))
        {
            double greenArea = jsonData["green_area"].d();
            double side = sqrt(greenArea);
            Point centerPoint = polygon1.calculateCentroid();
            vector<Point> newPoints{
                {centerPoint.getX() - side/2 , centerPoint.getY() - side/2},
                {centerPoint.getX() + side/2 , centerPoint.getY() - side/2},

                {centerPoint.getX() + side/2 , centerPoint.getY() + side/2},
                {centerPoint.getX() - side/2 , centerPoint.getY() + side/2},
            };
            Polygon1 p2(newPoints);

            PolygonsSubtraction PolygonsSubtraction1(polygon1 , p2);

            ans = PolygonsSubtraction1.getNewPolygons();

            cout<<"new Polygons after Green Area = "<<ans.size()<<"\n";

            if(jsonData.count("lots"))
            {
                int landSlots = jsonData["lots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("slots"))
            {
                int landSlots = jsonData["slots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }

            else if(jsonData.count("home_area") )
            {
                double homeArea = jsonData["home_area"].i();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("slot_area") )
            {
                double homeArea = jsonData["slot_area"].i();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("bedroom_count"))
            {
                int bedrooms = jsonData["bedroom_count"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
                ans = land.SplitLands(ans , design1);
            }
        }
        else if(jsonData.count("length"))
        {
            double height = jsonData["length"].d();
            double width = jsonData["width"].d();
            Point centerPoint = polygon1.calculateCentroid();
            vector<Point> newPoints{
                {centerPoint.getX() - width/2 , centerPoint.getY() - height/2},
                {centerPoint.getX() + width/2 , centerPoint.getY() - height/2},

                {centerPoint.getX() + width/2 , centerPoint.getY() + height/2},
                {centerPoint.getX() - width/2 , centerPoint.getY() + height/2},
            };
            Polygon1 p2(newPoints);

            PolygonsSubtraction PolygonsSubtraction1(polygon1 , p2);

            ans = PolygonsSubtraction1.getNewPolygons();

            if(jsonData.count("lots"))
            {
                int landSlots = jsonData["lots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("slots"))
            {
                int landSlots = jsonData["slots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }

            else if(jsonData.count("home_area") )
            {
                double homeArea = jsonData["home_area"].i();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("slot_area") )
            {
                double homeArea = jsonData["slot_area"].i();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("bedroom_count"))
            {
                int bedrooms = jsonData["bedroom_count"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
                ans = land.SplitLands(ans , design1);
            }
        }

        else if(jsonData.count("lots"))
        {
            int landSlots = jsonData["lots"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;

            landSlots += (landSlots * (percGreenArea/100)) / (100 - percGreenArea)/100;
            ans = land.SplitLand(landSlots , 1 , 1);

            // GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            GreenAreaSelector *greenSelector = new ClusteredGreenSelector();

            greenSelector->select(polygon1 , ans , percGreenArea/100);
        }
        else if(jsonData.count("slots"))
        {
            int landSlots = jsonData["slots"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;

            landSlots += (landSlots * (percGreenArea/100)) / (100 - percGreenArea)/100;
            ans = land.SplitLand(landSlots , 1 , 1);

            // GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            GreenAreaSelector *greenSelector = new ClusteredGreenSelector();

            greenSelector->select(polygon1,ans , percGreenArea/100);
        }

        else if(jsonData.count("home_area") )
        {
            double homeArea = jsonData["home_area"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            ans = land.SplitLand(homeArea);

            GreenAreaSelector *greenSelector = new UniformGreenDistributor();

            greenSelector->select(polygon1,ans , percGreenArea/100);
        }
        else if(jsonData.count("slot_area") )
        {
            double homeArea = jsonData["slot_area"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            ans = land.SplitLand(homeArea);

            GreenAreaSelector *greenSelector = new UniformGreenDistributor();

            greenSelector->select(polygon1,ans , percGreenArea/100);
        }
        else if(jsonData.count("bedroom_count"))
        {
            int bedrooms = jsonData["bedroom_count"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
            ans = land.SplitLand(design1);

            GreenAreaSelector *greenSelector = new UniformGreenDistributor();

            greenSelector->select(polygon1 , ans , percGreenArea/100);
        }

        for(int i = 0 ; i< polygon1.getPoints().size() ; i++)
        {
            response["outerLand"][i] = {
                {"x" , polygon1.getPoints()[i].getX()} ,
                {"y" , polygon1.getPoints()[i].getY()}
            };
        }

        for(int i = 0 ; i < ans.size() ; i++)
        {
            auto pol = ans[i];
            int index = 0;
            response["Inner"][i]["id"] = i + 1;
            response["Inner"][i]["area"] = pol.getArea();
            response["Inner"][i]["green_area"] = !pol.isDivisible();

            for(auto &p : pol.getPoints())
            {
                response["Inner"][i]["Points"][index++] = {
                    {"x" , p.getX()} ,
                    {"y" , p.getY()}
                };
            }
        }

        crow::response finalRes (200 , response);

        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

    CROW_ROUTE(app , "/Template").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;

        int bedrooms = -1 , rooms = -1 , spaces = -1 , home = -1; double area = - 1;

        if(jsonData.has("Bedrooms"))
        {
            bedrooms = jsonData["Bedrooms"].i();
        }
        if(jsonData.has("Rooms"))
        {
            rooms = jsonData["Rooms"].i();
        }
        if(jsonData.has("Area"))
        {
            area = jsonData["Area"].d();
        }

        if(jsonData.has("Spaces"))
        {
            spaces = jsonData["Spaces"].d();
        }

        if(jsonData.has("Home"))
        {
            home = jsonData["Home"].d();
        }

        Design design1;

        Room room1("Bedroom" , 0 , 0 , 525 , 420);
        Room room2("Kitchen" , 525 , 0 , 735 , 210);
        Room room3("Bath"    , 525 , 210 , 735 , 420);

        room2.addOpening(525 , 30 , 525 , 135);
        room1.addWindow(0 , 105 , 0 , 210);
        room3.addWindow(735 , 225 , 735 , 330);

        room1.addDoor(240 , 420 , 345 , 420);
        room3.addDoor(525 , 225 , 525 , 330);

        design1.addRoom(room1);
        design1.addRoom(room2);
        design1.addRoom(room3);

        // design1.scaleDesign(100);


        DesignToDoublyLines drawing(design1);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(design1);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
            response,
            design1,
            0,
            oldRectangle,
            newRectangle,
            oldLines,
            drawingLines,
            drawing.getThickness(),
            drawing.getMapTopLine(),
            drawing.getMapBottomLine(),
            drawing.getMapLeftLine(),
            drawing.getMapRightLine(),
            surface3D,
            sortedRectangles,
            dimensions,
            innerLines,
            area,
            room3DPrinting
        );

        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

    app.port(8080).multithreaded().run();
}







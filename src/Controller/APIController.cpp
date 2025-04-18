//
// Created by ahmed Ibrahim on 11-Feb-25.
//
#include "src/Model/BuildingDesigner/BuildingDesignerCorridorBesideLiving.h"
#include "src/Model/BuildingDesigner/BuildingDesignerCorridorAboveLiving.h"
#include "src/Model/BuildingDesigner/BuildingSecondFloorDesign.h"
#include "src/Model/BuildingDesigner/BuildingDesigner.h"
#include "APIController.h"
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
#include "src/Model/Land/Green Area/CentroidLineGreenSelector.h"
#include "src/Model/Land/LandDivisionRoads/LandDivisionRoadsByInnerDesign.h"
#include "src/Model/Land/LandDivisionRoads/LandDivisionRoadsByArea.h"
#include "src/Model/Land/LandDivisionRoads/LandDivisionRoadsByDivisionsCount.h"
#include "src/Model/Land/LandDivisionRoads/LandDivisionRoadsByRatios.h"
#include "src/Model/Land/DrawStreet.h"
APIController::APIController(crow::SimpleApp &app)
{
    preProcessing();
    templateRoutes(app);
    landDivisionRoutes(app);
    subLandDivisionRoutes(app);
    requestFramingRoutes(app);
    rotateDesignRoutes(app);
    modifyDesignRoutes(app);
    requestDesignRoutes(app);
    requestDesign2Routes(app);
    landDivisionRoutesStreets(app);
}

void APIController::preProcessing() {
    templatesDesigns = homeDesignController.extractRealTemplateDesign();
    model1Templates.attachRooms();
}

void APIController::templateRoutes(crow::SimpleApp &app)
{

    CROW_ROUTE(app , "/Template").methods( crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        crow::response finalRes ;
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }


        crow::json::wvalue response;
        int bedrooms = -1 , rooms = -1 , spaces = -1 , home = -1; double area = - 1;
        Design design1;
        vector<Design> designs;
        vector<Design> templates = model1Templates.designs;

        if(jsonData.has("Bedrooms"))
        {
            bedrooms = (int)jsonData["Bedrooms"].i();
        }
        if(jsonData.has("Rooms"))
        {
            rooms = (int)jsonData["Rooms"].i();
        }
        if(jsonData.has("Area"))
        {
            area = jsonData["Area"].d();
        }
        if(jsonData.has("Spaces"))
        {
            spaces = (int)jsonData["Spaces"].i();
        }
        if(jsonData.has("Home"))
        {
            home = (int)jsonData["Home"].i();
        }

        if(bedrooms != -1)
        {
            designs = model1Templates.bedroomsInput(bedrooms , templates);

            int index = (int)bedroomRequest[bedrooms]%(int)designs.size();

            if(!designs.empty()) design1 = designs[index];
            bedroomRequest[bedrooms]++;
        }
        else if(rooms != -1)
        {
            designs = model1Templates.roomsInput(rooms , templates);
            int index = roomsRequest[rooms]%(int)designs.size();

            if(!designs.empty()) design1 = designs[index];
            roomsRequest[rooms]++;
        }
        else if(area != -1)
        {
            designs = model1Templates.areaInput(area , templates);
            if(!designs.empty()) design1 = designs[0];
        }
        else if(spaces != -1)
        {
            designs = model1Templates.spacesInput(spaces , templates);
            int index = spacesRequest[spaces]%(int)designs.size();

            if(!designs.empty()) design1 = designs[index];
            spacesRequest[spaces]++;
        }
        else
        {
            if(!templates.empty()) design1 = templates[0];

            int index = general[0]%(int)templates.size();
            design1 = templates[index];
            general[0]++;
        }
        design1.scaleDesign(105);

        vector<string> zone1 {"Bedroom1", "Bedroom2", "Bedroom3", "Bedroom4" , "Bath1" , "Bath2" };
        vector<string> zone2 {"Dining", "Kitchen", "GBath", "Family", "Office" , "Garage"};

        vector<pair<string , string>> conn {
                {"Bedroom1", "Bedroom2"},
                {"Bath1", "Bedroom2"},
                {"Bath2", "Bedroom3"},
                {"Dining", "Bedroom4"},
                {"Kitchen", "GBath"},
                {"Family", "Office"},
        };

        map<string , double> areas;

        areas["Dining"] = 20;
        areas["Kitchen"] = 16;
        areas["GBath"] = 4;
        areas["Family"] = 24;
        areas["Office"] = 20;
        areas["Living"] = 35;
        areas["Garage"] = 18;

        areas["Bedroom1"] = 25;
        areas["Bedroom2"] = 20;
        areas["Bedroom3"] = 20;
        areas["Bedroom4"] = 20;
        areas["Bath1"] = 6;
        areas["Bath2"] = 6;


        vector<string> zone3 {"Bedroom1", "Bedroom2", "Bedroom3", "Bedroom4" , "Bath1" , "Bath2" };
        vector<string> zone4 {"Room1", "Room2", "Room3" , "Room4"};

        vector<pair<string , string>> conn2 {
                {"Bedroom1", "Bedroom2"},
                {"Bath1", "Bedroom2"},
                {"Bath2", "Bedroom3"},
        };

        map<string , double> areas2;

        areas2["Room1"] = 36;
        areas2["Room2"] = 28;
        areas2["Room3"] = 20;
        areas2["Room4"] = 18;

        areas2["Bedroom1"] = 25;
        areas2["Bedroom2"] = 20;
        areas2["Bedroom3"] = 20;
        areas2["Bedroom4"] = 20;
        areas2["Bath1"] = 6;
        areas2["Bath2"] = 6;



//        BuildingDesigner *buildingDesigner = new BuildingDesignerCorridorBesideLiving(zone1 , zone2 , conn , areas);
        BuildingDesigner *buildingDesigner = new BuildingDesignerCorridorBesideLiving(zone1 , zone2 , conn , areas);
        vector<Design> firstFloor = buildingDesigner->generateDesign();
        design1 = firstFloor[2];

        BuildingSecondFloorDesign *buildingSecondFloorDesign = new BuildingSecondFloorDesign(zone1 , zone2 , conn , areas);
        vector<Design> secondFloor = buildingSecondFloorDesign->generateDesign(design1);

        cout<<"Size = "<<secondFloor.size()<<"\n";
        design1 = secondFloor[5];
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

        BuildingDesigner *buildingDesigner2 = new BuildingDesignerCorridorBesideLiving(zone1 , zone2 , conn , areas);;
        vector<Design> resD2 = buildingDesigner2->generateDesign();
        crow::response res (200 , response);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });
}

void APIController::landDivisionRoutes(SimpleApp &app)
{
    CROW_ROUTE(app , "/LandDivision").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }

        double divisionArea = jsonData["dimensions"]["divisionArea"].d();
        double externalRoad = jsonData["dimensions"]["externalRoad"].d();
        double centralRoad = jsonData["dimensions"]["centralRoad"].d();
        double circularStreet = jsonData["dimensions"]["circularStreet"].d();
        double landDepth = jsonData["dimensions"]["landDepth"].d();
        double streetCut = jsonData["dimensions"]["streetCut"].d(); // for circular Streets


        crow::json::wvalue response;
        int strategy = (int)jsonData["strategy"].i();
        auto polygon = jsonData["polygon"];
        vector<Point> points;
        for(auto &point : polygon)
        {
            double x = point["x"].d();
            double y = point["y"].d();
            points.emplace_back(x , y);
        }

        Polygon1 polygon1(points);

        cout<<"Area = "<<polygon1.getArea() <<" \n";

        vector<Polygon1> ans;
        vector<Polygon1> streets;
        LandDivisionRoads * landDivisionRoads;
        Land land(polygon1 , divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
        auto oldDesign = jsonData.count("design")? jsonData["design"] : jsonData;

        Design design1 ;

        if( (jsonData.count("design") && jsonData["design"].t() != crow::json::type::Null))
        {
            const auto& designJson =  jsonData.count("design")?jsonData["design"] : jsonData["Design"];
            for(auto &room : designJson["Rooms"])
            {
                string id = room["id"].s();
                double x1 = room["x1"].d();
                double y1 = room["y1"].d();
                double x2 = room["x2"].d();
                double y2 = room["y2"].d();
                double width = (x2 - x1) / 100;
                double height = (y2 - y1) / 100;
                Room newRoom(id , x1/100 , y1/100 , x2/100 , y2/100);

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

                if(room.count("image")) newRoom.setImagePath(room["image"].s());

                design1.addRoom(newRoom);
            }

            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;

            design1.scaleDesign(100);

            ans = land.SplitLand(design1 , static_cast<LandDivisionSortingStrategy>(strategy));

            GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            greenSelector->select(polygon1 , ans , percGreenArea/100 , 0);

            landDivisionRoads = new LandDivisionRoadsByInnerDesign(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            Polygon1 innerHome (DesignOutlines::getRoofPoints(design1));
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , innerHome , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        else if(jsonData.count("green_area") && !jsonData.count("length"))
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
                int landSlots = (int)jsonData["lots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("slots"))
            {
                int landSlots = (int)jsonData["slots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("home_area") )
            {
                double homeArea = jsonData["home_area"].d();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("slot_area") )
            {
                double homeArea = jsonData["slot_area"].d();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("bedroom_count"))
            {
                int bedrooms = (int)jsonData["bedroom_count"].i();
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
                int landSlots = (int)jsonData["lots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("slots"))
            {
                int landSlots = (int)jsonData["slots"].i();
                ans = land.SplitLands(ans , landSlots , 1 , 1);
            }
            else if(jsonData.count("home_area") )
            {
                double homeArea = jsonData["home_area"].d();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("slot_area") )
            {
                double homeArea = jsonData["slot_area"].d();
                ans = land.SplitLands(ans ,homeArea);
            }
            else if(jsonData.count("bedroom_count"))
            {
                int bedrooms = (int)jsonData["bedroom_count"].i();
                if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
                ans = land.SplitLands(ans , design1);
            }
        }

        else if(jsonData.count("lots"))
        {
            int landSlots = (int)jsonData["lots"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            int greenAreasCount = (int)((landSlots * (percGreenArea/100)) / ((100 - percGreenArea)/100));
            landSlots += greenAreasCount;
            ans = land.SplitLand(landSlots , 1 , 1 , static_cast<LandDivisionSortingStrategy>(strategy));
            // GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            //  GreenAreaSelector *greenSelector = new ClusteredGreenSelector();

            landDivisionRoads = new LandDivisionRoadsByDivisionsCount(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , 1 , 1 , landSlots , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else streets = pols[0];

            GreenAreaSelector *greenSelector = new CentroidLineGreenSelector();
            cout << "greenAreasCount = "<<greenAreasCount<<"\n";
            cout << "landSlots = "<<landSlots<<"\n";
            percGreenArea = (greenAreasCount*1.0 / landSlots ) * 100.0;
            greenSelector->select(polygon1 , streets , percGreenArea/100.0 , 900000);
        }

        else if(jsonData.count("slots"))
        {
            int landSlots = (int)jsonData["slots"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            int greenAreasCount = (int)((landSlots * (percGreenArea/100)) / ((100 - percGreenArea)/100));
            landSlots += greenAreasCount;
            ans = land.SplitLand(landSlots , 1 , 1 , static_cast<LandDivisionSortingStrategy>(strategy));

            landDivisionRoads = new LandDivisionRoadsByDivisionsCount(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , 1 , 1 , landSlots , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];


            GreenAreaSelector *greenSelector = new CentroidLineGreenSelector();
            cout << "greenAreasCount = "<<greenAreasCount<<"\n";
            cout << "landSlots = "<<landSlots<<"\n";
            percGreenArea = (greenAreasCount*1.0 / landSlots ) * 100.0;
            greenSelector->select(polygon1 , streets , percGreenArea/100.0 , 900000);

        }

        else if(jsonData.count("home_area") )
        {
            double homeArea = jsonData["home_area"].d();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            ans = land.SplitLand(homeArea , static_cast<LandDivisionSortingStrategy>(strategy));
            GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            greenSelector->select(polygon1,ans , percGreenArea/100 , 0);

            landDivisionRoads = new LandDivisionRoadsByArea(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , homeArea , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        else if(jsonData.count("slot_area") )
        {
            double homeArea = jsonData["slot_area"].d();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            ans = land.SplitLand(homeArea , static_cast<LandDivisionSortingStrategy>(strategy)) ;
            GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            greenSelector->select(polygon1,ans , percGreenArea/100 , 0);

            landDivisionRoads = new LandDivisionRoadsByArea(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , homeArea , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        else if(jsonData.count("bedroom_count"))
        {
            int bedrooms = (int)jsonData["bedroom_count"].i();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;
            if(bedrooms != -1)design1 = templatesDesigns.getDesignByBedrooms(bedrooms);
            ans = land.SplitLand(design1 , static_cast<LandDivisionSortingStrategy>(strategy));
            GreenAreaSelector *greenSelector = new UniformGreenDistributor();
            greenSelector->select(polygon1 , ans , percGreenArea/100 , 0);

            landDivisionRoads = new LandDivisionRoadsByInnerDesign(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            Polygon1 innerHome (DesignOutlines::getRoofPoints(design1));
            vector<vector<Polygon1>> pols = landDivisionRoads->divideLand(polygon1 , innerHome , static_cast<LandDivisionSortingStrategy>(strategy) );
            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        else if(jsonData.count("ratios"))
        {
            cout<<"ratios = "<<"\n";
            auto ratiosA = jsonData["ratios"];
            vector<double> ratios;
            for(auto &r : ratiosA){
                ratios.push_back(r.d());
            }
            int landSlots = (int)ratios.size();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;


            if(landSlots > 0)ans = land.SplitLand(ratios  , static_cast<LandDivisionSortingStrategy>(strategy));
            else ans = land.SplitLand(divisionArea , static_cast<LandDivisionSortingStrategy>(strategy)) ;
            GreenAreaSelector *greenSelector = new CentroidLineGreenSelector();
            greenSelector->select(polygon1,ans , percGreenArea/100 , 0);


            vector<vector<Polygon1>> pols ;

            cout<<"landSlots = "<<landSlots<<"\n";


            if(landSlots > 0)
            {
                landDivisionRoads = new LandDivisionRoadsByRatios(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
                pols = landDivisionRoads->divideLand(polygon1 , ratios , static_cast<LandDivisionSortingStrategy>(strategy) );
            }
            else
            {
                landDivisionRoads = new LandDivisionRoadsByArea(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
                pols =landDivisionRoads->divideLand(polygon1 , divisionArea , static_cast<LandDivisionSortingStrategy>(strategy) );
            }


            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        else
        {
            vector<double> ratios;

            int landSlots = (int)ratios.size();
            double percGreenArea = jsonData.count("green_area_percentage")?jsonData["green_area_percentage"].d() : 0;

            ans = land.SplitLand(divisionArea , static_cast<LandDivisionSortingStrategy>(strategy)) ;
            GreenAreaSelector *greenSelector = new CentroidLineGreenSelector();
            greenSelector->select(polygon1,ans , percGreenArea/100 , 0);


            vector<vector<Polygon1>> pols ;

            cout<<"landSlots = "<<landSlots<<"\n";

            landDivisionRoads = new LandDivisionRoadsByArea(divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            pols =landDivisionRoads->divideLand(polygon1 , divisionArea , static_cast<LandDivisionSortingStrategy>(strategy) );


            if (pols.empty()) streets = land.buildRoads(ans);
            else  streets = pols[0];
        }

        int solutionIndex = 0;
        for(int i = 0 ; i< polygon1.getPoints().size() ; i++)
        {
            response[solutionIndex]["outerLand"][i] = {
                    {"x" , polygon1.getPoints()[i].getX()} ,
                    {"y" , polygon1.getPoints()[i].getY()}
            };
        }

        ans = streets;
        vector<Polygon1> ans2 ;

        for(int i = 0 ; i < ans.size() ; i++)
        {
            auto pol = ans[i];

            vector<Line> polLines = pol.getLines();
            bool flag = true;

            for(auto &l : polLines)
            {
                double length = l.getLength();
                if (length > 80)
                {
                    flag = false;
                    break;
                }
            }

            if (flag)
            {
                Land land2(pol);
                vector<Polygon1 >pols2 =land2.SplitLand(500 , static_cast<LandDivisionSortingStrategy>(strategy));

                ans2.insert(ans2.end() , pols2.begin() , pols2.end());
            }else ans2.push_back(ans[i]);
        }

        ans = ans2;

        PolygonHelper::renamePolygonsIds(polygon1 , ans);


        Polygon1 outerLand = PolygonHelper::getScalingPolygon(polygon1 , -externalRoad);

        if (outerLand.getArea() < polygon1.getArea())
        {
            outerLand = PolygonHelper::getScalingPolygon(polygon1 , externalRoad);
        }


        for(int i = 0 ; i < ans.size() ; i++)
        {
            auto pol = ans[i];

            vector<Line> polLines = pol.getLines();
            bool flag = true;

            for(auto &l : polLines)
            {
                double length = l.getLength();
                if (length > 80)
                {
                    flag = false;
                    break;
                }
            }

            int index = 0;
            response[solutionIndex]["Inner"][i]["id"] =  pol.getId();
            response[solutionIndex]["Inner"][i]["area"] = pol.getArea();
            response[solutionIndex]["Inner"][i]["green_area"] = !pol.isDivisible();
            response[solutionIndex]["Inner"][i]["roadExtension"] = std::vector<crow::json::wvalue>{};
            response[solutionIndex]["Inner"][i]["homeBorder"] = std::vector<crow::json::wvalue>{};

            for(auto &p : pol.getPoints())
            {
                response[solutionIndex]["Inner"][i]["Points"][index++] = {
                        {"x" , p.getX()} ,
                        {"y" , p.getY()}
                };
            }

            if(flag)continue;

            DrawStreet drawStreet;
            drawStreet.drawStreets(pol, divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
            vector<Line> centerLines = drawStreet.getCenterLines();
            vector<CityGrid> cities = drawStreet.getCities();

            for (int j = 0; j < centerLines.size(); ++j)
            {
                response[solutionIndex]["Inner"][i]["centerLines"][j] = {
                        {"x1" , centerLines[j].getX1()},
                        {"y1" , centerLines[j].getY1()},
                        {"x2" , centerLines[j].getX2()},
                        {"y2" , centerLines[j].getY2()},
                };
            }


            for(int j = 0 ; j< cities.size() ; j++)
            {
                CityGrid city = cities[j];
                vector<Line> roadExtension = city.getRoadExtension();
                vector<Line> homeBorder = city.getHomeBorder();
                vector<Line> roads = city.getInnerStreets();

                int n = roads.size();

                for (int m = 0; m < roads.size(); ++m) {
                    response[solutionIndex]["Inner"][i]["roads"][j][m] = {
                            {"x1" , roads[m].getX1()},
                            {"y1" , roads[m].getY1()},
                            {"x2" , roads[m].getX2()},
                            {"y2" , roads[m].getY2()},
                    };
                }
                roads = city.getOuterStreets();
                for (int m = 0; m < roads.size(); ++m) {
                    response[solutionIndex]["Inner"][i]["roads"][j][n + m] = {
                            {"x1" , roads[m].getX1()},
                            {"y1" , roads[m].getY1()},
                            {"x2" , roads[m].getX2()},
                            {"y2" , roads[m].getY2()},
                    };
                }

                for (int m = 0; m < roadExtension.size(); ++m) {
                    response[solutionIndex]["Inner"][i]["roadExtension"][j][m] = {
                            {"x1" , roadExtension[m].getX1()},
                            {"y1" , roadExtension[m].getY1()},
                            {"x2" , roadExtension[m].getX2()},
                            {"y2" , roadExtension[m].getY2()},
                    };
                }

                for (int m = 0; m < homeBorder.size(); ++m) {
                    response[solutionIndex]["Inner"][i]["homeBorder"][j][m] = {
                            {"x1" , homeBorder[m].getX1()},
                            {"y1" , homeBorder[m].getY1()},
                            {"x2" , homeBorder[m].getX2()},
                            {"y2" , homeBorder[m].getY2()},
                    };
                }
            }
        }

        cout<<"Polygon Area = "<<polygon1.getArea()<<"\n";

        response[solutionIndex]["Inner"][ans.size()]["id"] =  "";
        response[solutionIndex]["Inner"][ans.size()]["area"] = outerLand.getArea();
        response[solutionIndex]["Inner"][ans.size()]["green_area"] = !outerLand.isDivisible();
        response[solutionIndex]["Inner"][ans.size()]["roadExtension"] = std::vector<crow::json::wvalue>{};
        response[solutionIndex]["Inner"][ans.size()]["homeBorder"] = std::vector<crow::json::wvalue>{};

        int index1 = 0;
        for(auto &p : outerLand.getPoints())
        {
            response[solutionIndex]["Inner"][ans.size()]["Points"][index1++] = {
                    {"x" , p.getX()} ,
                    {"y" , p.getY()}
            };
        }

        for(int i = 0 ; i < streets.size(); i++)
        {
            auto pol = streets[i];
            int index = 0;
            response[solutionIndex]["streets"][i]["id"] = i + 1;
            response[solutionIndex]["streets"][i]["area"] = pol.getArea();
            response[solutionIndex]["streets"][i]["green_area"] = !pol.isDivisible();
            for(auto &p : pol.getPoints())
            {
                response[solutionIndex]["streets"][i]["Points"][index++] = {
                        {"x" , p.getX()} ,
                        {"y" , p.getY()}
                };
            }
        }

        crow::response finalRes (200 , response);

        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });
}

void APIController::landDivisionRoutesStreets(SimpleApp &app)
{
    CROW_ROUTE(app , "/LandDivision2").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;

        double divisionArea = jsonData["dimensions"]["divisionArea"].d();
        double externalRoad = jsonData["dimensions"]["externalRoad"].d();
        double centralRoad = jsonData["dimensions"]["centralRoad"].d();
        double circularStreet = jsonData["dimensions"]["circularStreet"].d();
        double landDepth = jsonData["dimensions"]["landDepth"].d();
        double streetCut = jsonData["dimensions"]["streetCut"].d(); // for circular Streets


        int strategy = (int)jsonData["strategy"].i();
        auto polygon = jsonData["polygon"];
        vector<Point> points;
        for(auto &point : polygon)
        {
            double x = point["x"].d();
            double y = point["y"].d();
            points.emplace_back(x , y);
        }

        Polygon1 polygon1(points);

        cout<<"Area = "<<polygon1.getArea() <<" \n";

        DrawStreet drawStreet;
        drawStreet.drawStreets(polygon1, divisionArea , externalRoad , centralRoad , circularStreet , landDepth , streetCut);
        vector<Line> centerLines = drawStreet.getCenterLines();
        vector<CityGrid> cities = drawStreet.getCities();

        for (int i = 0; i < centerLines.size(); ++i)
        {
            response[0]["centerLines"][i] = {
                    {"x1" , centerLines[i].getX1()},
                    {"y1" , centerLines[i].getY1()},
                    {"x2" , centerLines[i].getX2()},
                    {"y2" , centerLines[i].getY2()},
            };
        }

        for(int i = 0 ; i < cities.size() ; i++)
        {
            CityGrid city = cities[i];
            vector<Line> roadExtension = city.getRoadExtension();
            vector<Line> homeBorder = city.getHomeBorder();
            vector<Line> roads = city.getInnerStreets();
            int n = roads.size();

            for (int j = 0; j < roads.size(); ++j) {
                response[0]["roads"][i][j] = {
                        {"x1" , roads[j].getX1()},
                        {"y1" , roads[j].getY1()},
                        {"x2" , roads[j].getX2()},
                        {"y2" , roads[j].getY2()},
                };
            }

            roads = city.getOuterStreets();
            for (int j = 0; j < roads.size(); ++j) {
                response[0]["roads"][i][n+j] = {
                        {"x1" , roads[j].getX1()},
                        {"y1" , roads[j].getY1()},
                        {"x2" , roads[j].getX2()},
                        {"y2" , roads[j].getY2()},
                };
            }

            for (int j = 0; j < roadExtension.size(); ++j) {
                response[0]["roadExtension"][i][j] = {
                        {"x1" , roadExtension[j].getX1()},
                        {"y1" , roadExtension[j].getY1()},
                        {"x2" , roadExtension[j].getX2()},
                        {"y2" , roadExtension[j].getY2()},
                };
            }

            for (int j = 0; j < homeBorder.size(); ++j) {
                response[0]["homeBorder"][i][j] = {
                        {"x1" , homeBorder[j].getX1()},
                        {"y1" , homeBorder[j].getY1()},
                        {"x2" , homeBorder[j].getX2()},
                        {"y2" , homeBorder[j].getY2()},
                };
            }
        }

//        response["homeBorder"] = std::vector<crow::json::wvalue>{};


        crow::response finalRes (200 , response);

        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });
}

void APIController::subLandDivisionRoutes(SimpleApp &app)
{
    CROW_ROUTE(app , "/SubLandDivision").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        int strategy = jsonData.count("strategy")?(int)jsonData["strategy"].i() : 0;
        auto polygon = jsonData["polygon"]["Points"];
        vector<Point> points;
        for(auto &point : polygon)
        {
            double x = point["x"].d();
            double y = point["y"].d();
            points.emplace_back(x , y);
        }
        Polygon1 subLand(points);

        cout<<"Area = "<<subLand.getArea() <<" \n";
        vector<Polygon1> ans;
        Land land(subLand);
        Design design1 ;
        string text;
        response ["Details"] = {};

        for (int i = 3; i <= 5; ++i)
        {
            design1 = templatesDesigns.getDesignByBedrooms(i);
            if(design1.getRooms().empty())
            {
                response ["Details"][i-3] = "0";
                continue;
            }

            ans = land.SplitLand(design1 , static_cast<LandDivisionSortingStrategy>(strategy));

            text =  "For "+ to_string(i) + " Bedrooms : Land can be divided into " + to_string(ans.size());

            response ["Details"][i-3] = text;
        }
        crow::response finalRes (200 , response);
        finalRes.add_header("Access-Control-Allow-Origin", "*");
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });
}

void APIController::requestFramingRoutes(SimpleApp &app) {
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
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

}

void APIController::rotateDesignRoutes(SimpleApp &app)
{
    CROW_ROUTE(app , "/RotateDesign").methods(crow::HTTPMethod::POST)([&](const crow::request&req)
    {
        auto jsonData = crow::json::load(req.body);
        std::cout << "Request Body: " << req.body << std::endl;
        if (!jsonData) {
            return crow::response(400, "Invalid JSON format");
        }
        crow::json::wvalue response;
        double degree = jsonData["rotation"].d();
        const auto& designJson =  jsonData["Design"];
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
            Room newRoom(id , x1/100 , y1/100 , x2/100 , y2/100);

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

            string imagePath =  "";

            if(room.count("image")) imagePath = room["image"].s();
            newRoom.setImagePath(imagePath);

            mainDesign.addRoom(newRoom);
        }

        if(degree < 360)mainDesign.rotate(degree);
        else if(degree == 450)mainDesign.mirrorRecX();
        else if(degree == 540)mainDesign.mirrorRecY();


        mainDesign.scaleDesign(100);


        DesignToDoublyLines drawing(mainDesign);
        vector<Line>oldLines = drawing.getRecLines();
        vector<Line>innerLines = drawing.getInnerLines();
        vector<Room>surface3D = drawing.getNewSurfaces();
        map<string,int>sortedRectangles = drawing.getSortedRectangles();
        vector<Line> drawingLines = drawing.getNewLines();
        vector<Room>newRectangle = drawing.getNewRectangles();
        vector<Room>oldRectangle = drawing.getRectangles();
        map < string , vector<double> >dimensions = drawing.calculateHomeDimensions(mainDesign);
        vector<Room> room3DPrinting = drawing.getRoom3DPrinting();
        JsonExtraction::outputJsonFile
        (
                response,
                mainDesign,
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
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });
}

void APIController::modifyDesignRoutes(SimpleApp &app) {
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
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

}

void APIController::requestDesignRoutes(SimpleApp &app)
{
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
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });
}

void APIController::requestDesign2Routes(SimpleApp &app)
{
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
        finalRes.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        finalRes.add_header("Access-Control-Allow-Headers", "Content-Type");
        return finalRes;
    });

}



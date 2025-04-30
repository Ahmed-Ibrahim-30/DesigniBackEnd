#include "src/Controller/APIController.h"
#include "src/Model/BuildingDesigner/BuildingDesignerCorridorBesideLiving.h"
#include "src/Model/BuildingDesigner/BuildingDesigner.h"
int main() {

    SimpleApp app;

    APIController apiController(app);

    app.port(8080).multithreaded().run();


//    vector<string> zone1 {"Bedroom1","Bath1", "Bedroom2", "Bedroom3","Bath2", "Bedroom4" ,"Bedroom5","Bedroom6" ,"Bath3"  };
//    vector<string> zone2 {"Dining", "Kitchen", "GBath", "Family" , "Garage"};
//
//    vector<pair<string , string>> conn {
//            {"Bedroom1", "Bedroom2"},
//            {"Bath1", "Bedroom2"},
//            {"Bath2", "Bedroom3"},
//            {"Dining", "Bedroom4"},
//            {"Kitchen", "GBath"},
//            {"Family", "Office"},
//    };
//
//    map<string , double> areas;
//
//    areas["Dining"] = 20;
//    areas["Kitchen"] = 16;
//    areas["GBath"] = 4;
//    areas["Family"] = 24;
//    areas["Living"] = 35;
//    areas["Garage"] = 18;
//
//    areas["Bedroom1"] = 25;
//    areas["Bedroom2"] = 20;
//    areas["Bedroom3"] = 20;
//    areas["Bedroom4"] = 20;
//    areas["Bath1"] = 6;
//    areas["Bath2"] = 6;
//    areas["Bedroom5"] = 25;
//    areas["Bedroom6"] = 25;
//    areas["Bath3"] = 6;
//
//    BuildingDesigner *buildingDesigner = new BuildingDesignerCorridorBesideLiving(zone1 , zone2 , conn , areas);
//    vector<Design> firstFloor = buildingDesigner->generateDesign();
    return 0;
}

//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_JSONEXTRACTION_H
#define DESIGNI_JSONEXTRACTION_H
#include "crow.h"
#include <fstream>
#include "src/Model/Design.h"
#include "src/Model/DesignGeometryManager.h"
class JsonExtraction {
public:
    static void outputJsonFile(crow::json::wvalue &jsonFile ,
                               Design &roomCollection ,
                               int solutionIndex,
                               vector<Room>&rooms,
                               vector<Room>&newRooms,
                               vector<Line>&oldLines,
                               vector<Line>&drawingLines,
                               double thickness,
                               const map<pair<string,string>,pair<int,int>>&top,
                               const map<pair<string,string>,pair<int,int>>&bottom,
                               const map<pair<string,string>,pair<int,int>>&left,
                               const map<pair<string,string>,pair<int,int>>&right,
                               vector<Room>&surface3d,
                               map<string,int>&sortedRoom,
                               map < string , vector<double> >dimensions,
                               vector<Line>&innerLines,
                               double area,
                               vector<Room> room3DPrinting
    );

private:
    static void storeLines(int index,crow::json::wvalue &outputJson,vector<Line>&oldLines,double thickness,
                           const map<pair<string,string>,pair<int,int>>&top,
                           const map<pair<string,string>,pair<int,int>>&bottom,
                           const map<pair<string,string>,pair<int,int>>&left,
                           const map<pair<string,string>,pair<int,int>>&right,
                           vector<Line>&innerLines);
    static void storeSurfaces(int index,crow::json::wvalue &outputJson,vector<Room>&surface3d,map<string,int>&sortedRectangles);
    static void storeRooms(int index,crow::json::wvalue &outputJson,vector<Room>&surface3d,map<string,int>&sortedRectangles);

    static void storeSpaces(int index , crow::json::wvalue &outputJson ,Design &roomCollection  , vector<Line>&innerLines);
    static void storeDirectionsDimensions(int index, crow::json::wvalue &outputJson , map < string , vector<double> >dimensions );
    static void storeRoofs(int solutionIndex, crow::json::wvalue &jsonFile , const Design &roomCollection);

    static void store3DPrinting(int solutionIndex, crow::json::wvalue &jsonFile ,const vector<Room> &room3DPrinting);
};


#endif //DESIGNI_JSONEXTRACTION_H

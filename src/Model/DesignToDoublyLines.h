//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_DESIGNTODOUBLYLINES_H
#define DESIGNI_DESIGNTODOUBLYLINES_H
#include "Design.h"
#include "Line.h"

class DesignToDoublyLines{
private:

    vector<Point>freeCorners;
    double thickness,space,doorDistance;
    map<pair<string,string>,pair<int,int>>mapTopLine,mapBottomLine;
    map<pair<string,string>,pair<int,int>>mapLeftLine,mapRightLine;
    vector<Room>rectangles;
    vector<Room>newSurfaces;
    vector<Room>newRectangles;//after double Lines
    vector<Line>recLines,newLines;
    vector<Line> innerLines ;//innerLines
    map<string,int>sortedRectangles;

    vector<Room>room3DPrinting;

public:

    const map<string, int> &getSortedRectangles() const;
    explicit DesignToDoublyLines(Design &solution);

    /**
     * Extract 3D Printing
     * @param roomsLine
     * @return
     */
    vector<Room> extract3DPrinting(const vector<Line>&roomsLine );

    /**
     * Get Room 3DPrinting
     * @return
     */
    vector<Room> getRoom3DPrinting() const ;

    void getAnalysisData();
    void getMainCorners();
    void printAnalysisData();
    void printMainCorners();
    double getRoomArea(const string &roomId,double unit);
    double getRoomX(const string &roomId,double unit);
    double getRoomY(const string &roomId,double unit);
    vector<Line> getArchitecturalElementsInSpecificRoom(const string &roomId,const string &architecturalElement,double unit);
    /**
     * calculate if line is T1,T2,T3,T4 shape and each Sides
     */
    void setLineProperties();
    void applyLineThickness();
    void setRectanglesBesides();
    void printNewLinesAfterThickness();

    map < string , vector<double> > calculateHomeDimensions(const Design &home);

    //call after get newLines
    void assignNewRectangle();
    [[nodiscard]] const vector<Line> &getNewLines() const;
    [[nodiscard]] const vector<Line> &getInnerLines() const;
    [[nodiscard]] const vector<Room> &getNewRectangles() const ;

    [[nodiscard]] const vector<Room> &getNewSurfaces() const;

    void setNewSurfaces(const vector<Room> &newSurfaces);
    [[nodiscard]] const vector<Room> &getRectangles() const;

    [[nodiscard]] const vector<Line> &getRecLines() const;

    [[nodiscard]] double getThickness() const;

    [[nodiscard]] double getSpace() const;

    [[nodiscard]] double getDoorDistance() const;

    [[nodiscard]] const map<pair<string, string>, pair<int, int>> &getMapTopLine() const;

    [[nodiscard]] const map<pair<string, string>, pair<int, int>> &getMapBottomLine() const;

    [[nodiscard]] const map<pair<string, string>, pair<int, int>> &getMapLeftLine() const;

    [[nodiscard]] const map<pair<string, string>, pair<int, int>> &getMapRightLine() const;

};


#endif //DESIGNI_DESIGNTODOUBLYLINES_H

//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_DESIGNGEOMETRYMANAGER_H
#define DESIGNI_DESIGNGEOMETRYMANAGER_H
#include "Design.h"
#include "Line.h"
#include "Polygon.h"
/**
 * Responsible of all Design Functionality
 */
class DesignGeometryManager {
private:
    static bool onSegment(const Point& p, const Point& q, const Point& r);
    static int orientation(const Point& p, const Point& q, const Point& r);
    static bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) ;
public:
    static double calculateSharedWalls(const Design &rooms);

    /**
     * @brief isShapeFaceRequiredFaces
     * @param shape
     * @param views
     * @return
     */
    static bool isShapeFaceRequiredFaces( Design &shape, map<string, vector<char> > &views);
    /**
     * Is Room Face Dir
     * @param dir
     * @param rec
     * @param recs
     * @return
     */
    static bool isRoomFaceDir (char dir,const Room &rec,const vector<Room>&recs);

    static void scaleRectangles(vector<Room>&rectangles,double unit);
    static set<char>getRoomFaces(const Room &rec,vector<Room>&recs);

    static bool isPointInsidePolygon(double x, double y,const vector<Line> &main);
    static vector<Point> getLinesPoints(const vector<Line>&main);

    /**
     * Get all Lines from Rectangles
     * @param rectangles
     * @return
     */
    static vector<Line> getLines(vector<Room> &rooms);
    static void sortRoomsBasedOnIds(vector<Room> &rectangles,const string& id, int &curIndex);
    static vector<Point> getMainCorner(vector<Room> &rectangles);
    static void setLinesSides(vector<Line> &lines);
    static void setLineBottomSide(Line &myLine, vector<Line> &lines) ;
    static void setLineLeftSide(Line &myLine, vector<Line> &lines) ;
    static void setLineRightSide(Line &myLine, vector<Line> &lines);
    static void setLineTopSide(Line &myLine, vector<Line> &lines) ;
    static vector<Line> applyLineSideRule(Line &line,double thickness,
                                          map<pair<string,string>,pair<int,int>>top,
                                          map<pair<string,string>,pair<int,int>>bottom,
                                          map<pair<string,string>,pair<int,int>>left,
                                          map<pair<string,string>,pair<int,int>>right,
                                          vector<Line>&innerLines) ;

    static vector<Room> applyLine3DSideRule(Line &line,double thickness,
                                            map<pair<string,string>,pair<int,int>>top,
                                            map<pair<string,string>,pair<int,int>>bottom,
                                            map<pair<string,string>,pair<int,int>>left,
                                            map<pair<string,string>,pair<int,int>>right) ;

    static void setRoomBesides(vector<Room> &recs);
    static map<string,int> sortRooms(vector<Room> &recs) ;
    static vector<Room> create3DSurface(const string &wallName,double x1, double x2, double y1, double y2, double z1, double z2,char lineFace);
    static bool isLineExternal(Line &line,const vector<Room> &rectangles) ;
    static void DFS(const string &start,vector<string>&result,set<string>&visited,map<string,vector<string>>&graph) ;

    /**
     * Is Two Rooms overlapping?
     * @param one
     * @param two
     * @return
     */
    static bool isTwoRoomsIntersect(const Room &one, const Room &two);

    /**
     * IsTwo Rooms Connected
     * @param one
     * @param two
     * @return
     */
    static bool isTwoRoomsConnected(const Room &one , const Room &two ) ;

    /**
     * Are Rooms Overlapped
     * @param rect1
     * @param rect2
     * @return
     */
    static bool areRoomsOverlapped(const Room& rect1, const Room& rect2);

    /**
     * @brief getLinePoints
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param unit
     * @return
     */
    static vector<Point> getLinePoints(double x1,double y1,double x2,double y2, double unit) ;

    /**
     * @brief isPointOnPolygon
     * @param x
     * @param y
     * @param main
     * @param unit
     * @return
     */
     static bool isPointOnPolygon(double x, double y,const vector<Line> &main,double unit);

    /**
     * @brief isPolygonCompletelyInsidePolygon
     * @param input
     * @param zone
     * @param unit
     * @return
     */
    static bool isPolygonCompletelyInsidePolygon(const vector<Line>& input, const vector<Line>& zone,
                                                         double unit);

    /**
     * @brief positionPolygonInsideAnother
     * @param input
     * @param pol
     * @return
     */
    static bool positionPolygonInsideAnother(Polygon1 &input, Polygon1 &pol);

    /**
     * @brief positionPolygonInsideAnother
     * @param input
     * @param pol
     * @return
     */
    static bool positionPolygonInsideAnotherUsingAllAngles(Polygon1 &input, Polygon1 &pol);

    static bool isLeft(const Point& a, const Point& b, const Point& p);
    static bool isPointInsidePolygon(const vector<Point>& polygon, const Point& point);
    static bool doSegmentsIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) ;
    static bool isPolygonInsidePolygon(const Polygon1& inner, const Polygon1& outer) ;
};


#endif //DESIGNI_DESIGNGEOMETRYMANAGER_H

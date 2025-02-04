//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#include "DesignGeometryManager.h"
#include "deque"
bool DesignGeometryManager::isShapeFaceRequiredFaces( Design &shape, map<string, vector<char> > &views)
{
    for(auto &room : shape.getRooms())
    {
        bool flag = true;
        string id = room.getRoomId();
        transform(id.begin() , id.end() , id.begin() , ::tolower);
        for(auto dir : views[id])
        {
            flag = isRoomFaceDir(dir, room , shape.getRooms());
            if(!flag) return false;
        }
    }
    return true;
}

bool DesignGeometryManager::isRoomFaceDir(char dir,const Room &rec,const vector<Room> &recs)
{
    double x1 = rec.getX1() , y1 = rec.getY1() ,x2 = rec.getX2() , y2 = rec.getY2();
    for(const auto &rectangle:recs)
    {
        if(rec.getRoomId() == rectangle.getRoomId())continue;

        if(dir == 'N')
        {
            if(rectangle.getY2()<=y1)
            {//upper
                if((x1>=rectangle.getX1() && x1<rectangle.getX2()) ||
                   (rectangle.getX1()>=x1 && rectangle.getX1()<x2)){
                    return false;
                }
            }
        }
        else if(dir=='S'){
            if(rectangle.getY1()>=y1)
            {//upper
                if((x1>=rectangle.getX1() && x1<rectangle.getX2()) ||
                   (rectangle.getX1()>=x1 && rectangle.getX1()<x2)){
                    return false;
                }
            }
        }

        else if(dir=='E')
        {
            if(rectangle.getX1()>=x2)
            {//upper
                if((y1>=rectangle.getY1() && y1<rectangle.getY2()) ||
                   (rectangle.getY1()>=y1 && rectangle.getY1()<y2)){
                    return false;
                }
            }
        }
        else if(dir=='W')
        {
            if(rectangle.getX2()<=x1)
            {//upper
                if((y1>=rectangle.getY1() && y1<rectangle.getY2()) ||
                   (rectangle.getY1()>=y1 && rectangle.getY1()<y2)){
                    return false;
                }
            }
        }
    }
    return true;
}

double DesignGeometryManager::calculateSharedWalls(const Design &design)
{
    double sharedWallsArea=0;

    for(auto &room: design.getRooms())
    {
        double x1 = room.getX1(), y1 = room.getY1();
        double x2 = room.getX2(), y2 = room.getY2();

        for(auto &room2:design.getRooms())
        {
            if(room.getRoomId() == room2.getRoomId())continue;

            //shared walls in North,South
            if( y1==room2.getY2() || y2==room2.getY1()){
                if((x1>=room2.getX1() && x1<room2.getX2()) ||
                   (room2.getX1()>=x1 && room2.getX1()<x2) ){
                    double first = max(x1,room2.getX1());
                    double second = min(x2,room2.getX2());
                    sharedWallsArea+=(second-first);
                }
            }
            //shared walls in East,West
            if(x2==room2.getX1() || x1==room2.getX2())
            {
                if((y1>=room2.getY1() && y1<room2.getY2()) ||
                   (room2.getY1()>=y1 && room2.getY1()<y2) )
                {
                    double first = max(y1,room2.getY1());
                    double second = min(y2,room2.getY2());
                    sharedWallsArea+=(second-first);
                }
            }
        }
    }
    return sharedWallsArea;
}

bool DesignGeometryManager::onSegment(const Point& p, const Point& q, const Point& r)
{
    return q.getX() <= std::max(p.getX(), r.getX()) && q.getX() >= std::min(p.getX(), r.getX()) &&
           q.getY() <= std::max(p.getY(), r.getY()) && q.getY() >= std::min(p.getY(), r.getY());
}

int DesignGeometryManager::orientation(const Point& p, const Point& q, const Point& r)
{
    double val = (q.getY() - p.getY()) * (r.getX() - q.getX()) - (q.getX() - p.getX()) * (r.getY() - q.getY());

    if (std::abs(val) < 1e-9) return 0; // collinear
    return (val > 0) ? 1 : 2; // clock or counter clock wise
}

bool DesignGeometryManager::doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special cases
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

void DesignGeometryManager::scaleRectangles(vector<Room> &rooms, double unit) {
    for(auto &rec:rooms)
    {
        double x1, y1, x2, y2;
        x1 = rec.getX1();
        x2 = rec.getX2();
        y1 = rec.getY1();
        y2 = rec.getY2();
        rec.setX1(x1 * unit);
        rec.setY1(y1 * unit);
        rec.setX2(x2 * unit);
        rec.setY2(y2 * unit);
    }
}

set<char> DesignGeometryManager::getRoomFaces(const Room &rec, vector<Room> &recs)
{
    double x1 = rec.getX1() , y1 = rec.getY1() , x2 = rec.getX2() , y2=rec.getY2();

    set<char>faces={'N','S','E','W'};
    for(auto &room:recs)
    {
        if(room.getRoomId()==rec.getRoomId())continue;
        if(room.getY2()<=y1){//upper
            if((x1>=room.getX1() && x1<room.getX2()) ||
               (room.getX1()>=x1 && room.getX1()<x2)){
                faces.erase('N');
            }
        }
        if(room.getY1()>=y1){//upper
            if((x1>=room.getX1() && x1<room.getX2()) ||
               (room.getX1()>=x1 && room.getX1()<x2)){
                faces.erase('S');
            }
        }

        if(room.getX1()>=x1){//upper
            if((y1>=room.getY1() && y1<room.getY2()) ||
               (room.getY1()>=y1 && room.getY1()<y2)){
                faces.erase('E');
            }
        }

        if(room.getX2()<=x1){//upper
            if((y1>=room.getY1() && y1<room.getY2()) ||
               (room.getY1()>=y1 && room.getY1()<y2)){
                faces.erase('W');
            }
        }
    }
    return faces;
}

bool DesignGeometryManager::isPointInsidePolygon(double x, double y, const vector<Line> &main) {
    bool isInside = false;
    vector<Point>polygon = getLinesPoints(main);
    int n = (int)polygon.size();
    if (n < 3) return false;

    for(auto &line : main){
        if(x>=line.getX1() && x<=line.getX2() && y>=line.getY1() && y<=line.getY2()){
            return true;
        }
    }


    for (int i = 0, j = n - 1; i < n; j = i++) {
        // Check if the point is on the edge of the polygon
        if (orientation(polygon[i], {x,y}, polygon[j]) == 0 && onSegment(polygon[i], {x,y}, polygon[j])) {
            return true; // The point is on the boundary
        }

        // Handle horizontal edges and other tricky cases
        if (polygon[i].getY() == y && polygon[j].getY() == y && x >= min(polygon[i].getX(), polygon[j].getX()) && x <= max(polygon[i].getX(), polygon[j].getX())) {
            return true; // The point is on a horizontal edge
        }

        // Ray-casting algorithm to check if the point is inside the polygon
        if ((polygon[i].getY() > y) != (polygon[j].getY() > y) &&
            x < (polygon[j].getX() - polygon[i].getX()) * (y - polygon[i].getY()) / (polygon[j].getY() - polygon[i].getY()) + polygon[i].getX()) {
            isInside = !isInside;
        }
    }

    return isInside;
}

vector<Point> DesignGeometryManager::getLinesPoints(const vector<Line> &main)
{
    set<pair<double,double>>pnt;
    vector <Point>points;
    points.emplace_back(main[0].getX1(),main[0].getY1());
    pnt.emplace(main[0].getX1(),main[0].getY1());

    while (true)
    {
        double x1=points.back().getX();
        double y1=points.back().getY();
        bool flag=false;
        for(auto &lin:main)
        {

            if (x1==lin.getX1() && y1==lin.getY1() && !pnt.count({lin.getX2(),lin.getY2()})){
                points.emplace_back(lin.getX2(),lin.getY2());
                pnt.emplace(lin.getX2(),lin.getY2());
                flag=true;
                break;
            }
            if (x1==lin.getX2() && y1==lin.getY2()&& !pnt.count({lin.getX1(),lin.getY1()})){
                points.emplace_back(lin.getX1(),lin.getY1());
                pnt.emplace(lin.getX1(),lin.getY1());
                flag=true;
                break;
            }
        }
        if (!flag)break;
    }
    points.emplace_back(points[0]);
    return points;
}

vector<Line> DesignGeometryManager::getLines(vector<Room> &rooms) {
    vector<Line>lines;
    set<double>xValues,yValues;
    set<pair<double,double>>points;
    map<Line,string>lineMappingId;//store all windows and doors

    for(auto &rec:rooms)
    {
        xValues.insert(rec.getX1());
        xValues.insert(rec.getX2());
        yValues.insert(rec.getY1());
        yValues.insert(rec.getY2());
        points.emplace(rec.getX1(),rec.getY1());
        points.emplace(rec.getX1(),rec.getY2());
        points.emplace(rec.getX2(),rec.getY1());
        points.emplace(rec.getX2(),rec.getY2());

        for(auto &w :rec.getWindows()){
            Line line = w.getWindow();
            lineMappingId[line] = "Window";
            xValues.insert(line.getX1());
            xValues.insert(line.getX2());
            yValues.insert(line.getY1());
            yValues.insert(line.getY2());
            points.emplace(line.getX1(),line.getY1());
            points.emplace(line.getX1(),line.getY2());
            points.emplace(line.getX2(),line.getY1());
            points.emplace(line.getX2(),line.getY2());
        }
        for(auto &w :rec.getDoors())
        {
            Line line = w.getDoor();
            lineMappingId[line] = "Door";

            xValues.insert(line.getX1());
            xValues.insert(line.getX2());
            yValues.insert(line.getY1());
            yValues.insert(line.getY2());

            points.emplace(line.getX1(),line.getY1());
            points.emplace(line.getX2(),line.getY2());
        }
        for(auto &w :rec.getOpenings()){
            Line line = w.getOpening();
            lineMappingId[line] = "Openning";
            xValues.insert(line.getX1());
            xValues.insert(line.getX2());
            yValues.insert(line.getY1());
            yValues.insert(line.getY2());
            points.emplace(line.getX1(),line.getY1());
            points.emplace(line.getX1(),line.getY2());
            points.emplace(line.getX2(),line.getY1());
            points.emplace(line.getX2(),line.getY2());
        }
    }
    set<Line>visitedLine;
    for(auto &rec : rooms)
    {
        if (rec.getX1()==rec.getX2() || rec.getY1()==rec.getY2())continue;//line

        double x1 = rec.getX1() , y1 = rec.getY1() , x2 = rec.getX2() , y2 = rec.getY2() ;
        string id = rec.getRoomId();

        Line a(x1,y1,x2,y1);//top
        Line b(x2,y1,x2,y2);//right
        Line c(x1,y2,x2,y2);//bottom
        Line d(x1,y1,x1,y2);//left



        for(auto &w :rec.getDoors())
        {
            Line newLine (w.getDoor().getX1(),w.getDoor().getY1(),w.getDoor().getX2(),w.getDoor().getY2());

            Point p1(newLine.getX1(),newLine.getY1());
            Point p2(newLine.getX2(),newLine.getY2());

            if(newLine.getY1() == newLine.getY2())
            {
                if(newLine.getY1() == y1 || abs(newLine.getY1() - y1) < 0.1)//top
                {
                    if (p1.getX()==x1)p1.setId("C1");
                    if (p2.getX()==x2)p2.setId("C2");
                    newLine.setSideNum(1);
                }
                else if(newLine.getY1() == y2 || abs(newLine.getY1() - y2) < 0.1)//bottom
                {
                    if (p1.getX()==x1)p1.setId("C4");
                    if (p2.getX()==x2)p2.setId("C3");
                    newLine.setSideNum(3);
                }
            }
            else
            {
                if(newLine.getX1() == x1 || abs(newLine.getX1() - x1) < 0.1)//left
                {
                    if (p1.getY()==y1)p1.setId("C1");
                    if (p2.getY()==y2)p2.setId("C4");
                    newLine.setSideNum(4);
                }
                else if(newLine.getX1() == x2 || abs(newLine.getX1() - x2) < 0.1)//right
                {
                    if (p1.getY()==y1)p1.setId("C2");
                    if (p2.getY()==y2)p2.setId("C3");
                    newLine.setSideNum(2);
                }
            }

            newLine.setFirst(p1);
            newLine.setSecond(p2);
            newLine.setId(id);
            newLine.setId2("Door");
            lines.push_back(newLine);
        }

        //top --> y1 = y2 --> C1 C2
        vector<double>xP;//get All X set on Y value
        for(auto &x:xValues)
        {
            if (x<x1 || x>x2)continue;
            if (points.count({x,a.getY1()})){
                xP.push_back(x);
            }
        }
        for(int i = 1 ; i < xP.size(); i++)
        {
            if(!visitedLine.count({xP[i-1],a.getY1(),xP[i],a.getY1()}))
            {
                Line newLine(xP[i-1],a.getY1(),xP[i],a.getY1());
                Point p1(xP[i-1],a.getY1());
                Point p2(xP[i],a.getY1());
                if (xP[i-1]==x1)p1.setId("C1");
                if (xP[i]==x2)p2.setId("C2");
                newLine.setFirst(p1);
                newLine.setSecond(p2);
                newLine.setId(id);
                //window
                newLine.setSideNum(1);

                if (lineMappingId.count(newLine)){
                    newLine.setId2(lineMappingId[newLine]);
                }

                if(newLine.getId2() != "Door")lines.push_back(newLine);

                //lines.push_back(newLine);
                visitedLine.emplace(xP[i-1],a.getY1(),xP[i],a.getY1());
            }
        }

        //right --> x1 = x2 --> C2 C3
        vector<double>yP;//get All X set on Y value
        for(auto &y:yValues)
        {
            if(y<y1 || y>y2)continue;
            if (points.count({b.getX1(),y})){
                yP.push_back(y);
            }
        }
        for(int i = 1 ; i < yP.size(); i++)
        {
            if(!visitedLine.count({b.getX1(),yP[i-1],b.getX1(),yP[i]})){
                Line newLine(b.getX1(),yP[i-1],b.getX1(),yP[i]);
                Point p1(b.getX1(),yP[i-1]);
                Point p2(b.getX1(),yP[i]);
                if (yP[i-1]==y1)p1.setId("C2");
                if (yP[i]==y2)p2.setId("C3");
                newLine.setFirst(p1);
                newLine.setSecond(p2);
                newLine.setId(id);
                if (lineMappingId.count(newLine)){
                    newLine.setId2(lineMappingId[newLine]);
                }
                newLine.setSideNum(2);
                if(newLine.getId2() != "Door")lines.push_back(newLine);
                //lines.push_back(newLine);
                visitedLine.emplace(b.getX1(),yP[i-1],b.getX1(),yP[i]);
            }
        }
        //bottom --> y1 = y2
        xP.clear();//get All X set on Y value
        for(auto &x:xValues)
        {
            if (x<x1 || x>x2)continue;
            if (points.count({x,c.getY1()})){
                xP.push_back(x);
            }
        }
        for(int i = 1 ; i < xP.size(); i++)
        {
            if(!visitedLine.count({xP[i-1],c.getY1(),xP[i],c.getY1()})){
                Line newLine(xP[i-1],c.getY1(),xP[i],c.getY1());
                Point p1(xP[i-1],c.getY1());
                Point p2(xP[i],c.getY1());
                if (xP[i-1]==x1)p1.setId("C4");
                if (xP[i]==x2)p2.setId("C3");
                newLine.setFirst(p1);
                newLine.setSecond(p2);
                newLine.setId(id);
                if (lineMappingId.count(newLine)){
                    newLine.setId2(lineMappingId[newLine]);
                }
                newLine.setSideNum(3);
                if(newLine.getId2() != "Door")lines.push_back(newLine);
                //lines.push_back(newLine);
                visitedLine.emplace(xP[i-1],c.getY1(),xP[i],c.getY1());
            }
        }
        //left --> x1 = x2 --> C1 C4
        yP.clear();//get All X set on Y value
        for(auto &y:yValues)
        {
            if(y<y1 || y>y2)continue;
            if (points.count({d.getX1(),y})){
                yP.push_back(y);
            }
        }
        for(int i = 1 ; i < yP.size(); i++)
        {
            if(!visitedLine.count({d.getX1(),yP[i-1],d.getX1(),yP[i]})){
                Line newLine(d.getX1(),yP[i-1],d.getX1(),yP[i]);
                Point p1(d.getX1(),yP[i-1]);
                Point p2(d.getX1(),yP[i]);
                if (yP[i-1]==y1)p1.setId("C1");
                if (yP[i]==y2)p2.setId("C4");
                newLine.setFirst(p1);
                newLine.setSecond(p2);
                newLine.setId(id);
                if (lineMappingId.count(newLine)){
                    newLine.setId2(lineMappingId[newLine]);
                }
                newLine.setSideNum(4);
                if(newLine.getId2() != "Door")lines.push_back(newLine);
                //lines.push_back(newLine);
                visitedLine.emplace(d.getX1(),yP[i-1],d.getX1(),yP[i]);
            }
        }
    }

    //add external , internal property for each line
    for(auto &line:lines){
        //store status of each line (external or internal)
        //if external store face of each line (N,S,E,W)
        isLineExternal(line,rooms);
    }
    return lines;
}

vector<Point> DesignGeometryManager::getMainCorner(vector<Room> &rectangles)
{
    vector<Point>mainCorners;
    map<Point,int>pointsCounter;
    set<Point>points;

    for(auto &rec:rectangles)
    {
        points.emplace(rec.getX1(),rec.getY1());
        points.emplace(rec.getX1(),rec.getY2());
        points.emplace(rec.getX2(),rec.getY1());
        points.emplace(rec.getX2(),rec.getY2());
    }
    for(auto &point:points){
        for(auto &rec:rectangles){
            if (rec.getX1()==rec.getX2() || rec.getY1()==rec.getY2())continue;//line
            //left or right
            if ((point.getX()==rec.getX1() || point.getX()==rec.getX2()) && point.getY()>=rec.getY1() && point.getY()<=rec.getY2()){
                pointsCounter[point]++;
            }
            //top or bottom
            if ((point.getY()==rec.getY1() || point.getY()==rec.getY2()) && point.getX()>=rec.getX1() && point.getX()<=rec.getX2()){
                pointsCounter[point]++;
            }
        }
    }
    for(auto &rec:rectangles){
        if (rec.getX1()==rec.getX2() || rec.getY1()==rec.getY2())continue;//line
        Point a(rec.getX1(),rec.getY1());
        Point b(rec.getX1(),rec.getY2());
        Point c(rec.getX2(),rec.getY1());
        Point d(rec.getX2(),rec.getY2());
        if (pointsCounter[a]==2)mainCorners.push_back(a);
        if (pointsCounter[b]==2)mainCorners.push_back(b);
        if (pointsCounter[c]==2)mainCorners.push_back(c);
        if (pointsCounter[d]==2)mainCorners.push_back(d);
    }
    return mainCorners;
}

void DesignGeometryManager::sortRoomsBasedOnIds(vector<Room> &rectangles,const string& id, int &curIndex)
{
    for (int var = 0; var < rectangles.size(); ++var) {
        if(rectangles[var].getRoomId().find(id) != std::string::npos){
            swap(rectangles[var],rectangles[curIndex++]);
        }
    }
}

void DesignGeometryManager::setLinesSides(vector<Line> &lines)
{
    for (int line = 0; line < lines.size(); ++line)
    {
        if (lines[line].getDir()=='h'){//y1==y2
            setLineLeftSide(lines[line],lines);
            setLineRightSide(lines[line],lines);
        }else{
            setLineTopSide(lines[line],lines);
            setLineBottomSide(lines[line],lines);
        }
    }
}

void DesignGeometryManager::setLineLeftSide(Line &myLine, vector<Line> &lines)
{
    double x1= myLine.getX1(), y1=myLine.getY1();
    int cnt=0; bool wall=false;
    for(auto &line:lines)
    {
        if (myLine==line)continue;
        if (x1==line.getX2() && y1==line.getY2())cnt++;
        if (x1==line.getX1() && y1==line.getY1())cnt++;

        if (x1==line.getX2() && y1==line.getY2() && line.getDir()==myLine.getDir())wall=true;
    }
    if (!wall && cnt<2 && myLine.getFirst().getId()!="0"){
        myLine.setSideLeft(myLine.getFirst().getId());
        return;
    }
    if (wall && cnt<2){//window or door (Wall)
        myLine.setSideLeft("W");//C1 C2 C3 C4
        return;
    }else{
        bool top=false,bottom=false;
        for(auto &line:lines){
            if (myLine==line)continue;
            //top
            if (x1==line.getX2() && y1==line.getY2() && line.getDir()!=myLine.getDir()){
                top=true;
            }
            //bottom
            if (x1==line.getX1() && y1==line.getY1() && line.getDir()!=myLine.getDir()){
                bottom=true;
            }
        }
        if (top && bottom) myLine.setSideLeft("T2");
        else if (top) myLine.setSideLeft("T3");
        else if (bottom) myLine.setSideLeft("T4");
    }
}

void DesignGeometryManager::setLineRightSide(Line &myLine, vector<Line> &lines)
{
    double x2= myLine.getX2(), y2=myLine.getY2();
    int cnt=0; bool wall=false;
    for(auto &line:lines){
        if (myLine==line)continue;
        if (x2==line.getX2() && y2==line.getY2())cnt++;
        if (x2==line.getX1() && y2==line.getY1())cnt++;

        if (x2==line.getX1() && y2==line.getY1() && line.getDir()==myLine.getDir())wall=true;
    }
    if (!wall && cnt<2 && myLine.getSecond().getId()!="0"){
        myLine.setSideRight(myLine.getSecond().getId());//C1 C2 C3 C4
        return;
    }
    if (wall && cnt<2){//window or door (Wall)
        myLine.setSideRight("W");//C1 C2 C3 C4
        return;
    }else{
        bool top=false,bottom=false;
        for(auto &line:lines){
            if (myLine==line)continue;
            //top
            if (x2==line.getX2() && y2==line.getY2() && line.getDir()!=myLine.getDir()){
                top=true;
            }
            //bottom
            if (x2==line.getX1() && y2==line.getY1() && line.getDir()!=myLine.getDir()){
                bottom=true;
            }
        }
        if (top && bottom) myLine.setSideRight("T1");
        else if (top) myLine.setSideRight("T3");
        else if (bottom) myLine.setSideRight("T4");
    }
}

void DesignGeometryManager::setLineTopSide(Line &myLine, vector<Line> &lines)
{
    double x1= myLine.getX1(), y1=myLine.getY1();
    int cnt=0; bool wall=false;
    for(auto &line:lines){
        if (myLine==line)continue;
        if (x1==line.getX2() && y1==line.getY2())cnt++;
        if (x1==line.getX1() && y1==line.getY1())cnt++;

        if (x1==line.getX2() && y1==line.getY2() && line.getDir()==myLine.getDir())wall=true;
    }
    if (!wall && cnt<2 && myLine.getFirst().getId()!="0"){
        myLine.setSideLeft(myLine.getFirst().getId());
        return;
    }
    if (wall && cnt<2){//window or door (Wall)
        myLine.setSideLeft("W");//C1 C2 C3 C4
        return;
    }else{
        bool right=false,left=false;
        for(auto &line:lines){
            if (myLine==line)continue;
            //top
            if (x1==line.getX2() && y1==line.getY2() && line.getDir()!=myLine.getDir()){
                left=true;
            }
            //bottom
            if (x1==line.getX1() && y1==line.getY1() && line.getDir()!=myLine.getDir()){
                right=true;
            }
        }
        if (left && right) myLine.setSideLeft("T4");
        else if (left) myLine.setSideLeft("T1");
        else if (right) myLine.setSideLeft("T2");
    }
}

void DesignGeometryManager::setLineBottomSide(Line &myLine, vector<Line> &lines)
{
    double x2= myLine.getX2(), y2=myLine.getY2();
    int cnt=0; bool wall=false;
    for(auto &line:lines){
        if (myLine==line)continue;
        if (x2==line.getX2() && y2==line.getY2())cnt++;
        if (x2==line.getX1() && y2==line.getY1())cnt++;

        if (x2==line.getX1() && y2==line.getY1() && line.getDir()==myLine.getDir())wall=true;
    }
    if (!wall && cnt<2 && myLine.getSecond().getId()!="0"){
        myLine.setSideRight(myLine.getSecond().getId());//C1 C2 C3 C4
        return;
    }
    if (wall && cnt<2){//window or door (Wall)
        myLine.setSideRight("W");//C1 C2 C3 C4
        return;
    }else{
        bool right=false,left=false;
        for(auto &line:lines){
            if (myLine==line)continue;
            //top
            if (x2==line.getX2() && y2==line.getY2() && line.getDir()!=myLine.getDir()){
                left=true;
            }
            //bottom
            if (x2==line.getX1() && y2==line.getY1() && line.getDir()!=myLine.getDir()){
                right=true;
            }
        }
        if (left && right) myLine.setSideRight("T3");
        else if (left) myLine.setSideRight("T1");
        else if (right) myLine.setSideRight("T2");
    }
}

bool DesignGeometryManager::areRoomsOverlapped(const Room& rect1, const Room& rect2)
{
    // If one Room is to the left of the other
    if (rect1.getX2() <= rect2.getX1()  || rect2.getX2()  <= rect1.getX1())
        return false;

    // If one Room is above the other
    if (rect1.getY2() <= rect2.getY1() || rect2.getY2() <= rect1.getY1())
        return false;

    // Otherwise, they are Overlabed
    return true;
}


vector<Line> DesignGeometryManager::applyLineSideRule(Line &line,double thickness,
                                         map<pair<string,string>,pair<int,int>>top,
                                         map<pair<string,string>,pair<int,int>>bottom,
                                         map<pair<string,string>,pair<int,int>>left,
                                         map<pair<string,string>,pair<int,int>>right,
                                         vector<Line>&innerLines)
{
    string leftSide  = line.getSideLeft() ;
    string rightSide = line.getSideRight();
    double newThickness = thickness/2;
    double doorThickness=thickness/4;
    vector<Line>twoLines;
    if (line.getDir()=='h')//y1=y2
    {
        Line topLine(line.getX1()+top[{leftSide,rightSide}].first,line.getY1()-newThickness,line.getX2()+top[{leftSide,rightSide}].second,line.getY1()-newThickness);

        Line bottomLine(line.getX1()+bottom[{leftSide,rightSide}].first,line.getY1()+newThickness,line.getX2()+bottom[{leftSide,rightSide}].second,line.getY1()+newThickness);

        //add window and door for bottom and top line
        if (line.getSideNum()==1){
            bottomLine.setId(line.getId());
            bottomLine.setId2(line.getId2());
            if (line.getId2()!="Window" && line.getId2()!="Openning" && line.getId2()!="Door") innerLines.push_back(bottomLine);
        }else{//3
            topLine.setId(line.getId());
            topLine.setId2(line.getId2());
            if (line.getId2()!="Window" && line.getId2()!="Openning" && line.getId2()!="Door") innerLines.push_back(topLine);
        }

        if (line.getId2()=="Window" || line.getId2()=="Openning")
        {
            Line topWindow(line.getX1(),line.getY1()-(thickness/4),line.getX2(),line.getY2()-(thickness/4) );
            Line bottomWindow(line.getX1(),line.getY1()+(thickness/4),line.getX2(),line.getY2()+(thickness/4));


            Line three(line.getX1(),line.getY1()-newThickness,line.getX1(),line.getY1()+newThickness );
            Line four(line.getX2(),line.getY1()-newThickness,line.getX2(),line.getY1()+newThickness );


            topWindow.setColor("#E72929");//red
            bottomWindow.setColor("#E72929");

            topWindow.setCenterX1(line.getX1()); topWindow.setCenterY1(line.getY1());
            topWindow.setCenterX2(line.getX2()); topWindow.setCenterY2(line.getY2());

            bottomWindow.setCenterX1(line.getX1()); bottomWindow.setCenterY1(line.getY1());
            bottomWindow.setCenterX2(line.getX2()); bottomWindow.setCenterY2(line.getY2());

            twoLines.emplace_back(topWindow);
            twoLines.emplace_back(bottomWindow);
            twoLines.emplace_back(three);
            twoLines.emplace_back(four);
        }
        else if (line.getId2()=="Door")
        {//1 3
            double doorDistance=line.getX2()-line.getX1();
            doorDistance=line.getSideNum()==1?abs(doorDistance):abs(doorDistance)*-1;

            Line first(line.getX2()-doorThickness,line.getY1(),line.getX2()-doorThickness,line.getY1()+doorDistance );
            Line second(line.getX2(),line.getY1(),line.getX2(),line.getY1()+doorDistance );
            Line third(line.getX2()-doorThickness,line.getY1(),line.getX2(),line.getY1() );
            Line forth(line.getX2()-doorThickness,line.getY1()+ doorDistance,line.getX2(),line.getY1()+doorDistance );
            Line curvedLine(line.getX1(),line.getY1(),line.getX2(),line.getY1()+ doorDistance);
            Line six(line.getX1(),line.getY1()-newThickness,line.getX1(),line.getY1()+newThickness);
            Line seven(line.getX2(),line.getY2()-newThickness,line.getX2(),line.getY2()+newThickness);

            doorDistance=abs(doorDistance);
            curvedLine.setId2("Door");
            curvedLine.setSideNum(line.getSideNum());

            if (curvedLine.getSideNum()==1){//top
                curvedLine.setStartAngle(180);
                curvedLine.setEndAngle(270);
            }else if (curvedLine.getSideNum()==3){
                curvedLine.setStartAngle(90);
                curvedLine.setEndAngle(180);
            }

            first.setColor("#5BBCFF");
            second.setColor("#5BBCFF");
            third.setColor("#5BBCFF");
            forth.setColor("#5BBCFF");
            curvedLine.setColor("#5BBCFF");//blue

            first.setCenterX1(line.getX1()); first.setCenterY1(line.getY1());
            first.setCenterX2(line.getX2()); first.setCenterY2(line.getY2());

            second.setCenterX1(line.getX1()); second.setCenterY1(line.getY1());
            second.setCenterX2(line.getX2()); second.setCenterY2(line.getY2());

            twoLines.emplace_back(first); twoLines.emplace_back(second);
            twoLines.emplace_back(third); twoLines.emplace_back(forth);
            twoLines.emplace_back(curvedLine);
            twoLines.emplace_back(six); twoLines.emplace_back(seven);
        }

        if (line.getId2()!="Door" )
        {
            twoLines.emplace_back(topLine);
            twoLines.emplace_back(bottomLine);
        }
    }
    else{
        Line rightLine(line.getX1()+newThickness,line.getY1()+right[{leftSide,rightSide}].first,line.getX1()+newThickness,line.getY2()+right[{leftSide,rightSide}].second);
        Line leftLine(line.getX1()-newThickness,line.getY1()+left[{leftSide,rightSide}].first,line.getX1()-newThickness,line.getY2()+left[{leftSide,rightSide}].second);
        //add window and door for left line only
        if (line.getSideNum()==2){
            leftLine.setId(line.getId());
            leftLine.setId2(line.getId2());
            if (line.getId2()!="Window" && line.getId2()!="Openning" && line.getId2()!="Door") innerLines.push_back(leftLine);
        }else {//4
            rightLine.setId(line.getId());
            rightLine.setId2(line.getId2());
            if (line.getId2()!="Window" && line.getId2()!="Openning" && line.getId2()!="Door") innerLines.push_back(rightLine);
        }

        if (line.getId2()=="Window" || line.getId2()=="Openning")
        {
            Line leftWindow(line.getX1()-doorThickness,line.getY1(),line.getX2()-doorThickness,line.getY2());
            Line rightWindow(line.getX1()+doorThickness,line.getY1(),line.getX2()+doorThickness,line.getY2());


            Line three(line.getX1()-newThickness,line.getY1(),line.getX1()+newThickness,line.getY1());
            Line four(line.getX2()-newThickness,line.getY2(),line.getX2()+newThickness,line.getY2());

            leftWindow.setColor("#E72929");//red
            rightWindow.setColor("#E72929");

            leftWindow.setCenterX1(line.getX1()); leftWindow.setCenterY1(line.getY1());
            leftWindow.setCenterX2(line.getX2()); leftWindow.setCenterY2(line.getY2());

            rightWindow.setCenterX1(line.getX1()); rightWindow.setCenterY1(line.getY1());
            rightWindow.setCenterX2(line.getX2()); rightWindow.setCenterY2(line.getY2());

            twoLines.emplace_back(leftWindow);
            twoLines.emplace_back(rightWindow);
            twoLines.emplace_back(three);
            twoLines.emplace_back(four);
        }
        else if (line.getId2()=="Door")
        {
            double doorDistance=line.getY2()-line.getY1();
            doorDistance=line.getSideNum()==4?abs(doorDistance):abs(doorDistance)*-1;

            Line first(line.getX1(),line.getY1(),line.getX1()+doorDistance,line.getY1());
            Line second(line.getX1(),line.getY1()+doorThickness,line.getX1()+doorDistance,line.getY1()+doorThickness);

            Line third(line.getX1(),line.getY1(),line.getX1(),line.getY1()+doorThickness);
            Line forth(line.getX1()+doorDistance,line.getY1(),line.getX1()+doorDistance,line.getY1()+doorThickness);

            Line curvedLine(line.getX1()+doorDistance,line.getY1(),line.getX1(),line.getY2());

            Line six(line.getX1()-newThickness,line.getY1(),line.getX1()+newThickness,line.getY1());
            Line seven(line.getX2()-newThickness,line.getY2(),line.getX2()+newThickness,line.getY2());

            doorDistance=abs(doorDistance);
            curvedLine.setId2("Door");
            curvedLine.setSideNum(line.getSideNum());
            if (curvedLine.getSideNum()==2){
                curvedLine.setStartAngle(180);
                curvedLine.setEndAngle(270);
            }else if (curvedLine.getSideNum()==4){
                curvedLine.setStartAngle(270);
                curvedLine.setEndAngle(360);
            }

            first.setCenterX1(line.getX1()); first.setCenterY1(line.getY1());
            first.setCenterX2(line.getX2()); first.setCenterY2(line.getY2());

            second.setCenterX1(line.getX1()); second.setCenterY1(line.getY1());
            second.setCenterX2(line.getX2()); second.setCenterY2(line.getY2());


            first.setColor("#5BBCFF");
            second.setColor("#5BBCFF");
            third.setColor("#5BBCFF");
            forth.setColor("#5BBCFF");
            curvedLine.setColor("#5BBCFF");//blue


            twoLines.emplace_back(first);
            twoLines.emplace_back(second);
            twoLines.emplace_back(third);
            twoLines.emplace_back(forth);
            twoLines.emplace_back(curvedLine);
            twoLines.emplace_back(six);
            twoLines.emplace_back(seven);
        }
        if (line.getId2()!="Door")
        {
            twoLines.emplace_back(leftLine);
            twoLines.emplace_back(rightLine);
        }
    }
    return twoLines;
}

vector<Room> DesignGeometryManager::applyLine3DSideRule(
        Line &line,
        double thickness,
        map<pair<string,string>,pair<int,int>>top,
        map<pair<string,string>,pair<int,int>>bottom,
        map<pair<string,string>,pair<int,int>>left,
        map<pair<string,string>,pair<int,int>>right
)
{
    string leftSide=line.getSideLeft();
    string rightSide=line.getSideRight();
    double newThickness = thickness/2;
    double doorThickness= thickness/4;
    double zHeight=300;

    vector<Room>lines3D;
    vector<Room>windows;
    vector<Room>opennings;
    vector<Room>doors;

    if (line.getDir()=='h'){//y1=y2
        Line topLine(line.getX1()+top[{leftSide,rightSide}].first,line.getY1()-newThickness,line.getX2()+top[{leftSide,rightSide}].second,line.getY1()-newThickness);
        Line bottomLine(line.getX1()+bottom[{leftSide,rightSide}].first,line.getY1()+newThickness,line.getX2()+bottom[{leftSide,rightSide}].second,line.getY1()+newThickness);

        double X13D= min(topLine.getX1(),bottomLine.getX1()),
                X23D= max(topLine.getX2(),bottomLine.getX2()),
                Y13D= min(topLine.getY1(),bottomLine.getY1()),
                Y23D=max(topLine.getY2(),bottomLine.getY2()),
                Z13D=0,Z23D=zHeight;

        //add window and door for bottom and top line
        if (line.getSideNum()==1){
            bottomLine.setId(line.getId());
            bottomLine.setId2(line.getId2());
        }else{//3
            topLine.setId(line.getId());
            topLine.setId2(line.getId2());
        }

        if (line.getId2()=="Window" )
        {
            Line topWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2() );
            Line bottomWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2());

            vector<Room>allLines = create3DSurface(line.getId2() ,topWindow.getX1(),topWindow.getX2(),Y13D,Y23D,220,300,line.getFace());

            windows.insert(windows.end(),allLines.begin(),allLines.end());

            allLines = create3DSurface(line.getId2() ,bottomWindow.getX1(),bottomWindow.getX2(),Y13D,Y23D,0,100,line.getFace());

            windows.insert(windows.end(),allLines.begin(),allLines.end());
        }
        else if (line.getId2()=="Door" )//1 3
        {
            double doorDistance=line.getX2()-line.getX1();
            doorDistance=line.getSideNum()==1?abs(doorDistance):abs(doorDistance)*-1;

            vector<Room>allLines = create3DSurface("Door",line.getX1(),line.getX2(),Y13D,Y23D,220,zHeight,line.getFace());
            doors.insert(doors.end(),allLines.begin(),allLines.end());
        }
        else if (line.getId2()=="Openning")
        {
            Line topWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2() );

            vector<Room>allLines = create3DSurface("Openning" ,topWindow.getX1(),topWindow.getX2(),Y13D,Y23D,220,300,line.getFace());

            opennings.insert(opennings.end(),allLines.begin(),allLines.end());

        }
        else
        {
            lines3D= create3DSurface("wall",X13D,X23D,Y13D,Y23D,Z13D,Z23D,line.getFace());
        }
    }
    else{
        Line rightLine(line.getX1()+newThickness,line.getY1()+right[{leftSide,rightSide}].first,line.getX1()+newThickness,line.getY2()+right[{leftSide,rightSide}].second);
        Line leftLine(line.getX1()-newThickness,line.getY1()+left[{leftSide,rightSide}].first,line.getX1()-newThickness,line.getY2()+left[{leftSide,rightSide}].second);

        double X13D= min(leftLine.getX1(),rightLine.getX1()),
                X23D= max(leftLine.getX2(),rightLine.getX2()),
                Y13D= min(leftLine.getY1(),rightLine.getY2()),
                Y23D=max(leftLine.getY2(),rightLine.getY2()),
                Z13D=0,Z23D=zHeight;

        //add window and door for left line only
        if (line.getSideNum()==2){
            leftLine.setId(line.getId());
            leftLine.setId2(line.getId2());
        }else {//4
            rightLine.setId(line.getId());
            rightLine.setId2(line.getId2());
        }

        if (line.getId2()=="Window")
        {
            Line leftWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2());
            Line rightWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2());

            vector<Room>allLines = create3DSurface(line.getId2() ,X13D,X23D,leftWindow.getY1(),leftWindow.getY2(),220,300,line.getFace());

            windows.insert(windows.end(),allLines.begin(),allLines.end());

            allLines=create3DSurface(line.getId2(),X13D,X23D,rightWindow.getY1(),rightWindow.getY2(),0,100,line.getFace());
            windows.insert(windows.end(),allLines.begin(),allLines.end());
        }
        else if (line.getId2()=="Door" )
        {
            vector<Room>allLines = create3DSurface("Door",X13D,X23D,line.getY1(),line.getY2(),220,zHeight,line.getFace());
            doors.insert(doors.end(),allLines.begin(),allLines.end());
        }
        else if (line.getId2()=="Openning")
        {
            Line leftWindow(line.getX1(),line.getY1(),line.getX2(),line.getY2());

            vector<Room>allLines=create3DSurface("Openning" ,X13D,X23D,leftWindow.getY1(),leftWindow.getY2(),220,300,line.getFace());

            opennings.insert(opennings.end(),allLines.begin(),allLines.end());
        }
        else
        {
            lines3D= create3DSurface("wall",X13D,X23D,Y13D,Y23D,Z13D,Z23D,line.getFace());
        }
    }

    lines3D.insert( lines3D.end() , windows.begin() ,windows.end() );
    lines3D.insert( lines3D.end() , doors.begin() , doors.end() );
    lines3D.insert( lines3D.end() , opennings.begin() ,opennings.end() );

    return lines3D;
}

void DesignGeometryManager::setRoomBesides(vector<Room> &recs)
{
    for (int i = 0; i < recs.size(); ++i) {
        if(recs[i].getX1()==recs[i].getX2())continue;
        if(recs[i].getY1()==recs[i].getY2())continue;
        if (recs[i].getRoomId().find("Corridor")!= std::string::npos)continue;
        vector<string>besides;
        double x1=recs[i].getX1(),x2=recs[i].getX2(),y1=recs[i].getY1(),y2=recs[i].getY2();
        for (int j = 0; j < recs.size(); ++j) {
            if(recs[j].getX1()==recs[j].getX2())continue;
            if(recs[j].getY1()==recs[j].getY2())continue;
            if (recs[j].getRoomId().find("Corridor") != std::string::npos)continue;
            if (i==j)continue;
            // check vertically
            if ((recs[j].getX1()==x2 || recs[j].getX2()==x1)&&((recs[j].getY1()>=recs[i].getY1() && recs[j].getY1()<recs[i].getY2()) || (recs[j].getY1()<recs[i].getY1() && recs[j].getY2()>recs[i].getY1()))  ){
                besides.emplace_back(recs[j].getRoomId());
            }
            // check horizontally
            if ((recs[j].getY1()==y2 || recs[j].getY2()==y1) && ((recs[j].getX1()>=recs[i].getX1() && recs[j].getX1()<recs[i].getX2()) || (recs[j].getX1()<recs[i].getX1() && recs[j].getX2()>recs[i].getX1()))  ){
                besides.emplace_back(recs[j].getRoomId());
            }
        }
        recs[i].setRoomsBesides(besides);
    }
}

map<string,int> DesignGeometryManager::sortRooms(vector<Room> &recs) {
    map<string,vector<string>>graph;
    set<string>visited;
    vector<string>result;
    map<string,int>sortedRecs;
    //store graph
    for(auto &rec:recs){
        if(rec.getX1()==rec.getX2())continue;
        if(rec.getY1()==rec.getY2())continue;
        if (rec.getRoomId().find("Corridor")!= std::string::npos)continue;
        for(auto &beside:rec.getRoomsBesides())
            graph[rec.getRoomId()].push_back(beside);
    }
    for(auto &rec:recs){
        if(rec.getX1()==rec.getX2())continue;
        if(rec.getY1()==rec.getY2())continue;
        if (rec.getRoomId().find("Corridor") != std::string::npos)continue;
        if (!visited.count(rec.getRoomId())){
            DFS(rec.getRoomId(),result,visited,graph);
        }
    }
    int cnt=0;
    for(auto &rec:result){
        sortedRecs[rec]=cnt++;
    }
    return sortedRecs;
}

vector<Room> DesignGeometryManager::create3DSurface(const string &wallName,double x1, double x2, double y1, double y2, double z1, double z2,char lineFace)
{
    vector<Room>lines3D;

    lines3D.emplace_back(wallName,x1,x2,y1,y1,z1,z2);
    lines3D.emplace_back(wallName,x1,x2,y2,y2,z1,z2);
    lines3D.emplace_back(wallName,x1,x2,y1,y2,z2,z2);
    lines3D.emplace_back(wallName,x1,x2,y1,y2,z1,z1);
    lines3D.emplace_back(wallName,x1,x1,y1,y2,z1,z2);
    lines3D.emplace_back(wallName,x2,x2,y1,y2,z1,z2);

    if (lineFace=='N'|| lineFace=='n'){
        lines3D[0].setSurfaceSide("external");//y1 external
        lines3D[1].setSurfaceSide("internal");
        lines3D[2].setSurfaceSide("top");
        lines3D[3].setSurfaceSide("internal");
        lines3D[4].setSurfaceSide("external");
        lines3D[5].setSurfaceSide("external");
    }else if (lineFace=='S' || lineFace=='s'){
        lines3D[0].setSurfaceSide("internal");
        lines3D[1].setSurfaceSide("external");//y2 external
        lines3D[2].setSurfaceSide("top");
        lines3D[3].setSurfaceSide("internal");
        lines3D[4].setSurfaceSide("external");
        lines3D[5].setSurfaceSide("external");
    }else if (lineFace=='W' || lineFace=='w'){
        lines3D[0].setSurfaceSide("external");
        lines3D[1].setSurfaceSide("external");//y2 external

        lines3D[2].setSurfaceSide("top");
        lines3D[3].setSurfaceSide("internal");

        lines3D[4].setSurfaceSide("external");
        lines3D[5].setSurfaceSide("internal");
    }else if (lineFace=='E' || lineFace=='e'){
        lines3D[0].setSurfaceSide("external");
        lines3D[1].setSurfaceSide("external");

        lines3D[2].setSurfaceSide("top");
        lines3D[3].setSurfaceSide("internal");

        lines3D[4].setSurfaceSide("internal");
        lines3D[5].setSurfaceSide("external");//y2 external
    }else{//internal Line
        lines3D[0].setSurfaceSide("internal");
        lines3D[1].setSurfaceSide("internal");
        lines3D[2].setSurfaceSide("top");
        lines3D[3].setSurfaceSide("internal");
        lines3D[4].setSurfaceSide("internal");
        lines3D[5].setSurfaceSide("internal");
    }
    return lines3D;
}


bool DesignGeometryManager::isLineExternal(Line &line,const vector<Room> &rectangles)
{
    double x1=line.getX1(),y1=line.getY1();
    double x2=line.getX2(),y2=line.getY2();
    int counts=0;
    RoomBuilder roomBuilder;
    Room ownRec ( roomBuilder.createRoom("0",0,0,0,0));
    for(auto &rec:rectangles){
        if (rec.getX1()==rec.getX2())continue;
        if (rec.getY1()==rec.getY2())continue;
        if (line.getDir()=='h'){//y1 = y2
            if ((y1==rec.getY1() || y1==rec.getY2()) &&
                (x1>=rec.getX1() && x2<=rec.getX2())){
                counts++;
                ownRec=rec;
            }
        }
        if (line.getDir()=='v'){//x1 = x2
            if ((x1 == rec.getX1() || x1 == rec.getX2()) &&
                (y1>=rec.getY1() && y2<=rec.getY2())){
                counts++;
                ownRec=rec;
            }
        }
    }
    if (counts==1){
        line.setRelationToRectangles("external");
        if (line.getDir()=='h'){//y1=y2 (1,3)
            if (y1==ownRec.getY1())line.setFace('N');
            if (y1==ownRec.getY2())line.setFace('S');
        }if (line.getDir()=='v'){//x1=x2 (2,4)
            if (x1==ownRec.getX1())line.setFace('W');
            if (x1==ownRec.getX2())line.setFace('E');
        }
    }else{
        line.setRelationToRectangles("internal");
    }
    return counts==1;
}

void DesignGeometryManager::DFS(const string &start,vector<string>&result,set<string>&visited,map<string,vector<string>>&graph) {
    deque<string>q;
    q.push_back(start);
    while (!q.empty()){
        string current = q.front();
        q.pop_front();
        if (!visited.count(current)) {
            visited.insert(current);
            result.push_back(current);
            for (string &neighbor : graph[current]) {
                if (!visited.count(neighbor)) {
                    q.push_front(neighbor);
                }
            }
        }
    }
}

bool DesignGeometryManager::isTwoRoomsConnected(const Room &one, const Room &two)
{
    double x1 = one.getX1(), x2 = one.getX2(), y1 = one.getY1(), y2 = one.getY2();
    double tx1 = two.getX1(), tx2 = two.getX2(), ty1 = two.getY1(), ty2 = two.getY2();

    // Check vertical adjacency
    if ((tx1 == x2 || tx2 == x1) && ((ty1 < y2 && ty1 >= y1) || (ty2 > y1 && ty2 <= y2) || (ty1 <= y1 && ty2 >= y2))) {
        return true;
    }

    // Check horizontal adjacency
    if ((ty1 == y2 || ty2 == y1) && ((tx1 < x2 && tx1 >= x1) || (tx2 > x1 && tx2 <= x2) || (tx1 <= x1 && tx2 >= x2))) {
        return true;
    }
    return false;
}

bool DesignGeometryManager::isTwoRoomsIntersect(const Room &one, const Room &two) {
    // If one rectangle is on left side of other
    if (one.getX1() >= two.getX2() || two.getX1() >= one.getX2())
        return false;

    // If one rectangle is above other
    if (one.getY1() >= two.getY2() || two.getY1() >= one.getY2())
        return false;

    return true;
}

bool DesignGeometryManager::positionPolygonInsideAnotherUsingAllAngles(Polygon1 &input, Polygon1 &pol)
{
    for (int angle = 0; angle <= 90; angle += 45) {
        Polygon1 cur = pol;
        cur.rotate(angle);
        bool valid = positionPolygonInsideAnother(input , cur);
        if (valid) return true;
    }
    return false;
}

bool DesignGeometryManager::positionPolygonInsideAnother(Polygon1 &input, Polygon1 &pol)
{
    double unit = 1;
    set<pair<double,double>>points;
    vector<Line> inputLines = input.getLines();
    //store all points on polygon
    for(auto &p: inputLines)
    {
        vector<Point> pnts = getLinePoints(p.getX1(),p.getY1(),p.getX2(),p.getY2(),unit);
        for(auto &allP:pnts)points.emplace(allP.getX(),allP.getY());
    }

    Point minPoint = input.minPoint();
    Point maxPoint = input.maxPoint();

    Point minPoint2 = pol.minPoint();
    Point maxPoint2 = pol.maxPoint();

    double x1 = minPoint.getX() , x2 = maxPoint.getX();
    double y1 = minPoint.getY() , y2 = maxPoint.getY();


    Point centerPoint (((x2 - x1) / 2) + x1 , (y2 - y1) / 2 + y1);

    Point centerPoint2 (((maxPoint2.getX() - minPoint2.getX()) / 2) + minPoint2.getX() , (maxPoint2.getY() - minPoint2.getY()) / 2 + minPoint2.getY());


    vector<Point> validPoints;


    double cnt1 = (x2-x1) * 0.1 , cnt2 = (y2 - y1) * 0.1;

    for (double x = 0; x <= (x2 -  x1 ) / 2; x += cnt1)
    {
        for (double y = 0; y <= (y2 -  y1 ) / 2; y += cnt2)
        {
            // double dx = x - minPoint2. getX();
            // double dy = y - minPoint2. getY();
            // Polygon1 newPol = pol;
            // newPol.transformPolygon(dx,dy);//isPolygonInsidePolygon

            // // if (isPolygonCompletelyInsidePolygon(inputLines,newPol.getLines(),1)){
            // //     validPoints.emplace_back(dx,dy);
            // //     // pol = newPol;
            // //     // return true;
            // // }

            double cX = centerPoint.getX() + x , cY = centerPoint.getY() + y;


            double dx = cX - centerPoint2. getX();
            double dy = cY - centerPoint2. getY();
            Polygon1 newPol = pol;
            newPol.shiftX(dx);
            newPol.shiftY(dy);

            if (isPolygonInsidePolygon(input , newPol ) )
            {
                pol = newPol;
                return true;
            }
        }
    }

    if(points.empty() || validPoints.empty()) return false;


    // vector<pair<double , Point>> nearPoints;
    // for(auto &p : validPoints)
    // {
    //     double x1 = p.getX() , y1 = p.getY();
    //     double x2 = centerPoint.getX() , y2 = centerPoint.getY();

    //     Polygon1 newPol = pol;
    //     newPol.transformPolygon(x1 , y1);

    //     Point minPoint = newPol.minPoint();
    //     Point maxPoint = newPol.maxPoint();

    //     Point centerPoint (((maxPoint.getX() - minPoint.getX()) / 2) + minPoint.getX() , (maxPoint.getY() - minPoint.getY()) / 2 + minPoint.getY());

    //     x1 = centerPoint.getX() , y1 = centerPoint.getY();

    //     double length1 =  sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
    //     nearPoints.emplace_back(length1 , p);
    // }
    // sort(nearPoints.begin() , nearPoints.end());


    // Point curPoint = nearPoints[0].second;

    // pol.transformPolygon(curPoint.getX() , curPoint.getY());

    return true;
}

bool
DesignGeometryManager::isPolygonCompletelyInsidePolygon(const vector<Line>& input, const vector<Line>& zone,
                                                     double unit) {
    vector<Point>points;
    for(auto &line:zone){
        vector<Point>linePoints = getLinePoints(line.getX1(),line.getY1(),line.getX2(),line.getY2(),unit);
        for(auto &p:linePoints)points.emplace_back(p);
    }
    for(auto &point:points){
        if (!isPointInsidePolygon(point.getX(),point.getY(),input)&& !isPointOnPolygon(point.getX(),point.getY(),input,unit))return false;
    }
    return true;
}


bool DesignGeometryManager::isLeft(const Point& a, const Point& b, const Point& p) {
    return ((b.getX() - a.getX()) * (p.getY() - a.getY()) -
            (b.getY() - a.getY()) * (p.getX() - a.getX())) > 0;
}

// Check if a point is inside a polygon
bool DesignGeometryManager::isPointInsidePolygon(const vector<Point>& polygon, const Point& point) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].getY() > point.getY()) != (polygon[j].getY() > point.getY())) &&
            (point.getX() < (polygon[j].getX() - polygon[i].getX()) *
            (point.getY() - polygon[i].getY()) /
            (polygon[j].getY() - polygon[i].getY()) + polygon[i].getX())) {
            inside = !inside;
        }
    }
    return inside;
}

// Check if two line segments intersect
bool DesignGeometryManager::doSegmentsIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    auto orientation = [](const Point& a, const Point& b, const Point& c) {
        double val = (b.getY() - a.getY()) * (c.getX() - b.getX()) -
                     (b.getX() - a.getX()) * (c.getY() - b.getY());
        if (fabs(val) < 1e-9) return 0; // Collinear
        return (val > 0) ? 1 : 2;       // Clockwise or Counterclockwise
    };

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;

    return false;
}

bool
DesignGeometryManager::isPolygonInsidePolygon(const Polygon1& innerPol, const Polygon1& outerPol) {
    // Check if all vertices of the inner polygon are inside the outer polygon
    Polygon1 p1 = innerPol , p2 = outerPol;
    vector<Point> inner = p1.getPoints() , outer = p2.getPoints();
    for ( auto& point : outer) {
        if (!isPointInsidePolygon(inner , point)) {
            return false;
        }
    }

    // Check if any edge of the inner polygon intersects with any edge of the outer polygon
    for (int i = 0; i < inner.size(); ++i) {
        Point p1 = inner[i];
        Point q1 = inner[(i + 1) % inner.size()];

        for (size_t j = 0; j < outer.size(); ++j) {
            Point p2 = outer[j];
            Point q2 = outer[(j + 1) % outer.size()];

            if (doSegmentsIntersect(p1, q1, p2, q2)) {
                return false;
            }
        }
    }

    return true;
}

vector<Point> DesignGeometryManager::getLinePoints(double x1,double y1,double x2,double y2, double unit)
{
    vector<Point>points;
    double dx= abs(x2-x1);
    double dy= -abs(y2-y1);
    double sx =x1 < x2 ? unit : -unit;
    double sy =y1 < y2 ? unit : -unit;
    double err=dx+dy,e2;
    while (true){
        points.emplace_back(x1 , y1);
        if ((x1==x2 && y1==y2) ||
            (x1<x2 && sx<0) ||
            (x1>x2 && sx>0)||
            (y1<y2 && sy<0)||
            (y1>y2 && sy>0))
            break;
        e2=2*err;
        if (e2>=dy){err+=dy; x1+=sx;}
        if (e2<=dx){err+=dx; y1+=sy;}
    }
    return points;
}

bool DesignGeometryManager::isPointOnPolygon(double x, double y,const vector<Line> &main,double unit) {
    vector<Point>polygonPoints;
    for(auto &lin:main){
        vector<Point> linePoints = getLinePoints(lin.getX1(),lin.getY1(),lin.getX2(),lin.getY2(),unit);
        for(auto &p:linePoints)
        {
            polygonPoints.push_back(p);
        }
    }
    for(auto &point:polygonPoints)
        if (x==point.getX() && y==point.getY())return true;
    return false;
}

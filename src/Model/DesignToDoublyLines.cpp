//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#include "DesignToDoublyLines.h"
#include "DesignGeometryManager.h"
DesignToDoublyLines::DesignToDoublyLines(Design &solution) : rectangles(solution.getRooms()) {
    space=5;
    thickness=10;//-thickness/2 +thickness/2
    doorDistance=80;
    mapTopLine={
            {{"W","T1"},{0,-space}},
            {{"C1","T1"},{-space,-space}},
            {{"C4","T1"},{space,-space}},
            {{"T2","T1"},{space,-space}},

            {{"T2","W"},{space,0}},
            {{"T2","C2"},{space,space}},
            {{"T2","C3"},{space,-space}},
            {{"T2","T1"},{space,-space}},

            {{"W","T3"},{0,-space}},
            {{"C1","T3"},{-space,-space}},
            {{"C4","T3"},{+space,-space}},
            {{"T2","T3"},{+space,-space}},

            {{"T3","W"},{space,0}},
            {{"T3","C2"},{space,space}},
            {{"T3","T4"},{space,space}},
            {{"T3","C3"},{+space,-space}},
            {{"T3","T3"},{+space,-space}},
            {{"T3","T1"},{+space,-space}},

            {{"W","T4"},{0,space}},
            {{"C1","T4"},{-space,space}},
            {{"C4","T4"},{+space,+space}},
            {{"T2","T4"},{+space,+space}},

            {{"T4","W"},{0,0}},
            {{"T4","C2"},{0,space}},
            {{"T4","C3"},{0,-space}},
            {{"T4","T3"},{0,-space}},
            {{"T4","T1"},{0,-space}},

            {{"C1","C2"},{-space,+space}},
            {{"C1","W"},{-space,0}},
            {{"W","C2"},{0,+space}},

            {{"C4","C3"},{space,-space}},
            {{"C4","W"},{space,0}},
            {{"W","C3"},{0,-space}},
            {{"W","W"},{0,0}},
    };
    mapBottomLine={
            {{"W","T1"},{0,-space}},
            {{"C1","T1"},{+space,-space}},
            {{"C4","T1"},{-space,-space}},
            {{"T2","T1"},{+space,-space}},

            {{"T2","W"},{space,0}},
            {{"T2","C2"},{space,-space}},
            {{"T2","C3"},{space,space}},
            {{"T2","T1"},{space,-space}},

            {{"W","T3"},{0,0}},
            {{"C1","T3"},{space,0}},
            {{"C4","T3"},{-space,0}},
            {{"T2","T3"},{+space,0}},

            {{"T3","W"},{0,0}},
            {{"T3","C2"},{0,-space}},
            {{"T3","T4"},{0,-space}},
            {{"T3","C3"},{0,+space}},
            {{"T3","T3"},{0,+space}},
            {{"T3","T1"},{0,-space}},

            {{"W","T4"},{0,-space}},
            {{"C1","T4"},{+space,space}},
            {{"C4","T4"},{-space,-space}},
            {{"T2","T4"},{+space,-space}},

            {{"T4","W"},{+space,0}},
            {{"T4","C2"},{+space,-space}},
            {{"T4","C3"},{+space,+space}},
            {{"T4","T3"},{+space,+space}},
            {{"T4","T1"},{+space,-space}},

            {{"C1","C2"},{+space,-space}},
            {{"C1","W"},{+space,0}},
            {{"W","C2"},{0,-space}},

            {{"C4","C3"},{-space,+space}},
            {{"C4","W"},{-space,0}},
            {{"W","C3"},{0,+space}},

            {{"W","W"},{0,0}},
    };
    mapLeftLine={
            {{"W","T1"},{0,-space}},
            {{"C1","T1"},{-space,-space}},
            {{"C2","T1"},{+space,-space}},
            {{"T4","T1"},{+space,-space}},

            {{"T1","W"},{space,0}},
            {{"T1","C3"},{space,-space}},
            {{"T1","C4"},{space,space}},
            {{"T1","T2"},{space,space}},
            {{"T1","T3"},{space,-space}},

            {{"W", "T2"},{0,0}},
            {{"C1","T2"},{-space,0}},
            {{"T2","T2"},{-space,0}},
            {{"C2","T2"},{+space,0}},
            {{"T1","T2"},{+space,0}},
            {{"T4","T2"},{+space,0}},

            {{"T2","W"},{0,0}},
            {{"T2","C3"},{0,-space}},
            {{"T2","T1"},{0,-space}},
            {{"T2","C4"},{0,+space}},
            {{"T2","T2"},{0,+space}},
            {{"T2","T3"},{0,-space}},

            {{"W","T3"},{0,-space}},
            {{"C1","T3"},{-space,-space}},
            {{"C2","T3"},{+space,-space}},
            {{"T4","T3"},{+space,-space}},

            {{"T4","W"},{+space,0}},
            {{"T4","C3"},{+space,-space}},
            {{"T4","C4"},{+space,+space}},
            {{"T4","T3"},{+space,-space}},

            {{"C1","C4"},{-space,+space}},
            {{"C1","W"},{-space,0}},
            {{"W","C4"},{0,+space}},
            {{"C2","C3"},{+space,-space}},
            {{"C2","W"},{+space,0}},
            {{"W","C3"},{0,-space}},

            {{"W","W"},{0,0}},
    };
    mapRightLine={
            {{"W","T1"},{0,0}},
            {{"C1","T1"},{+space,0}},
            {{"C2","T1"},{-space,0}},
            {{"T4","T1"},{+space,0}},

            {{"T1","W"},{0,0}},
            {{"T1","C3"},{0,+space}},
            {{"T1","C4"},{0,-space}},
            {{"C4","C4"},{0,-space}},
            {{"T1","T2"},{0,-space}},
            {{"T1","T3"},{0,-space}},

            {{"W", "T2"},{0,-space}},
            {{"C1","T2"},{+space,-space}},
            {{"T2","T2"},{+space,-space}},
            {{"C2","T2"},{-space,-space}},
            {{"T1","T2"},{-space,-space}},
            {{"T4","T2"},{+space,-space}},

            {{"T2","W"},{+space,0}},
            {{"T2","C3"},{+space,+space}},
            {{"T2","T1"},{+space,+space}},
            {{"T2","C4"},{+space,-space}},
            {{"T2","T2"},{+space,-space}},
            {{"T2","T3"},{+space,-space}},

            {{"W","T3"},{0,-space}},
            {{"C1","T3"},{+space,-space}},
            {{"C2","T3"},{-space,-space}},
            {{"T4","T3"},{+space,-space}},

            {{"T4","W"},{+space,0}},
            {{"T4","C3"},{+space,+space}},
            {{"T4","C4"},{+space,-space}},
            {{"T4","T3"},{+space,-space}},

            {{"C1","C4"},{+space,-space}},
            {{"C1","W"},{+space,0}},
            {{"W","C4"},{0,-space}},

            {{"C2","C3"},{-space,+space}},
            {{"C2","W"},{-space,0}},
            {{"W","C3"},{0,+space}},

            {{"W","W"},{0,0}},
    };

    setRectanglesBesides();
    getAnalysisData();
    setLineProperties();
    applyLineThickness();
    assignNewRectangle();
    room3DPrinting = extract3DPrinting(recLines);
}

vector<Room> DesignToDoublyLines::extract3DPrinting(const vector<Line> &roomsLine)
{
    vector<Room> rooms3D;
    //store each point related to wall or window or door or opennings
    map<pair<double,double> , string>mapPointsToID;
    //store all X values on specific Y values
    map<double , set<double>>YPoints;//for width
    map<double , set<double>>XPoints;//for height
    for(auto &line : roomsLine)
    {
        mapPointsToID[ pair<double,double>( line.getX1() , line.getY1()) ] = line.getId2() ;
        if(line.getDir() == 'h') //y1 = y2
        {
            YPoints[line.getY1()] .emplace(line.getX1());
            YPoints[line.getY1()] .emplace(line.getX2());
        }
        else
        {
            XPoints[line.getX1()] .emplace(line.getY1());
            XPoints[line.getX1()] .emplace(line.getY2());
        }
    }

    map<double , vector<double>>YPointsVec;//for width
    map<double , vector<double>>XPointsVec;//for height
    //move data to new maps
    for(auto &y : YPoints)
    {
        for(auto &x : y.second)
        {
            YPointsVec[y.first].push_back(x);
        }
    }

    for(auto &x : XPoints)
    {
        for(auto &y : x.second)
        {
            XPointsVec[x.first].push_back(y);
        }
    }


    for (double Z1 = 0;  Z1 < 300; Z1+= 20)
    {
        double Z2 = Z1 + 20;

        for(auto &yP : YPointsVec)
        {
            double y = yP.first ;
            for (int XI = 1; XI < yP.second.size(); ++XI)
            {
                double X1 = yP.second[XI-1];
                double X2 = yP.second[XI];

                string wallName = mapPointsToID[pair<double,double>(X1 , y)];

                if(wallName == "Window" && Z1 >=100 && Z2<=220)
                {
                    continue;
                }
                if((wallName == "Door" || wallName == "Openning") && Z1 >=0 && Z2<=220)
                {
                    continue;
                }

                Room room3D(wallName , X1 , X2 , y , y , Z1 , Z2);
                rooms3D.push_back(room3D);
            }
        }

        for(auto &xP : XPointsVec)
        {
            double x = xP.first ;
            for (int YI = 1; YI < xP.second.size(); ++YI)
            {
                double Y1 = xP.second[YI-1];
                double Y2 = xP.second[YI];

                string wallName = mapPointsToID[pair<double,double>(x , Y1)];

                if(wallName == "Window" && Z1 >=100 && Z2<=220)
                {
                    continue;
                }
                if((wallName == "Door" || wallName == "Oppenning") && Z1 >=0 && Z2<=220)
                {
                    continue;
                }

                Room room3D(wallName , x , x , Y1 , Y2 , Z1 , Z2);
                rooms3D.push_back(room3D);
            }
        }
    }
    return rooms3D;
}

void DesignToDoublyLines::getAnalysisData() {
    recLines = DesignGeometryManager::getLines(rectangles);
}

void DesignToDoublyLines::printAnalysisData() {
    for(auto &line:recLines)line.print();
}

void DesignToDoublyLines::setLineProperties() {
    DesignGeometryManager::setLinesSides(recLines);
}

void DesignToDoublyLines::applyLineThickness() {

    for(auto &line:recLines)
    {
        vector<Line>twoLines = DesignGeometryManager::applyLineSideRule(line,thickness,mapTopLine,mapBottomLine,mapLeftLine,mapRightLine , innerLines);

        for(auto &line2:twoLines)line2.setRoomName(line.getId());

        vector<Room>twoLines3D=DesignGeometryManager::applyLine3DSideRule(line,thickness,mapTopLine,mapBottomLine,mapLeftLine,mapRightLine);
        for(auto &rec:twoLines3D)rec.changeId(line.getId());


        newLines.insert(newLines.end(),twoLines.begin(),twoLines.end());
        newSurfaces.insert(newSurfaces.end(),twoLines3D.begin(),twoLines3D.end());
    }
}

void DesignToDoublyLines::printNewLinesAfterThickness() {
    cout<<newLines.size()<<endl;
    for(auto &line:newLines)line.print();
}
void DesignToDoublyLines::assignNewRectangle(){
    for(auto &rec:rectangles){
        if (rec.getX1()==rec.getX2() || rec.getY1()==rec.getY2())continue;//line
        string id = rec.getRoomId();
        double x1=100000,y1=1000000,x2=-1000000,y2=-100000000;
        for(auto &line:newLines){
            if(line.getId()==id || line.getId().find(id) != std::string::npos ||
               id.find(line.getId()) != std::string::npos){
                x1=min(x1,line.getX1());
                y1=min(y1,line.getY1());
                x2=max(x2,line.getX2());
                y2=max(y2,line.getY2());
            }
        }
        newRectangles.emplace_back(id,x1,y1,x2,y2);
    }
}

const vector<Line> &DesignToDoublyLines::getNewLines() const {
    return newLines;
}
const vector<Room> &DesignToDoublyLines::getNewRectangles() const {
    return newRectangles;
}
const vector<Line> &DesignToDoublyLines::getInnerLines() const {
    return innerLines;
}

vector<Line> DesignToDoublyLines::getArchitecturalElementsInSpecificRoom(const string &roomId, const string &architecturalElement,double unit) {
    vector<Line>outLines;
    for(auto &rec:rectangles)
    {
        if(rec.getRoomId()==roomId){
            double x1=rec.getX1(),y1=rec.getY1();
            double x2=rec.getX2(),y2=rec.getY2();
            for(auto &line:rectangles){
                if (line.getX1()==line.getX2() || line.getY1()==line.getY2()){
                    if(line.getRoomId()==architecturalElement){
                        Line newLine(line.getX1(),line.getY1(),line.getX2(),line.getY2());
                        newLine.setId(line.getRoomId());
                        if (newLine.getDir()=='h'){//1 3
                            if (line.getY1()==y1)newLine.setSideNum(1);
                            if (line.getY1()==y2)newLine.setSideNum(3);
                            if ((newLine.getY1()==y1 || newLine.getY1()==y2) &&
                                (newLine.getX1()>=x1 && newLine.getX2()<=x2)){
                                newLine.setX1((newLine.getX1()-x1)/unit);
                                newLine.setY1((newLine.getY1()-y1)/unit);
                                newLine.setX2((newLine.getX2()-x1)/unit);
                                newLine.setY2((newLine.getY2()-y1)/unit);
                                outLines.push_back(newLine);
                            }
                        }else if (newLine.getDir()=='v'){//2 4
                            if ((newLine.getX1()==x1 || newLine.getX1()==x2) &&
                                (newLine.getY1()>=y1 && newLine.getY2()<=y2)){
                                if (line.getX1()==x1)newLine.setSideNum(4);
                                if (line.getX1()==x2)newLine.setSideNum(2);
                                newLine.setX1((newLine.getX1()-x1)/unit);
                                newLine.setY1((newLine.getY1()-y1)/unit);
                                newLine.setX2((newLine.getX2()-x1)/unit);
                                newLine.setY2((newLine.getY2()-y1)/unit);

                                outLines.push_back(newLine);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    return outLines;
}

double DesignToDoublyLines::getRoomArea(const string &roomId,double unit) {
    for(auto &rec:rectangles){
        if (rec.getRoomId()==roomId){
            double X=(rec.getX2()-rec.getX1())/unit;
            double Y=(rec.getY2()-rec.getY1())/unit;
            return X*Y;
        }
    }
    return 0.0;
}

double DesignToDoublyLines::getRoomX(const string &roomId, double unit) {
    for(auto &rec:rectangles){
        if (rec.getRoomId()==roomId){
            double X=(rec.getX2()-rec.getX1())/unit;
            return X;
        }
    }
    return 0.0;
}
double DesignToDoublyLines::getRoomY(const string &roomId, double unit) {
    for(auto &rec:rectangles){
        if (rec.getRoomId()==roomId){
            double Y=(rec.getY2()-rec.getY1())/unit;
            return Y;
        }
    }
    return 0.0;
}

void DesignToDoublyLines::setRectanglesBesides()
{
    DesignGeometryManager::setRoomBesides(rectangles);
    sortedRectangles = DesignGeometryManager::sortRooms(rectangles);
}

const map<string, int> &DesignToDoublyLines::getSortedRectangles() const {
    return sortedRectangles;
}

void DesignToDoublyLines::getMainCorners()
{
    freeCorners = DesignGeometryManager::getMainCorner(rectangles);
}

void DesignToDoublyLines::printMainCorners()
{
    cout<<"Free Corners"<<endl;
    for(auto &point:freeCorners){
        cout<<point.getX()<<" "<<point.getY()<<endl;
    }
}

const map<pair<string, string>, pair<int, int>> &DesignToDoublyLines::getMapTopLine() const
{
    return mapTopLine;
}

const map<pair<string, string>, pair<int, int>> &DesignToDoublyLines::getMapBottomLine() const
{
    return mapBottomLine;
}

const map<pair<string, string>, pair<int, int>> &DesignToDoublyLines::getMapLeftLine() const
{
    return mapLeftLine;
}

const map<pair<string, string>, pair<int, int>> &DesignToDoublyLines::getMapRightLine() const
{
    return mapRightLine;
}

const vector<Room> &DesignToDoublyLines::getNewSurfaces() const {
    return newSurfaces;
}

void DesignToDoublyLines::setNewSurfaces(const vector<Room> &newSurfaces1) {
    DesignToDoublyLines::newSurfaces = newSurfaces1;
}

const vector<Room> &DesignToDoublyLines::getRectangles() const {
    return rectangles;
}

const vector<Line> &DesignToDoublyLines::getRecLines() const {
    return recLines;
}

double DesignToDoublyLines::getThickness() const {
    return thickness;
}

double DesignToDoublyLines::getSpace() const {
    return space;
}

double DesignToDoublyLines::getDoorDistance() const {
    return doorDistance;
}

map < string , vector<double> > DesignToDoublyLines::calculateHomeDimensions(const Design &home)
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

vector<Room> DesignToDoublyLines::getRoom3DPrinting()const
{
    return room3DPrinting;
}

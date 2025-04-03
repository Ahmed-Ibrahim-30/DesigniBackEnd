//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#include "JsonExtraction.h"
#include "deque"
#include "src/Model/Line.h"
class OutLines {
public:
    vector<Line> getRectangleOutSideLines(const vector<Room> &recs){
        vector<Line> outSideLines;
        map<double, set<double>> xValues, yValues;
        for (auto &rec: recs) {
            if (rec.getX1() == rec.getX2())continue;
            if (rec.getY1() == rec.getY2())continue;
            xValues[rec.getX1()].insert(rec.getY1());
            xValues[rec.getX1()].insert(rec.getY2());
            xValues[rec.getX2()].insert(rec.getY1());
            xValues[rec.getX2()].insert(rec.getY2());

            yValues[rec.getY1()].insert(rec.getX1());
            yValues[rec.getY1()].insert(rec.getX2());
            yValues[rec.getY2()].insert(rec.getX1());
            yValues[rec.getY2()].insert(rec.getX2());
        }
        for (auto &x: xValues) {
            vector<double> yValue;
            yValue.reserve(x.second.size());
            for (auto &y: x.second)yValue.push_back(y);
            for (int y = 0; y < yValue.size() - 1; ++y) {
                outSideLines.emplace_back(x.first, yValue[y], x.first, yValue[y + 1]);
            }
        }
        for (auto &y: yValues) {
            vector<double> xValue;
            xValue.reserve(y.second.size());
            for (auto &x: y.second)xValue.push_back(x);
            for (int x = 0; x < xValue.size() - 1; ++x) {
                outSideLines.emplace_back(xValue[x], y.first, xValue[x + 1], y.first);
            }
        }
        return outSideLines;
    }
    vector<Line>getOutSideLines(const vector<Room> &rectangles){
        vector<Line> outSideLines;
        vector<Line> allRectangleLines = getRectangleOutSideLines(rectangles);
        for (auto &line: allRectangleLines) {
            double x1 = line.getX1(), y1 = line.getY1();
            double x2 = line.getX2(), y2 = line.getY2();
            int counts = 0;
            for (auto &rec: rectangles) {
                if (line.getY1() == line.getY2()) {//y1 = y2
                    if ((y1 == rec.getY1() || y1 == rec.getY2()) &&
                        (x1 >= rec.getX1() && x2 <= rec.getX2())) {
                        counts++;
                    }
                }
                if (line.getX1() == line.getX2()) {//x1 = x2
                    if ((x1 == rec.getX1() || x1 == rec.getX2()) &&
                        (y1 >= rec.getY1() && y2 <= rec.getY2())) {
                        counts++;
                    }
                }
            }
            if (counts == 1)outSideLines.push_back(line);
        }
        return outSideLines;
    }
    deque<Line> Lines;
    std::vector<Line> clockwiseSort(const std::vector<Line>& lines_){
        vector<Line> v;
        set<pair<double, double>> vis;
        v.emplace_back(lines_.front());
        while (true) {
            if (v.size() == lines_.size())break;
            auto pi = findAnotherLineConnected(v.back(), lines_, vis);
            if (pi.second == 1) {
                vis.emplace(pi.first.getX1(), pi.first.getY1());
            } else if (pi.second == 2) {
                vis.emplace(pi.first.getX1(), pi.first.getY1());
            } else if (pi.second == 3) {
                vis.emplace(pi.first.getX2(), pi.first.getY2());
            } else if (pi.second == 4) {
                vis.emplace(pi.first.getX2(), pi.first.getY2());
            }
            v.emplace_back(pi.first);
        }
        return v;
    }
    pair<Line,int> findAnotherLineConnected(Line l, const vector<Line> &lines_,const set<pair<double,double>>&vis){
        for (const auto &line: lines_) {
            if (line.getX1() == l.getX1() && line.getX2() == l.getX2() && line.getY1() == l.getY1() && line.getY2() == l.getY2())continue;
            if (line.getX1() == l.getX1() && line.getY1() == l.getY1() && !vis.count(make_pair(line.getX1(), line.getY1()))) {//p1==p1
                return {line, 1};
            } else if (line.getX1() == l.getX2() && line.getY1() == l.getY2() && !vis.count(make_pair(line.getX1(), line.getY1()))) {//p1==p2
                return {line, 2};
            } else if (line.getX2() == l.getX1() && line.getY2() == l.getY1() && !vis.count(make_pair(line.getX2(), line.getY2()))) {//p2==p1
                return {line, 3};
            } else if (line.getX2() == l.getX2() && line.getY2() == l.getY2() && !vis.count(make_pair(line.getX2(), line.getY2()))) {//p2==p2
                return {line, 4};
            }
        }
        return {};
    }
// Calculate cross product of vectors (p1, p2) and (p1, p3)
    double crossProduct(const Point& p1, const Point& p2, const Point& p3){
        return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY()) - (p3.getX() - p1.getX()) * (p2.getY()- p1.getY());
    }
// Check if the Points are ordered in clockwise direction
    bool isClockwise(const std::vector<Point>& points){
        int n = points.size();
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            sum += crossProduct(points[i], points[(i + 1) % n], points[(i + 2) % n]);
        }
        return sum < 0.0;
    }
    vector<Point> sortingPoints(vector<Line> lines_){
        vector<Point> points;
        for (const auto &l: lines_) {
            points.emplace_back(l.getX1(), l.getY1());
            points.emplace_back(l.getX2(), l.getY2());
        }

        vector<Point> points_Finals;
        if (points[1] != points[2] && points[1] != points[3]) {
            points_Finals.emplace_back(points[1]);
            points_Finals.emplace_back(points[0]);
            swap(points[0], points[1]);
        } else {
            points_Finals.emplace_back(points[0]);
            points_Finals.emplace_back(points[1]);
        }
        auto p2 = points[2];
        auto p3 = points[3];
        if (p2.getX() == points[1].getX() && p2.getY() == points[1].getY()) {
            points_Finals.emplace_back(p2);
            points_Finals.emplace_back(p3);
        } else {
            points_Finals.emplace_back(p3);
            points_Finals.emplace_back(p2);
            swap(points[3], points[2]);
        }
        for (int i = 4; i < points.size(); i += 2) {
            auto curr = points[i];
            auto next = points[i + 1];
            if (curr.getX() == points[i - 1].getX() && curr.getY() == points[i - 1].getY()) {
                points_Finals.emplace_back(curr);
                points_Finals.emplace_back(next);
            } else {
                points_Finals.emplace_back(next);
                points_Finals.emplace_back(curr);
                swap(points[i + 1], points[i]);
            }
        }
        vector<Point> ff;
        ff.emplace_back(points_Finals.front());
        for (int i = 1; i <= points_Finals.size(); ++i) {
            if (i % 2 == 1)ff.emplace_back(points_Finals[i]);
        }
        auto first = ff.front();
        auto last = ff.back();
        if (first != last)ff.push_back(first);
        //insure unticlock wise
        if (isClockwise(ff)) {
            reverse(ff.begin(), ff.end());
        }
        return ff;
    }
//    OutLines(const vector<rectangle>& vector1);
    vector<Point> getRoofPoints(const vector<Room>& recs){
        vector<Point> points;
        if (recs.empty())return points;
        auto OuterLines = getOutSideLines(recs);

        auto sort = clockwiseSort(OuterLines);
        points = sortingPoints(sort);

        return points;
    }
};


void JsonExtraction::outputJsonFile(crow::json::wvalue &jsonFile,
                                            Design &design,
                                            int solutionIndex,
                                            vector<Room> &oldRecs,
                                            vector<Room> &newRecs,
                                            vector<Line> &oldLines,
                                            vector<Line> &drawingLines,
                                            double thickness,
                                            const map<pair<string, string>, pair<int, int> > &top,
                                            const map<pair<string, string>, pair<int, int> > &bottom,
                                            const map<pair<string, string>, pair<int, int> > &left,
                                            const map<pair<string, string>, pair<int, int> > &right,
                                            vector<Room> &surface3d,
                                            map<string, int> &sortedRectangles,
                                            map < string , vector<double> >dimensions,
                                            vector<Line>&innerLines,
                                            double area ,
                                            vector<Room> room3DPrinting)
{
    LOG(LogLevel::Error, "OutputDesign");
    for(auto &room : design.getRooms())
    {
        room.printDetails();
    }
    cout<<" JsonExtraction::outputJsonFile\n";
    string roomCollectionId = "1";
    int roomsCount = design.getRoomsCount();
    int bedroomsCount = design.getBedroomsCount();
//    double roomCollectionMinArea = design.MinArea;
//    double roomCollectionMaxArea = design.MaxArea;
    double roomCollectionArea = design.getArea();

    jsonFile[solutionIndex]["id"] = roomCollectionId;
//    jsonFile[solutionIndex]["rotatedDegree"] = 360 - design.rotatedDegree;
//    cout << design.rotatedDegree<<"\n";
    jsonFile[solutionIndex]["roomsNumbers"] = roomsCount;
    jsonFile[solutionIndex]["bedroomsNumbers"] = bedroomsCount;
    jsonFile[solutionIndex]["area"] = roomCollectionArea;

    jsonFile[solutionIndex]["spaces"]={};
    jsonFile[solutionIndex]["roofs"] = {} ;
    jsonFile[solutionIndex]["lines"]={};
    jsonFile[solutionIndex]["doors"]= std::vector<crow::json::wvalue>{};
    jsonFile[solutionIndex]["windows"]= std::vector<crow::json::wvalue>{};
    //jsonFile[solutionIndex]["furnitures"]={};
    jsonFile[solutionIndex]["surfaces"]={};
    jsonFile[solutionIndex]["rooms"]={};
    jsonFile[solutionIndex]["Design"]={};

    int roomIndex = 0;
    for(auto &room : design.getRooms())
    {
        jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex] =
        {
            {"id" , room.getRoomId()},
            {"x1" , room.getX1()},
            {"y1" , room.getY1()},
            {"x2" , room.getX2()},
            {"y2" , room.getY2()},
        };

        jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Door"] = {};
        jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Window"] = {};
        jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Opening"] = {};


        int doorIndex = 0;
        Room room2 = room;
        for(auto &door : room2.getDoors())
        {
            jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Door"][doorIndex++]=
                    {
                            {"x1" , door.getDoor().getX1()},
                            {"y1" , door.getDoor().getY1()},
                            {"x2" , door.getDoor().getX2()},
                            {"y2" , door.getDoor().getY2()},
                    };
        }

        int windowIndex = 0;
        for(auto &window : room2.getWindows())
        {
            jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Window"][windowIndex++]=
                    {
                            {"x1" , window.getWindow().getX1()},
                            {"y1" , window.getWindow().getY1()},
                            {"x2" , window.getWindow().getX2()},
                            {"y2" , window.getWindow().getY2()},
                    };
        }

        int openingIndex = 0;
        for(auto &open : room2.getOpenings())
        {
            jsonFile[solutionIndex]["Design"]["Rooms"][roomIndex]["Opening"][openingIndex++]=
                    {
                            {"x1" , open.getOpening().getX1()},
                            {"y1" , open.getOpening().getY1()},
                            {"x2" , open.getOpening().getX2()},
                            {"y2" , open.getOpening().getY2()},
                    };
        }

        roomIndex++;
    }

    roomIndex = 0;
    for(auto &conn : design.getConnections())
    {
        for(auto &c : conn.second)
        {
            jsonFile[solutionIndex]["Design"]["connections"][roomIndex++] =
            {
                {"first" , conn.first},
                {"second" , c},
            };
        }
    }

    vector<Room>old = oldRecs,newR=newRecs;

    //remove Corridor id
    for(auto &rec:old){
        if(rec.getRoomId().find("Corridor")!= std::string::npos
        || rec.getRoomType() == RoomType::Corridor)rec.changeId("");
    }
    for(auto &rec:newR){
        if(rec.getRoomId().find("Corridor")!= std::string::npos
           || rec.getRoomType() == RoomType::Corridor)rec.changeId("");
    }
    //remove Corridor id
    for(auto &rec:surface3d){
        if(rec.getRoomId().find("Corridor")!= std::string::npos
           || rec.getRoomType() == RoomType::Corridor)rec.changeId("");
    }
    //remove Corridor id
    for(auto &rec:sortedRectangles){
        if(rec.first.find("Corridor")!= std::string::npos || rec.first.find("corridor")!= std::string::npos){
            sortedRectangles[""]=rec.second;
            sortedRectangles.erase(rec.first);
        }
    }
    //store Roof points
    storeRoofs(solutionIndex , jsonFile , design);
    //store rooms
    storeRooms(solutionIndex,jsonFile,surface3d,sortedRectangles);
    //store surface3D
    storeSurfaces(solutionIndex,jsonFile,surface3d,sortedRectangles);

    //store spaces
    storeSpaces(solutionIndex,jsonFile,design , innerLines);
    //store Spaces and Lines and doors and windows
    storeLines(solutionIndex,jsonFile,oldLines,thickness,top,bottom,left,right,innerLines);
    //store directions Dimensions

    if(area == -1)
    {
        jsonFile[solutionIndex]["scaleFactor"] = 1;
    }else{
        double homeArea = design.getArea();
        double scaleFactor = sqrt(area / homeArea);
        jsonFile[solutionIndex]["scaleFactor"]=scaleFactor;
    }

    storeDirectionsDimensions(solutionIndex , jsonFile , dimensions);

    //store 3D Prining
    store3DPrinting(solutionIndex , jsonFile , room3DPrinting);
}
void JsonExtraction::store3DPrinting(int solutionIndex, crow::json::wvalue &jsonFile,const vector<Room> &room3DPrinting)
{
    jsonFile[solutionIndex]["3DPrining"] = {};
    int layer  = -1;
    int index2 = 0;
    double z2 = 0;
    for(auto &line : room3DPrinting)
    {
        if(line.getZ2() == z2)index2++;
        else z2 = line.getZ2() , layer++ , index2 = 0;

        jsonFile[solutionIndex]["3DPrining"][layer][index2] =
                {
                        {"wallName" , line.getRoomId()},
                        {"x1" , line.getX1()},
                        {"y1" , line.getY1()},
                        {"z1" , line.getZ1()},
                        {"x2" , line.getX2()},
                        {"y2" , line.getY2()},
                        {"z2" , line.getZ2()},
                };
    }
}

void JsonExtraction::storeDirectionsDimensions(int solutionIndex, crow::json::wvalue &jsonFile,map<string, vector<double> > dimensions)
{
    jsonFile[solutionIndex]["Dimenions"]={};
    int dimIndex = 0;
    for(auto &dir : dimensions)
    {
        string dirName = dir.first;
        vector<double>values = dir.second;
        double sumValues = 0.0;
        jsonFile[solutionIndex]["Dimenions"][dimIndex]["direction"] = dirName;
        jsonFile[solutionIndex]["Dimenions"][dimIndex]["values"] = {};
        for (int var = 0; var < values.size(); ++var) {
            jsonFile[solutionIndex]["Dimenions"][dimIndex]["values"][var] = values[var];
            sumValues += values[var];
        }
        jsonFile[solutionIndex]["Dimenions"][dimIndex]["sumValues"] = sumValues;
        dimIndex++;
    }
}

void JsonExtraction::storeSpaces(int index, crow::json::wvalue &outputJson, Design &roomCollection , vector<Line>&innerLines)
{
    int spaceIndex=0;
    //store Spaces
    for( auto &rec : roomCollection.getRoomsRef())
    {
        //store Spaces
        // Rectangle1 innerRec(rec.getId(),100000 , 1000000 , -1000000 , -100000);
        // for(auto &inner : innerLines)
        // {
        //     if(inner.getId() == rec.getName() || inner.getId()==rec.getId())
        //     {
        //         innerRec.setX1(min(innerRec.getX1() , inner.getX1()));
        //         innerRec.setY1(min(innerRec.getY1() , inner.getY1()));
        //         innerRec.setX2(max(innerRec.getX2() , inner.getX2()));
        //         innerRec.setY2(max(innerRec.getY2() , inner.getY2()));
        //     }
        // }

        if( rec.getRoomType() == RoomType::Corridor)
            outputJson[index]["spaces"][spaceIndex]["name"]="";

        else outputJson[index]["spaces"][spaceIndex]["name"]=rec.getRoomId();

        outputJson[index]["spaces"][spaceIndex]["image"] = "";

        outputJson[index]["spaces"][spaceIndex]["center"]={
                {"x1", rec.getX1()},
                {"y1", rec.getY1()},
                {"x2", rec.getX2()},
                {"y2", rec.getY2()},
        };

        outputJson[index]["spaces"][spaceIndex]["inner"] = {
                {"x1", rec.getX1() + 5},
                {"y1", rec.getY1() + 5},
                {"x2", rec.getX2() - 5},
                {"y2", rec.getY2() - 5},
        };

        //store spaces Windows and doors and opennings

        int windowsIndex=0;
        outputJson[index]["spaces"][spaceIndex]["windows"]={};
        for(auto &wd : rec.getWindows())
        {
            Line line = wd.getWindow();

            string dir;

            if(line.getDir() == 'h')
            {
                if (line.getY1() == rec.getY1()) dir="N";
                else if (line.getY1() == rec.getY2()) dir="S";
            }
            else if(line.getDir() == 'v')
            {
                if (line.getX1() == rec.getX1()) dir="W" ;
                else if (line.getX1() == rec.getX2()) dir="E" ;
            }

            outputJson[index]["spaces"][spaceIndex]["windows"][windowsIndex++]={
                    {"dir",dir},
                    {"x1", line.getX1()},
                    {"y1", line.getY1()},
                    {"x2", line.getX2()},
                    {"y2", line.getY2()}
            };
        }

        int doorIndex=0;
        outputJson[index]["spaces"][spaceIndex]["doors"]={};
        for(auto &dr : rec.getDoors())
        {
            Line line = dr.getDoor();

            string dir;

            if(line.getDir() == 'h')
            {
                if (line.getY1() == rec.getY1()) dir="N";
                else if (line.getY1() == rec.getY2()) dir="S";
            }
            else if(line.getDir() == 'v')
            {
                if (line.getX1() == rec.getX1()) dir="W" ;
                else if (line.getX1() == rec.getX2()) dir="E" ;
            }

            outputJson[index]["spaces"][spaceIndex]["doors"][doorIndex++]={
                    {"dir",dir},
                    {"x1", line.getX1()},
                    {"y1", line.getY1()},
                    {"x2", line.getX2()},
                    {"y2", line.getY2()}
            };
        }

        int openningIndex = 0;
        outputJson[index]["spaces"][spaceIndex]["opennings"]={};
        for(auto &op : rec.getOpenings())
        {
            Line line = op.getOpening();

            string dir;

            if(line.getDir() == 'h')
            {
                if (line.getY1() == rec.getY1()) dir="N";
                else if (line.getY1() == rec.getY2()) dir="S";
            }
            else if(line.getDir() == 'v')
            {
                if (line.getX1() == rec.getX1()) dir="W" ;
                else if (line.getX1() == rec.getX2()) dir="E" ;
            }

            outputJson[index]["spaces"][spaceIndex]["opennings"][openningIndex++]={
                    {"dir",dir},
                    {"x1", line.getX1()},
                    {"y1", line.getY1()},
                    {"x2", line.getX2()},
                    {"y2", line.getY2()}
            };

        }

        spaceIndex++;
    }
}

void JsonExtraction::storeLines(int index, crow::json::wvalue &outputJson,vector<Line>&oldLines,double thickness,
                                        const map<pair<string,string>,pair<int,int>>&top,
                                        const map<pair<string,string>,pair<int,int>>&bottom,
                                        const map<pair<string,string>,pair<int,int>>&left,
                                        const map<pair<string,string>,pair<int,int>>&right,
                                        vector<Line>&innerLines) {

    //lines
    vector<Line>singleLine;
    vector<Line>windows;//each window 6 line
    vector<Line>doors;//each door 7 line
    vector<Line>opennings;
    for(auto &line:oldLines){
        vector<Line>twoLines=DesignGeometryManager::applyLineSideRule(line,thickness,top,bottom,left,right,innerLines);
        for(auto &line2:twoLines)line2.setRoomName(line.getId());
        if(line.getId2()=="Window")
        {
            windows.insert(windows.end(),twoLines.begin(),twoLines.end());
        }
        else if(line.getId2()=="Door")
        {
            doors.insert(doors.end(),twoLines.begin(),twoLines.end());
        }
        else if(line.getId2()=="Openning")
        {
            opennings.insert(opennings.end(),twoLines.begin(),twoLines.end());
        }
        else
        {
            singleLine.insert(singleLine.end(),twoLines.begin(),twoLines.end());
        }
    }
    int lineIndex=0,doorsIndex=0,windowsIndex=0,openningIndex = 0;
    for(auto &line:singleLine){
        string roomName = line.getRoomName();
        if(line.getRoomName().find("Corridor") != std::string::npos )roomName = "";

        outputJson[index]["lines"][lineIndex++]={
                {"x1", line.getX1()},
                {"y1", line.getY1()},
                {"x2", line.getX2()},
                {"y2", line.getY2()},
                {"roomName", roomName},
                {"color",line.getColor()}
        };
    }
    for (int i = 0; i < doors.size(); i+=7) {
        Line curve(0,0,0,0);
        int count=0;
        outputJson[index]["doors"][doorsIndex]["centerLines"]={
                {"x1", doors[i].getCenterX1()},
                {"y1", doors[i].getCenterY1()},
                {"x2", doors[i].getCenterX2()},
                {"y2", doors[i].getCenterY2()},
                {"color",doors[i].getColor()}
        };
        for (int k = i; k < i+7; ++k) {
            if (doors[k].getStartAngle()!=0.0 || doors[k].getEndAngle()!=0.0){//!=red
                curve=doors[k];
            }else {
                if(doors[k].getColor()=="#ffffff"){
                    outputJson[index]["lines"][lineIndex++]={
                            {"x1", doors[k].getX1()},
                            {"y1", doors[k].getY1()},
                            {"x2", doors[k].getX2()},
                            {"y2", doors[k].getY2()},
                            {"color",doors[k].getColor()}
                    };
                }else{
                    outputJson[index]["doors"][doorsIndex]["lines"][count++]={
                            {"x1", doors[k].getX1()},
                            {"y1", doors[k].getY1()},
                            {"x2", doors[k].getX2()},
                            {"y2", doors[k].getY2()},
                            {"color",doors[k].getColor()},
                    };
                }
            }
        }

        outputJson[index]["doors"][doorsIndex]["center"]={{"x",curve.getX2()},{"y",curve.getY1()}};
        double radius=0.0;
        if (curve.getSideNum()==1 ||curve.getSideNum()==3 )radius=curve.getX2()-curve.getX1();

        if (curve.getSideNum()==4 ||curve.getSideNum()==2 )radius=curve.getY2()-curve.getY1();
        outputJson[index]["doors"][doorsIndex]["radius"]=radius;
        outputJson[index]["doors"][doorsIndex]["roomName"]=doors[i].getRoomName();
        outputJson[index]["doors"][doorsIndex]["startangle"]=curve.getStartAngle();
        outputJson[index]["doors"][doorsIndex]["endangle"]=curve.getEndAngle();
        doorsIndex++;
    }

    for (int i = 0; i < windows.size(); i+=6) {
        outputJson[index]["windows"][windowsIndex]["centerLines"]={
                {"x1", windows[i].getCenterX1()},
                {"y1", windows[i].getCenterY1()},
                {"x2", windows[i].getCenterX2()},
                {"y2", windows[i].getCenterY2()},
                {"color",windows[i].getColor()}
        };
        outputJson[index]["windows"][windowsIndex]["roomName"]=windows[i].getRoomName();
        vector<Line> curves;
        double x1=10000,y1=10000,x2=-100000,y2=-1000000;
        for (int m = i; m < i+6; ++m) {
            if (windows[m].getColor()!="#E72929"){//!=red
                x1= min(x1,windows[m].getX1());
                y1= min(y1,windows[m].getY1());
                x2= max(x2,windows[m].getX2());
                y2= max(y2,windows[m].getY2());
            }else curves.push_back(windows[m]);
        }
        outputJson[index]["windows"][windowsIndex]["center"]={
                {"x1", x1},
                {"y1", y1},
                {"x2", x2},
                {"y2", y2},
        };
        if(x2-x1>y2-y1){
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y1},
                    {"x2", x1},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x2},
                    {"y1", y1},
                    {"x2", x2},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
        }else{
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y1},
                    {"x2", x2},
                    {"y2", y1},
                    {"color","#ffffff"}
            };
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y2},
                    {"x2", x2},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
        }
        outputJson[index]["windows"][windowsIndex]["centerLine1"]={
                {"x1", curves[0].getX1()},
                {"y1", curves[0].getY1()},
                {"x2", curves[0].getX2()},
                {"y2", curves[0].getY2()},
        };
        outputJson[index]["windows"][windowsIndex]["centerLine2"]={
                {"x1", curves[1].getX1()},
                {"y1", curves[1].getY1()},
                {"x2", curves[1].getX2()},
                {"y2", curves[1].getY2()},
        };
        windowsIndex++;
    }

    for (int i = 0; i < opennings.size(); i+=6) {
        outputJson[index]["opennings"][openningIndex]["centerLines"]={
                {"x1", opennings[i].getCenterX1()},
                {"y1", opennings[i].getCenterY1()},
                {"x2", opennings[i].getCenterX2()},
                {"y2", opennings[i].getCenterY2()},
                {"color",opennings[i].getColor()}
        };
        outputJson[index]["opennings"][openningIndex]["roomName"]=opennings[i].getRoomName();
        vector<Line> curves;
        double x1=10000,y1=10000,x2=-100000,y2=-1000000;
        for (int m = i; m < i+6; ++m) {
            if (opennings[m].getColor()!="#E72929"){//!=red
                x1= min(x1,opennings[m].getX1());
                y1= min(y1,opennings[m].getY1());
                x2= max(x2,opennings[m].getX2());
                y2= max(y2,opennings[m].getY2());
            }else curves.push_back(opennings[m]);
        }
        outputJson[index]["opennings"][openningIndex]["center"]={
                {"x1", x1},
                {"y1", y1},
                {"x2", x2},
                {"y2", y2},
        };
        if(x2-x1>y2-y1){
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y1},
                    {"x2", x1},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x2},
                    {"y1", y1},
                    {"x2", x2},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
        }else{
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y1},
                    {"x2", x2},
                    {"y2", y1},
                    {"color","#ffffff"}
            };
            outputJson[index]["lines"][lineIndex++]={
                    {"x1", x1},
                    {"y1", y2},
                    {"x2", x2},
                    {"y2", y2},
                    {"color","#ffffff"}
            };
        }
        outputJson[index]["opennings"][openningIndex]["centerLine1"]={
                {"x1", curves[0].getX1()},
                {"y1", curves[0].getY1()},
                {"x2", curves[0].getX2()},
                {"y2", curves[0].getY2()},
        };
        outputJson[index]["opennings"][openningIndex]["centerLine2"]={
                {"x1", curves[1].getX1()},
                {"y1", curves[1].getY1()},
                {"x2", curves[1].getX2()},
                {"y2", curves[1].getY2()},
        };
        openningIndex++;
    }
}

void JsonExtraction::storeRooms(int index, crow::json::wvalue &outputJson,vector<Room>&surface3d,map<string,int>&sortedRectangles) {
    for(auto &room:sortedRectangles){
        outputJson[index]["rooms"][room.second]={
                {"name", room.first},
                {"number", room.second},
        };
    }
    map<int,int>wallNumbers;
    for(int i = 0 ; i < surface3d.size() ; i+=6 ){
        int roomIndex = sortedRectangles[surface3d[i].getRoomId()];
        int roomNumber=wallNumbers[roomIndex];
        for (int k = i; k < i+6; ++k) {
            outputJson[index]["rooms"][roomIndex]["walls"][roomNumber][k%6]={
                    {"name", surface3d[k].getRoomId()},
                    {"roomName", surface3d[k].getRoomId()},
                    {"side", surface3d[k].getSurfaceSide()},
                    {"x1", surface3d[k].getX1()},
                    {"y1", surface3d[k].getY1()},
                    {"x2", surface3d[k].getX2()},
                    {"y2", surface3d[k].getY2()},
                    {"z1", surface3d[k].getZ1()},
                    {"z2", surface3d[k].getZ2()},
            };
        }
        wallNumbers[roomIndex]++;
    }
}

void JsonExtraction::storeSurfaces(int index, crow::json::wvalue &outputJson, vector<Room> &surface3d,map<string,int>&sortedRectangles) {
    for(int i = 0 ; i < surface3d.size() ; i++ ){
        int roomIndex=sortedRectangles[surface3d[i].getRoomId()];
        outputJson[index]["surfaces"][i] = {
                {"name", surface3d[i].getRoomId()},
                {"side", surface3d[i].getSurfaceSide()},
                {"roomName", surface3d[i].getRoomId()},
                {"x1", surface3d[i].getX1()},
                {"y1", surface3d[i].getY1()},
                {"x2", surface3d[i].getX2()},
                {"y2", surface3d[i].getY2()},
                {"z1", surface3d[i].getZ1()},
                {"z2", surface3d[i].getZ2()},
        };
    }
}

void JsonExtraction::storeRoofs(int solutionIndex, crow::json::wvalue &jsonFile, const Design &roomCollection)
{
    OutLines o;
    vector<Room>recs;
    for(auto &rec:roomCollection.getRooms())
    {
        recs.emplace_back(rec.getRoomId(),rec.getX1(),rec.getY1(),rec.getX2(),rec.getY2());
    }
    auto points = o.getRoofPoints(recs);
    for (int i = 0; i < points.size(); ++i)
    {
        jsonFile[solutionIndex]["roofs"][i] =
                {
                        {"x",points[i].getX()},
                        {"y",points[i].getY()},
                };
    }
}


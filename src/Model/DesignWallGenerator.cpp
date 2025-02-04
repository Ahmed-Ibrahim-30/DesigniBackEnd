#include "DesignWallGenerator.h"

vector<Wall> DesignWallGenerator::generateWalls(const Design &design)
{
    vector<Wall> walls;

    Room room("Bath" , -7 , 0 , 0 , 8);
    Room room2("Bath2" , 0 , 0 , 9 , 8);

    room.addDoor(-6 , 8 , -3 , 8);
    room2.addWindow(4 , 8 , 7 , 8);
    room.addWindow(-6 , 0 , -3 , 0);
    room2.addDoor(4 , 0 , 7 , 0);
    Design myD;
    myD.addRoom(room);
    myD.addRoom(room2);

    Design newDesign = design;

    double unit = 100;
    newDesign.scaleDesign(unit);

    // newDesign.deleteRoom("Bedroom");
    // newDesign.deleteRoom("Living");
    // newDesign.deleteRoom("Kitchen");
    // newDesign.deleteRoom("GB");

    vector<Door> doors;
    vector<Window> windows;
    vector<Opening> openings;

    for(auto &room : newDesign.getRooms())
    {
        Room newRoom = room;
        doors.insert(doors.end() , newRoom.getDoors().begin() , newRoom.getDoors().end());
        windows.insert(windows.end() , newRoom.getWindows().begin() , newRoom.getWindows().end());
        openings.insert(openings.end() , newRoom.getOpenings().begin() , newRoom.getOpenings().end());
    }

    cout<<"Doors Size = "<<doors.size()<<"\n";
    cout<<"windows Size = "<<windows.size()<<"\n";
    cout<<"Opening Size = "<<openings.size()<<"\n";



    double studHeight= 300;
    double studWidth= 5.08;
    double studDepth= 15.2;
    double doorHeight = 220;
    double windowHeight = 120;

    double studDistance = 0.5 * unit;

    double topBottomPlateHeight= 10;


    vector<Line> designLines = getDesignLines(newDesign);
    bool flag1 = false , flag2 = true;

    for(auto &line : designLines)
    {
        double x1 = line.getX1() , x2 = line.getX2() ;
        double y1 = line.getY1() , y2 = line.getY2();

        // if(line.getDir() == 'h' && flag1) continue;
        // if(line.getDir() == 'v' && flag2) {flag2 = false; continue;}

        if(line.getDir() == 'h')//y1 = y2
        {
            flag1=true;
            x1 -= studDepth/2;
            x2 += studDepth/2;

            walls.emplace_back(x1 , 0 , y1 - studDepth/2 , x1+studWidth , studHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x2-studWidth , 0 , y1 - studDepth/2 , x2 , studHeight , y1 + studDepth/2 , "#563232");

            for (int x = x1 + studDistance; x < x2; x+= studDistance) {
                if(x+studWidth >=x2) break;
                walls.emplace_back(x , 0 , y1 - studDepth/2 , x+studWidth , studHeight , y1 + studDepth/2 , "#563232");
            }

            //Bottom Plate
            walls.emplace_back(x1 , -10 , y1 - studDepth/2 , x2 , -10 + topBottomPlateHeight , y1 + studDepth/2 , "#2B1919");

            //Top Plate
            walls.emplace_back(x1 , studHeight , y1 - studDepth/2 , x2 , studHeight + topBottomPlateHeight , y1 + studDepth/2 , "#2B1919");

        }
        else
        {
            flag2= true;

            walls.emplace_back(x1 - studDepth/2, 0 , y1 , x1 + studDepth/2 , studHeight ,  y1+studWidth , "#563232");

            walls.emplace_back(x1 - studDepth/2, 0 , y2 -  studWidth, x1 + studDepth/2 , studHeight ,  y2 , "#563232");

            for (int y = y1 + studDistance; y < y2; y+= studDistance) {
                if(y+studWidth >=y2) break;
                walls.emplace_back(x1 - studDepth/2, 0 , y , x1 + studDepth/2 , studHeight ,  y+studWidth , "#563232");
            }

            //Bottom Plate
            walls.emplace_back(x1 - studDepth/2, -10  , y1 , x1 + studDepth/2 , -10 + topBottomPlateHeight ,  y2 , "#2B1919");

            //Top Plate
            walls.emplace_back(x1 - studDepth/2, studHeight  , y1 , x1 + studDepth/2 , studHeight + topBottomPlateHeight ,  y2 , "#2B1919");

        }
    }


    for(auto &door : doors)
    {
        Line doorL = door.getDoor();

        double x1 = doorL.getX1() , x2 = doorL.getX2() ;
        double y1 = doorL.getY1() , y2 = doorL.getY2();

        if(doorL.getDir() == 'h')
        {
            walls.emplace_back(x1 , 0 , y1 - studDepth/2 , x1+studWidth , doorHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x2-studWidth , 0 , y1 - studDepth/2 , x2 , doorHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x1 ,  doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x1+studWidth , studHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x2-studWidth ,  doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x2 , studHeight , y1 + studDepth/2 , "#563232");

            for (int x = x1 + studDistance; x < x2; x+= studDistance) {
                if(x+studWidth >=x2) break;
                walls.emplace_back(x , doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x+studWidth , studHeight , y1 + studDepth/2 , "#563232");
            }

            walls.emplace_back(x1 , doorHeight , y1 - studDepth/2 , x2 , doorHeight + topBottomPlateHeight , y1 + studDepth/2 , "#563232");
            walls.emplace_back(x1 , studHeight , y1 - studDepth/2 , x2 , studHeight + topBottomPlateHeight , y1 + studDepth/2 , "#563232");
        }
        else {

            walls.emplace_back(x1 - studDepth/2, 0 , y1  , x1+ studDepth/2 , doorHeight , y1 +studWidth , "#563232");

            walls.emplace_back(x1 - studDepth/2, 0 , y2 -studWidth , x1+ studDepth/2 , doorHeight , y2 , "#563232");

            walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y1  , x1+ studDepth/2 , studHeight , y1 +studWidth , "#563232");

            walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y2 -studWidth , x1+ studDepth/2 , studHeight , y2 , "#563232");

            for (int y = y1 + studDistance; y < y2; y+= studDistance) {
                if(y+studWidth >=y2) break;
                walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y  , x1+ studDepth/2 , studHeight , y +studWidth , "#563232");
            }

            walls.emplace_back(x1 - studDepth/2, doorHeight , y1 , x2 + studDepth/2, doorHeight + topBottomPlateHeight , y2  , "#563232");
            walls.emplace_back(x1 - studDepth/2, studHeight , y1 , x2 + studDepth/2, studHeight + topBottomPlateHeight , y2  , "#563232");
        }
    }

    for(auto &door : openings)
    {
        Line doorL = door.getOpening();

        double x1 = doorL.getX1() , x2 = doorL.getX2() ;
        double y1 = doorL.getY1() , y2 = doorL.getY2();

        if(doorL.getDir() == 'h')
        {
            walls.emplace_back(x1 , 0 , y1 - studDepth/2 , x1+studWidth , doorHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x2-studWidth , 0 , y1 - studDepth/2 , x2 , doorHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x1 ,  doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x1+studWidth , studHeight , y1 + studDepth/2 , "#563232");

            walls.emplace_back(x2-studWidth ,  doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x2 , studHeight , y1 + studDepth/2 , "#563232");

            for (int x = x1 + studDistance; x < x2; x+= studDistance) {
                if(x+studWidth >=x2) break;
                walls.emplace_back(x , doorHeight + topBottomPlateHeight , y1 - studDepth/2 , x+studWidth , studHeight , y1 + studDepth/2 , "#563232");
            }

            walls.emplace_back(x1 , doorHeight , y1 - studDepth/2 , x2 , doorHeight + topBottomPlateHeight , y1 + studDepth/2 , "#563232");
            walls.emplace_back(x1 , studHeight , y1 - studDepth/2 , x2 , studHeight + topBottomPlateHeight , y1 + studDepth/2 , "#563232");
        }
        else {

            walls.emplace_back(x1 - studDepth/2, 0 , y1  , x1+ studDepth/2 , doorHeight , y1 +studWidth , "#563232");

            walls.emplace_back(x1 - studDepth/2, 0 , y2 -studWidth , x1+ studDepth/2 , doorHeight , y2 , "#563232");

            walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y1  , x1+ studDepth/2 , studHeight , y1 +studWidth , "#563232");

            walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y2 -studWidth , x1+ studDepth/2 , studHeight , y2 , "#563232");

            for (int y = y1 + studDistance; y < y2; y+= studDistance) {
                if(y+studWidth >=y2) break;
                walls.emplace_back(x1 - studDepth/2, doorHeight + topBottomPlateHeight , y  , x1+ studDepth/2 , studHeight , y +studWidth , "#563232");
            }

            walls.emplace_back(x1 - studDepth/2, doorHeight , y1 , x2 + studDepth/2, doorHeight + topBottomPlateHeight , y2  , "#563232");
            walls.emplace_back(x1 - studDepth/2, studHeight , y1 , x2 + studDepth/2, studHeight + topBottomPlateHeight , y2  , "#563232");
        }
    }

    for(auto &door : windows)
    {
        Line doorL = door.getWindow();

        // doorL = Line(-150 , 40 , -150 , 180);

        double x1 = doorL.getX1() , x2 = doorL.getX2() ;
        double y1 = doorL.getY1() , y2 = doorL.getY2();

        if(doorL.getDir()=='v')
        {
            swap(x1 , y1) , swap(x2,y2);
        }

        vector<Wall> wWalls;

        wWalls.emplace_back(x1 ,  studHeight - topBottomPlateHeight , y1 - studDepth/2 , x1+studWidth , studHeight , y1 + studDepth/2 , "#563232");

        wWalls.emplace_back(x2-studWidth ,  studHeight - topBottomPlateHeight , y1 - studDepth/2 , x2 , studHeight , y1 + studDepth/2 , "#563232");

        for (int x = x1 + studDistance; x < x2; x+= studDistance) {
            if(x+studWidth >=x2) break;
            wWalls.emplace_back(x , studHeight - topBottomPlateHeight , y1 - studDepth/2 , x+studWidth , studHeight , y1 + studDepth/2 , "#563232");
        }

        wWalls.emplace_back(x1 , studHeight - 2*topBottomPlateHeight , y1 - studDepth/2 , x2 , studHeight - topBottomPlateHeight , y1 + studDepth/2 , "#563232");


        wWalls.emplace_back(x1 , windowHeight , y1 - studDepth/2 , x1+studWidth , studHeight - 2*topBottomPlateHeight , y1 + studDepth/2 , "#563232");

        wWalls.emplace_back(x2-studWidth , windowHeight , y1 - studDepth/2 , x2 , studHeight - 2*topBottomPlateHeight , y1 + studDepth/2 , "#563232");

        wWalls.emplace_back(x1 , windowHeight - topBottomPlateHeight , y1 - studDepth/2 , x2 , windowHeight  , y1 + studDepth/2 , "#563232");
        wWalls.emplace_back(x1 , windowHeight - (topBottomPlateHeight*2) , y1 - studDepth/2 , x2 , windowHeight - (topBottomPlateHeight)  , y1 + studDepth/2 , "#563232");

        wWalls.emplace_back(x1 ,  0 , y1 - studDepth/2 , x1+studWidth , windowHeight - (topBottomPlateHeight*2) , y1 + studDepth/2 , "#563232");

        wWalls.emplace_back(x2-studWidth ,  0 , y1 - studDepth/2 , x2 , windowHeight - (topBottomPlateHeight*2) , y1 + studDepth/2 , "#563232");

        for (int x = x1 + studDistance; x < x2; x+= studDistance) {
            if(x+studWidth >=x2) break;
            wWalls.emplace_back(x , 0 , y1 - studDepth/2 , x+studWidth , windowHeight - (topBottomPlateHeight*2) , y1 + studDepth/2 , "#563232");
        }

        walls.emplace_back(x1 , -10 , y1 - studDepth/2 , x2 , -10 + topBottomPlateHeight , y1 + studDepth/2 , "#2B1919");

        //top header
        wWalls.emplace_back(x1 , studHeight , y1 - studDepth/2 , x2 , studHeight + topBottomPlateHeight , y1 + studDepth/2 , "#563232");

        if(doorL.getDir() == 'v')
        {
            for(auto &wall : wWalls)
            {
                swap(wall.x1 , wall.z1);
                swap(wall.x2 , wall.z2);
            }
        }

        walls.insert(walls.end() , wWalls.begin() , wWalls.end());
    }




    for(auto &room : newDesign.getRooms())
    {
        double x1 = room.getX1(),x2 = room.getX2() ;
        double y1 = room.getY1(),y2 = room.getY2();
        double width = room.getWidth() , height = room.getHeight();

        if(width < height)
        {
            walls.emplace_back(x1 -studDepth/2, studHeight + topBottomPlateHeight , y1 , x2 +studDepth/2, studHeight + 2*topBottomPlateHeight ,  y1+studWidth , "#563232");

            for (int y = y1 + studDistance; y < y2; y+= studDistance) {
                if(y+studWidth >=y2) break;
                walls.emplace_back(x1-studDepth/2, studHeight + topBottomPlateHeight , y , x2 + studDepth/2, studHeight + 2*topBottomPlateHeight ,  y+studWidth , "#563232");
            }
        }
        else {
            walls.emplace_back(x1 , studHeight + topBottomPlateHeight, y1 - studDepth/2 , x1+studWidth , studHeight + 2*topBottomPlateHeight  , y2+ studDepth/2 , "#563232");

            for (int x = x1 + studDistance; x < x2; x+= studDistance) {
                if(x+studWidth >=x2) break;
                walls.emplace_back(x , studHeight + topBottomPlateHeight , y1 - studDepth/2 , x+studWidth , studHeight + 2*topBottomPlateHeight , y2 + studDepth/2 , "#563232");
            }
        }
    }


    return walls;

}

vector<Line> DesignWallGenerator::getDesignLines(const Design &design)
{
    map<double , set<double>> xP , yP;
    map<double , vector<double>> xPV , yPV;
    vector<Line> designLines;
    Design newDesign = design;

    vector<Door> doors;
    vector<Window> windows;
    vector<Opening> openings;

    for(auto &room : newDesign.getRooms())
    {
        xP[room.getX1()].insert(room.getY1());
        xP[room.getX1()].insert(room.getY2());

        xP[room.getX2()].insert(room.getY1());
        xP[room.getX2()].insert(room.getY2());

        yP[room.getY1()].insert(room.getX1());
        yP[room.getY1()].insert(room.getX2());

        yP[room.getY2()].insert(room.getX1());
        yP[room.getY2()].insert(room.getX2());

        Room newRoom = room;

        doors.insert(doors.end() , newRoom.getDoors().begin() , newRoom.getDoors().end());
        windows.insert(windows.end() , newRoom.getWindows().begin() , newRoom.getWindows().end());
        openings.insert(openings.end() , newRoom.getOpenings().begin() , newRoom.getOpenings().end());
    }

    for(auto &x : xP)
    {
        for(auto &x1 : x.second)xPV[x.first].push_back(x1);
    }

    for(auto &y : yP)
    {
        for(auto &y1 : y.second)yPV[y.first].push_back(y1);
    }

    for(auto &xV : xPV)
    {
        double x = xV.first;
        for (int i = 1; i < xV.second.size(); ++i)
        {

            designLines.emplace_back(x , xV.second[i-1] , x , xV.second[i]);
        }
    }

    for(auto &yV : yPV)
    {
        double y = yV.first;
        for (int i = 1; i < yV.second.size(); ++i) {
            designLines.emplace_back(yV.second[i-1] , y , yV.second[i] , y);
        }
    }

    vector<Line> newDesignLines;

    for(auto &line : designLines)
    {
        bool breaked = false;
        for(auto &door : doors)
        {
            Line doorL = door.getDoor();
            if(line.getDir() =='h' && doorL.getDir() == 'h'
                && line.getY1() == doorL.getY1() && doorL.getX1() >= line.getX1() && doorL.getX2() <= line.getX2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , doorL.getX1() , line.getY1());
                newDesignLines.emplace_back(doorL.getX2() , line.getY1() , line.getX2() , line.getY1());
                breaked = true;
                break;
            }

            else if(line.getDir() =='v' && doorL.getDir() == 'v'
                && line.getX1() == doorL.getX1() && doorL.getY1() >= line.getY1() && doorL.getY2() <= line.getY2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , line.getX1() , doorL.getY1());
                newDesignLines.emplace_back(line.getX1() , doorL.getY2() , line.getX1() , line.getY2());

                breaked = true;
                break;
            }
        }
        if(!breaked) newDesignLines.push_back(line);
    }

    designLines = newDesignLines;
    newDesignLines.clear();

    for(auto &line : designLines)
    {
        bool breaked = false;
        for(auto &door : windows)
        {
            Line doorL = door.getWindow();
            if(line.getDir() =='h' && doorL.getDir() == 'h'
                && line.getY1() == doorL.getY1() && doorL.getX1() >= line.getX1() && doorL.getX2() <= line.getX2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , doorL.getX1() , line.getY1());
                newDesignLines.emplace_back(doorL.getX2() , line.getY1() , line.getX2() , line.getY1());
                breaked = true;
                break;
            }

            else if(line.getDir() =='v' && doorL.getDir() == 'v'
                && line.getX1() == doorL.getX1() && doorL.getY1() >= line.getY1() && doorL.getY2() <= line.getY2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , line.getX1() , doorL.getY1());
                newDesignLines.emplace_back(line.getX1() , doorL.getY2() , line.getX1() , line.getY2());

                breaked = true;
                break;
            }
        }
        if(!breaked) newDesignLines.push_back(line);
    }


    designLines = newDesignLines;
    newDesignLines.clear();

    for(auto &line : designLines)
    {
        bool breaked = false;
        for(auto &door : openings)
        {
            Line doorL = door.getOpening();
            if(line.getDir() =='h' && doorL.getDir() == 'h'
                && line.getY1() == doorL.getY1() && doorL.getX1() >= line.getX1() && doorL.getX2() <= line.getX2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , doorL.getX1() , line.getY1());
                newDesignLines.emplace_back(doorL.getX2() , line.getY1() , line.getX2() , line.getY1());
                breaked = true;
                break;
            }

            else if(line.getDir() =='v' && doorL.getDir() == 'v'
                && line.getX1() == doorL.getX1() && doorL.getY1() >= line.getY1() && doorL.getY2() <= line.getY2())
            {
                newDesignLines.emplace_back(line.getX1() , line.getY1() , line.getX1() , doorL.getY1());
                newDesignLines.emplace_back(line.getX1() , doorL.getY2() , line.getX1() , line.getY2());

                breaked = true;
                break;
            }
        }
        if(!breaked) newDesignLines.push_back(line);
    }

    designLines = newDesignLines;
    return designLines;
}

vector<Wall> DesignWallGenerator::constructRoof(const Polygon1 &polygon , double spacing) {
    vector<Wall> walls;

    vector<Point> pnts  = polygon.getPoints();

    vector<Point> guicePoints ;

    int diffLineFactor = 6;


    // Determine the bounding box of the polygon
    double minX = pnts[0].getX(), maxX = pnts[0].getX() , minY = pnts[0].getY() , maxY =pnts[0].getY();

    for (const auto &point : pnts) {
        minX = min(minX, point.getX());
        maxX = max(maxX, point.getX());


        minY = min(minY, point.getY());
        maxY = max(maxY, point.getY());

    }

    Line lastLine(0,0,0,0);

    if(maxX - minX >= maxY - minY) //lastLine have Big Y
    {
        lastLine = Line( 100000 , maxY , -100000 , maxY );

        for (const auto &point : pnts)
        {
            if(point.getY() == maxY)
            {
                lastLine.setX1(min(lastLine.getX1() , point.getX())) ;
                lastLine.setX2(max(lastLine.getX2() , point.getX())) ;
            }
            else {
                guicePoints.push_back(point);
            }
        }

        guicePoints.emplace_back(lastLine.getX1() , lastLine.getY1());
        guicePoints.emplace_back(lastLine.getX2() , lastLine.getY2());
        sort(guicePoints.begin() , guicePoints.end());
    }
    else
    {
        lastLine = Line( maxX , 100000 , maxX , -100000 );

        for (const auto &point : pnts)
        {
            if(point.getX() == maxX)
            {
                lastLine.setY1(min(lastLine.getY1() , point.getY())) ;
                lastLine.setY2( max(lastLine.getY2() , point.getY()));
            }
            else {
                guicePoints.push_back(point);
            }
        }

        guicePoints.emplace_back(lastLine.getX1() , lastLine.getY1());
        guicePoints.emplace_back(lastLine.getX2() , lastLine.getY2());
        sort(guicePoints.begin() , guicePoints.end() , [](const Point &p1 ,const Point &p2) {return p1.getY() < p2.getY();});
    }

    if(maxX - minX >= maxY - minY)
    {
        vector<double> allValues;
        for (int i = 1; i < guicePoints.size(); ++i)
        {
            Point prevP = guicePoints[i-1];
            Point curP = guicePoints[i];
            if(i+1 != guicePoints.size()) allValues.push_back(curP.getX());

            int diff = curP.getX() - prevP.getX();

            if(diff == 0) continue;

            diff--;
            double num = diff/diffLineFactor ;

            spacing = diff / num;

            if(i == 1 )
            {
                for (double x = curP.getX() - spacing; x - spacing > prevP.getX();  x-= spacing)
                {
                    allValues.push_back(x);
                }
            }
            else
            {
                for (double x = prevP.getX() + spacing; x + spacing< curP.getX();  x+= spacing)
                {
                    allValues.push_back(x);
                }
            }
        }

        // Loop through vertical lines
        for (auto &x : allValues) {
            vector<float> intersections;

            // Find intersection points of the vertical line with each polygon edge
            for (size_t i = 0; i < pnts.size(); i++) {
                Point p1 = pnts[i];
                Point p2 = pnts[(i + 1) % pnts.size()]; // Next vertex (cyclic)

                // Check if the line x intersects the edge
                if (isBetween(x, p1.getX(), p2.getX())) {
                    double y = getIntersectionY(x, p1, p2);
                    if (y != -1) intersections.push_back(y);
                }
            }

            // Sort intersections by y-coordinate
            sort(intersections.begin(), intersections.end());

            // Draw vertical lines between consecutive intersection points
            for (size_t i = 1; i < intersections.size(); i += 1) {
                float yStart = intersections[i-1];
                float yEnd = intersections[i];

                walls.emplace_back(x , 300 , yStart , x , 400, yEnd , "");
                cout << "{ x1:" << x << ", y1:" << yStart << ", x2:" << x << ", y2:" << yEnd << "},\n";
            }
        }
    }

    else
    {

        vector<double> allValues;
        for (int i = 1; i < guicePoints.size(); ++i)
        {
            Point prevP = guicePoints[i-1];
            Point curP = guicePoints[i];
            if(i+1 != guicePoints.size()) allValues.push_back(curP.getY());

            int diff = curP.getY() - prevP.getY();

            if(diff == 0) continue;

            diff--;

            double num = diff/diffLineFactor ;

            spacing = diff / num;

            if(i == 1 )
            {
                for (double y = curP.getY() - spacing; y - spacing> prevP.getY();  y-= spacing)
                {
                    allValues.push_back(y);
                }
            }
            else
            {
                for (double y = prevP.getY() + spacing; y + spacing< curP.getY();  y+= spacing)
                {
                    allValues.push_back(y);
                }
            }
        }

        // Loop through vertical lines
        for (auto &y : allValues) {
            vector<float> intersections;

            // Find intersection points of the vertical line with each polygon edge
            for (size_t i = 0; i < pnts.size(); i++) {
                Point p1 = pnts[i];
                Point p2 = pnts[(i + 1) % pnts.size()]; // Next vertex (cyclic)

                // Check if the line x intersects the edge
                if (isBetween(y, p1.getY(), p2.getY())) {
                    double x = getIntersectionX(y, p1, p2);
                    if (x != -1) intersections.push_back(x);
                }
            }

            // Sort intersections by y-coordinate
            sort(intersections.begin(), intersections.end());

            // Draw vertical lines between consecutive intersection points
            for (size_t i = 1; i < intersections.size(); i += 1) {
                float xStart = intersections[i-1];
                float xEnd = intersections[i];

                walls.emplace_back(xStart ,300, y , xEnd ,400 , y, "");
                cout << "{ x1:" << xStart << ", y1:" << y << ", x2:" << xEnd << ", y2:" << y << "},\n";
            }
        }
    }


    return walls;
}

bool DesignWallGenerator::isBetween(double value, double bound1, double bound2) {
    return (value >= min(bound1, bound2) && value <= max(bound1, bound2));
}

double DesignWallGenerator::getIntersectionY(double x, const Point &p1, const Point &p2) {
    if (p1.getX() == p2.getX()) return -1; // Parallel to the vertical line, no intersection
    double slope = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
    double yIntercept = p1.getY() - slope * p1.getX();
    return slope * x + yIntercept;
}

double DesignWallGenerator::getIntersectionX(double y, const Point &p1, const Point &p2) {
    if (p1.getY() == p2.getY()) return -1; // Parallel to the vertical line, no intersection
    double slope = (p2.getX() - p1.getX()) / (p2.getY() - p1.getY());
    double xIntercept = p1.getX() - slope * p1.getY();
    return slope * y + xIntercept;
}

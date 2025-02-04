//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#include "DesignCompactnessRefactor.h"

void DesignCompactnessRefactor::refactorDesign(Design &shape) {
    Design mainShape = shape;

    vector<Line> emptyLines = emptyWallsCalculation(shape.getRooms());
    set<Line> uniqueEmpty;
    for (auto &line: emptyLines) {
        uniqueEmpty.insert(line);
    }

    for (int i = 0; i < emptyLines.size(); i++) {
        double x1 = emptyLines[i].getX1(), y1 = emptyLines[i].getY1();
        double x2 = emptyLines[i].getX2(), y2 = emptyLines[i].getY2();
        if (y2 - y1 > 1 || x2 - x1 > 1) continue;
        bool changed = false;

        int mainRoomIndex = -1, secondRoomIndex = -1;
        tie(mainRoomIndex, secondRoomIndex) = getTwoRoomsIndexRelatedToEmptyLine(shape, x1, y1, x2, y2);
        if (y1 == y2) {
            if (~mainRoomIndex && ~secondRoomIndex) {
                Room &mainRoom = shape.getRoomsRef()[mainRoomIndex];
                Room &secondRoom = shape.getRoomsRef()[secondRoomIndex];
                double oppY = mainRoom.getY1() != y1 ? mainRoom.getY1() : mainRoom.getY2();

                if (isEmptyLineFromTwoSide(emptyLines, {x1, oppY, x2, oppY}) &&
                    (x1 == mainRoom.getX1() || x2 == mainRoom.getX2())) {
                    if (validRefactor(shape, mainRoom, true, false, false, false) && x1 == mainRoom.getX1()) {
                        mainRoom.setX1(x2);
                        changed = true;
                    } else if (validRefactor(shape, mainRoom, false, true, false, false) && x2 == mainRoom.getX2()) {
                        mainRoom.setX2(x1);
                        changed = true;
                    }
                } else {
                    if (x1 == secondRoom.getX2()) {
                        if (validRefactor(shape, secondRoom, false, true, false, false)) {
                            secondRoom.setX2(x2);
                            changed = true;
                        }
                    } else if (x2 == secondRoom.getX1()) {
                        if (validRefactor(shape, secondRoom, true, false, false, false)) {
                            secondRoom.setX1(x1);
                            changed = true;
                        }
                    }
                }
            }
        } else {
            if (~mainRoomIndex && ~secondRoomIndex) {
                Room &mainRoom = shape.getRoomsRef()[mainRoomIndex];
                Room &secondRoom = shape.getRoomsRef()[secondRoomIndex];
                double oppX = mainRoom.getX1() != x1 ? mainRoom.getX1() : mainRoom.getX2();

                if (isEmptyLineFromTwoSide(emptyLines, {oppX, y1, oppX, y2}) &&
                    (y1 == mainRoom.getY1() || y2 == mainRoom.getY2())) {
                    if (validRefactor(shape, mainRoom, false, false, true, false) && y1 == mainRoom.getY1()) {
                        mainRoom.setY1(y2);
                        changed = true;
                    } else if (validRefactor(shape, mainRoom, false, false, false, true) && y2 == mainRoom.getY2()) {
                        mainRoom.setY2(y1);
                        changed = true;
                    }
                } else {
                    if (y1 == secondRoom.getY2()) {
                        if (validRefactor(shape, secondRoom, false, false, false, true)) {
                            secondRoom.setY2(y2);
                            changed = true;
                        }

                    } else if (y2 == secondRoom.getY1()) {
                        if (validRefactor(shape, secondRoom, false, false, true, false)) {
                            secondRoom.setY1(y1);
                            changed = true;
                        }

                    }
                }
            }
        }

        if (changed) {
            i = 0;
            emptyLines = emptyWallsCalculation(shape.getRoomsRef());
        }
    }

    bool validateShapeAspectRatio = true;

    for (auto &room: shape.getRooms()) {
        double aspectRatio = max(room.getWidth(), room.getHeight()) / min(room.getWidth(), room.getHeight());

        if (aspectRatio > RoomDefault::getDefaultAspectRatio(room.getRoomType())) {
            validateShapeAspectRatio = false;
            break;
        }
    }
    if (!validateShapeAspectRatio) {
        shape = mainShape;
    }
}

bool DesignCompactnessRefactor::isEmptyLineFromTwoSide(const vector<Line> &lines, const Line &line)
{
    double x1 = line.getX1() , y1 = line.getY1();
    double x2 = line.getX2() , y2 = line.getY2();

    for(auto &l : lines)
    {
        if(l == line) continue;
        if(x1 >= l.getX1() && x2 <= l.getX2() && y1>= l.getY1() && y2<=l.getY2())return true;
    }
    return false;
}

bool DesignCompactnessRefactor::validRefactor(const Design &shape, const Room &room, bool x1Refactor ,bool x2Refactor, bool y1Refactor , bool y2Refactor)
{
    double x1 = room.getX1() , y1 = room.getY1();
    double x2 = room.getX2() , y2 = room.getY2();

    for(auto &r : shape.getRooms())
    {
        if(r.getRoomId() == room.getRoomId()) continue;

        if(x1Refactor && r.getX2() == x1 && ((y1 >= r.getY1() && y1<r.getY2()) || (r.getY1() >= y1 && r.getY1()<y2)))
        {
            return false;
        }

        if(x2Refactor && r.getX1() == x2 && ((y1 >= r.getY1() && y1<r.getY2()) || (r.getY1() >= y1 && r.getY1()<y2)))
        {
            return false;
        }

        if(y1Refactor && r.getY2() == y1 && ((x1 >= r.getX1() && x1<r.getX2()) || (r.getX1() >= x1 && r.getX1()<x2)))
        {
            return false;
        }

        if(y2Refactor && r.getY1() == y2 && ((x1 >= r.getX1() && x1<r.getX2()) || (r.getX1() >= x1 && r.getX1()<x2)))
        {
            return false;
        }
    }
    return true;

}

vector<Line> DesignCompactnessRefactor::emptyWallsCalculation( const vector<Room> &rooms)
{
    map<double , set<double>>X,Y;//all X,Y values in rooms
    for(auto &room : rooms)
    {
        X[room.getX1()].insert(room.getY1()); X[room.getX1()].insert(room.getY2());
        X[room.getX2()].insert(room.getY1()); X[room.getX2()].insert(room.getY2());

        Y[room.getY1()].insert(room.getX1()); Y[room.getY1()].insert(room.getX2());
        Y[room.getY2()].insert(room.getX1()); Y[room.getY2()].insert(room.getX2());
    }

    map<double,vector<double> > XValues , YValues;
    //push values in set inside vector
    for(auto &x : X){
        for(auto &y : x.second)XValues[x.first].push_back(y);
    }
    for(auto &y : Y){
        for(auto &x : y.second)YValues[y.first].push_back(x);
    }

    vector<Line> emptyLines;

    for(auto &curY : YValues)
    {
        double y = curY.first;
        for (int i = 1; i < curY.second.size(); i++)
        {
            double prevX = curY.second[i-1];
            double curX  = curY.second[i];

            int lineSharedInNRooms = 0;

            for(auto &room : rooms)
            {
                double y1 = room.getY1() , y2 = room.getY2();
                double x1 = room.getX1() , x2 = room.getX2();

                if((y == y1 || y == y2) && prevX >= x1 && curX <= x2)
                {
                    lineSharedInNRooms++;
                    string roomId = room.getRoomId();
                    transform(roomId.begin(), roomId.end(), roomId.begin(), ::tolower);
                }
            }

            if(lineSharedInNRooms == 1)
            {
                emptyLines.emplace_back(prevX , y , curX , y);
            }
        }
    }

    for(auto &curX : XValues)
    {
        double x = curX.first;
        for (int i = 1; i < curX.second.size(); i++)
        {
            double prevY = curX.second[i-1];
            double curY  = curX.second[i];

            int lineSharedInNRooms = 0;

            for(auto &room : rooms)
            {
                double y1 = room.getY1() , y2 = room.getY2();
                double x1 = room.getX1() , x2 = room.getX2();

                if((x == x1 || x == x2) && prevY >= y1 && curY <= y2)
                {
                    lineSharedInNRooms++;
                    string roomId = room.getRoomId();
                    transform(roomId.begin(), roomId.end(), roomId.begin(), ::tolower);
                }
            }

            if(lineSharedInNRooms == 1)
            {
                emptyLines.emplace_back(x , prevY , x , curY);
            }
        }
    }

    return emptyLines;
}

pair<int , int> DesignCompactnessRefactor::getTwoRoomsIndexRelatedToEmptyLine(Design &shape,double x1, double y1, double x2, double y2)
{
    int mainRoomIndex = -1 , secondRoomIndex = -1;
    if(y1 == y2)
    {
        for(int j = 0; j < shape.getRooms().size() ; j++)
        {
            Room room = shape.getRooms()[j];
            if((y1 == room.getY1() || y1 == room.getY2()) && x1 >= room.getX1() && x2 <= room.getX2())
            {
                mainRoomIndex = j;
            }
            else if((y1 == room.getY1() || y1 == room.getY2()) && (x1 == room.getX2() || x2 == room.getX1()) )
            {
                secondRoomIndex = j;
            }
        }
    }
    else
    {
        for(int j = 0; j < shape.getRooms().size() ; j++)
        {
            Room room = shape.getRooms()[j];
            if((x1 == room.getX1() || x1 == room.getX2()) && y1 >= room.getY1() && y2 <= room.getY2())
            {
                mainRoomIndex = j;
            }
            else if((x1 == room.getX1() || x1 == room.getX2()) && (y1 == room.getY2() || y2 == room.getY1()) )
            {
                secondRoomIndex = j;
            }
        }
    }
    return {mainRoomIndex , secondRoomIndex};
}

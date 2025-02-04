//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#include "DesignCompactnessSorter.h"

void DesignCompactnessSorter::sortDesigns(vector<Design> &designs)
{
    vector<pair<double,int>>coreSolutionsSort;
    int index = 0;
    for(auto &design : designs)
    {
        // double sharedWalls = Reusable::calculateSharedWalls(design);
        double sharedWalls = emptyWallsCalculation(design.getRooms());
        coreSolutionsSort.emplace_back(sharedWalls,index++);
    }
    sort(coreSolutionsSort.begin(),coreSolutionsSort.end());
    vector<Design>coreSolutionsCopy = designs;
    designs.clear();
    for(auto &conn : coreSolutionsSort)
    {
        Design tst = coreSolutionsCopy[conn.second];
        designs.push_back(tst);
    }
}

double DesignCompactnessSorter::emptyWallsCalculation( const vector<Room> &rooms)
{
    double emptyWallsSpace = 0;
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

    for(auto &curY : YValues)
    {
        double y = curY.first;
        for (int i = 1; i < curY.second.size(); i++)
        {
            double prevX = curY.second[i-1];
            double curX  = curY.second[i];

            int lineSharedInNRooms = 0;
            bool isCorridor = false;

            for(auto &room : rooms)
            {
                double y1 = room.getY1() , y2 = room.getY2();
                double x1 = room.getX1() , x2 = room.getX2();

                if((y == y1 || y == y2) && prevX >= x1 && curX <= x2)
                {
                    lineSharedInNRooms++;
                    string roomId = room.getRoomId();
                    transform(roomId.begin(), roomId.end(), roomId.begin(), ::tolower);
                    if(room.getRoomType() == RoomType::Corridor ) isCorridor = true;
                }
            }

            if(lineSharedInNRooms == 1)
            {
                emptyWallsSpace += curX - prevX;
                if(isCorridor)emptyWallsSpace += (curX - prevX) * 4;
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
            bool isCorridor = false;

            for(auto &room : rooms)
            {
                double y1 = room.getY1() , y2 = room.getY2();
                double x1 = room.getX1() , x2 = room.getX2();

                if((x == x1 || x == x2) && prevY >= y1 && curY <= y2)
                {
                    lineSharedInNRooms++;
                    string roomId = room.getRoomId();
                    transform(roomId.begin(), roomId.end(), roomId.begin(), ::tolower);
                    if(room.getRoomType() == RoomType::Corridor ) isCorridor = true;
                }
            }

            if(lineSharedInNRooms == 1)
            {
                emptyWallsSpace += curY - prevY;
                if(isCorridor)emptyWallsSpace += (curY - prevY)*4;
            }
        }
    }

    return emptyWallsSpace;
}

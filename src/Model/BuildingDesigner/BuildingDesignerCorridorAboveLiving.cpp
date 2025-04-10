//
// Created by ahmed Ibrahim on 10-Apr-25.
//

#include "BuildingDesignerCorridorAboveLiving.h"

BuildingDesignerCorridorAboveLiving::BuildingDesignerCorridorAboveLiving(const vector<string> &zone1Ids,
                                                                           const vector<string> &zone2Ids,
                                                                           const vector<pair<string, string>> &connections,
                                                                           map<string, pair<double, double>> &dimensionsLimits)
        : BuildingDesigner(zone1Ids, zone2Ids, connections, dimensionsLimits) {}

BuildingDesignerCorridorAboveLiving::BuildingDesignerCorridorAboveLiving(const vector<string> &zone1Ids,
                                                                           const vector<string> &zone2Ids,
                                                                           const vector<pair<string, string>> &connections,
                                                                           map<string, double> &roomsAreas)
        : BuildingDesigner(zone1Ids, zone2Ids, connections, roomsAreas) {}

vector<Room> BuildingDesignerCorridorAboveLiving::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
{
    vector<Room> ans;
    double width = mainRoom.getWidth();
    double height = mainRoom.getHeight();

    int n = (int)roomE.size() , index = -1;

    vector<vector<double>> values;
    vector<double> minLimits;
    double minLimitsSum = 0.0;

    for(auto &room : roomE)
    {
        double firstL = room.getDimensionLimit().first;
        double secL = room.getDimensionLimit().second;

        minLimitsSum += firstL;
        minLimits.push_back(firstL);

        double diff = (secL -firstL) * 5;
        double counter = (secL -firstL) / diff;

        vector<double> val;
        for (double i = firstL; i <= secL; i+= counter)
        {
            val.push_back(i);
        }
        values.push_back(val);
    }

    double curTopLimits = 0.0 , diffLimits = 0.0; int topRoomsSize = 0;

    double newCorridorWidth = 0.0;
    double curX = width;
    vector<vector<double>>tempV;

    Room lastTopRoom;

    for (int i = 0; i < n; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width + 1 , tempV , roomE);
        if (res.first >= width)
        {
            index = i + 1;
            vector<double> out = res.second;
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[j].getRoomId();
                double curWidth = out[j];
                double curHeight = roomsArea[id] / curWidth;
                Room newRoom(id , curX - curWidth, -curHeight , curX  , 0);
                lastTopRoom = newRoom;
                curX = newRoom.getX1();
                ans.push_back(newRoom);
            }
            break;
        }
    }

    cout<<"newCorridorWidth = "<<width<<"\n";

    Room corridor("" , 0 , 0 , width , height);
    corridor.addDoor(0 , 0.4 , 0 , 1.6);
    corridor.addDoor(width , 0.4 , width , 1.6);

    ans.push_back(corridor);

    string leftRoomId = roomE[index].getRoomId();

    curX = 0.0;
    tempV.clear();
    vector<RoomEntity> newRoomsE;

    for (int i = index+1; i < n-1; ++i)
    {
        newRoomsE.push_back(roomE[i]);
        tempV.push_back(values[i]);
    }

    int otherSize = n- (index+1);

    cout<<"Other Size = "<<otherSize<<"\n";

    pair<double , vector<double>> res = findClosestSum(width +1.5, tempV , newRoomsE);

    vector<double> out = res.second;

    reverse(out.begin(), out.end());

    index ++;
    Room firstRoomBottom ;
    for (int j = 0; j < out.size(); ++j)
    {
        string id = roomE[index].getRoomId();
        double curWidth = out[j];
        double curHeight = roomsArea[id] / curWidth;
        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
        curX = newRoom.getX2();
        if (j==0) firstRoomBottom = newRoom;
        ans.push_back(newRoom);
        index++;
    }


    double minLimit = dimensionsLimits[leftRoomId].first;
    double maxLimit = dimensionsLimits[leftRoomId].second;

    double preferRoomHeight = firstRoomBottom.getY2() - corridor.getY1();

    double preferRoomWidth = corridor.getX1() - lastTopRoom.getX1();

    if (preferRoomHeight+0.5 >= minLimit && preferRoomHeight<= maxLimit+0.5)
    {
        preferRoomWidth = roomsArea[leftRoomId] / preferRoomHeight;
    }
    else if (preferRoomWidth+0.5 >= minLimit && preferRoomWidth<= maxLimit+0.5)
    {
        preferRoomHeight = roomsArea[leftRoomId] / preferRoomWidth;
    }
    else
    {
        preferRoomHeight = minLimit + (maxLimit+minLimit)/2;
        preferRoomWidth = roomsArea[leftRoomId] / preferRoomHeight;
    }

    Room leftRoom (leftRoomId , corridor.getX1()  - preferRoomWidth, corridor.getY1() , corridor.getX1() , corridor.getY1() + preferRoomHeight);

    ans.push_back(leftRoom);
    string rightRoomID = roomE.back().getRoomId();
    minLimit = dimensionsLimits[rightRoomID].first;
    maxLimit = dimensionsLimits[rightRoomID].second;

    preferRoomHeight = corridor.getY2() - ans[0].getY1();

    if (preferRoomHeight < minLimit) preferRoomHeight = minLimit;
    else if (preferRoomHeight > maxLimit) preferRoomHeight = maxLimit;

    double rightRoomWidth = roomsArea[leftRoomId] / preferRoomHeight;

    Room rightRoom (rightRoomID , corridor.getX2() , corridor.getY2() - preferRoomHeight, corridor.getX2() +rightRoomWidth, corridor.getY2());

    ans.push_back(rightRoom);

    return ans;
}

vector<Room> BuildingDesignerCorridorAboveLiving::generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom)
{
    vector<Room> ans;
    double width = mainRoom.getWidth();
    double height = mainRoom.getHeight();

    int n = (int)roomE.size();
    vector<vector<double>> values , tempV;

    for(auto &room : roomE)
    {
        double firstL = room.getDimensionLimit().first;
        double secL = room.getDimensionLimit().second;
        double diff = (secL -firstL) * 5;
        double counter = (secL -firstL) / diff;

        vector<double> val;
        for (double i = firstL; i <= secL; i+= counter)
        {
            val.push_back(i);
        }
        values.push_back(val);
    }

    int roomSize = 0;

    int index = 0;
    for (int i = 0; i < n; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(height , tempV , roomE);
        if (res.first >= height)
        {
            index = i+1;
            vector<double> out = res.second;
            double curY = mainRoom.getY1();
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[j].getRoomId();
                double curHeight =  out[j];
                double curWidth = roomsArea[id] / curHeight;

                Room newRoom(id , mainRoom.getX2() , curY , mainRoom.getX2() + curWidth , curY + curHeight);
                curY = newRoom.getY2();
                ans.push_back(newRoom);
            }
            break;
        }
    }

    vector<RoomEntity> newRooms;
    for (int i = index; i < n; ++i)
    {
        roomSize++;
        newRooms.push_back(roomE[i]);
    }
    double curX = mainRoom.getX2();

    tempV.clear();

    for (int i = index; i < index + roomSize; ++i)
    {
        string id = roomE[i].getRoomId();
        double curWidth = roomE[i].getDimensionLimit().first;
        double curHeight = roomsArea[id] / curWidth;

        Room newRoom(id , curX - curWidth , mainRoom.getY2() , curX , mainRoom.getY2() + curHeight);
        curX = newRoom.getX1();
        ans.push_back(newRoom);

        if (curX < mainRoom.getX1() + 1.5)break;

        if (i== index)
        {
            int m = ans.size();

            double diff = abs(ans[m-2].getY2() - ans[m-1].getY2());

            //reposition again last Room Bottom and Right Bottom
            if (ans[m-2].getY2() < ans[m-1].getY2() && diff <= 0.5)
            {
                ans[m-2].setY2(ans[m-1].getY2() + diff);
            }
            else if (ans[m-2].getY2() > ans[m-1].getY2()  && diff <= 0.5)
            {
                ans[m-1].setY2(ans[m-1].getY2() + diff);
            }
        }
    }

    return ans;
}

Design BuildingDesignerCorridorAboveLiving::generateDesign()
{

//    cout<<"ZONE1 = "<<"\n";
//    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
//    cout<<"\n";
//
//    cout<<"ZONE2 = "<<"\n";
//    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
//    cout<<"\n";

    zone1 = sortZone(zone1);
    zone2 = sortZone(zone2);

    reverse(zone1.begin(), zone1.end());

    RoomEntity backRoom = zone1.back();

    zone1.pop_back();

    RoomEntity living ("Living" , {backRoom.getRoomId()} , {4.8 , 7.3});
    zone1.push_back(living);
    zone1.push_back(backRoom);

    cout<<"ZONE1 = "<<"\n";
    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    cout<<"ZONE2 = "<<"\n";
    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
    vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);
    Room Living = rooms[rooms.size()-3];

    vector<Room> newRooms = generateLivingLayout(zone2 , Living);
    rooms.insert(rooms.end() , newRooms.begin() , newRooms.end());

    Design design("" , rooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);

    return design;
}


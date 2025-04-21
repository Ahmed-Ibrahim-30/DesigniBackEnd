//
// Created by ahmed Ibrahim on 10-Apr-25.
//

#include "BuildingDesignerCorridorBesideLiving.h"

BuildingDesignerCorridorBesideLiving::BuildingDesignerCorridorBesideLiving(const vector<string> &zone1Ids,
                                                                           const vector<string> &zone2Ids,
                                                                           const vector<pair<string, string>> &connections,
                                                                           map<string, pair<double, double>> &dimensionsLimits)
        : BuildingDesigner(zone1Ids, zone2Ids, connections, dimensionsLimits) {}

BuildingDesignerCorridorBesideLiving::BuildingDesignerCorridorBesideLiving(const vector<string> &zone1Ids,
                                                                           const vector<string> &zone2Ids,
                                                                           const vector<pair<string, string>> &connections,
                                                                           map<string, double> &roomsAreas)
        : BuildingDesigner(zone1Ids, zone2Ids, connections, roomsAreas) {}

vector<Room> BuildingDesignerCorridorBesideLiving::generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom)
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
    for (int i = 0; i < n - 2; ++i)
    {
        double firstL = roomE[i].getDimensionLimit().first;
        double secL = roomE[i].getDimensionLimit().second;

        diffLimits+= (secL - firstL);
        double firstL2 = roomE[i+1].getDimensionLimit().first;
        curTopLimits += firstL;
        double remaining = (minLimitsSum - curTopLimits - firstL2);

        remaining -= roomE.back().getDimensionLimit().first;
        remaining += 1.5;
        topRoomsSize ++;

        if (curTopLimits >= remaining)
        {
            width = curTopLimits + (diffLimits * 0.5);
            break;
        }
    }

    double newCorridorWidth = 0.0;
    double curX = 0.0;
    vector<vector<double>>tempV;

    Room lastTopRoom;

    for (int i = 0; i < topRoomsSize; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width , tempV , roomE);
        if (i == topRoomsSize-1)
        {
            index = i + 1;
            vector<double> out = res.second;
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[j].getRoomId();
                double curWidth = out[j];
                double curHeight = roomsArea[id] / curWidth;
                Room newRoom(id , curX , -curHeight , curX + curWidth , 0);
                lastTopRoom = newRoom;
                curX = newRoom.getX2();
                newCorridorWidth = curX;
                ans.push_back(newRoom);
            }
            break;
        }
    }

    cout<<"newCorridorWidth = "<<newCorridorWidth<<"\n";

    Room corridor("" , 0 , 0 , newCorridorWidth , height);
    corridor.addDoor(0 , 0.4 , 0 , 1.6);
    corridor.addDoor(newCorridorWidth , 0.4 , newCorridorWidth , 1.6);

    ans.push_back(corridor);

    string rightRoomId = roomE[index].getRoomId();

    corridorWidth = newCorridorWidth;
    width = newCorridorWidth;

    curX = 0.0;
    tempV.clear();
    vector<RoomEntity> newRoomsE;

    for (int i = index+1; i < n; ++i)
    {
        newRoomsE.push_back(roomE[i]);
        tempV.push_back(values[i]);
    }

    pair<double , vector<double>> res = findClosestSum(width +1.5, tempV , newRoomsE);
    vector<double> out = res.second;
    std::reverse(out.begin(), out.end());
    index = n-1;
    for (int j = 0; j < out.size(); ++j)
    {
        string id = roomE[index].getRoomId();
        double curWidth = out[j];
        double curHeight = roomsArea[id] / curWidth;

        Room newRoom(id , curX , corridor.getY2() , curX + curWidth , corridor.getY2() + curHeight);
        curX = newRoom.getX2();
        ans.push_back(newRoom);
        index--;
    }


    Room lastRoomBottom = ans.back();


    double minLimit = dimensionsLimits[rightRoomId].first;
    double maxLimit = dimensionsLimits[rightRoomId].second;

    double remainingBottomWidth = lastRoomBottom.getX2() - corridor.getX2();

    double lastTopRoomHeight = lastTopRoom.getHeight();

    double preferHeight = lastTopRoomHeight + height;

    if (preferHeight < minLimit) preferHeight = minLimit;
    else if (preferHeight > maxLimit) preferHeight = maxLimit;

    double rightRoomWidth = roomsArea[rightRoomId] / preferHeight;

    if (preferHeight != lastTopRoomHeight + height && remainingBottomWidth >= minLimit && remainingBottomWidth<= maxLimit)
    {
        rightRoomWidth = remainingBottomWidth;
        preferHeight = roomsArea[rightRoomId] / rightRoomWidth;
    }

    Room rightRoom (roomE[index].getRoomId() , corridor.getX2() , corridor.getY2() - preferHeight , corridor.getX2() + rightRoomWidth , corridor.getY2());

    ans.push_back(rightRoom);

    int m = ans.size();

    double diff = abs(ans[m-2].getX2() - ans[m-1].getX2());

    //reposition again last Room Bottom and Right Bottom
    if (ans[m-2].getX2() > ans[m-1].getX2() && diff <= 0.5)
    {
        ans[m-1].setX2(ans[m-1].getX2() + diff);
    }
    else if (ans[m-1].getX2() > ans[m-2].getX2() && diff <= 0.5)
    {
        ans[m-2].setX2(ans[m-2].getX2() + diff);
    }

    return ans;
}

vector<Room> BuildingDesignerCorridorBesideLiving::generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom , int corridorCount)
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

    int roomSize = 0; double minLimits = 0.0;
    for (int i = 0; i < n; ++i) {
        if (minLimits + 1.5 > width)break;
        minLimits += roomE[i].getDimensionLimit().first;
        roomSize++;
    }

    int index = 0;
    double curX = 0;
    for (int i = 0; i < roomSize; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width , tempV , roomE);
        if (i == roomSize-1)
        {
            index = i+1;
            vector<double> out = res.second;
            curX = mainRoom.getX2();
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[j].getRoomId();
                double curWidth = out[j];
                double curHeight = roomsArea[id] / curWidth;

                Room newRoom(id , curX - curWidth , mainRoom.getY2() , curX , mainRoom.getY2() + curHeight);
                curX = newRoom.getX1();
                ans.push_back(newRoom);
            }
            break;
        }
    }

    roomSize = 0, minLimits = 0.0;

    vector<RoomEntity> newRooms;
    for (int i = index; i < n; ++i) {
        if (minLimits + 1.5 > height)break;
        minLimits += roomE[i].getDimensionLimit().first;
        roomSize++;
        newRooms.push_back(roomE[i]);
    }

    if (corridorCount == 1)
    {
        double curY = mainRoom.getY2();
        tempV.clear();

        for (int i = index; i < n; ++i)
        {
            tempV.push_back(values[i]);
            pair<double , vector<double>> res = findClosestSum(height , tempV , newRooms);
            if (res.first >= height)
            {
                int firstIndex = index;
                index = i+1;
                vector<double> out = res.second;
                curX = mainRoom.getY2();
                for (int j = firstIndex; j < index; ++j)
                {
                    string id = roomE[j].getRoomId();
                    double curHeight = out[j - firstIndex];
                    double curWidth = roomsArea[id] / curHeight;

                    Room newRoom(id , mainRoom.getX1() - curWidth , curY - curHeight , mainRoom.getX1() , curY);
                    curY = newRoom.getY1();
                    ans.push_back(newRoom);

                    if (j==firstIndex)
                    {
                        int m = ans.size();

                        double diff = abs(ans[m-2].getX1() - ans[m-1].getX1());

                        //reposition again last Room Bottom and Right Bottom
                        if (ans[m-2].getX1() < ans[m-1].getX1() && diff <= 0.5)
                        {
                            ans[m-1].setX1(ans[m-1].getX1() - diff);
                        }
                        else if (ans[m-1].getX1() < ans[m-2].getX1() && diff <= 0.5)
                        {
                            ans[m-2].setX1(ans[m-2].getX1() - diff);
                        }
                    }
                }
                break;
            }
        }
    }

    //TOP
    curX = mainRoom.getX1();
    tempV.clear();
    roomE.clear();
    newRooms.clear();



    for (int i = index; i < n; ++i) {
        newRooms.push_back(roomE[i]);
    }


    for (int i = index; i < n; ++i)
    {
        tempV.push_back(values[i]);
        pair<double , vector<double>> res = findClosestSum(width , tempV , newRooms);


        if (i == n-1)
        {
            vector<double> out = res.second;
            curX = mainRoom.getX1();
            for (int j = 0; j < out.size(); ++j)
            {
                string id = roomE[index++].getRoomId();
                double curWidth = out[j];
                double curHeight = roomsArea[id] / curWidth;

                Room newRoom(id , curX , mainRoom.getY1() -curHeight, curX+curWidth, mainRoom.getY1() );
                curX = newRoom.getX2();
                ans.push_back(newRoom);

                if (mainRoom.getX2() - curX  < 1)
                {
                    break;
                }
            }
        }
    }

    ans.push_back(mainRoom);
    return ans;
}

vector<Design> BuildingDesignerCorridorBesideLiving::generateDesign()
{
    vector<Design> designs ;//= generateDiffDesign();
    if (zone1.size() <= 5)
    {
        designs=  generateDiffDesign();
        return designs;

        zone1 = sortZone(zone1);
        zone2 = sortZone(zone2);

        reverse(zone1.begin(), zone1.end());

        cout<<"ZONE1 = "<<"\n";
        for(auto &room : zone1)cout<<room.getRoomId()<<" ";
        cout<<"\n";

        cout<<"ZONE2 = "<<"\n";
        for(auto &room : zone2)cout<<room.getRoomId()<<" ";
        cout<<"\n";

        Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
        vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);

        double livingX = 6 , livingY = 7;
        Room Living ("Living" , Corridor.getX1() - livingX , Corridor.getY1() - ((livingY-corridorHeight)/2), Corridor.getX1() , Corridor.getY2() + ((livingY-corridorHeight)/2)); // 6*7

        vector<Room> newRooms = generateLivingLayout(zone2 , Living , 1);
        vector<Room> copyRooms = rooms;
        copyRooms.insert(copyRooms.end() , newRooms.begin() , newRooms.end());

        Design design("" , copyRooms , 1 , 0 , 28 , 0 ,36);
        design.scaleDesign(105);
        designs.push_back(design);

        /**
         * Second Design
         */
        double YChanged = Corridor.getY1() - Living.getY1() ;
        Design livingCore("Living Core" , newRooms);
        livingCore.shiftDesignY(YChanged);
        copyRooms = rooms;
        vector<Room> livingRooms = livingCore.getRooms();
        copyRooms.insert(copyRooms.end() , livingRooms.begin() , livingRooms.end());

        design = Design ("" , copyRooms , 1 , 0 , 28 , 0 ,36);
        design.scaleDesign(105);
        designs.push_back(design);

        /**
         * Third Design
         */
        YChanged = Corridor.getY2() - Living.getY2() ;
        cout<< "YChanged = "<<YChanged<<"\n";
        livingCore = Design ("Living Core" , newRooms);
        livingCore.shiftDesignY(YChanged);
        copyRooms = rooms;
        livingRooms = livingCore.getRooms();
        copyRooms.insert(copyRooms.end() , livingRooms.begin() , livingRooms.end());

        design = Design ("" , copyRooms , 1 , 0 , 28 , 0 ,36);
        design.scaleDesign(105);
        designs.push_back(design);
    }
    else
    {
        vector<RoomEntity> corridor1Rooms , corridor2Rooms , livingRooms = zone2;
        int zone1Size = (int)zone1.size();
        for (int i = 0; i < zone1Size; ++i)
        {
            if (i<(zone1Size+1)/2)corridor1Rooms.push_back(zone1[i]);
            else corridor2Rooms.push_back(zone1[i]);
        }

        corridor1Rooms = sortZone(corridor1Rooms);
        corridor2Rooms = sortZone(corridor2Rooms);
        livingRooms = sortZone(livingRooms);

        reverse(corridor1Rooms.begin(), corridor1Rooms.end());
        reverse(corridor2Rooms.begin(), corridor2Rooms.end());

        cout<<"ZONE1 = "<<"\n";
        for(auto &room : corridor1Rooms)cout<<room.getRoomId()<<" ";
        cout<<"\n";

        cout<<"ZONE11 = "<<"\n";
        for(auto &room : corridor2Rooms)cout<<room.getRoomId()<<" ";
        cout<<"\n";

        Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
        vector<Room> rooms = generateCorridorLayout(corridor1Rooms , Corridor);

        double livingX = 6 , livingY = 7;
        Room Living ("Living" , Corridor.getX1() - livingX , Corridor.getY1() - ((livingY-corridorHeight)/2), Corridor.getX1() , Corridor.getY2() + ((livingY-corridorHeight)/2)); // 6*7

        vector<Room> newRooms = generateLivingLayout(livingRooms , Living , 2);
        vector<Room> copyRooms = rooms;
        copyRooms.insert(copyRooms.end() , newRooms.begin() , newRooms.end());

        Design design("" , copyRooms , 1 , 0 , 28 , 0 ,36);
        design.rotate(180);
        Living.rotate(180);

        //Second Corridor Design
        Corridor  = Room("" , Living.getX2() , ((Living.getY1() + Living.getY2())/2) - corridorHeight/2 ,Living.getX2() + corridorWidth ,  ((Living.getY1() + Living.getY2())/2) + corridorHeight/2);

//        rooms = generateCorridorLayout(corridor2Rooms , Corridor);
//
//        for(auto &room : rooms)
//        {
//            design.addRoom(room);
//        }

        design.scaleDesign(105);
        designs.push_back(design);

    }


    return designs;
}

vector<Design> BuildingDesignerCorridorBesideLiving::generateDiffDesign() {

    vector<Design> designs;

    zone1 = sortZone(zone1);
    zone2 = sortZone(zone2);

    reverse(zone1.begin(), zone1.end());

    int index1 = -1 , index2 = -1;

    for (int i = 0; i < zone1.size()-1; ++i)
    {
        string curId = zone1[i].getRoomId();
        string nextId = zone1[i+1].getRoomId();
        string prevId = i==0 ? "": zone1[i-1].getRoomId();

        if (i==1 && !connections[curId].count(nextId))
        {
            index1 = 0;
            index2 = 1;
            break;
        }

        if(connections[curId].count(nextId) || connections[curId].count(prevId))continue;

        if (index1 == -1)
        {
            index1 = i;
            continue;
        }

        if (index2 == -1)
        {
            index2 = i;
            continue;
        }
    }

    if (~index1 && ~index2) swap(zone1[index1] , zone1[index2]);


    index1 = -1 , index2 = -1;

    for (int i = 1; i < zone2.size(); ++i)
    {
        string curId = zone2[i].getRoomId();
        string nextId = i== zone2.size()-1 ? "": zone2[i+1].getRoomId();
        string prevId = zone2[i-1].getRoomId();

        if (i==zone2.size()-2  && !connections[curId].count(prevId))
        {
            index1 = zone2.size()-2;
            index2 = zone2.size()-1;
            break;
        }

        if(connections[curId].count(nextId) || connections[curId].count(prevId))continue;

        if (index1 == -1)
        {
            index1 = i;
            continue;
        }

        if (index2 == -1)
        {
            index2 = i;
            continue;
        }
    }

    if (index1 != -1 && index2 != -1) swap(zone2[index1] , zone2[index2]);

    cout<<"ZONE1 = "<<"\n";
    for(auto &room : zone1)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    cout<<"ZONE2 = "<<"\n";
    for(auto &room : zone2)cout<<room.getRoomId()<<" ";
    cout<<"\n";

    Room Corridor ("" , 0 , 0 ,corridorWidth ,  corridorHeight);
    vector<Room> rooms = generateCorridorLayout(zone1 , Corridor);

    double livingX = 6 , livingY = 7;
    Room Living ("Living" , Corridor.getX1() - livingX , Corridor.getY1() - ((livingY-corridorHeight)/2), Corridor.getX1() , Corridor.getY2() + ((livingY-corridorHeight)/2)); // 6*7

    vector<Room> newRooms = generateLivingLayout(zone2 , Living , 1);
    vector<Room> copyRooms = rooms;
    copyRooms.insert(copyRooms.end() , newRooms.begin() , newRooms.end());

    Design design("" , copyRooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);
    designs.push_back(design);

    /**
     * Second Design
     */
    double YChanged = Corridor.getY1() - Living.getY1() ;
    Design livingCore("Living Core" , newRooms);
    livingCore.shiftDesignY(YChanged);
    copyRooms = rooms;
    vector<Room> livingRooms = livingCore.getRooms();
    copyRooms.insert(copyRooms.end() , livingRooms.begin() , livingRooms.end());

    design = Design ("" , copyRooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);
    designs.push_back(design);

    /**
     * Third Design
     */
    YChanged = Corridor.getY2() - Living.getY2() ;
    cout<< "YChanged = "<<YChanged<<"\n";
    livingCore = Design ("Living Core" , newRooms);
    livingCore.shiftDesignY(YChanged);
    copyRooms = rooms;
    livingRooms = livingCore.getRooms();
    copyRooms.insert(copyRooms.end() , livingRooms.begin() , livingRooms.end());

    design = Design ("" , copyRooms , 1 , 0 , 28 , 0 ,36);
    design.scaleDesign(105);
    designs.push_back(design);

    return designs;
}


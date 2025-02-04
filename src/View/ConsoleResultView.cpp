//
// Created by ahmed Ibrahim on 28-Nov-24.
//

#include "ConsoleResultView.h"

void ConsoleResultView::displayDesigns(const vector<Design> &designs)
{

    for(auto &design : designs)
    {
        cout<<"*************** Design Details ************* \n";
        cout<<"Number Of Spaces = "<<design.getSpacesCount() <<"\n";
        cout<<"Number Of Rooms = "<<design.getRoomsCount() <<"\n";
        cout<<"Number Of Bedrooms = "<<design.getBedroomsCount() <<"\n";
        cout<<"Design Area = "<<design.getArea() <<"\n";

        cout<<"Room Details\n";
        for(auto &room : design.getRooms())
        {
            room.printDetails();
        }

        cout<<"*************** ******** ************* \n";
    }

}

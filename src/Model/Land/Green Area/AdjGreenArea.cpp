//
// Created by ahmed Ibrahim on 29-Apr-25.
//

#include "AdjGreenArea.h"

void
AdjGreenArea::select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas)
{
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
    cout<<"GREEN AREA = "<<greenAreas<<"\n";


}

//
// Created by ahmed Ibrahim on 16-Dec-24.
//

#include "Cluster.h"

Cluster::Cluster(const Design &mainDesign, int _clusterCount) : mainDesign(mainDesign), clusterCount(_clusterCount) {
    buildCluster();
}

void Cluster::buildCluster() {
    finalCluster.push_back(mainDesign);

    if(clusterCount==1)return;

    // mainDesign.shiftDesignX(mainDesign.getMainDesign().getX1());
    // mainDesign.shiftDesignY(mainDesign.getMainDesign().getY1());

    Design main = mainDesign;
    Design main180 = mainDesign; main180.rotate(180);
    Design main180MirrorX = mainDesign; main180MirrorX.rotate(180); main180MirrorX.mirrorRecX();
    Design main180MirrorX180 = mainDesign; main180MirrorX.rotate(180);

    int row = clusterCount>8 ? 8 : clusterCount >=4 ? 4 : 2;

    int col = clusterCount/row;

    double width = main.getMainDesign().getWidth();
    double height = main.getMainDesign().getHeight();

    int shiftX = 4.0 , shiftY = -20 , lastX2 = main.getMainDesign().getX1();
    finalCluster.clear();

    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < row; ++j) {
            Design newDesign;

            if((j%4==0 && i%2==0) || (j%4==2 && i%2==1))
            {
                newDesign = main;
                double newShiftX = lastX2 - newDesign.getMainDesign().getX1() + shiftX;
                newDesign.shiftDesignX(newShiftX);
            }
            else if((j%4==1 && i%2==0) || (j%4==3 && i%2==1))
            {
                newDesign = main180;
                double newShiftX = lastX2 - newDesign.getMainDesign().getX1() + shiftX;
                newDesign.shiftDesignX(newShiftX);
                newDesign.shiftDesignY(shiftY);
            }
            else if((j%4==2 && i%2==0) || (j%4==0 && i%2==1))
            {
                newDesign = main180MirrorX;
                double newShiftX = lastX2 - newDesign.getMainDesign().getX1() + shiftX;
                newDesign.shiftDesignX(newShiftX);
                newDesign.shiftDesignY(shiftY);
            }
            else if((j%4==3 && i%2==0) || (j%4==1 && i%2==1))
            {
                newDesign = main180MirrorX180;
                double newShiftX = lastX2 - newDesign.getMainDesign().getX1() + shiftX;
                newDesign.shiftDesignX(newShiftX);
            }
            lastX2 = newDesign.getMainDesign().getX2();
            finalCluster.push_back(newDesign);
        }
        // shiftX= 0.0;
        lastX2 = main.getMainDesign().getX1();
        main.shiftDesignY(height*2 + 20);
        main180.shiftDesignY(height*2 + 20);
        main180MirrorX.shiftDesignY(height*2 + 20);
        main180MirrorX180.shiftDesignY(height*2 + 20);
    }

}

const vector<Design> &Cluster::getFinalCluster() const {
    return finalCluster;
}

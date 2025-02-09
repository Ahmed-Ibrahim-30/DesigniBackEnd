//
// Created by ahmed Ibrahim on 09-Feb-25.
//

#ifndef DESIGNI_BACKEND_CENTRALLANDGREENSELECTOR_H
#define DESIGNI_BACKEND_CENTRALLANDGREENSELECTOR_H
#include "GreenAreaSelector.h"
/*
 * CentroidBasedGreenAreaSelector selects green areas by drawing lines between land centroids
 * and choosing the central land along each line as the designated green area.
 */
class CentralLandGreenSelector : public GreenAreaSelector{
public:
    void select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas) override;
};


#endif //DESIGNI_BACKEND_CENTRALLANDGREENSELECTOR_H

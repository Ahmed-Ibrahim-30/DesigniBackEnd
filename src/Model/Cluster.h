//
// Created by ahmed Ibrahim on 16-Dec-24.
//

#ifndef DESIGNI_CLUSTER_H
#define DESIGNI_CLUSTER_H
#include "Design.h"

class Cluster {
private:
    Design mainDesign;
    int clusterCount = 1;
    vector<Design> finalCluster;
    void buildCluster();
public:
    Cluster(const Design &mainDesign, int clusterCount);

    const vector<Design> &getFinalCluster() const;
};


#endif //DESIGNI_CLUSTER_H

//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_DESIGNRESULTVIEW_H
#define DESIGNI_DESIGNRESULTVIEW_H

#include "src/Model/Design.h"

class DesignResultView {
public:
    virtual void displayDesigns(const vector<Design> &designs) = 0;
};


#endif //DESIGNI_DESIGNRESULTVIEW_H

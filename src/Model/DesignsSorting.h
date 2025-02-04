//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#ifndef DESIGNI_DESIGNSSORTING_H
#define DESIGNI_DESIGNSSORTING_H

#include "Design.h"


class DesignsSorting{
public:
    /**
     * Sort Designs
     * @param design
     */
    virtual void sortDesigns(vector<Design> &design) = 0;
};
#endif //DESIGNI_DESIGNSSORTING_H

//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#ifndef DESIGNI_DESIGNREFACTOR_H
#define DESIGNI_DESIGNREFACTOR_H
#include "Design.h"
/**
 * Interface for Refactoring Design
 */
class DesignRefactor{
public:
    /**
     * Modify Design
     * @param shape
     */
    virtual void refactorDesign(Design &shape) = 0;
};
#endif //DESIGNI_DESIGNREFACTOR_H

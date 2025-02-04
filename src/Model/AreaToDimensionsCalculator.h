//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_AREATODIMENSIONSCALCULATOR_H
#define DESIGNI_AREATODIMENSIONSCALCULATOR_H
#include "RoomTypes.h"
#include "RoomDefaults.h"
#include "src/Utils/RoomTypeUtils.h"
#include <cmath>
// Responsible for convert Area To Width and Height
class AreaToDimensionsCalculator {
public:
    /*
     * Make Width and height closer to each other
     */
    static pair<double , double> setWidthHeightNarrowing(double area );

    /**
     * Make Height Max as possible
     * @param area
     * @param roomType
     * @return
     */
    static pair<double , double> setMaxHeightMinWidthByAspectRatio(double area , RoomType roomType);

    /**
     * Make Width Max as possible
     * @param area
     * @param roomType
     * @return
     */
    static pair<double , double> setMINHeightMAXWidthByAspectRatio(double area , RoomType roomType);

    /**
     * @brief getMaxValueFromArea
     * @param area
     * @return
     */
    static double getMaxValueFromArea(double area , RoomType roomType);

    /**
     * @brief getMinValueFromArea
     * @param area
     * @return
     */
    static double getMinValueFromArea(double area , RoomType roomType);

    /**
     * @brief getMaxValueFromArea
     * @param area
     * @return
     */
    static double getMaxValueFromArea(double area , const string& roomId);

    /**
     * @brief getMinValueFromArea
     * @param area
     * @return
     */
    static double getMinValueFromArea(double area , const string& roomId);
};


#endif //DESIGNI_AREATODIMENSIONSCALCULATOR_H

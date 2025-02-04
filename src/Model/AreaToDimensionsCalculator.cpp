//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "AreaToDimensionsCalculator.h"

pair<double, double> AreaToDimensionsCalculator::setWidthHeightNarrowing(double area)
{
    double width = sqrt(area);
    double height = area / width;

    return {width , height};
}

pair<double, double> AreaToDimensionsCalculator::setMaxHeightMinWidthByAspectRatio(double area, RoomType roomType) {
    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double width = sqrt(area / aspectRatio);
    double height = width * aspectRatio;
    return {width , height};
}

pair<double, double> AreaToDimensionsCalculator::setMINHeightMAXWidthByAspectRatio(double area, RoomType roomType) {
    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double height = sqrt(area / aspectRatio);
    double width = height * aspectRatio;
    return {width , height};
}

double AreaToDimensionsCalculator::getMaxValueFromArea(double area, RoomType roomType) {
    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double maxValue = round ((sqrt(area / aspectRatio) * aspectRatio) * 10) / 10;

    return maxValue;
}

double AreaToDimensionsCalculator::getMinValueFromArea(double area, RoomType roomType) {
    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double maxValue = round ((sqrt(area / aspectRatio) * aspectRatio) * 10) / 10;
    double minValue = round( (area / maxValue) * 10) / 10;

    return minValue;
}


double AreaToDimensionsCalculator::getMaxValueFromArea(double area, const string& roomId) {
    RoomType roomType = RoomTypeUtils::parseStringToRoomType(roomId);

    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double maxValue = round ((sqrt(area / aspectRatio) * aspectRatio) * 10) / 10;

    return maxValue;
}

double AreaToDimensionsCalculator::getMinValueFromArea(double area, const string& roomId) {
    RoomType roomType = RoomTypeUtils::parseStringToRoomType(roomId);
    double aspectRatio = RoomDefault::getDefaultAspectRatio(roomType);

    double maxValue = round ((sqrt(area / aspectRatio) * aspectRatio) * 10) / 10;
    double minValue = round( (area / maxValue) * 10) / 10;

    return minValue;
}

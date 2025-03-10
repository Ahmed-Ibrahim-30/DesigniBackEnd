//
// Created by ahmed Ibrahim on 10-Mar-25.
//

#include "CityGrid.h"

CityGrid::CityGrid(const vector<Line> &streets, const vector<Line> &roadExtension, const vector<Line> &homeBorder)
        : streets(streets), roadExtension(roadExtension), homeBorder(homeBorder) {}

const vector<Line> &CityGrid::getStreets() const {
    return streets;
}

void CityGrid::setStreets(const vector<Line> &streets) {
    CityGrid::streets = streets;
}

const vector<Line> &CityGrid::getRoadExtension() const {
    return roadExtension;
}

void CityGrid::setRoadExtension(const vector<Line> &roadExtension) {
    CityGrid::roadExtension = roadExtension;
}

const vector<Line> &CityGrid::getHomeBorder() const {
    return homeBorder;
}

void CityGrid::setHomeBorder(const vector<Line> &homeBorder) {
    CityGrid::homeBorder = homeBorder;
}

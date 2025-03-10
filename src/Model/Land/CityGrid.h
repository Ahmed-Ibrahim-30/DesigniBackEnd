//
// Created by ahmed Ibrahim on 10-Mar-25.
//

#ifndef DESIGNI_BACKEND_CITYGRID_H
#define DESIGNI_BACKEND_CITYGRID_H
#include "src/Model/Line.h"

class CityGrid {
private:
    vector<Line> streets;
    vector<Line> roadExtension;
    vector<Line> homeBorder;

public:
    CityGrid(const vector<Line> &streets, const vector<Line> &roadExtension, const vector<Line> &homeBorder);
    CityGrid() = default;

    [[nodiscard]] const vector<Line> &getStreets() const;

    void setStreets(const vector<Line> &streets);

    [[nodiscard]] const vector<Line> &getRoadExtension() const;

    void setRoadExtension(const vector<Line> &roadExtension);

    [[nodiscard]] const vector<Line> &getHomeBorder() const;

    void setHomeBorder(const vector<Line> &homeBorder);

};


#endif //DESIGNI_BACKEND_CITYGRID_H

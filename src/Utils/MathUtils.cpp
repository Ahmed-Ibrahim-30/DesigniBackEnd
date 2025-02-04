//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "MathUtils.h"
#include "iostream"

double MathUtils::roundingToDecimal(double number, int decimal) {
    double factor = decimal == 1 ? 10 : std::pow(10 , decimal);
    double ans = (std::round(number*factor) / factor) * 1.0;
    return ans;
}

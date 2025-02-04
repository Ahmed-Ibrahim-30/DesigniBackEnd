//
// Created by ahmed Ibrahim on 28-Nov-24.
//

#ifndef DESIGNI_CONSOLERESULTVIEW_H
#define DESIGNI_CONSOLERESULTVIEW_H
#include "DesignResultView.h"

class ConsoleResultView : public DesignResultView{
public:
    void displayDesigns(const vector<Design> &designs) override;
};


#endif //DESIGNI_CONSOLERESULTVIEW_H

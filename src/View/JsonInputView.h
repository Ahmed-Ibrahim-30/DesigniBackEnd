//
// Created by ahmed Ibrahim on 27-Nov-24.
//

#ifndef DESIGNI_JSONINPUTVIEW_H
#define DESIGNI_JSONINPUTVIEW_H
#include "UserInputView.h"
#include "crow.h"
#include "nlohmann/json.hpp"
using namespace crow;

class JsonInputView : public UserInputView{
public:
    SimpleApp app;
    explicit JsonInputView(SimpleApp &app1);
    Design getUserInput() override;
};


#endif //DESIGNI_JSONINPUTVIEW_H

//
// Created by ahmed Ibrahim on 27-Nov-24.
//

#ifndef DESIGNI_CONSTINPUTVIEW_H
#define DESIGNI_CONSTINPUTVIEW_H
#include "UserInputView.h"

class ConstInputView : public UserInputView{
public:
    Design getUserInput() override;
};


#endif //DESIGNI_CONSTINPUTVIEW_H

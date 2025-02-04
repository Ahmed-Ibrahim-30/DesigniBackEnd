//
// Created by ahmed Ibrahim on 27-Nov-24.
//

#include "JsonInputView.h"

Design JsonInputView::getUserInput() {
    return {};
}

JsonInputView::JsonInputView(SimpleApp &app1) :app(app1){
    app.port(8080).multithreaded().run();
}

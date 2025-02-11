#include "src/Controller/APIController.h"

int main() {

    SimpleApp app;

    APIController apiController(app);

    app.port(8080).multithreaded().run();
    return 0;
}

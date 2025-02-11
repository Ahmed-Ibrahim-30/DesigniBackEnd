//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_HOMEDESIGNCONTROLLER_H
#define DESIGNI_HOMEDESIGNCONTROLLER_H
//////////////////////////////////////////

#include "src/Model/TemplateRepository.h"
#include "src/View/ConstInputView.h"
#include "src/View/JsonInputView.h"
#include "src/View/DesignResultView.h"
#include "src/View/ConsoleResultView.h"
#include "src/Model/DesignGenerator.h"
#include "src/Model/DesignRepository.h"
#include "src/Model/DesignToDoublyLines.h"
#include "src/Model/Cluster.h"


class HomeDesignController {
private:

    /**
     * User Input View
     */
    UserInputView * userInputView;

    /**
     * pointer to out all designs
     */
    DesignResultView* designResultView;

    /**
     * Object from designGenerator
     */
    DesignGenerator designGenerator;

    /**
     * Input user Design
     */
    Design design;

    /**
     * all Output designs
     */
    vector<Design> designs;

    /**
     * 18 templates designs
     */
    DesignRepository templatesDesigns;

    /**
     * Store Model 2 Templates Designs Data
     * @return vector<Design> contains data for each template
     */
    vector<Design> storeTemplateDesigns();

public:
    explicit HomeDesignController();

    /**
     * run Design Process
     */
    void designProcess();

    /**
     * take User Input
     */
    void initiateDesign();

    /**
     * showDesignResult
     */
    void showDesignResult();

    /**
     *
     * @return
     */
    DesignRepository extractRealTemplateDesign();
};


#endif //DESIGNI_HOMEDESIGNCONTROLLER_H

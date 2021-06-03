//
// Created by Magnus Nordlander on 2021-02-06.
//

#ifndef PANORAMA_NAVIGATIONCONTROLLER_H
#define PANORAMA_NAVIGATIONCONTROLLER_H

#include <stack>

class BaseViewController;

class NavigationController {
public:
    explicit NavigationController(BaseViewController* rootViewController);

    void push(BaseViewController*);
    void pop();
    BaseViewController* top();

private:
    std::stack<BaseViewController *> viewControllerStack;
};

#include "ViewControllers/BaseViewController.h"

#endif //PANORAMA_NAVIGATIONCONTROLLER_H

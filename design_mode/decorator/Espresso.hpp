//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_ESPRESSO_HPP
#define STUDY_CPP_ESPRESSO_HPP

#include "Beverage.hpp"

class Espresso : public Beverage {
public:
    Espresso() {
        _description = "Espresso";
    }

    double cost() override {
        return 1.99;
    }
};


#endif //STUDY_CPP_ESPRESSO_HPP

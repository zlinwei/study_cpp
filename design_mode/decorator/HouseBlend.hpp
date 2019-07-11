//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_HOUSEBLEND_HPP
#define STUDY_CPP_HOUSEBLEND_HPP

#include "Beverage.hpp"

class HouseBlend : public Beverage {
public:
    HouseBlend() {
        _description = "House Blend Coffee";
    }

    double cost() override {
        return .89;
    }
};


#endif //STUDY_CPP_HOUSEBLEND_HPP

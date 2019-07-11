//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_CONDIMENTDECORATOR_HPP
#define STUDY_CPP_CONDIMENTDECORATOR_HPP

#include "Beverage.hpp"

class CondimentDecorator : public Beverage {
public:
    void setSize(CAPACITY size) override = 0;

    virtual CAPACITY getSize() = 0;
};


#endif //STUDY_CPP_CONDIMENTDECORATOR_HPP

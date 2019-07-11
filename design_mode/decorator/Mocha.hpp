#include <utility>

//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_MOCHA_HPP
#define STUDY_CPP_MOCHA_HPP

#include "CondimentDecorator.hpp"

class Mocha : public CondimentDecorator {
public:
    explicit Mocha(Beverage::uptr &&beverage) : _beverage(std::move(beverage)) {

    }

    std::string getDestription() override {
        return _beverage->getDestription() + " ,Mocha";
    }

    double cost() override {
        return .20 + _beverage->cost();
    }

private:
    Beverage::uptr _beverage;
};


#endif //STUDY_CPP_MOCHA_HPP

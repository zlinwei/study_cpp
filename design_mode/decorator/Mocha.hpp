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

    void setSize(CAPACITY size) override {
        _beverage->setSize(size);
    }

    CAPACITY getSize() override {
        return _beverage->getSize();
    }

    std::string getDescription() override {
        return _beverage->getDescription() + " ,Mocha";
    }

    double cost() override {
        double cost = _beverage->cost();
        switch (_beverage->getSize()) {
            case TALL:
                cost += 0.10;
                break;
            case GRANDE:
                cost += 0.15;
                break;
            case VENTI:
                cost += 0.20;
                break;
        }
        return cost;
    }

private:
    Beverage::uptr _beverage;
};


#endif //STUDY_CPP_MOCHA_HPP

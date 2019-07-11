//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_WHIP_HPP
#define STUDY_CPP_WHIP_HPP

#include "CondimentDecorator.hpp"

class Whip : public CondimentDecorator {
public:
    explicit Whip(Beverage::uptr &&beverage) : _beverage(std::move(beverage)) {

    }

    void setSize(CAPACITY size) override {
        _beverage->setSize(size);
    }

    CAPACITY getSize() override {
        return _beverage->getSize();
    }

    std::string getDescription() override {
        return _beverage->getDescription() + " ,Whip";
    }

    double cost() override {
        return 0.13 + _beverage->cost();
    }

private:
    Beverage::uptr _beverage;

};


#endif //STUDY_CPP_WHIP_HPP

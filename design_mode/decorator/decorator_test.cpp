//
// Created by linwei on 2019-07-11.
//
#include <glog/logging.h>

#include "Beverage.hpp"
#include "CondimentDecorator.hpp"
#include "Espresso.hpp"
#include "Mocha.hpp"
#include "HouseBlend.hpp"
#include "Whip.hpp"


int main(int, char *[]) {
    Beverage::uptr beverage = Beverage::uptr(new Espresso());
    LOG(INFO) << beverage->getDescription() << " $" << beverage->cost();


    Beverage::uptr mocha = Beverage::uptr(new Mocha(std::move(beverage)));
    LOG(INFO) << mocha->getDescription() << " $" << mocha->cost();

    Beverage::uptr whip = Beverage::uptr(new Whip(std::move(mocha)));
    LOG(INFO) << whip->getDescription() << " $" << whip->cost();
    return 0;
}
//
// Created by linwei on 2019-07-11.
//

#ifndef STUDY_CPP_BEVERAGE_HPP
#define STUDY_CPP_BEVERAGE_HPP

#include <memory>
#include <string>

class Beverage {
public:
    typedef std::unique_ptr<Beverage> uptr;

    virtual std::string getDescription() {
        return _description;
    }

    virtual double cost() = 0;

    virtual ~Beverage() = default;

protected:
    std::string _description = "unknown beverage";
};


#endif //STUDY_CPP_BEVERAGE_HPP

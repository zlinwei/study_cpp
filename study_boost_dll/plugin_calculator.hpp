//
// Created by Administrator on 2019/10/9.
//

#ifndef STUDY_CPP_PLUGIN_CALCULATOR_HPP
#define STUDY_CPP_PLUGIN_CALCULATOR_HPP

#include <memory>
#include <boost/dll/alias.hpp>

class BOOST_SYMBOL_VISIBLE plugin_calculator {
public:
    typedef std::shared_ptr<plugin_calculator> sptr;

    virtual const std::string name() = 0;

    virtual double sum(double a, double b) = 0;

    /**
     * c = a * b + c
     * @param a
     * @param b
     * @param c
     * @param len
     */
    virtual void mul(double *a, double *b, double *c, size_t len) = 0;

    virtual ~plugin_calculator() = default;

    static sptr make();
};


#endif //STUDY_CPP_PLUGIN_CALCULATOR_HPP

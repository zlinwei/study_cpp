//
// Created by Administrator on 2019/10/9.
//

#ifndef STUDY_CPP_MY_PLUGIN_API_HPP
#define STUDY_CPP_MY_PLUGIN_API_HPP

#include <boost/config.hpp>
#include <string>

class BOOST_SYMBOL_VISIBLE my_plugin_api {
public:
    virtual std::string name() const = 0;
    virtual float calculate(float x, float y) = 0;
    virtual ~my_plugin_api() = default;
};


#endif //STUDY_CPP_MY_PLUGIN_API_HPP

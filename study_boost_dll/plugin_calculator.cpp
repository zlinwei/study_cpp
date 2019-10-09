//
// Created by zlinwei on 2019/10/9.
//

#include "plugin_calculator.hpp"

class plugin_calculator_impl : public plugin_calculator {
public:
    double sum(double a, double b) override {
        return a + b;
    }

    void mul(double *a, double *b, double *c, size_t len) override {
        for (size_t i = 0; i < len; ++i) {
            c[i] = c[i] + a[i] * b[i];
        }
    }

    ~plugin_calculator_impl() override = default;

    const std::string name() override;
};

plugin_calculator::sptr plugin_calculator::make() {
    return plugin_calculator::sptr(new plugin_calculator_impl());
}


BOOST_DLL_ALIAS(
        plugin_calculator::make, // <-- this function is exported with...
        plugin_calculator_creator                               // <-- ...this alias name
)

const std::string plugin_calculator_impl::name() {
    return "plugin_calculator";
}

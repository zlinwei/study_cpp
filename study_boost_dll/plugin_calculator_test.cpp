//
// Created by zlinwei on 2019/10/9.
// export factory function instead of instance;
//

#include <boost/dll/import.hpp> // for import_alias
#include <boost/function.hpp>
#include <iostream>
#include "plugin_calculator.hpp"

namespace dll = boost::dll;

int main(int argc, char *argv[]) {
    boost::dll::fs::path shared_library_path(
            argv[1]);                  // argv[1] contains path to directory with our plugin library
    shared_library_path /= "plugin_calculator";
    typedef plugin_calculator::sptr (plugin_calculator_create_t)();
    boost::function<plugin_calculator_create_t> creator;

    creator = boost::dll::import_alias<plugin_calculator_create_t>(             // type of imported symbol must be explicitly specified
            shared_library_path,                                            // path to library
            "plugin_calculator_creator",                                                // symbol to import
            dll::load_mode::append_decorations                              // do append extensions and prefixes
    );

    plugin_calculator::sptr plugin = creator();
    std::cout << "plugin->calculate(1.5, 1.5) call:  " << plugin->sum(1.5, 1.5) << std::endl;
    std::cout << "plugin->calculate(1.5, 1.5) second call:  " << plugin->sum(13.5, 31.5) << std::endl;
    std::cout << "Plugin Name:  " << plugin->name() << std::endl;
}

//
// Created by Administrator on 2019/10/9.
//
#include <boost/dll/import.hpp> // for import_alias
#include <boost/filesystem.hpp>
#include <iostream>
#include "my_plugin_api.hpp"

namespace dll = boost::dll;

int main(int argc, char *argv[]) {

    boost::filesystem::path lib_path(
            argv[1]);             // argv[1] contains path to directory with our plugin library
    boost::shared_ptr<my_plugin_api> plugin;            // variable to hold a pointer to plugin variable
    std::cout << "Loading the plugin" << std::endl;

    plugin = dll::import<my_plugin_api>(          // type of imported symbol is located between `<` and `>`
            lib_path / "my_plugin_api",                     // path to the library and library name
            "plugin",                                       // name of the symbol to import
            dll::load_mode::append_decorations              // makes `libmy_plugin_sum.so` or `my_plugin_sum.dll` from `my_plugin_sum`
    );

    std::cout << "plugin->calculate(1.5, 1.5) call:  " << plugin->calculate(1.5, 1.5) << std::endl;
}

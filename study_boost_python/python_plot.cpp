//
// Created by Administrator on 2019/7/11.
//

#include <boost/python.hpp>
#include <glog/logging.h>

namespace py = boost::python;

wchar_t *GetWC(const char *c) {
    const size_t cSize = strlen(c) + 1;
    wchar_t *wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);

    return wc;
}

int main() {
    try {
        Py_Initialize();
        py::object main_module = py::import("__main__");
        py::object main_namespace = main_module.attr("__dict__");

        py::object res = py::exec("print(\"hello world\")", main_namespace);
        py::object rand_mod = boost::python::import("random");
        py::object rand_func = rand_mod.attr("random");
        py::object rand2 = rand_func();
        LOG(INFO) << py::extract<float>(rand2);

        py::object np = py::import("numpy");

        LOG(INFO) << "hello";
        Py_Finalize();
    } catch (std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...){
        LOG(ERROR) << "Uncaught exception";
    }

    return 0;
}
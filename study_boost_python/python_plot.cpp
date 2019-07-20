//
// Created by Administrator on 2019/7/11.
//

//#include <Python.h>

#include <glog/logging.h>

int main() {
	try {
//		Py_Initialize();
//
//		PyRun_SimpleString("import matplotlib.pyplot as plt");
//		PyRun_SimpleString("import numpy as np");
//        PyRun_SimpleString("plt.figure()");
//        PyRun_SimpleString("plt.plot([1,2,2,2]");
//		PyRun_SimpleString("plt.show()");
//		Py_Finalize();

		LOG(INFO) << "hello";
	}
	catch (std::exception& e) {
		LOG(ERROR) << e.what();
	}
	catch (...) {
		LOG(ERROR) << "Uncaught exception";
	}

	return 0;
}
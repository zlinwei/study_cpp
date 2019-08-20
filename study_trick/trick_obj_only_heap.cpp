//
// Created by linwei on 2019-08-20.
//
#include <iostream>
#include <memory>

class Test {
protected:
    Test() {
        std::cout << "Test()" << std::endl;
    }

public:
    ~Test() {
        std::cout << "~Test()" << std::endl;
    }

public:
    static Test *create() {
        return new Test();
    }
};

int main(int, char *[]) {
    auto t = Test::create();
    auto t1 = std::shared_ptr<Test>(t);
    return 0;
}
//
// Created by linwei on 5/9/20.
//

#include <iostream>

using namespace std;

class A {
public:
    virtual void test() {
        cout << "A" << endl;
    }

    void call() {
        this->test();
    }

    virtual ~A() {
        test();
    }
};

class B : public A {
public:
    virtual void test() {
        cout << "B" << endl;
    }
};

int main(int, char *[]) {
    A *a = new A();
    A *b = new B();

    a->call();
    b->call();

    delete a;
    delete b;
    return 0;
}
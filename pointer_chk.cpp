#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/console_color.h"

using namespace std;
struct A {
    int a;
    A();
};

A::A() : a(1) {}

class B {
   public:
    B(A* a);
    A *aa;
};

class C {
   public:
    C(A* a);
    ~C();
    B *bb;
};

//B::B(A a) { aa = &a; }
B::B(A *a) { aa = a; }
C::C(A *a) { bb = new B(a); }
C::~C(){delete[] bb;}


int main() {
    A a;
    C c(&a);

    cout << "A:" << a.a << endl;

    B b(&a);
    cout << "B a: " << b.aa->a << endl;
    cout << "C a: " << c.bb->aa->a << endl;
    a.a = 5;
    cout << "B a: " << b.aa->a << endl;
    cout << "C a: " << c.bb->aa->a << endl;
    return 0;
}

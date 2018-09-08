#include <iostream>
#include <memory>
#include <map>

struct A {
    int a;
    A(int aa) : a(aa) {}
    A() = default;
    ~A() {
        std::cout << "destructing A: " << a << std::endl;
    }
};

int main() {

    std::map<int, A> m;
    m[1] = A(1);
    m[1] = A(2);
    std::cout << "----------------------" << std::endl;
}

#include <iostream>
#include <string>
#include <thread>
#include <functional>

#include "thread.h"

class A {
public:
    void operator()(const std::string &name) {
        std::cout << "operator()(" << name << ")\n";
    }
    void run(const std::string &name) {
        std::cout << R"("run ")" << name << "\n";
    }
};

int main(int argc, char **argv) {

    // yf::MutexLock mutex;
    // yf::Thread::ptr t1(new yf::Thread([](const std::string &name) mutable {
    //     std::cout << "thraedName: " << yf::Thread::GetThreadName() << "\n"
    //               << "threadId: " << yf::Thread::GetThreadId() << "\n";
    // }, "thread1"));
    // t1->join();
    // yf::Thread::ptr t2(new yf::Thread([]{
    //     std::cout << "hello t2\n";
    // }));
    A a;
    // yf::Thread::ptr t3(new yf::Thread(&A::run, &a, "123"));
    yf::Thread::ptr t4(new yf::Thread(a, "456"));
    return 0;
}
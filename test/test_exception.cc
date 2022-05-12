#include <iostream>

#include "exception.h"

int main(int argc, char **argv) {
    yf::LogicError le("logicerror");
    std::cout << le.what() << std::endl;
    yf::LogicError le1(le);
    std::cout << le.what() << ":" << le1.what() << std::endl;
    yf::LogicError le2("le2");
    // std::cout << le.what() << ":" << le2.what() << std::endl;
    le2 = std::move(le1);
    std::cout << le1.what() << ":" << le2.what() << std::endl;
    std::logic_error le3("logic_error");
    std::logic_error le4("le4");
    le4 = std::move(le3);
    std::cout << le3.what() << ":" << le4.what() << std::endl;
    return 0;
}
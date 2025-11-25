#include "MathFunctions.h"
#include <iostream>

int main() {
    try {
        auto f = FunctionFactory::Create("power", 2); // x^2
        auto g = FunctionFactory::Create("polynomial", std::vector<double>{7, 0, 3}); // 7 + 3x^2
        
        std::cout << "f(x) = " << f->ToString() << std::endl;
        std::cout << "g(x) = " << g->ToString() << std::endl;
        
        double x = 2.0;
        std::cout << "f(" << x << ") = " << (*f)(x) << std::endl;
        std::cout << "g(" << x << ") = " << (*g)(x) << std::endl;
        
        std::cout << "f'(" << x << ") = " << f->GetDeriv(x) << std::endl;
        std::cout << "g'(" << x << ") = " << g->GetDeriv(x) << std::endl;
        
        auto h = f + g;
        std::cout << "h(x) = f(x) + g(x) = " << h->ToString() << std::endl;
        std::cout << "h(" << x << ") = " << (*h)(x) << std::endl;
        std::cout << "h'(" << x << ") = " << h->GetDeriv(x) << std::endl;
        
        auto linear = FunctionFactory::Create("polynomial", std::vector<double>{-5, 2}); // 2x - 5
        double root = FindRoot(linear, 0.0, 0.01, 10000);
        std::cout << "Root of " << linear->ToString() << " is approximately " << root << std::endl;

        auto expect_error = f + "a";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
#include "gtest/gtest.h"
#include "MathFunctions.h"
#include <cmath>

TEST(BasicFunctionsTest, IdentityFunction) {
    auto f = FunctionFactory::Create("ident");
    EXPECT_DOUBLE_EQ((*f)(5.0), 5.0);
    EXPECT_DOUBLE_EQ(f->GetDeriv(5.0), 1.0);
    EXPECT_EQ(f->ToString(), "x");
}

TEST(BasicFunctionsTest, ConstantFunction) {
    auto f = FunctionFactory::Create("const", 3.14);
    EXPECT_DOUBLE_EQ((*f)(5.0), 3.14);
    EXPECT_DOUBLE_EQ(f->GetDeriv(5.0), 0.0);
    EXPECT_EQ(f->ToString(), "3.140000");
}

TEST(BasicFunctionsTest, PowerFunction) {
    auto f = FunctionFactory::Create("power", 3);
    EXPECT_DOUBLE_EQ((*f)(2.0), 8.0);
    EXPECT_DOUBLE_EQ(f->GetDeriv(2.0), 12.0);
    EXPECT_EQ(f->ToString(), "x^3");
}

TEST(BasicFunctionsTest, ExponentialFunction) {
    auto f = FunctionFactory::Create("exp", 0.0);
    EXPECT_DOUBLE_EQ((*f)(0.0), 1.0);
    EXPECT_DOUBLE_EQ(f->GetDeriv(1.0), std::exp(1.0));
    EXPECT_EQ(f->ToString(), "exp(x)");
}

TEST(BasicFunctionsTest, PolynomialFunction) {
    auto f = FunctionFactory::Create("polynomial", std::vector<double>{1, 2, 3}); // 1 + 2x + 3x^2
    EXPECT_DOUBLE_EQ((*f)(2.0), 17.0); // 1 + 4 + 12
    EXPECT_DOUBLE_EQ(f->GetDeriv(2.0), 14.0); // 2 + 12
    EXPECT_TRUE(f->ToString().find("1.000000") != std::string::npos);
}

TEST(FactoryTest, UnsupportedTypeThrows) {
    EXPECT_THROW(FunctionFactory::Create("unknown", 1.0), std::logic_error);
}

TEST(ArithmeticOperationsTest, Addition) {
    auto f = FunctionFactory::Create("ident"); // x
    auto g = FunctionFactory::Create("const", 5.0); // 5
    auto h = f + g; // x + 5
    
    EXPECT_DOUBLE_EQ((*h)(3.0), 8.0);
    EXPECT_DOUBLE_EQ(h->GetDeriv(3.0), 1.0);
}

TEST(ArithmeticOperationsTest, Subtraction) {
    auto f = FunctionFactory::Create("power", 2); // x^2
    auto g = FunctionFactory::Create("ident"); // x
    auto h = f - g; // x^2 - x
    
    EXPECT_DOUBLE_EQ((*h)(3.0), 6.0); // 9 - 3
    EXPECT_DOUBLE_EQ(h->GetDeriv(3.0), 5.0); // 2*3 - 1
}

TEST(ArithmeticOperationsTest, Multiplication) {
    auto f = FunctionFactory::Create("ident"); // x
    auto g = FunctionFactory::Create("const", 3.0); // 3
    auto h = f * g; // 3x
    
    EXPECT_DOUBLE_EQ((*h)(4.0), 12.0);
    EXPECT_DOUBLE_EQ(h->GetDeriv(4.0), 3.0);
}

TEST(ArithmeticOperationsTest, Division) {
    auto f = FunctionFactory::Create("power", 2); // x^2
    auto g = FunctionFactory::Create("ident"); // x
    auto h = f / g; // x^2 / x = x
    
    EXPECT_DOUBLE_EQ((*h)(4.0), 4.0);
    EXPECT_DOUBLE_EQ(h->GetDeriv(4.0), 1.0);
}

TEST(ArithmeticOperationsTest, MixedTypesThrow) {
    auto f = FunctionFactory::Create("ident");
    
    EXPECT_THROW(f + "abc", std::logic_error);
    EXPECT_THROW(f - 123, std::logic_error);
    EXPECT_THROW(f * 3.14, std::logic_error);
    EXPECT_THROW(f / std::vector<int>{}, std::logic_error);
    EXPECT_THROW("abc" + f, std::logic_error);
}

TEST(ComplexExpressionsTest, MultipleOperations) {
    auto x = FunctionFactory::Create("ident");
    auto c = FunctionFactory::Create("const", 2.0);
    auto expr = (x + c) * (x - c); // (x+2)(x-2) = x^2 - 4
    
    EXPECT_DOUBLE_EQ((*expr)(3.0), 5.0); // 9 - 4
    EXPECT_DOUBLE_EQ(expr->GetDeriv(3.0), 6.0); // 2*3
}

TEST(DerivativeTest, CompositeFunctions) {
    auto f = FunctionFactory::Create("power", 2); // x^2
    auto g = FunctionFactory::Create("ident"); // x
    auto h = f * g; // x^3
    
    EXPECT_DOUBLE_EQ(h->GetDeriv(2.0), 12.0); // 3*4
}

TEST(RootFindingTest, LinearEquation) {
    // f(x) = 2x - 6, root at x = 3
    auto f = FunctionFactory::Create("polynomial", std::vector<double>{-6, 2});
    double root = FindRoot(f, 0.0, 0.1, 100);
    EXPECT_NEAR(root, 3.0, 0.001);
}

TEST(RootFindingTest, QuadraticEquation) {
    // f(x) = x^2 - 4, roots at x = ±2
    auto x_sq = FunctionFactory::Create("power", 2);
    auto four = FunctionFactory::Create("const", 4.0);
    auto f = x_sq - four;
    
    double root = FindRoot(f, 1.5, 0.1, 100);
    EXPECT_NEAR(root, 2.0, 0.001);
}

TEST(RootFindingTest, ZeroDerivativeThrows) {
    auto f = FunctionFactory::Create("const", 5.0); // f(x) = 5, derivative = 0
    EXPECT_THROW(FindRoot(f, 0.0, 0.1, 10), std::runtime_error);
}

TEST(ToStringTest, BasicFunctionsFormat) {
    auto ident = FunctionFactory::Create("ident");
    EXPECT_EQ(ident->ToString(), "x");
    
    auto power = FunctionFactory::Create("power", 2);
    EXPECT_EQ(power->ToString(), "x^2");
    
    auto exp_func = FunctionFactory::Create("exp", 0.0);
    EXPECT_EQ(exp_func->ToString(), "exp(x)");

    auto const_func = FunctionFactory::Create("const", 0.0);
    EXPECT_EQ(const_func->ToString(), "0.000000");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
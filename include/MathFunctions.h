#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <concepts>

class IFunction {
public:
    virtual ~IFunction() = default;
    virtual double operator()(double x) const = 0;
    virtual double GetDeriv(double x) const = 0;
    virtual std::string ToString() const = 0;
};

using FunctionPtr = std::shared_ptr<IFunction>;

class FunctionFactory {
public:
    static FunctionPtr Create(const std::string& type);
    static FunctionPtr Create(const std::string& type, int param);
    static FunctionPtr Create(const std::string& type, double param);
    static FunctionPtr Create(const std::string& type, const std::vector<double>& params);
};

FunctionPtr operator+(const FunctionPtr& lhs, const FunctionPtr& rhs);
FunctionPtr operator-(const FunctionPtr& lhs, const FunctionPtr& rhs);
FunctionPtr operator*(const FunctionPtr& lhs, const FunctionPtr& rhs);
FunctionPtr operator/(const FunctionPtr& lhs, const FunctionPtr& rhs);

template<typename T>
concept IsFunctionPtr = std::is_same_v<T, FunctionPtr>;

template<typename T>
concept NotFunctionPtr = !std::is_same_v<T, FunctionPtr>;

template<typename A, typename B>
concept XORFunctionPtr = 
        (IsFunctionPtr<A> && NotFunctionPtr<B>) ||
        (NotFunctionPtr<A> && IsFunctionPtr<B>);

template<typename A, typename B>
requires XORFunctionPtr<A, B>
FunctionPtr operator+(const A& lhs, const B& rhs) {
    throw std::logic_error("Unsupported types for arithmetic operation");
}

template<typename A, typename B>
requires XORFunctionPtr<A, B>
FunctionPtr operator-(const A& lhs, const B& rhs) {
    throw std::logic_error("Unsupported types for arithmetic operation");
}

template<typename A, typename B>
requires XORFunctionPtr<A, B>
FunctionPtr operator*(const A& lhs, const B& rhs) {
    throw std::logic_error("Unsupported types for arithmetic operation");
}

template<typename A, typename B>
requires XORFunctionPtr<A, B>
FunctionPtr operator/(const A& lhs, const B& rhs) {
    throw std::logic_error("Unsupported types for arithmetic operation");
}

double FindRoot(const FunctionPtr& func, double initialGuess, double learningRate, int iterations);
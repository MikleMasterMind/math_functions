#include "MathFunctions.h"
#include <cmath>
#include <iostream>

class IdentityFunction : public IFunction {
public:
    double operator()(double x) const override { return x; }
    double GetDeriv(double x) const override { return 1.0; }
    std::string ToString() const override { return "x"; }
};

class ConstanIFunction : public IFunction {
    double value_;
public:
    ConstanIFunction(double value) : value_(value) {}
    double operator()(double x) const override { return value_; }
    double GetDeriv(double x) const override { return 0.0; }
    std::string ToString() const override { return std::to_string(value_); }
};

class PowerFunction : public IFunction {
    int power_;
public:
    PowerFunction(int power) : power_(power) {}
    double operator()(double x) const override { return std::pow(x, power_); }
    double GetDeriv(double x) const override { return power_ * std::pow(x, power_ - 1); }
    std::string ToString() const override { return "x^" + std::to_string(power_); }
};

class ExponentialFunction : public IFunction {
public:
    double operator()(double x) const override { return std::exp(x); }
    double GetDeriv(double x) const override { return std::exp(x); }
    std::string ToString() const override { return "exp(x)"; }
};

class PolynomialFunction : public IFunction {
    std::vector<double> coefficients_;
public:
    PolynomialFunction(const std::vector<double>& coefficients) : coefficients_(coefficients) {}
    
    double operator()(double x) const override {
        double result = 0.0;
        double x_power = 1.0;
        for (double coef : coefficients_) {
            result += coef * x_power;
            x_power *= x;
        }
        return result;
    }
    
    double GetDeriv(double x) const override {
        double result = 0.0;
        double x_power = 1.0;
        for (size_t i = 1; i < coefficients_.size(); ++i) {
            result += coefficients_[i] * i * x_power;
            x_power *= x;
        }
        return result;
    }
    
    std::string ToString() const override {
        std::string result;
        for (size_t i = 0; i < coefficients_.size(); ++i) {
            if (i > 0) result += " + ";
            result += std::to_string(coefficients_[i]) + "*x^" + std::to_string(i);
        }
        return result;
    }
};

class AddExpression : public IFunction {
    FunctionPtr lhs_, rhs_;
public:
    AddExpression(FunctionPtr lhs, FunctionPtr rhs) : lhs_(lhs), rhs_(rhs) {}
    double operator()(double x) const override { return (*lhs_)(x) + (*rhs_)(x); }
    double GetDeriv(double x) const override { return lhs_->GetDeriv(x) + rhs_->GetDeriv(x); }
    std::string ToString() const override { return "(" + lhs_->ToString() + " + " + rhs_->ToString() + ")"; }
};

class SubtractExpression : public IFunction {
    FunctionPtr lhs_, rhs_;
public:
    SubtractExpression(FunctionPtr lhs, FunctionPtr rhs) : lhs_(lhs), rhs_(rhs) {}
    double operator()(double x) const override { return (*lhs_)(x) - (*rhs_)(x); }
    double GetDeriv(double x) const override { return lhs_->GetDeriv(x) - rhs_->GetDeriv(x); }
    std::string ToString() const override { return "(" + lhs_->ToString() + " - " + rhs_->ToString() + ")"; }
};

class MultiplyExpression : public IFunction {
    FunctionPtr lhs_, rhs_;
public:
    MultiplyExpression(FunctionPtr lhs, FunctionPtr rhs) : lhs_(lhs), rhs_(rhs) {}
    double operator()(double x) const override { return (*lhs_)(x) * (*rhs_)(x); }
    double GetDeriv(double x) const override { 
        return lhs_->GetDeriv(x) * (*rhs_)(x) + (*lhs_)(x) * rhs_->GetDeriv(x); 
    }
    std::string ToString() const override { return "(" + lhs_->ToString() + " * " + rhs_->ToString() + ")"; }
};

class DivideExpression : public IFunction {
    FunctionPtr lhs_, rhs_;
public:
    DivideExpression(FunctionPtr lhs, FunctionPtr rhs) : lhs_(lhs), rhs_(rhs) {}
    double operator()(double x) const override { return (*lhs_)(x) / (*rhs_)(x); }
    double GetDeriv(double x) const override {
        double f = (*lhs_)(x), g = (*rhs_)(x);
        double f_prime = lhs_->GetDeriv(x), g_prime = rhs_->GetDeriv(x);
        return (f_prime * g - f * g_prime) / (g * g);
    }
    std::string ToString() const override { return "(" + lhs_->ToString() + " / " + rhs_->ToString() + ")"; }
};

FunctionPtr FunctionFactory::Create(const std::string& type) {
    if (type == "ident") return std::make_shared<IdentityFunction>();
    throw std::logic_error("Unsupported function type: " + type);
}  

FunctionPtr FunctionFactory::Create(const std::string& type, int param) {
    if (type == "power") return std::make_shared<PowerFunction>(param);
    throw std::logic_error("Unsupported function type: " + type);
}

FunctionPtr FunctionFactory::Create(const std::string& type, double param) {
    if (type == "const") return std::make_shared<ConstanIFunction>(param);
    if (type == "exp") return std::make_shared<ExponentialFunction>();
    throw std::logic_error("Unsupported function type: " + type);
}

FunctionPtr FunctionFactory::Create(const std::string& type, const std::vector<double>& params) {
    if (type == "polynomial") return std::make_shared<PolynomialFunction>(params);
    throw std::logic_error("Unsupported function type: " + type);
}

FunctionPtr operator+(const FunctionPtr& lhs, const FunctionPtr& rhs) {
    return std::make_shared<AddExpression>(lhs, rhs);
}

FunctionPtr operator-(const FunctionPtr& lhs, const FunctionPtr& rhs) {
    return std::make_shared<SubtractExpression>(lhs, rhs);
}

FunctionPtr operator*(const FunctionPtr& lhs, const FunctionPtr& rhs) {
    return std::make_shared<MultiplyExpression>(lhs, rhs);
}

FunctionPtr operator/(const FunctionPtr& lhs, const FunctionPtr& rhs) {
    return std::make_shared<DivideExpression>(lhs, rhs);
}

double FindRoot(const FunctionPtr& func, double initialGuess, double learningRate, int iterations) {
    double x = initialGuess;
    for (int i = 0; i < iterations; ++i) {
        double y = (*func)(x);
        double dydx = func->GetDeriv(x);
        if (dydx == 0.0) {
            throw std::runtime_error("Zero derivative encountered during gradient descent");
        }
        x = x - learningRate * y / dydx;
    }
    return x;
}
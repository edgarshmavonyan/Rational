#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#pragma once
#include "biginteger.h"

using namespace std;

class Rational{
private:

    BigInteger numerator_, denominator_;

    static const size_t DOUBLE_PRECISION = 18;

    static const int BASE = 10;

    void getRightForm();

    Rational inverse() const;

    bool isNegative() const {
        return (numerator_ < 0);
    }

    Rational abs() const;

    static BigInteger gcd(const BigInteger& firstOperand, const BigInteger& secondOperand);
    static BigInteger lcm(const BigInteger& first, const BigInteger& second);

    BigInteger getMultiplier(const BigInteger& product) const;

public:

    Rational() : numerator_(0), denominator_(1) {}

    Rational(int val) : numerator_(val), denominator_(1) {}

    Rational(const BigInteger& val) : numerator_(val), denominator_(1) {}

    Rational(const BigInteger& numerator, const BigInteger& denominator) : numerator_(numerator), denominator_(denominator) {}

    string toString();

    string asDecimal(size_t precision = 0) const;

    explicit operator double() const {
        return stod(asDecimal(DOUBLE_PRECISION));
    }

    friend istream& operator>>(istream& is, Rational& element);

    Rational& operator=(const Rational& other);

    Rational operator-() const;

    Rational& operator*=(const Rational& other);

    Rational& operator+=(const Rational& other);

    Rational& operator-=(const Rational& other);

    Rational& operator/=(const Rational& other);

    bool operator<(const Rational& other) const;

    bool operator==(const Rational& other) const;

    bool operator!=(const Rational& other) const;

    bool operator<=(const Rational& other) const;

    bool operator>(const Rational& other) const;

    bool operator>=(const Rational& other) const;
};

Rational operator+(const Rational& first, const Rational& second);

Rational operator-(const Rational& first, const Rational& second);

Rational operator*(const Rational& first, const Rational& second);

Rational operator/(const Rational& first, const Rational& second);

BigInteger Rational::gcd(const BigInteger& firstOperand, const BigInteger& secondOperand) {
    BigInteger first(firstOperand), second(secondOperand);
    if (first < 0) first.toPositive();
    if (second < 0) second.toPositive();
    while (first != 0 && second != 0) {
        if (first > second)
            first %= second;
        else
            second %= first;
    }
    if (first) return first;
    return second;
}

BigInteger Rational::lcm(const BigInteger& first, const BigInteger& second) {
    return (first*second)/gcd(first, second);
}

void Rational::getRightForm() {
    if (denominator_ < 0)
        numerator_ = -numerator_, denominator_ = -denominator_;
    BigInteger divider = gcd(numerator_, denominator_);
    numerator_ /= divider;
    denominator_ /= divider;
}

Rational Rational::abs() const {
    if (isNegative())
        return -(*this);
    return *this;
}

string Rational::toString() {
    string s = "";
    s += numerator_.toString();
    if (denominator_ != 1) {
        s += '/';
        s += denominator_.toString();
    }
    return s;
}

BigInteger Rational::getMultiplier(const BigInteger& product) const {
    return product / denominator_;
}

Rational& Rational::operator=(const Rational& other) {
    numerator_ = other.numerator_;
    denominator_ = other.denominator_;
    getRightForm();
    return *this;
}

Rational Rational::operator-() const {
    return Rational(-numerator_, denominator_);
}

Rational& Rational::operator*=(const Rational& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    getRightForm();
    return *this;
}

Rational& Rational::operator+=(const Rational& other) {
    BigInteger newDenominator = lcm(denominator_, other.denominator_);
    numerator_ *= getMultiplier(newDenominator);
    numerator_ += other.numerator_ * other.getMultiplier(newDenominator);
    denominator_ = newDenominator;
    getRightForm();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    return *this += (-other);
}

Rational& Rational::operator/=(const Rational& other) {
    return *this *= other.inverse();
}

Rational Rational::inverse() const {
    Rational res(*this);
    swap(res.numerator_, res.denominator_);
    res.getRightForm();
    return res;
}

Rational operator+(const Rational& first, const Rational& second) {
    Rational res(first);
    return res += second;
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational res(first);
    return res -= second;
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational res(first);
    return res *= second;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational res(first);
    return res /= second;
}

bool Rational::operator<(const Rational& other) const {
    return (numerator_ * other.denominator_ < other.numerator_ * denominator_);
}

bool Rational::operator==(const Rational& other) const {
    return (numerator_ == other.numerator_ && denominator_ == other.denominator_);
}

bool Rational::operator!=(const Rational& other) const {
    return !(*this == other);
}

bool Rational::operator<=(const Rational& other) const {
    return (*this == other || *this < other);
}

bool Rational::operator>(const Rational& other) const {
    return !(*this <= other);
}

bool Rational::operator>=(const Rational& other) const {
    return !(*this < other);
}

string Rational::asDecimal(size_t precision) const {
    string res = "";
    BigInteger integerPart = numerator_ / denominator_;
    BigInteger remainder = numerator_ % denominator_;

    if (integerPart == 0 && precision != 0 && remainder < 0)
        res += '-';
    res += integerPart.toString();

    if (precision != 0)
        res += '.';

    if (remainder < 0)
        remainder = -remainder;

    for (size_t i = 0; i < precision; i++) {
        remainder *= BASE;
        res += (remainder / denominator_).toString();
        remainder %= denominator_;
    }

    return res;
}

istream& operator>>(istream& is, Rational& element) {
    is >> element.numerator_ >> element.denominator_;
    return is;
}





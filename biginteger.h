#include <iostream>
#include <vector>
#include <string>

#pragma once

using namespace std;

class BigInteger{
    friend class Rational;

private:
    vector<int> field;
    bool below;

    bool checkZero() {
        while (!field.empty() && field.back() == 0) field.pop_back();
        if (field.empty() || field.back() == 0) below = false;
        return (field.empty() || field.back() == 0);
    }

    BigInteger& div_2();

    friend void swap(BigInteger& a, BigInteger& b);

    bool isNegative() const;

    inline int& operator[](int i) {
        return field[i];
    }

    inline const int& operator[](int i) const {
        return field[i];
    }

    inline int size() const {
        return int(field.size());
    }

    inline void toPositive() {
        below = false;
    }

    static BigInteger absoluteDecrease(const BigInteger& first, const BigInteger& other);

    static BigInteger absoluteIncrease(const BigInteger& first, const BigInteger& other);

    static bool lessAbs(const BigInteger& first, const BigInteger& other);

    static bool equalAbs(const BigInteger& first, const BigInteger& other);

    static bool lessOrEqAbs(const BigInteger& first, const BigInteger& other);

    static int getKaratsubaSize(const BigInteger& first, const BigInteger& other);

    void normalize(int n);

public:
    BigInteger() : field(0) , below(false) {}

    BigInteger(bool below, const vector<int>& field) : field(field), below(below) {}

    BigInteger(int n);

    explicit operator bool() const {
        return (!field.empty() && field.back() != 0);
    }

    bool operator!() const {
        return !bool(*this);
    }

    BigInteger operator-() const {
        if (!(*this)) return *this;
        return BigInteger(!below, field);
    }

    BigInteger& operator=(const BigInteger& other);

    string toString();

    friend ostream& operator<<(ostream& os,const BigInteger& number);

    friend istream& operator>>(istream& is, BigInteger& number);

    bool friend operator==(const BigInteger& first, const BigInteger& other);

    bool friend operator!=(const BigInteger& first, const BigInteger& other);

    bool friend operator<(const BigInteger& first, const BigInteger& other);

    bool friend operator<=(const BigInteger& first, const BigInteger& other);

    bool friend operator>(const BigInteger& first, const BigInteger& other);

    bool friend operator>=(const BigInteger& first, const BigInteger& other);

    BigInteger friend operator+(const BigInteger& first, const BigInteger& other);

    BigInteger friend operator-(const BigInteger& first, const BigInteger& other);

    BigInteger& operator--();

    BigInteger operator--(int);

    BigInteger& operator-=(const BigInteger& other);

    BigInteger& operator+=(const BigInteger& other);

    BigInteger& operator++();

    BigInteger operator++(int);

    BigInteger friend operator*(const BigInteger& first, const BigInteger& other);

    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator/=(const BigInteger& other);

    BigInteger friend operator/(const BigInteger& first, const BigInteger& other);

    BigInteger& operator%=(const BigInteger& other);

    BigInteger friend operator%(const BigInteger& first, const BigInteger& other);

    BigInteger& operator^=(const BigInteger& other);

    BigInteger friend operator^(const BigInteger& first, const BigInteger& other);
};

BigInteger::BigInteger(int n) {
    below = (n < 0);
    n = abs(n);
    do {
        field.push_back(n % 10);
        n /= 10;
    } while (n > 0);
    checkZero();
}

BigInteger BigInteger::absoluteDecrease(const BigInteger& first, const BigInteger& other) {
    const BigInteger *greater = &first;
    const BigInteger *less = &other;
    bool change = false;
    if (lessAbs(first, other)) {
        swap(greater, less);
        change = true;
    }
    BigInteger result(first.below != change, vector<int>(greater->field.size()));
    for (int i = 0; i < result.size(); ++i) {
        if (i >= int(less->field.size()))
            result[i] = greater->field[i];
        else
            result[i] = greater->field[i] - less->field[i];
    }

    for (int i = 0; i < result.size() - 1; ++i) {
        if (result[i] < 0)
            result[i] += 10, --result[i+1];
    }
    result.checkZero();
    return result;
}

BigInteger operator+(const BigInteger& first, const BigInteger& other) {
    if (first.below != other.below)
        return BigInteger::absoluteDecrease(first, other);
    return BigInteger::absoluteIncrease(first, other);
}

BigInteger& BigInteger::operator--() {
    return *this = *this - 1;
}

BigInteger& BigInteger::operator++() {
    return *this = *this + 1;
}

BigInteger BigInteger::operator--(int) {
    auto* temp = new BigInteger(*this);
    --(*this);
    return *temp;
}

BigInteger BigInteger::operator++(int) {
    auto* temp = new BigInteger(*this);
    ++(*this);
    return *temp;
}

bool BigInteger::isNegative() const {
    return below;
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    field = other.field;
    below = other.below;
    checkZero();
    return *this;
}

void swap(BigInteger& a, BigInteger& b) {
    using std::swap;
    swap(a.field, b.field);
    bool c = a.below;
    a.below = b.below;
    b.below = c;
}

string BigInteger::toString() {
    string a = "";
    checkZero();
    if (!(*this)) {
        a += '0';
        return a;
    }
    if (below) a += '-';
    for (int i = (int) field.size() - 1; i >= 0; i--) {
        a += '0' + field[i];
    }
    return a;
}

ostream& operator<<(ostream& os,const BigInteger& number) {
    if (!number || number.field.empty()) {
        os << 0;
        return os;
    }
    if (number.below) os << '-';
    for (int i = (int) number.field.size() - 1; i >= 0; i--)
        os << number.field[i];
    return os;
}

istream& operator>>(istream& is, BigInteger& number) {
    string str = "";
    is >> str;
    if (str.empty()) return is;
    number.below = (str[0] == '-');
    number.field = vector<int>(str.size() - number.below,0);
    for (int i = (int) str.size() - 1, j = 0; i >= number.below; i--, j++) {
        number.field[j] = str[i] - '0';
    }
    number.checkZero();
    return is;
}

bool operator==(const BigInteger& first, const BigInteger& other) {
    return (bool(first) == bool(other) && first.below == other.below && first.field == other.field);
}

bool operator!=(const BigInteger& first, const BigInteger& other) {
    return !(first == other);
}

bool BigInteger::lessAbs(const BigInteger& first, const BigInteger& other) {
    if (first.field.size() < other.field.size()) return true;
    if (first.field.size() > other.field.size()) return false;
    int i;
    for (i = (int) first.field.size() - 1; i > 0 && first.field[i] == other.field[i]; i--);
    return (i != -1 && first.field[i] < other.field[i]);
}

bool BigInteger::equalAbs(const BigInteger& first, const BigInteger& other) {
    return (bool(first) == bool(other) && first.field == other.field);
}

bool BigInteger::lessOrEqAbs(const BigInteger& first, const BigInteger& other) {
    return (lessAbs(first, other) || equalAbs(first, other));
}

bool operator<(const BigInteger& first, const BigInteger& other) {
    if (first == other) return false;
    if (other.field.empty())
        return (first.isNegative());
    if (first.below && !other.below) return true;
    if (!first.below && other.below) return false;
    return (BigInteger::lessAbs(first, other) != first.below);
}

bool operator<=(const BigInteger& first, const BigInteger& other) {
    return (first == other || first < other);
}

bool operator>(const BigInteger& first, const BigInteger& other) {
    return !(first <= other);
}

bool operator>=(const BigInteger& first, const BigInteger& other) {
    return !(first < other);
}

BigInteger BigInteger::absoluteIncrease(const BigInteger& first, const BigInteger& other) {
    BigInteger result(first.below, vector<int>(max(first.size(), other.size()) + 1, 0));
    for (int i = 0; i < result.size() - 1; ++i) {
        if (i >= other.size())
            result[i] = first.field[i];
        else if (i >= first.size())
            result[i] = other.field[i];
        else
            result[i] = first.field[i] + other.field[i];
    }
    for (int i = 0; i < result.size() - 1; ++i) {
        if (result[i] > 9)
            result[i] -= 10, ++result[i+1];
    }
    result.checkZero();
    return result;
}

BigInteger operator-(const BigInteger& first, const BigInteger& other) {
    if (first.below != other.below)
        return BigInteger::absoluteIncrease(first, other);
    return BigInteger::absoluteDecrease(first, other);
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this = (*this) - other;
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    *this = (*this) + other;
    return *this;
}

BigInteger operator*(const BigInteger& first, const BigInteger& other) {
    BigInteger ans = BigInteger(first.below != other.below, vector<int>(first.field.size() + other.field.size(), 0));
    int next = 0;

    for (int i = 0; i < (int) first.field.size(); i++) {
        for (int j = 0; j < (int) other.field.size() || next ; j++) {
            int num = ans.field[i+j] + next;
            if (j < (int) other.field.size()) num += first.field[i] * other.field[j];
            ans.field[i+j] = num % 10;
            next = num / 10;
        }
    }

    ans.checkZero();
    return ans;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    *this = (*this) * other;
    return *this;
}

BigInteger& BigInteger::div_2() {
    if (!(*this)) return *this;
    vector<int> operand(field.size());
    for (int i = 0; i < (int) field.size(); i++)
        operand[i] = (int) field[i];
    int st = (int) operand.size() - 1;
    vector<int> res;

    for (int i = st; i >= 0; i--) {
        if (i < st && operand[i+1] > 0) {
            res.push_back((operand[i+1]*10 + operand[i])/2);
            operand[i] %= 2;
            operand[i+1] = 0;
            continue;
        }
        if (i < st && operand[i] < 2) {
            res.push_back(0);
            continue;
        }
        res.push_back(operand[i]/2);
        operand[i] %= 2;
    }
    field = vector<int>(res.size());
    for (int i = (int) res.size() - 1; i >= 0; i--)
        field[i] = (int) res[res.size() - 1 - i];
    checkZero();
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (!other) return *this;
    bool sign = (below != other.below);
    BigInteger l(0), r = *this;
    r.toPositive();
    ++r;
    BigInteger mid;
    while (l != (r - 1)) {

        mid = absoluteIncrease(l, r);
        mid.toPositive();
        mid.div_2();

        if (lessOrEqAbs(other * mid,*this)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    *this = l;
    this->below = sign;
    checkZero();
    return *this;
}

BigInteger operator/(const BigInteger& first, const BigInteger& other) {
    BigInteger result(first);
    result /= other;
    return result;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this - (((*this) / other) * other);
    return *this;
}

BigInteger operator%(const BigInteger& first, const BigInteger& other) {
    BigInteger result(first);
    result %= other;
    return result;
}

void BigInteger::normalize(int newSize) {
    field.resize(newSize);
}

int BigInteger::getKaratsubaSize(const BigInteger& first, const BigInteger& other) {
    int newSize = 1;
    while (newSize < first.size() || newSize < other.size())
        newSize <<= 1;
    return newSize;
}

BigInteger& BigInteger::operator^=(const BigInteger& other) {
    if (size() == 0 || other.size() == 0)
        return *this = 0;

    int newSize = max(size(), other.size());
    if (newSize % 2 != 0) newSize++;

    if (newSize <= 128)
        return *this *= other;


    auto firstDivider = min(field.end(), field.begin() + newSize / 2);
    auto otherDivider = min(other.field.end(), other.field.begin() + newSize / 2);

    BigInteger b;
    b.field = vector<int>(firstDivider, field.end());
    BigInteger a;
    a.field = vector<int>(field.begin(), firstDivider);

    BigInteger d;
    d.field = vector<int>(otherDivider, other.field.end());
    BigInteger c;
    c.field = vector<int>(other.field.begin(), otherDivider);


    BigInteger e = (a + b) ^ (c + d);

    a ^= c;
    b ^= d;
    e -= a + b;

    field.resize(2*newSize, 0);

    for (int i = 0; i < newSize/2; i++)
        field[i] = a[i];

    for (int i = newSize/2; i < newSize; i++) {
        field[i] = (i < a.size() ? a[i] : 0) + (i - newSize / 2 < e.size() ? e[i - newSize / 2] : 0);
        if (field[i] > 9)
            field[i] -= 10, field[i+1]++;
    }

    for (int i = newSize; i < 2 * newSize; i++) {
        if (i - newSize / 2 < e.size())
            field[i] = e[i - newSize / 2];

        if (i - newSize < b.size())
            field[i] += b[i - newSize];
        if (field[i] > 9)
            field[i] -= 10, field[i+1]++;
    }

    checkZero();
    return *this;
}

BigInteger operator^(const BigInteger& first, const BigInteger& other) {
    BigInteger result(first);
    result ^= other;
    return result;
}


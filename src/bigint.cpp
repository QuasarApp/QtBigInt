//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "bigint.h"
#include <limits>
#include <cstring>
#include <cmath>
#include <cstdlib>

// constructors
BigInt::BigInt() {
    mpz_init(data);
}

BigInt::BigInt(const BigInt &val, int bitCount) {
    if (bitCount > 0) {
        mpz_init2(data, static_cast<unsigned int>(bitCount));
    } else {
        mpz_init(data);
    }

    mpz_set(data, val.data);

}

BigInt::BigInt(const std::string &str, int base):
    BigInt() {
    mpz_set_str(data, str.c_str(), base);
}

BigInt::BigInt(intMpz val):
    BigInt() {
    mpz_set_si(data, val);
}

BigInt::BigInt(char item, unsigned int size, int base = 2):
    BigInt(std::string(size, item),base) {
}

std::string BigInt::getString(int base) const {
    char *str = mpz_get_str(nullptr, base, data); // mpz_get_str allocates memory - it must be freed
    std::string result = str;
    free(str);
    return result;
}

BigInt::~BigInt() {
    mpz_clear(data);
}

BigInt &BigInt::powm(const BigInt &pow, const BigInt &mod) {
    mpz_powm(data, data, pow.data, mod.data);
    return *this;
}

BigInt BigInt::powm(BigInt val, const BigInt &pow, const BigInt &mod) {
    return val.powm(pow, mod);
}

BigInt &BigInt::pow(uIntMpz pow) {
    mpz_pow_ui(data, data, pow);
    return *this;
}

int BigInt::sizeBits() const {
    return sizeBytes() * 8;
}

int BigInt::sizeBytes() const {
    return static_cast<int>(mpz_size(data) * sizeof ((*data->_mp_d)));
}

int BigInt::longBits() const {
    return static_cast<int>(getString(2).size());
}

int BigInt::longBytes() const {
    return static_cast<int>(std::ceil(static_cast<double>(longBits()) / 8));
}

#define GMP_ABS(x) ((x) >= 0 ? (x) : -(x))
int BigInt::sizeType() const {
    return static_cast<int>(static_cast<size_t>(GMP_ABS( data->_mp_alloc)) *
                            sizeof ((*data->_mp_d)));

}

bool BigInt::isPrime(bool absalut) const {
    return (mpz_probab_prime_p(data, 50) - (absalut? 1: 0)) > 0;
}

BigInt& BigInt::gcd(const BigInt &a, const BigInt &b) {
    mpz_gcd(data, a.data, b.data);
    return *this;

}

void BigInt::fromHex(const std::string &hex) {
    mpz_set_str(data, hex.c_str(), 16);
}

BigInt BigInt::bigPow10(unsigned short pow) {
    return "1" + std::string(pow, '0');
}

BigInt &BigInt::toNegative() {
    mpz_neg(data, data);
    return *this;
}

BigInt &BigInt::operator =(const BigInt &val) {
    mpz_set(data, val.data);
    return *this;
}

BigInt &BigInt::operator =(const std::string &imput) {
    mpz_set_str(data, imput.c_str(), 10);
    return *this;
}

BigInt &BigInt::operator =(intMpz val) {
    mpz_set_si(data, val);
    return *this;
}

//  add operators

BigInt operator +(BigInt left, intMpz right) {
    if (right >= 0) {
        mpz_add_ui(left.data, left.data, static_cast<uIntMpz>(right));
        return left;
    }

    return left -= std::abs(right);
}

BigInt operator +(intMpz left, BigInt right) {
    return right += left;
}

BigInt operator +(BigInt left, const BigInt &right) {
    mpz_add(left.data, left.data, right.data);
    return left;
}

BigInt operator +(BigInt left, const std::string &right) {
    return left += BigInt(right);
}

BigInt operator +(const std::string &left, const BigInt &right) {
    return BigInt(left) + right;
}

BigInt& operator +=(BigInt &left, intMpz right) {
    if (right >= 0) {
        mpz_add_ui(left.data, left.data, static_cast<uIntMpz>(right));
        return left;
    }
    return left -= std::abs(right);
}

BigInt& operator +=(BigInt &left, const BigInt &right) {
    mpz_add(left.data, left.data, right.data);
    return left;
}

BigInt& operator +=(BigInt &left, const std::string &right) {
    return left += BigInt(right);
}

//  sub operators

BigInt operator -(BigInt left, const BigInt &right) {
    mpz_sub(left.data, left.data, right.data);
    return left;
}

BigInt operator -(BigInt left, intMpz right) {
    if (right >= 0) {
        mpz_sub_ui(left.data, left.data, static_cast<uIntMpz>(right));
        return left;
    }
    return left += std::abs(right);
}

BigInt operator -(intMpz left, BigInt right) {
    if (left >= 0) {
        mpz_ui_sub(right.data, static_cast<uIntMpz>(left), right.data);
        return right;
    }
    return right += std::abs(left);
}

BigInt operator-(BigInt val) {
    mpz_neg(val.data, val.data);
    return val;
}

BigInt operator -(BigInt left, const std::string &right) {
    return left -= BigInt(right);
}

BigInt operator -(const std::string & left, const BigInt &right) {
    return BigInt(left) - right;
}

BigInt& operator -=(BigInt &left, const BigInt &right) {
    mpz_sub(left.data, left.data, right.data);
    return left;
}

BigInt& operator -=(BigInt &left, const std::string &right) {
    return left -= BigInt(right);
}

BigInt& operator -=(BigInt &left, intMpz right) {
    if (right >= 0) {
        mpz_sub_ui(left.data, left.data, static_cast<uIntMpz>(right));
        return left;
    }
    return left += std::abs(right);
}

//  div operators

BigInt operator /(BigInt left, const BigInt &right) {
    mpz_tdiv_q(left.data, left.data, right.data);
    return left;
}

BigInt operator /(BigInt left, intMpz right) {
    mpz_tdiv_q_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));

    if (right >= 0) {
        return left;
    }
    return -left;
}

BigInt operator /(BigInt left, const std::string &right) {
    return left /= BigInt(right);
}

BigInt operator /(intMpz left, BigInt right) {
    return BigInt(left) / right;
}

BigInt operator /(const std::string & left, const BigInt &right) {
    return BigInt(left) / right;
}

BigInt& operator /=(BigInt &left, const BigInt &right) {
    mpz_tdiv_q(left.data, left.data, right.data);
    return left;
}

BigInt& operator /=(BigInt &left,  const std::string &right) {
    return left /= BigInt(right);
}

BigInt& operator /=(BigInt &left, intMpz right) {
    mpz_tdiv_q_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));

    if (right >= 0) {
        return left;
    }
    return left.toNegative();
}

// mul operators
BigInt operator *(BigInt left, const BigInt &right) {
    mpz_mul(left.data, left.data, right.data);
    return left;
}

BigInt operator *(BigInt left, intMpz right) {
    mpz_mul_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));

    if (right >= 0) {
        return left;
    }
    return -left;
}

BigInt operator *(intMpz left, BigInt right) {
    return right *= left;
}

BigInt operator *(const std::string & left, BigInt right) {
    return right *= BigInt(left);
}

BigInt operator *(BigInt left, const std::string &right) {
    return left *= BigInt(right);
}

BigInt& operator *=(BigInt &left, const BigInt &right) {
    mpz_mul(left.data, left.data, right.data);
    return left;
}

BigInt& operator *=(BigInt &left, const std::string &right) {
    return left *= BigInt(right);
}

BigInt& operator *=(BigInt &left, intMpz right) {
    mpz_mul_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));

    if (right >= 0) {
        return left;
    }
    return left.toNegative();
}

//mod operations
BigInt operator %(BigInt left, const BigInt &right) {
    mpz_tdiv_r(left.data, left.data, right.data);
    return left;
}

BigInt operator %(BigInt left, intMpz right) {
    mpz_tdiv_r_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));
    return left;
}

BigInt operator %(intMpz left, BigInt right) {
    return BigInt(left) % right;
}

BigInt operator %(BigInt left, const std::string & right) {
    return left %= BigInt(right);
}

BigInt operator %(const std::string & left, const BigInt &right) {
    return BigInt(left) % right;
}

BigInt& operator %=(BigInt& left, const BigInt &right) {
    mpz_tdiv_r(left.data, left.data, right.data);
    return left;
}

BigInt& operator %=(BigInt& left, intMpz right) {
    mpz_tdiv_r_ui(left.data, left.data, static_cast<uIntMpz>(std::abs(right)));
    return left;
}

BigInt& operator %=(BigInt &left, const std::string &right) {
    return left %= BigInt(right);
}

// incriment and dicriment
BigInt &BigInt::operator--() {
    *this -= 1;
    return *this;
}

BigInt &BigInt::operator++() {
    *this += 1;
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp(*this);
    --*this;
    return temp;
}

BigInt BigInt::operator++(int) {
    BigInt temp(*this);
    ++*this;
    return temp;
}

// move operators

BigInt operator >>(BigInt left, int right) {
    if (right >= 0) {
        mpn_rshift(left.data->_mp_d,
                   left.data->_mp_d,
                   left.data->_mp_size,
                   static_cast<unsigned int>(std::abs(right)));
        return left;
    }

    return left << right;
}

BigInt operator <<(BigInt left, int right) {
    if (right >= 0) {
        mpn_lshift(left.data->_mp_d,
                   left.data->_mp_d,
                   left.data->_mp_size,
                   static_cast<unsigned int>(std::abs(right)));
        return left;
    }

    return left >> right;
}

BigInt& operator >>=(BigInt &left, int right) {
    if (right >= 0) {
        mpn_rshift(left.data->_mp_d,
                   left.data->_mp_d,
                   left.data->_mp_size,
                   static_cast<unsigned int>(std::abs(right)));
        return left;
    }

    return left <<= right;
}

BigInt& operator <<=(BigInt &left, int right) {
    if (right >= 0) {
        mpn_lshift(left.data->_mp_d,
                   left.data->_mp_d,
                   left.data->_mp_size,
                   static_cast<unsigned int>(std::abs(right)));
        return left;
    }

    return left >>= right;
}

// other bin operators
BigInt operator ~(BigInt left) {
    mpz_com(left.data, left.data);
    return left;
}

BigInt operator |(BigInt left, const BigInt &right) {
    mpz_ior(left.data, left.data, right.data);
    return left;
}

BigInt operator |(const BigInt &left, intMpz right) {
    return left | BigInt(right);
}

BigInt& operator |=(BigInt &left, const BigInt &right) {
    mpz_ior(left.data, left.data, right.data);
    return left;
}

BigInt& operator |=(BigInt &left, intMpz right) {
    return left |= BigInt(right);
}

BigInt operator &(BigInt left, const BigInt &right) {
    mpz_and(left.data, left.data, right.data);
    return left;
}

BigInt operator &(const BigInt &left, intMpz right) {
    return left & BigInt(right);
}


BigInt& operator &=(BigInt &left, const BigInt &right) {
    mpz_and(left.data, left.data, right.data);
    return left;
}

BigInt& operator &=(BigInt &left, intMpz right) {
    return left &= BigInt(right);
}

BigInt operator ^(BigInt left, const BigInt &right) {
    mpz_xor(left.data, left.data, right.data);
    return left;
}

BigInt operator ^(const BigInt &left, intMpz right) {
    return left ^ BigInt(right);
}

BigInt& operator ^=(BigInt &left, const BigInt &right) {
    mpz_xor(left.data, left.data, right.data);
    return left;
}

BigInt& operator ^=(BigInt &left, intMpz right) {
    return left ^= BigInt(right);
}


// logic operators

bool operator!(const BigInt &val) {
    return val == 0;
}

bool operator == (const BigInt& left, const BigInt& right) {
    return mpz_cmp(left.data, right.data) == 0;
}

bool operator == (const BigInt& left, intMpz right) {
    return mpz_cmp_si(left.data, right) == 0;
}

bool operator == (const BigInt &left, const std::string &right) {
    return left == BigInt(right);
}

bool operator == ( intMpz left, const BigInt & right) {
    return right == left;
}

bool operator == ( const std::string & left, const BigInt & right) {
    return right == BigInt(left);
}

bool operator != (const BigInt &left, const BigInt& right) {
    return !(left == right);
}

bool operator != (const BigInt &left, intMpz right) {
    return !(left == right);
}

bool operator != (const BigInt &left, const std::string &right) {
    return left != BigInt(right);
}

bool operator != ( intMpz left, const BigInt & right) {
    return right != left;
}

bool operator != ( const std::string & left, const BigInt & right) {
    return right != BigInt(left);
}

bool operator < ( const BigInt &left, const BigInt& right) {
    return mpz_cmp(left.data, right.data) < 0;
}

bool operator < ( const BigInt &left, intMpz right) {
    return mpz_cmp_si(left.data, right) < 0;
}

bool operator < ( const BigInt &left, const std::string &right) {
    return left < BigInt(right);
}

bool operator < ( intMpz left, const BigInt & right) {
    return right > left;
}

bool operator < ( const std::string & left, const BigInt & right) {
    return right > BigInt(left);
}

bool operator > ( const BigInt &left, const BigInt& right) {
    return mpz_cmp(left.data, right.data) > 0;
}

bool operator > ( const BigInt &left, intMpz right) {
    return mpz_cmp_si(left.data, right) > 0;
}

bool operator > ( const BigInt &left, const std::string &right) {
    return left > BigInt(right);
}

bool operator > ( intMpz left, const BigInt & right) {
    return right < left;
}

bool operator > ( const std::string & left, const BigInt & right) {
    return right < BigInt(left);
}

bool operator <= ( const BigInt &left, const BigInt& right) {
    return mpz_cmp(left.data, right.data) <= 0;
}

bool operator <= ( const BigInt &left, intMpz right) {
    return mpz_cmp_si(left.data, right) <= 0;
}

bool operator <= ( const BigInt &left, const std::string &right) {
    return left <= BigInt(right);
}

bool operator <= ( intMpz left, const BigInt & right) {
    return right >= left;
}

bool operator <= ( const std::string & left, const BigInt & right) {
    return right >= BigInt(left);
}

bool operator >= ( const BigInt &left, const BigInt& right) {
    return mpz_cmp(left.data, right.data) >= 0;
}

bool operator >= ( const BigInt &left, intMpz right) {
    return mpz_cmp_si(left.data, right) >= 0;
}

bool operator >= ( const BigInt &left, const std::string &right) {
    return left >= BigInt(right);
}

bool operator >= ( intMpz left, const BigInt & right) {
    return right <= left;
}

bool operator >= ( const std::string & left, const BigInt & right) {
    return right <= BigInt(left);
}

//// cast operations

//BigInt::operator bool() const {
//    return *this != 0;
//}

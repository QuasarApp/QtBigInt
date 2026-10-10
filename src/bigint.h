//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef BIGINT_H
#define BIGINT_H
#include "mini-gmp.h"
#include <string>
#include <vector>
#include "minigmp_global.h"

/**
 * @brief The BigInt class - c++ minigmp wrapper
 */

class MINIGMPSHARED_EXPORT BigInt
{
    mpz_t data;
public:
    BigInt();    
    BigInt(const BigInt& val, int bitCount = -1);
    BigInt(const std::string &imput, int base = 10);
    BigInt(intMpz val);
    BigInt(char item, unsigned int size, int base);

    std::string getString(int base = 10) const;
    ~BigInt();

    BigInt& powm(const BigInt &pow, const BigInt &mod);
    static BigInt powm(BigInt val, const BigInt & pow, const BigInt &mod);

    BigInt& pow(uIntMpz pow);
    BigInt& log(int base);
    /**
     * @brief Returns the size of the number in bits.
     *
     * The size of the number is the amount of memory allocated for the
     * number in bits (sizeBytes() * 8).
     * @return The size of the number in bits.
     */
    int sizeBits() const;
    int sizeBytes() const;

    /**
     * @brief Returns the current length of the number in bits.
     *
     * The length of the number is the number of bits in the base 2
     * (binary) representation of the number.
     * @return The current length of the number in bits.
     */
    int longBits() const;
    int longBytes() const;

    int sizeType() const;
    bool isPrime(bool absalut = false) const;
    BigInt& gcd(const BigInt &a, const BigInt &b);
    void fromHex(const std::string& hex);

    /**
     * @brief Returns 10 raised to the power of @a pow (10 ^ pow).
     *
     * The result is a new BigInt with the decimal representation "1"
     * followed by @a pow zeros.
     * @param pow The exponent of 10, i.e. the number of zeros following the
     *            leading "1" in the decimal representation of the result.
     * @return A new BigInt equal to 10 ^ pow.
     */
    static BigInt bigPow10(unsigned short pow);

    BigInt& toNegative();

    BigInt& operator = (const BigInt& val);
    BigInt& operator = (const std::string &imput);
    BigInt& operator = (intMpz val);

    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, const std::string &right);
    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT operator + ( intMpz left, BigInt right);
    friend BigInt MINIGMPSHARED_EXPORT operator + ( const std::string &left, const BigInt &right);

    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, const std::string &right);

    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, const std::string &right);

    friend BigInt MINIGMPSHARED_EXPORT operator - ( intMpz right, BigInt left);
    friend BigInt MINIGMPSHARED_EXPORT operator - ( const std::string &right, const BigInt &left);

    friend BigInt MINIGMPSHARED_EXPORT operator-(BigInt val);

    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, const std::string &right);

    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, const std::string &right);
    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT operator / ( intMpz left, BigInt right);
    friend BigInt MINIGMPSHARED_EXPORT operator / ( const std::string &left, const BigInt &right);

    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, const std::string &right);
    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, const BigInt& right);

    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, const std::string &right);
    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT operator * ( intMpz left, BigInt right);

    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, const std::string &right);

    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, const std::string &right);
    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT operator % ( intMpz left, BigInt right);
    friend BigInt MINIGMPSHARED_EXPORT operator % ( const std::string & left, const BigInt &right);

    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, intMpz right);
    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, const std::string &right);

    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, const BigInt& right);

    friend BigInt MINIGMPSHARED_EXPORT operator << ( BigInt left, int right);
    friend BigInt MINIGMPSHARED_EXPORT operator >> ( BigInt left, int right);

    friend BigInt MINIGMPSHARED_EXPORT & operator <<= ( BigInt &left, int right);
    friend BigInt MINIGMPSHARED_EXPORT & operator >>= ( BigInt &left, int right);

    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const std::string& right);
    friend bool MINIGMPSHARED_EXPORT operator == ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator == ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const std::string& str);
    friend bool MINIGMPSHARED_EXPORT operator != ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator != ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const std::string& str);
    friend bool MINIGMPSHARED_EXPORT operator < ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator < ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const std::string& str);
    friend bool MINIGMPSHARED_EXPORT operator > ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator > ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const std::string& str);
    friend bool MINIGMPSHARED_EXPORT operator <= ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator <= ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const BigInt& right);
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, intMpz right);
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const std::string& str);
    friend bool MINIGMPSHARED_EXPORT operator >= ( const std::string&  left, const BigInt&  right);
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const std::string&  right);
    friend bool MINIGMPSHARED_EXPORT operator >= ( intMpz left, const std::string&  right);

    friend bool MINIGMPSHARED_EXPORT operator!(const BigInt& val);

    BigInt& operator-- ();
    BigInt& operator++ ();

    BigInt operator-- (int);
    BigInt operator++ (int);


    friend BigInt MINIGMPSHARED_EXPORT operator~ (BigInt val);

    friend BigInt MINIGMPSHARED_EXPORT operator| (BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator| (const BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT & operator|= (BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator|= (BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT operator& (BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator& (const BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT & operator&= (BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator&= (BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT operator^ (BigInt left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT operator^ (const BigInt &left, intMpz right);

    friend BigInt MINIGMPSHARED_EXPORT & operator^= (BigInt &left, const BigInt& right);
    friend BigInt MINIGMPSHARED_EXPORT & operator^= (BigInt &left, intMpz right);

};

#endif // BIGINT_H

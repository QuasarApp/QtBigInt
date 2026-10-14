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
 * @brief The BigInt class - an arbitrary precision (unlimited size) integer type.
 *
 * BigInt is a small C++ wrapper around the bundled mini-gmp single-file
 * library (GMP 6.3.0). It stores integers of arbitrary size and overloads
 * the usual C++ operators, so it can be used almost everywhere a normal
 * integer type is expected:
 *
 * - **Arbitrary precision**: numbers of any size (e.g. 2^100000) without
 *   overflow, positive or negative.
 * - **Easy construction**: directly from C++ integers (64-bit `intMpz`)
 *   and from decimal / hex / any-base (2..36) strings.
 * - **Full operator set**: `+ - * / % << >> | & ^ ! ~` and the
 *   comparisons `== != < <= > >=`, plus the compound assignments
 *   `+= -= *= /= %= <<= >>= |= &= ^=` and `++ --`.
 * - **String conversion**: convert the value back to a string in any base
 *   from 2 to 36 (`getString()`), or set it from a hex string
 *   (`fromHex()`).
 * - **Math helpers**: `pow()`, `powm()` (modular exponentiation),
 *   `gcd()`, `isPrime()` and bit / byte size queries.
 *
 * The binary operators are available with the other operand as a `BigInt`,
 * a 64-bit C++ integer (`intMpz`) or a decimal `std::string`; the mixed
 * forms convert the operand automatically.
 *
 * Division (`/`) and the remainder (`%`) truncate toward zero, like in
 * C/C++; the result of `%` has the sign of the dividend.
 *
 * @warning BigInt instances are **not thread-safe**: do not modify the
 *          same object from several threads at the same time.
 *
 * ### Example
 *
 * The most common operations in a few lines:
 *
 * @code
 * #include <bigint.h>
 *
 * BigInt a = 1234567890;                                   // from an integer
 * BigInt b("9876543210123456789098765432101234567890");     // from a string
 *
 * BigInt c = a * b * 123456;                               // arbitrary precision
 *
 * std::cout << c.getString() << std::endl;
 * // Output: 1505331490682966620443288524512589666204282352096057600
 * @endcode
 *
 * A complete, ready-to-compile example is provided in the repository at
 * `examples/bigint_usage_example.cpp`:
 *
 * @snippet examples/bigint_usage_example.cpp usage
 */

class MINIGMPSHARED_EXPORT BigInt
{
    mpz_t data;
public:
    /**
     * @brief Default constructor.
     *
     * Creates a new BigInt initialized to zero.
     */
    BigInt();

    /**
     * @brief Copy constructor.
     *
     * Creates a copy of the number @a val.
     * @param val The number to copy.
     * @param bitCount Optional pre-allocation size in bits. When positive,
     *                 the internal storage is reserved for at least
     *                 @a bitCount bits (useful when many in-place
     *                 operations are expected); when @c -1 (default) the
     *                 storage is sized automatically.
     */
    BigInt(const BigInt& val, int bitCount = -1);

    /**
     * @brief Constructs a number from a string.
     *
     * Parses @a input as a number in the given @a base. The string may be
     * signed, e.g. `"-42"` or `"ff"` with base 16.
     * @param input The string to parse, e.g. `"12345678901234567890"`.
     * @param base The base of the number, from 2 to 36 (default 10,
     *             decimal).
     *
     * Note: use direct initialization when passing a string literal, e.g.
     * `BigInt b("123");` - the copy-initialization form
     * `BigInt b = "123";` is not valid C++, because a string literal
     * would require two implicit user-defined conversions in that case.
     */
    BigInt(const std::string &input, int base = 10);

    /**
     * @brief Constructs a number from a 64-bit signed integer.
     *
     * @param val A signed 64-bit value (`intMpz` is `long long`).
     */
    BigInt(intMpz val);

    /**
     * @brief Constructs a number from a run of identical digits.
     *
     * Creates a BigInt from a string consisting of @a size copies of the
     * character @a item, interpreted in the given @a base.
     * @param item The digit to repeat, e.g. `'1'`.
     * @param size How many times @a item is repeated.
     * @param base The base of the resulting number.
     *
     * Example: `BigInt('1', 3)` builds the string `"111"`, which is 7 in
     * base 10.
     */
    BigInt(char item, unsigned int size, int base);

    /**
     * @brief Returns the number as a string.
     * @param base The base of the result, from 2 to 36 (default 10,
     *             decimal).
     * @return The string representation of the number, e.g.
     *         `getString(16)` of 255 is `"ff"`.
     */
    std::string getString(int base = 10) const;

    /**
     * @brief Destructor. Releases the memory of the underlying GMP number.
     */
    ~BigInt();

    /**
     * @brief Modular exponentiation (in place).
     *
     * Replaces the number with `this ^ pow mod mod` using the fast
     * "square and multiply" algorithm of GMP, so it stays fast even for
     * very large exponents (e.g. RSA-sized numbers).
     * @param pow The exponent.
     * @param mod The modulus, must be greater than zero.
     * @return A reference to the updated object (*this).
     *
     * Example: `BigInt x = 3; x.powm(13, 50);` // x is now 42, because
     *          3^13 mod 50 == 42.
     */
    BigInt& powm(const BigInt &pow, const BigInt &mod);

    /**
     * @brief Modular exponentiation (static, returns a new number).
     *
     * @param val The base.
     * @param pow The exponent.
     * @param mod The modulus, must be greater than zero.
     * @return A new BigInt equal to `val ^ pow mod mod`.
     */
    static BigInt powm(BigInt val, const BigInt & pow, const BigInt &mod);

    /**
     * @brief Raises the number to an integer power (in place).
     *
     * Replaces the number with `this ^ pow`.
     * @param pow The exponent (an unsigned 64-bit value).
     * @return A reference to the updated object (*this).
     *
     * Example: `BigInt x = 2; x.pow(10);` // x is now 1024.
     */
    BigInt& pow(uIntMpz pow);

    /**
     * @brief Integer logarithm in the given base (in place).
     *
     * @note This method is declared but **not implemented** in the
     *       current version of QtBigInt; it is kept only for API
     *       compatibility. Do not use it. Use the bit size queries
     *       (`sizeBits()`, `longBits()`) or repeated multiplication with
     *       `pow()` as alternatives.
     * @param base The base of the logarithm.
     * @return A reference to the updated object (*this).
     */
    BigInt& log(int base);

    /**
     * @brief Returns the size of the number in bits.
     *
     * The size of the number is the amount of memory allocated for the
     * number in bits (sizeBytes() * 8).
     * @return The size of the number in bits.
     */
    int sizeBits() const;

    /**
     * @brief Returns the amount of memory allocated for the number in bytes.
     * @return The allocated size of the number in bytes.
     */
    int sizeBytes() const;

    /**
     * @brief Returns the current length of the number in bits.
     *
     * The length of the number is the number of bits in the base 2
     * (binary) representation of the number. For negative numbers the
     * sign character of the string representation is counted as well.
     * @return The current length of the number in bits.
     */
    int longBits() const;

    /**
     * @brief Returns the current length of the number in bytes.
     *
     * The length in bytes is the bit length (`longBits()`) rounded up to
     * the next full byte.
     * @return The current length of the number in bytes.
     */
    int longBytes() const;

    /**
     * @brief Returns the size of the internal number type in bytes.
     *
     * The returned value is the amount of storage used by the internal
     * GMP representation of the number, including the sign limb.
     * @return The size of the number type in bytes.
     */
    int sizeType() const;
    /**
     * @brief Tests if the number is prime.
     *
     * Uses the probable prime test of GMP (Miller-Rabin with 50 rounds,
     * plus trial division for small numbers). For numbers below 961 the
     * result is exact.
     * @param absolute When @c false (default), the number is reported as
     *                 prime if GMP says it is definitely **or** probably
     *                 prime. When @c true, only numbers that GMP proves
     *                 to be prime are reported as prime; "probably
     *                 prime" results are reported as @c false (in
     *                 practice this means only small numbers can return
     *                 @c true).
     * @return True if the number is (probably) prime, false otherwise.
     */
    bool isPrime(bool absolute = false) const;

    /**
     * @brief Computes the greatest common divisor of two numbers (in place).
     *
     * Replaces this number with `gcd(a, b)` of @a a and @a b. The result
     * is always non-negative.
     * @param a The first number.
     * @param b The second number.
     * @return A reference to the updated object (*this).
     *
     * Example: `BigInt g; g.gcd(1071, 462);` // g is now 21.
     */
    BigInt& gcd(const BigInt &a, const BigInt &b);

    /**
     * @brief Sets the number from a hexadecimal string.
     *
     * Parses @a hex as a base-16 (hexadecimal) number and replaces the
     * current value. The `0x` prefix is not required.
     * @param hex The hexadecimal string, e.g. `"ff"` for 255.
     */
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

    /**
     * @brief Negates the number in place.
     *
     * Changes the sign of the number, i.e. the value becomes `-value`.
     * @return A reference to the updated object (*this).
     */
    BigInt& toNegative();

    /**
     * @brief Assignment from another BigInt (copy).
     * @param val The number to copy.
     * @return A reference to the updated object (*this).
     */
    BigInt& operator = (const BigInt& val);

    /**
     * @brief Assignment from a decimal string.
     * @param input The decimal number as a string, e.g. `"12345"`.
     * @return A reference to the updated object (*this).
     */
    BigInt& operator = (const std::string &input);

    /**
     * @brief Assignment from a 64-bit signed integer.
     * @param val The value to store (`intMpz` is `long long`).
     * @return A reference to the updated object (*this).
     */
    BigInt& operator = (intMpz val);

    /** @brief Returns @c left + @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, const BigInt& right);

    /** @brief Returns @c left + @c right, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, const std::string &right);

    /** @brief Returns @c left + @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator + ( BigInt left, intMpz right);

    /** @brief Returns @c left + @c right, where @c left is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator + ( intMpz left, BigInt right);

    /** @brief Returns @c left + @c right, where @c left is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator + ( const std::string &left, const BigInt &right);

    /** @brief Adds @c right (a 64-bit integer) to @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, intMpz right);

    /** @brief Adds @c right to @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, const BigInt& right);

    /** @brief Adds @c right (a decimal string) to @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator += ( BigInt &left, const std::string &right);

    /** @brief Returns @c left - @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, const BigInt& right);

    /** @brief Returns @c left - @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, intMpz right);

    /** @brief Returns @c left - @c right, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator - ( BigInt left, const std::string &right);

    /** @brief Returns @c right - @c left, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator - ( intMpz right, BigInt left);

    /** @brief Returns @c right - @c left, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator - ( const std::string &right, const BigInt &left);

    /** @brief Unary minus: returns the negation of @c val. */
    friend BigInt MINIGMPSHARED_EXPORT operator-(BigInt val);

    /** @brief Subtracts @c right (a 64-bit integer) from @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, intMpz right);

    /** @brief Subtracts @c right from @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, const BigInt& right);

    /** @brief Subtracts @c right (a decimal string) from @c left and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator -= ( BigInt &left, const std::string &right);

    /** @brief Returns @c left / @c right (truncating division, toward zero). */
    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, const BigInt& right);

    /** @brief Returns @c left / @c right, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, const std::string &right);

    /** @brief Returns @c left / @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator / ( BigInt left, intMpz right);

    /** @brief Returns @c left / @c right, where @c left is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator / ( intMpz left, BigInt right);

    /** @brief Returns @c left / @c right, where @c left is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator / ( const std::string &left, const BigInt &right);

    /** @brief Divides @c left by @c right (a 64-bit integer) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, intMpz right);

    /** @brief Divides @c left by @c right (a decimal string) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, const std::string &right);

    /** @brief Divides @c left by @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator /= ( BigInt &left, const BigInt& right);

    /** @brief Returns @c left * @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, const BigInt& right);

    /** @brief Returns @c left * @c right, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, const std::string &right);

    /** @brief Returns @c left * @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator * ( BigInt left, intMpz right);

    /** @brief Returns @c left * @c right, where @c left is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator * ( intMpz left, BigInt right);

    /** @brief Multiplies @c left by @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, const BigInt& right);

    /** @brief Multiplies @c left by @c right (a 64-bit integer) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, intMpz right);

    /** @brief Multiplies @c left by @c right (a decimal string) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator *= ( BigInt &left, const std::string &right);

    /** @brief Returns @c left % @c right (the sign of the result is the sign of @c left). */
    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, const BigInt& right);

    /** @brief Returns @c left % @c right, where @c right is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, const std::string &right);

    /** @brief Returns @c left % @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator % ( BigInt left, intMpz right);

    /** @brief Returns @c left % @c right, where @c left is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator % ( intMpz left, BigInt right);

    /** @brief Returns @c left % @c right, where @c left is a decimal string. */
    friend BigInt MINIGMPSHARED_EXPORT operator % ( const std::string & left, const BigInt &right);

    /** @brief Replaces @c left with @c left % @c right (a 64-bit integer) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, intMpz right);

    /** @brief Replaces @c left with @c left % @c right (a decimal string) and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, const std::string &right);

    /** @brief Replaces @c left with @c left % @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator %= ( BigInt &left, const BigInt& right);

    /** @brief Returns @c left shifted left by @c right bits (multiplication by 2^right). */
    friend BigInt MINIGMPSHARED_EXPORT operator << ( BigInt left, int right);

    /** @brief Returns @c left shifted right by @c right bits (division by 2^right). */
    friend BigInt MINIGMPSHARED_EXPORT operator >> ( BigInt left, int right);

    /** @brief Shifts @c left left by @c right bits and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator <<= ( BigInt &left, int right);

    /** @brief Shifts @c left right by @c right bits and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator >>= ( BigInt &left, int right);

    /** @brief Returns true if @c left and @c right have the same value. */
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left equals the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left equals the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const std::string& right);

    /** @brief Returns true if the decimal string @c left equals @c right. */
    friend bool MINIGMPSHARED_EXPORT operator == ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left equals the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator == ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left equals the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator == ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c left and @c right have different values. */
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left differs from the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left differs from the decimal string @c str. */
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const std::string& str);

    /** @brief Returns true if the decimal string @c left differs from @c right. */
    friend bool MINIGMPSHARED_EXPORT operator != ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left differs from the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator != ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left differs from the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator != ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c left < @c right. */
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left < the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left < the decimal string @c str. */
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const std::string& str);

    /** @brief Returns true if the decimal string @c left < @c right. */
    friend bool MINIGMPSHARED_EXPORT operator < ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left < the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator < ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left < the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator < ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c left > @c right. */
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left > the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left > the decimal string @c str. */
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const std::string& str);

    /** @brief Returns true if the decimal string @c left > @c right. */
    friend bool MINIGMPSHARED_EXPORT operator > ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left > the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator > ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left > the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator > ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c left <= @c right. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left <= the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left <= the decimal string @c str. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const std::string& str);

    /** @brief Returns true if the decimal string @c left <= @c right. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left <= the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left <= the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator <= ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c left >= @c right. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const BigInt& right);

    /** @brief Returns true if @c left >= the 64-bit integer @c right. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, intMpz right);

    /** @brief Returns true if @c left >= the decimal string @c str. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const std::string& str);

    /** @brief Returns true if the decimal string @c left >= @c right. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( const std::string&  left, const BigInt&  right);

    /** @brief Returns true if @c left >= the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( const BigInt& left, const std::string&  right);

    /** @brief Returns true if the 64-bit integer @c left >= the decimal string @c right. */
    friend bool MINIGMPSHARED_EXPORT operator >= ( intMpz left, const std::string&  right);

    /** @brief Returns true if @c val is zero (logical negation). */
    friend bool MINIGMPSHARED_EXPORT operator!(const BigInt& val);

    /**
     * @brief Prefix decrement: subtracts 1 and returns the updated object.
     * @return A reference to the updated object (*this).
     */
    BigInt& operator-- ();

    /**
     * @brief Prefix increment: adds 1 and returns the updated object.
     * @return A reference to the updated object (*this).
     */
    BigInt& operator++ ();

    /**
     * @brief Postfix decrement: returns the old value, then subtracts 1.
     * @return A copy of the object with the value before the decrement.
     */
    BigInt operator-- (int);

    /**
     * @brief Postfix increment: returns the old value, then adds 1.
     * @return A copy of the object with the value before the increment.
     */
    BigInt operator++ (int);

    /** @brief Bitwise NOT: returns the one's complement of @c val. */
    friend BigInt MINIGMPSHARED_EXPORT operator~ (BigInt val);

    /** @brief Bitwise OR: returns @c left | @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator| (BigInt left, const BigInt& right);

    /** @brief Bitwise OR: returns @c left | @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator| (const BigInt &left, intMpz right);

    /** @brief Bitwise OR assignment: replaces @c left with @c left | @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator|= (BigInt &left, const BigInt& right);

    /** @brief Bitwise OR assignment with a 64-bit integer, returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator|= (BigInt &left, intMpz right);

    /** @brief Bitwise AND: returns @c left & @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator& (BigInt left, const BigInt& right);

    /** @brief Bitwise AND: returns @c left & @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator& (const BigInt &left, intMpz right);

    /** @brief Bitwise AND assignment: replaces @c left with @c left & @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator&= (BigInt &left, const BigInt& right);

    /** @brief Bitwise AND assignment with a 64-bit integer, returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator&= (BigInt &left, intMpz right);

    /** @brief Bitwise XOR: returns @c left ^ @c right. */
    friend BigInt MINIGMPSHARED_EXPORT operator^ (BigInt left, const BigInt& right);

    /** @brief Bitwise XOR: returns @c left ^ @c right, where @c right is a 64-bit integer. */
    friend BigInt MINIGMPSHARED_EXPORT operator^ (const BigInt &left, intMpz right);

    /** @brief Bitwise XOR assignment: replaces @c left with @c left ^ @c right and returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator^= (BigInt &left, const BigInt& right);

    /** @brief Bitwise XOR assignment with a 64-bit integer, returns @c left. */
    friend BigInt MINIGMPSHARED_EXPORT & operator^= (BigInt &left, intMpz right);

};

/** \example examples/bigint_usage_example.cpp
 * A complete, ready-to-compile usage example for the BigInt class:
 * construction from integers and strings, arbitrary precision arithmetic,
 * powers, modular exponentiation, gcd, primality testing, comparisons and
 * string conversions in different bases.
 */

#endif // BIGINT_H

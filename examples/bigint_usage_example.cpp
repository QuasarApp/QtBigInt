//#
//# Copyright (C) 2021-2026 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

/**
 * @brief A complete, ready-to-compile usage example for the BigInt class.
 *
 * This program shows the most common operations of the @ref BigInt class:
 * construction from integers and strings, arbitrary precision arithmetic,
 * powers, modular exponentiation, gcd, primality testing, comparisons and
 * string conversions in different bases.
 *
 * It can be compiled directly against the library sources, e.g.:
 *
 * @code
 * g++ -std=c++17 -I ../src bigint_usage_example.cpp \
 *     ../src/bigint.cpp -x c++ ../src/mini-gmp.c -o bigint_usage_example
 * @endcode
 *
 * or through CMake by linking your target with the `QtBigInt` target.
 */

// [usage]
#include <bigint.h>

#include <iostream>

/**
 * @brief Entry point of the usage example.
 * @return 0 on success.
 */
int main() {
    // ------------------------------------------------------------------
    // 1. Construction: from a 64-bit integer, from a decimal string,
    //    from a hex string, or from a run of identical digits.
    // ------------------------------------------------------------------
    BigInt a = 1234567890;                                   // from an integer
    BigInt b("9876543210123456789098765432101234567890");    // from a string
    BigInt hex;
    hex.fromHex("deadbeef");                                 // 3735928559

    std::cout << "a   = " << a.getString() << "\n";
    std::cout << "b   = " << b.getString() << "\n";
    std::cout << "hex = " << hex.getString() << "\n";

    // ------------------------------------------------------------------
    // 2. Arbitrary precision arithmetic with the built-in operators.
    // ------------------------------------------------------------------
    BigInt product = a * b * 123456;                         // no overflow!
    BigInt sum     = a + b + "1";
    BigInt diff    = b - a;

    std::cout << "a * b * 123456 = " << product.getString() << "\n";
    std::cout << "a + b + 1      = " << sum.getString() << "\n";
    std::cout << "b - a          = " << diff.getString() << "\n";

    // ------------------------------------------------------------------
    // 3. Powers, modular exponentiation, gcd and primality.
    // ------------------------------------------------------------------
    BigInt pow2 = 2;
    pow2.pow(100);                                           // 2^100

    BigInt modExp = 3;
    modExp.powm(13, 50);                                     // 3^13 mod 50

    BigInt g;
    g.gcd(1071, 462);                                        // 21

    std::cout << "2^100        = " << pow2.getString() << "\n";
    std::cout << "3^13 mod 50  = " << modExp.getString() << "\n";
    std::cout << "gcd(1071,462) = " << g.getString() << "\n";
    std::cout << "isPrime(97)  = " << (BigInt(97).isPrime() ? "true" : "false") << "\n";

    // ------------------------------------------------------------------
    // 4. Comparisons (BigInt, 64-bit integers and decimal strings).
    // ------------------------------------------------------------------
    std::cout << "(a < b)   = " << (a < b ? "true" : "false") << "\n";
    std::cout << "(a == 1234567890) = " << (a == 1234567890 ? "true" : "false") << "\n";

    // ------------------------------------------------------------------
    // 5. String conversions in different bases (2 .. 36).
    // ------------------------------------------------------------------
    std::cout << "b in hex   = " << b.getString(16) << "\n";
    std::cout << "b in bin   = " << b.getString(2) << "\n";
    std::cout << "hex as bin = " << hex.getString(2) << "\n";

    return 0;
}
// [usage]
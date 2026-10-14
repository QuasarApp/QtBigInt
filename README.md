# QtBigInt

**QtBigInt** - Arbitrary-sized integer class for C++ and build system cmake. Powered by minigmp 6.3.0.

![QtBigInt Logo](res/png/QtBigint_Banner_web.png)


# Features
* No additional dependencies other than the standard library.
* Support for all arithmetic operators, which allows you to easily integrate the library into any of your algorithms.
* Based on the minigmp 6.3.0 library that provides high performance.

# Documentation
The documentation and the API reference are hosted on [GitHub Pages](https://quasarapp.github.io/QtBigInt/).
The API reference is generated with Doxygen from the source code.

# Include (cmake)

#### The cmake build does not require Qt libraries. 
 
 * cd yourRepo
 * git submodule add https://github.com/QuasarApp/QtBigInt.git # add the repository of QtBigInt into your repo like submodule
 * git submodule update --init --recursive
 * Disable Building of tests (because tests require Qt libraries). Add before including of QtBigInt next line :
  >> set(WITHOUT_TESTS 1)
 * Include in your CMakeLists.txt file the main CMakeLists.txt file of QtBigInt library
  >> add_subdirectory(QtBigInt)
 * Rebuild your project

``` cmake
set(WITHOUT_TESTS 1)
add_subdirectory(QtBigInt)
target_link_libraries(MyBinary PUBLIC QtBigInt)
```

### Note 
By Default QtBigInt is built as a static library. If you want to create a shared library just add the BUILD_SHARED_LIBS into your main CMakeLists.txt file.
Example :

``` cmake
set(BUILD_SHARED_LIBS ON)
set(WITHOUT_TESTS 1)
add_subdirectory(QtBigInt)
target_link_libraries(MyBinary PUBLIC QtBigInt)

```

## For other build system  
 
 * cd yourRepo
 * git submodule add https://github.com/QuasarApp/QtBigInt.git # add the repository of QtBigInt into your repo like submodule
 * git submodule update --init --recursive
 * Add the rule for build QtBigInt
 * Add INCLUDEPATH and LIBS for your build system 
 * Rebuild your project


# Usage
Create objects of the BigInt class, and do what you got to do!

``` cpp
#include <bigint.h>

BigInt big1 = 1234567890, big2;
big2 = "9876543210123456789098765432101234567890";

std::cout << (big1 * big2 * 123456).getString() << "\n";
// Output: 1505331490682966620443288524512589666204282352096057600
```

A complete, ready-to-compile example is available in
[examples/bigint_usage_example.cpp](https://github.com/QuasarApp/QtBigInt/blob/main/examples/bigint_usage_example.cpp).

# License
QtBigInt is distributed under the [GNU Lesser General Public License v3.0](LICENSE) (LGPL-3.0).


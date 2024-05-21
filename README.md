# QuickCheck in C++

[![en](https://img.shields.io/badge/lang-en-green.svg)](./README.md)
[![de](https://img.shields.io/badge/lang-de-red.svg)](./README.de.md)

## What is QuickCheck?

[GitHub Repository](https://github.com/emil-e/rapidcheck)<br>
[Website](https://www.cse.chalmers.se/~rjmh/QuickCheck/)<br>
[Wikipedia](https://en.wikipedia.org/wiki/QuickCheck)

QuickCheck is a software testing tool that helps developers test their code.

Originally developed for the Haskell programming language,
it uses a technique called property-based testing to automatically generate test cases.

Developers define general properties that their code should satisfy,
such as "every sorted list should be in non-decreasing order",
and QuickCheck generates random input data to test those properties.

This approach helps find problems that traditional testing might miss,
making it a great tool for ensuring robust software development.

QuickCheck has been adapted for several other programming languages,
making it useful in a variety of programming environments.

### What is Property-based testing?

Property-based testing is a software testing method where tests are designed around the properties
that a program should always satisfy.

## Object Oriented (OO)

This code snippet is a simple framework for implementing generic object generators 
and testing them against specified conditions or predicates. 

It's a basic form of what might be used in automated testing frameworks 
where various types of objects need to be automatically generated and validated against expected conditions.

```c++ 
template<typename T>
class GenOO {
  public:
    virtual T generate() = 0;
};

template<typename T>
void quickCheckOO(GenOO<T>* g, bool p(T)) {
    for(int i =0; i < 20; i++) {
        auto b = p(g->generate());
        if (b) {
            cout << "\n +++ OK";
        } else {
            cout << "\n *** Failed";
        }
    }
}
```

## Functional Programming (FP)

```c++ 
template<typename T>
class Gen {
  public:
    Gen() {};
    Gen(std::function<T()> gen_) { gen = std::function<T()>(gen_); }
    
    std::function<T()> gen;
    T generate() { return gen(); };
};

template<typename T>
Gen<T> arbitrary();

template<typename T>
void quickCheck(bool p(T)) {
    Gen<T> g = arbitrary<T>();

    for(int i =0; i < 20; i++) {
        auto b = p(g.generate());
        if (b) {
            cout << "\n +++ OK";
        } else {
            cout << "\n *** Failed";
        }
    }
}
```

## Rapid RapidCheck (RC)

[Github](https://github.com/emil-e/rapidcheck)

`target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest rapidcheck)`

```c++ 
#include "rapidcheck.h"

#include <vector>
#include <algorithm>

int main() {
    rc::check("double reversal yields the original value", [](const std::vector<int> &l0) {
        auto l1 = l0;
        std::reverse(begin(l1), end(l1));
        std::reverse(begin(l1), end(l1));
        RC_ASSERT(l0 == l1);
    });
    
    return 0;
}
```

### RC with Google Tests

`target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest rapidcheck rapidcheck_gtest)`

```c++ 
#include "gtest/gtest.h"
#include "rapidcheck/gtest.h"

RC_GTEST_PROP(MyTestCase, copyOfStringIsIdenticalToOriginal, (const std::string &str)) {
    const auto strCopy = str;
    RC_ASSERT(strCopy == str);
}
```


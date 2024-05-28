# QuickCheck in C++

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

The provided code demonstrates a functional programming approach in C++ 
using template-based generators to create random values of various types.

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

### Bool Generator

It uses std::random_device and std::mt19937 for random number generation.

It creates a uniform distribution over integers 0 and 1, 
casting the result to bool to return either true or false.

```c++ 
template<>
Gen<bool> arbitrary<bool>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(0, 1);
        return static_cast<bool>(dis(gen));
    }};
}
```

### String Generator

- The generated string has a length between 1 and 40 lowercase characters.
- It uses std::uniform_int_distribution to randomly decide the string length and whether to insert a space or a character.
- The total number of spaces is limited to 10, and no more than 5 spaces can appear consecutively.
- If a string contains more than 5 consecutive spaces, they are replaced by exactly 5 spaces.

```c++ 
template<>
Gen<std::string> arbitrary<std::string>() {
return {[]() {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<size_t> len_dis(1, 40); // length
static std::uniform_int_distribution<int> space_dis(0, 1); // space

        size_t len = len_dis(gen);
        std::string str;
        str.reserve(len);

        int space_count = 0;
        int consecutive_spaces = 0;

        for (size_t i = 0; i < len; ++i) {
            if (space_count < 10 && space_dis(gen) == 1 && consecutive_spaces < 5) {
                str += ' ';
                ++space_count;
                ++consecutive_spaces;
            } else {
                str += arbitrary<char>().generate();
                consecutive_spaces = 0;
            }
        }

        // replace spaces > 5
        size_t pos = 0;
        while ((pos = str.find("      ", pos)) != std::string::npos) {
            str.replace(pos, 6, "     "); // replace
            pos += 5;
        }

        return str;
    }};
}
```

### String List Generator

generates a vector of strings, where the number of strings is between 0 and 10.

Each string in the vector is generated using the arbitrary<std::string> generator, 
ensuring they meet the previously defined constraints.
The vector's length is determined using std::uniform_int_distribution.

```c++
template<>
Gen<std::vector<std::string>> arbitrary<std::vector<std::string>>() {
    return {[]() {
        std::vector<std::string> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> lenDist(0, 10); // length

        int length = lenDist(gen);
        result.reserve(length); // reserve

        for (int i = 0; i < length; ++i) {
            result.push_back(arbitrary<std::string>().generate());
        }

        return result;
    }};
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


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

## Object Oriented (OO) Approach

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

## Functional Programming (FP) Approach

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

It uses `std::random_device` and `std::mt19937` for random number generation.

It creates a uniform distribution over integers `0` and `1`, 
casting the result to `bool` to return either `true` or `false`.

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

- The generated `string` has a length between `1` and `40` lowercase characters.
- It uses `std::uniform_int_distribution` to randomly decide the string length and number of spaces to insert.
- The total number of spaces is limited to `10`, and no more than `5` spaces can appear consecutively.
- After all the `chars` are generated the spaces are appended.
- The `string` then gets shuffled using the random generator.
- If a `string` contains more than `5` consecutive spaces, they are replaced by exactly `5` spaces.

```c++ 
template<>
Gen<std::string> arbitrary<std::string>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> len_dis(1, 40);
        static std::uniform_int_distribution<size_t> space_dis(0, 10);

        size_t length = len_dis(gen);
        size_t numSpaces = 0;
        size_t numChars = 0;

        if(length > 0) {
            numSpaces = space_dis(gen);
            numSpaces = numSpaces >= length ? length - 1: numSpaces;
            numChars = length - numSpaces;
        }

        std::string str;
        str.reserve(length);

        // generate chars and append them
        for (size_t i = 0; i < numChars; ++i) {
            str.append(1, arbitrary<char>().generate());
        }

        // append spaces
        str.append(numSpaces, ' ');

        // shuffle the string to distribute spaces and characters more randomly
        std::shuffle(str.begin(), str.end(), gen);

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

generates a vector of strings, where the number of strings is between `0` and `10`.

Each string in the vector is generated using the `arbitrary<std::string>` generator, 
ensuring they meet the previously defined constraints.
The vector's length is determined using `std::uniform_int_distribution`.

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

### Comparison

#### Usage
- GenOO: Create an instance of the `IntGen` class and call its `generate` method
    ```c++
    IntGen intGen();
    int randomInt = intGen.generate();
    ```
- Gen: Define a generator using a lambda function and call the `generate` method
    ```c++
    Gen<int> intGen = arbitrary<int>();
    int randomInt = intGen.generate();
    ```

#### Extending
- GenOO: Adding new types of generators can be done by creating new classes inheriting from GenOO.
    ```c++
    class FloatGen : public GenOO<float> {
        // Implementation
    };  
    ```
- Gen: Adding new types of generators involves specializing the arbitrary function template.
    ```c++
    template<>
    Gen<float> arbitrary<float>() {
    return {[]() { return /* generate float values */ 0.0 }};
    };
    ```
#### Differences
The main differences between the two approaches are in how the code is generated and managed by the compiler.

The class-based approach involves more complex setup and management of virtual method tables, while the function-based approach relies on templates and lambda expressions, which can result in efficient inline code.

1. **GenOO Object Oriented Approach**
   - **Class Definition:**
     - The compiler processes the class definitions and sets up the necessary structures for inheritance and virtual methods.
     - Generates constructors, destructors, and virtual tables.
   - **Virtual Method Tables:**
     - For each class with virtual methods, the compiler generates a virtual method table that maps virtual function calls to the actual method implementations.
   - **Object Code Generation:**
     - Generates object code for the class methods and manages the virtual method calls through the virtual table.


2. **Gen Function Based Approach**
   - **Template Instantiation:**
     - The compiler instantiates the `Gen<T>` template for each type `T` used.
     - Generates code for the lambda expressions and inline functions.
   - **Function Objects:**
     - For each lambda, the compiler generates a unique function object that captures any necessary state.
     - If lambdas are stateless, the function objects are minimal and can often be inlined.
   - **Inlined Code:**
     - If possible, the compiler inlines the lambda function calls, resulting in efficient, direct function calls without additional overhead.

## Rapid RapidCheck (RC)
RapidCheck offers a higher-level abstraction focused on testing properties.

[Github RapidCheck](https://github.com/emil-e/rapidcheck)

`target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest rapidcheck)`

`RC_PARAMS` environment variable. The configuration string has the format `<key1>=<value1> <key2>=<value2> ...`.
The following settings are provided:
- `seed` - The global random seed used
- `max_success` - The maximum number of successful test cases to run before deciding that a property holds. Defaults to `100`.
- `max_discard_ratio` - The maximum number of discarded test cases per successful test case. If exceeded, RapidCheck gives up on the property. Defaults to `10`.
- `noshrink` - If set to `1`, disables test case shrinking. Defaults to `0`.
- `reproduce` - String that encodes the information necessary to reproduce minimal failures for properties.

### Templating
RapidCheck uses templates extensively for type-safe property definitions and generators.

The template instantiation can increase compile times, similar to the functional (FP) approach.

```c++ 
#include "rapidcheck.h"

#include <vector>
#include <algorithm>

int main() {
    rc::check("addition is commutative", [](int a, int b) {
        RC_ASSERT(a + b == b + a);
    });
    
    return 0;
}
```
`RC_ASSERT` is used to validate properties. 
If the assertion fails, RapidCheck will print the failing case and attempt to simplify it to find the smallest failing input.

### Shrinking
When a test fails, RapidCheck tries to minimize the failing input to find the simplest case that still fails, helping to diagnose the problem.

```c++
// Generates four boolean values
// asserts that a specific combination does not occur (1001)
RC_GTEST_PROP(RapidCheckTests, BooleanTest, (bool val1, bool val2, bool val3, bool val4)) {
    std::cout << val1 << " " << val2 << " " << val3 << " " << val4 << std::endl;
    RC_ASSERT(!(val1 == true && val2 == false && val3 == false && val4 == true));
}
```
```
Testing started at 17:49 ...
Using configuration: seed=1719892174
Some of your RapidCheck properties had failures. To reproduce these, run with:
RC_PARAMS="reproduce=BshUhBXakNEalN2aUV2c0N3LC92bsVWYuRVZzRnz4NoZAAAAA4MeDaGAAAAAOj3gmBAAAAgz4NoZAAAAAAIBAoQCAAAAAA"
0 0 0 0
0 0 0 0
1 0 1 1
0 0 0 0
1 0 1 1
0 0 0 0
1 1 1 1
0 1 0 1
0 0 0 1
1 0 0 1
1 0 0 1
0 0 0 1
1 0 0 0
1 0 0 1
C:/.../gtest.h:29: Failure
Failed
Falsifiable after 10 tests

std::tuple<bool, bool, bool, bool>:
(true, false, false, true)
```

**Another Example:** If a property fails for a large list, RapidCheck might shrink it down to the smallest list that still fails.

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
`RC_GTEST_PROP`: These are RapidCheck tests integrated with Google Test.

### Arbitrary support for custom types
RapidCheck allows users to define how arbitrary instances of their custom types should be generated. 

This is done by specializing the Arbitrary template for the custom type.

This feature ensures that the generated data is meaningful and tailored to the specific needs of the type being tested.

#### Custom Generator for Role Enum:
This is a implementation of a custom generator for the Role enum. 
The arbitrary function generates either STUDENT or TEACHER randomly.

```c++
namespace rc {
    // Custom generator for Role enum
    template<>
    struct Arbitrary<Role> {
        static Gen<Role> arbitrary() {
            return gen::element(STUDENT, TEACHER);
        }
    };
} // namespace rc
```

#### Custom Generator for Person Class:
This is an implementation of a custom generator for the Person class. 
It uses gen::build to construct a Person object by setting its fields with randomly chosen values:

- `firstName` is chosen from a list of first names.
- `lastName` is chosen from a list of last names.
- `age` is chosen from a range between `0` and `150`.
- `role` is set using the custom generator for `Role`.

```c++
namespace rc {
    // Custom generator for Person class
    template<>
    struct Arbitrary<Person> {
        static Gen<Person> arbitrary() {
            std::vector<std::string> firstNames{"John", "Jane", "Daniel", "Marvin"};
            std::vector<std::string> lastNames{"Cindric", "Müller", "Doe"};
            return gen::build<Person>(
                    gen::set(&Person::firstName, gen::elementOf(firstNames)),
                    gen::set(&Person::lastName, gen::elementOf(lastNames)),
                    gen::set(&Person::age, gen::inRange(0, 150)),
                    gen::set(&Person::role));
        }
    };
} // namespace rc
```

### Grouping generated values with Tags

RapidCheck allows users to tag generated values in test properties. 
This feature helps in understanding the distribution of generated values 
and can be useful for debugging and analysis purposes.

#### Tagging Role Enum:
```c++
RC_GTEST_PROP(RapidCheckTests, RoleTest, (Role r)) {
    const auto rStr = r == STUDENT ? "Student" : "Lehrer";
    RC_TAG(rStr);
}
```

```
Testing started at 14:38 ...
Using configuration: seed=1719891501
OK, passed 100 tests
59.00% - "Lehrer"
41.00% - "Student"
```

#### Tagging Person Class
```c++
RC_GTEST_PROP(RapidCheckTests, PersonTest, (Person p)) {
    RC_TAG(p.firstName);
    RC_TAG(p.lastName);
    RC_TAG(p.role == STUDENT ? "Student" : "Lehrer");
}
```

```
Testing started at 15:27 ...
Using configuration: seed=1719891323

OK, passed 100 tests
 9.00% - "Marvin", "Müller", "Lehrer"
 9.00% - "John", "Müller", "Student"
 8.00% - "Daniel", "Doe", "Lehrer"
 7.00% - "Marvin", "Müller", "Student"
 7.00% - "Daniel", "Müller", "Student"
 ...
 2.00% - "Marvin", "Doe", "Student"
 2.00% - "Daniel", "Doe", "Student"
 1.00% - "John", "Müller", "Lehrer"
 1.00% - "Marvin", "Doe", "Lehrer"

Process finished with exit code 0
```
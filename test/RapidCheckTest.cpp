#include "gtest/gtest.h"
#include "rapidcheck/gtest.h"
#include "reverseMethods.h"
#include "multiplicationMethods.h"
#include "Person.h"

#include <vector>

//// first examples

// Generates a random integer
// asserts that doubling it equals multiplying by 2
RC_GTEST_PROP(RapidCheckFirstTests, IntTest, ()) {
    const auto num = *rc::gen::inRange(0, 1000);
    std::cout << num << std::endl;
    RC_ASSERT((num + num) == (num * 2));
}

// Generates a random character between 'A' and 'Z'
// asserts that it is an alphabetic character
RC_GTEST_PROP(RapidCheckFirstTests, CharTest, ()) {
    const auto chr = *rc::gen::inRange('A', 'Z');
    std::cout << chr << std::endl;
    RC_ASSERT(std::isalpha(chr));
}

// Generates two random integers, one positive and one negative
// asserts that their sum is commutative
RC_GTEST_PROP(RapidCheckFirstTests, TwoIntsTest, ()) {
    const auto num1 = *rc::gen::inRange(0, 1000);
    const auto num2 = *rc::gen::inRange(-1000, 0);
    std::cout << num1 << ", " << num2 << std::endl;
    RC_ASSERT((num1 + num2) == (num2 + num1));
}

// Generates a random string
// asserts that it is empty if its size is 0
RC_GTEST_PROP(RapidCheckFirstTests, StringTest, ()) {
    const auto str = *rc::gen::arbitrary<std::string>();
    std::cout << str << std::endl;
    RC_ASSERT(str.empty() == (str.size() == 0));
}

// Generates a vector of integers with a maximum size of 10
RC_GTEST_PROP(RapidCheckTests, VectorTest, ()) {
    const auto listGen = *rc::gen::container<std::vector<int>>(rc::gen::inRange(0, 100));
    RC_PRE(listGen.size() <= 10);

    for (auto v : listGen)
        std::cout << v << ", ";
    std::cout << std::endl;
}

// Generates a string from a predefined set and asserts that reversing it using two methods yields the same result.
RC_GTEST_PROP(RapidCheckTest, CompareReverseMethods, ()) {
    const auto a = *rc::gen::sizedElement(
            std::string("one"),
            std::string("two"),
            std::string("three"));

    std::cout << a << std::endl;
    auto res1 = reverseWithStdReverse(a);
    auto res2 = reverseWithSwap(a);
    RC_ASSERT(res1 == res2);
}


//// property-based checks

// Generates a random integer
// asserts that doubling it equals multiplying by 2
TEST(RapidCheckPropertiesTests, IntTest) {
    rc::check([](const int &num) {
        std::cout << num << std::endl;
        RC_ASSERT((num + num) == (num * 2));
    });
}

// Generates a random string
// asserts that it is empty if its size is 0
TEST(RapidCheckPropertiesTests, StringTest) {
    rc::check([](const std::string &str) {
        std::cout << str << std::endl;
        RC_ASSERT(str.empty() == (str.size() == 0));
    });
}


//// Gtest integration

// Generates a random integer
// asserts that doubling it equals multiplying by 2
RC_GTEST_PROP(RapidCheckGTests, IntTest, (const int &num)) {
    std::cout << num << std::endl;
    RC_ASSERT((num + num) == (num * 2));
}

// Generates a random string
// asserts that it is empty if its size is 0
RC_GTEST_PROP(RapidCheckGTests, StringTest, (const std::string &str)) {
    std::cout << str << std::endl;
    RC_ASSERT(str.empty() == (str.size() == 0));
}

//// failure handling

// Generates four boolean values
// asserts that a specific combination does not occur (1001)
RC_GTEST_PROP(RapidCheckTests, BooleanTest, (bool val1, bool val2, bool val3, bool val4)) {
    std::cout << val1 << " " << val2 << " " << val3 << " " << val4 << std::endl;
    RC_ASSERT(!(val1 == true && val2 == false && val3 == false && val4 == true));
}


// Tests that multiplication of two integers yields the same result when using two different multiplication methods.
RC_GTEST_PROP(RapidCheckTest, CheckMultiplicationPositiveVal, (int32_t a, int32_t b)) {
        std::cout << a << " * " << b << std::endl;
        auto res1 = multiplyWithOperator(a, b);
        auto res2 = multiplyWithLoop(a, b);
        RC_ASSERT(res1 == res2);
}

// Similar to the previous test, this one verifies multiplication results for any pair of integers.
RC_GTEST_PROP(RapidCheckTest, CheckMultiplicationUnsigned, (uint32_t a, uint32_t b)) {
    std::cout << a << " * " << b << std::endl;
    auto res1 = multiplyWithOperator(a, b);
    auto res2 = multiplyWithLoop(a, b);
    RC_ASSERT(res1 == res2);
}

//// arbitrary support for custom type

// NOTE: Must be in rc namespace!
namespace rc {

    // Custom generator for Role enum
    template<>
    struct Arbitrary<Role> {
        static Gen<Role> arbitrary() {
            return gen::element(STUDENT, TEACHER);
        }
    };

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

// Generates a random Role and tags them
RC_GTEST_PROP(RapidCheckTests, RoleTest, (Role r)) {
    //const auto r = *rc::gen::arbitrary<Role>();
    const auto rStr = r == STUDENT ? "Student" : "Lehrer";
    RC_TAG(rStr);
    std::cout << rStr << std::endl;
}

// Generates a random Person and tags their properties
RC_GTEST_PROP(RapidCheckTests, PersonTest, (Person p)) {
    //const auto p = *rc::gen::arbitrary<Person>();
    RC_TAG(p.firstName);
    RC_TAG(p.lastName);
    RC_TAG(p.role == STUDENT ? "Student" : "Lehrer");
    std::cout << p << std::endl;
}
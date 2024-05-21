#include "gtest/gtest.h"
#include "rapidcheck/gtest.h"
#include "reverseMethods.h"
#include "multiplicationMethods.h"
#include "Person.h"

#include <vector>


// Overview
RC_GTEST_PROP(RapidCheckTest, WithFunction, ()) {
    const auto range = *rc::gen::arbitrary<std::pair<int, int>>();
    const auto x = *rc::gen::inRange(range.first, range.second);
    std::cout << range.first << " and " << range.second << std::endl;
    RC_ASSERT(x >= range.first);
    RC_ASSERT(x < range.second);
}

RC_GTEST_PROP(RapidCheckTest, WithParameter, (int a, int b)) {
    std::cout << a << " and " << b << std::endl;
}

RC_GTEST_PROP(RapidCheckTest, WithParameterAndPrecondition, (int a, int b)) {
    RC_PRE(a >= 0 && b >= 0);
    std::cout << a << " and " << b << std::endl;
}






// Test case for integer
// RapidCheck generates random values for the parameters a and b.
// If the assertion holds for all generated pairs, the test passes.
RC_GTEST_PROP(RapidCheckTest, CheckMultiplicationPositiveVal, (int32_t a, int32_t b)) {
        RC_PRE(a >= 0 && b >= 0); // If the precondition does not hold, the test case will be discarded
        std::cout << a << " * " << b << std::endl;
        auto res1 = multiplyWithOperator(a, b);
        auto res2 = multiplyWithLoop(a, b);
        RC_ASSERT(res1 == res2);
}

// If the assertion fails for any pair, RapidCheck reports the failure, including the specific values of a and b that caused the failure.
RC_GTEST_PROP(RapidCheckTest, CheckMultiplicationUnsigned, (int32_t a, int32_t b)) {
    std::cout << a << " * " << b << std::endl;
    auto res1 = multiplyWithOperator(a, b);
    auto res2 = multiplyWithLoop(a, b);

    // If a failure occurs, RapidCheck attempts to "shrink" the input values to find the smallest values that still cause the failure.
    // Shrinking helps to identify the simplest case that causes the problem, making it easier to debug.
    RC_ASSERT(res1 == res2);
}






// Test case for strings
RC_GTEST_PROP(RapidCheckTest, CompareReverseMethods, (const std::string& a)) {
    std::cout << a << std::endl;
    auto res1 = reverseWithStdReverse(a);
    auto res2 = reverseWithSwap(a);
    RC_ASSERT(res1 == res2);
}







// Test case for booleans
RC_GTEST_PROP(RapidCheckTest, BooleanTest, (bool val1, bool val2, bool val3, bool val4)) {
    std::cout << val1 << " " << val2 << " " << val3 << " " << val4 << std::endl;
    RC_ASSERT(!(val1 == true && val2 == false && val3 == false && val4 == true));
}







// arbitrary support for custom type
// NOTE: Must be in rc namespace!
namespace rc {

    template<>
    struct Arbitrary<Person> {
        static Gen<Person> arbitrary() {
            return gen::build<Person>(
                    gen::set(&Person::firstName),
                    gen::set(&Person::lastName),
                    gen::set(&Person::age, gen::inRange(0, 100)));
        }
    };

} // namespace rc

RC_GTEST_PROP(RapidCheckTest, PersonTestParameter, (Person p)) {
    std::cout << "First name: " << p.firstName << std::endl;
    std::cout << "Last name: " << p.lastName << std::endl;
    std::cout << "Age: " << p.age << std::endl;
}


RC_GTEST_PROP(RapidCheckTest, PersonTestFunction, ()) {
    const auto p = *rc::gen::arbitrary<Person>();
    std::cout << "First name: " << p.firstName << std::endl;
    std::cout << "Last name: " << p.lastName << std::endl;
    std::cout << "Age: " << p.age << std::endl;
}
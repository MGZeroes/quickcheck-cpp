#include "gtest/gtest.h"
#include "Gen/Gen.h"
#include "reverseMethods.h"
#include "multiplicationMethods.h"


// Property function for integer generator
bool checkingMultiplication(int n) {
    return multiplyWithOperator(n, n) == multiplyWithLoop(n, n);
}
bool checkingMultiplication(unsigned int n) {
    return multiplyWithOperator(n, n) == multiplyWithLoop(n, n);
}

// Test case for integer generator
TEST(QuickCheckTest, IntGenTest) {
    quickCheck<int>(checkingMultiplication);
}

TEST(QuickCheckTest, UnsignedIntGenTest) {
    quickCheck<unsigned int>(checkingMultiplication);
}



// Property function for string generator
bool comparingReverseMethods(std::string str) {
    return reverseWithStdReverse(str) == reverseWithSwap(str);
}

// Test case for string generator
TEST(QuickCheckTest, StringGenTest) {
    quickCheck<std::string>(comparingReverseMethods);
}



// Property function for boolean generator
bool checkingBooleanValue(bool value) {
    return value || !value;
}

// Test case for boolean generator
TEST(QuickCheckTest, BoolTest) {
    quickCheck<bool>(checkingBooleanValue);
}

// Test case for vector generator
TEST(QuickCheckTest, ListTest) {
    auto g = arbitrary<std::vector<std::string>>();

    for (size_t i = 0; i < 25; ++i) {
        auto list = g.generate();
        bool result = list.size() <= 10;

        if (result) {
            std::cout << "[       OK ] value: ";
            for (auto v : list)
                std::cout << v << "\n";
            std::cout << std::endl;
        } else {
            std::cout << "[   Failed ] value: ";
        }

        for (auto v : list)
            std::cout << v << ", ";
        std::cout << std::endl;
    }
}




// Property function for Person generator
bool checkPersonAge(Person person) {
    return person.validateAge();
}

// Test case for Person generator
TEST(QuickCheckTest, PersonTest) {
    quickCheck<Person>(checkPersonAge);
}
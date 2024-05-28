#include "gtest/gtest.h"
#include "GenOO/GenOO.h"
#include "reverseMethods.h"
#include "multiplicationMethods.h"


// Property function for integer generator
bool checkMultiplication(int n) {
    return multiplyWithOperator(n, n) == multiplyWithLoop(n, n);
}

// Test case for integer generator
TEST(QuickCheckOOTest, IntGenTest) {
    IntGen intGen;
    bool passed = quickCheckOO(&intGen, checkMultiplication);
    ASSERT_TRUE(passed);
}

TEST(QuickCheckOOTest, IntGenTestCustom) {
    IntGen intGen(-10, 20);
    bool passed = quickCheckOO(&intGen, checkMultiplication);
    ASSERT_TRUE(passed);
}



// Property function for string generator
bool compareReverseMethods(std::string str) {
    return reverseWithStdReverse(str) == reverseWithSwap(str);
}

// Test case for string generator
TEST(QuickCheckOOTest, StringGenTest) {
    StringGen stringGen;
    bool passed = quickCheckOO(&stringGen, compareReverseMethods);
    ASSERT_TRUE(passed);
}

// Test case for string generator
TEST(QuickCheckOOTest, StringGenTestCustom) {
    StringGen stringGen(3, 4, 'a', 'h');
    bool passed = quickCheckOO(&stringGen, compareReverseMethods);
    ASSERT_TRUE(passed);
}



// Property function for boolean generator
bool checkBooleanValue(bool value) {
    return value || !value;
}

// Test case for boolean generator
TEST(QuickCheckOOTest, BoolTest) {
    BoolGen boolGen;
    bool passed = quickCheckOO(&boolGen, checkBooleanValue);
    ASSERT_TRUE(passed);
}


// Property function for vector generator
bool checkVector(std::vector<std::string> list) {
    return list.size() <= 10;
}

// Test case for vector generator
TEST(QuickCheckOOTest, VectorTest) {
    VectorStringGen listGen;
    for (size_t i = 0; i < 25; ++i) {
        auto list = listGen.generate();
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
bool checkingPersonAge(Person person) {
    return person.validateAge();
}

// Test case for Person generator
TEST(QuickCheckOOTest, PersonTest) {
    PersonGen personGen;
    bool passed = quickCheckOO(&personGen, checkingPersonAge);
    ASSERT_TRUE(passed);
}

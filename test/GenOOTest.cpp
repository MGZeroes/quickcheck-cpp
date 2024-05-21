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



// Property function for boolean generator
bool checkBooleanValue(bool value) {
    return value == true || value == false;
}

// Test case for boolean generator
TEST(QuickCheckOOTest, BoolTest) {
    BoolGen boolGen;
    bool passed = quickCheckOO(&boolGen, checkBooleanValue);
    ASSERT_TRUE(passed);
}



/*
// Property function for Person generator
bool checkingPersonAge(Person person) {
    return person.age >= 0 && person.age < 100;
}

// Test case for Person generator
TEST(QuickCheckOOTest, PersonTest) {
    PersonGen personGen;
    bool passed = quickCheckOO(&personGen, checkingPersonAge);
    ASSERT_TRUE(passed);
}
*/
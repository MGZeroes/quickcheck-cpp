#include "gtest/gtest.h"
#include "Gen/Gen.h"
#include "reverseMethods.h"
#include "multiplicationMethods.h"


// Property function for integer generator
bool checkingMultiplication(int n) {
    return multiplyWithOperator(n, n) == multiplyWithLoop(n, n);
}

// Test case for integer generator
TEST(QuickCheckTest, IntGenTest) {
    quickCheck<int>(checkingMultiplication);
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
    return value == true || value == false;
}

// Test case for boolean generator
TEST(QuickCheckTest, BoolTest) {
    quickCheck<bool>(checkingBooleanValue);
}




/*
// Property function for Person generator
bool checkPersonAge(Person person) {
    return person.age >= 0 && person.age < 100;
}

// Test case for Person generator
TEST(QuickCheckTest, PersonTest) {
    quickCheck<Person>(checkPersonAge);
}*/
#include "reverseMethods.h"

// Using std::reverse
std::string reverseWithStdReverse(const std::string& str) {
    std::string reversedStr = str;
    std::reverse(reversedStr.begin(), reversedStr.end());
    return reversedStr;
}

// Using a loop and std::swap
std::string reverseWithSwap(const std::string& str) {
    std::string reversedStr = str;
    size_t n = reversedStr.length();
    for (size_t i = 0; i < n / 2; ++i) {
        std::swap(reversedStr[i], reversedStr[n - i - 1]);
    }
    return reversedStr;
}
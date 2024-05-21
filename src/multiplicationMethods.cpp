#include "multiplicationMethods.h"

int multiplyWithOperator(const int a, const int b) {
    return a * b;
}

int multiplyWithLoop(const int a, const int b) {
    int result = 0;
    for (int i = 0; i < b; ++i) {
        result += a;
    }
    return result;
}
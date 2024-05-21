#ifndef GEN_H
#define GEN_H

#include "Person.h"

#include <functional>
#include <iostream>
#include <random>
#include <string>

// Gen class definition
template<typename T>
class Gen {
public:
    Gen() = default;
    Gen(std::function<T()> gen_) { gen = std::function<T()>(gen_); }

    std::function<T()> gen;
    T generate() { return gen(); };
};

template<typename T>
Gen<T> arbitrary();


// Arbitrary generators for common types


// Integer generator
template<>
Gen<int> arbitrary<int>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(0, 100);
        return dis(gen);
    }};
}

// String generator
template<>
Gen<std::string> arbitrary<std::string>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> len_dis(5, 15);
        static std::uniform_int_distribution<char> char_dis('a', 'z');

        size_t len = len_dis(gen);
        std::string str(len, '\0');
        for (size_t i = 0; i < len; ++i) {
            str[i] = char_dis(gen);
        }
        return str;
    }};
}

// bool generator
template<>
Gen<bool> arbitrary<bool>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(0, 1);
        return static_cast<bool>(dis(gen));
    }};
}


// Person generator
template<>
Gen<Person> arbitrary<Person>() {
    return {[]() {
        Person person;
        person.firstName = arbitrary<std::string>().generate();
        person.lastName = arbitrary<std::string>().generate();
        person.age = arbitrary<int>().generate();
        return person;
    }};
}


// QuickCheck function
template<typename T>
void quickCheck(bool (*p)(T), const size_t n = 20) {
    Gen<T> g = arbitrary<T>();

    for (size_t i = 0; i < n; ++i) {
        T value = g.generate();
        bool result = p(value);
        if (result) {
            std::cout << "[       OK ] value: " << value << std::endl;
        } else {
            std::cout << "[   Failed ] value: " << value << std::endl;
        }
    }
}

#endif // GEN_H

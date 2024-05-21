#ifndef GENOO_H
#define GENOO_H

#include "Person.h"

#include <iostream>
#include <random>
#include <string>

// Base generator class
template<typename T>
class GenOO {
public:
    virtual T generate() = 0;
};

// Integer generator
class IntGen : public GenOO<int> {
private:
    int min = 0;
    int max = 100;

public:
    IntGen() = default;
    IntGen(const int min, const int max) : min(min), max(max) {}

    int generate() override {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }
};

// String generator
class StringGen : public GenOO<std::string> {
private:
    size_t minLen = 5;
    size_t maxLen = 15;
    char minChar = 'a';
    char maxChar = 'z';

public:
    StringGen() = default;
    //StringGen(const size_t minLen, const size_t maxLen) : minLen(minLen), maxLen(maxLen) {}
    //StringGen(const char minChar, const char maxChar) : minChar(minChar), maxChar(maxChar) {}
    StringGen(const size_t minLen, const size_t maxLen, const char minChar, const char maxChar) :
        minLen(minLen), maxLen(maxLen), minChar(minChar), maxChar(maxChar) {}

    std::string generate() override {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> len_dis(minLen, maxLen);
        static std::uniform_int_distribution<char> char_dis(minChar, maxChar);

        size_t len = len_dis(gen);
        std::string str(len, '\0');
        for (size_t i = 0; i < len; ++i) {
            str[i] = char_dis(gen);
        }
        return str;
    }
};

// Boolean generator
class BoolGen : public GenOO<bool> {
public:
    BoolGen() = default;

    bool generate() override {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(0, 1);
        return dis(gen) == 1;
    }
};


// Person generator
class PersonGen : public GenOO<Person> {
private:
    StringGen firstNameGen;
    StringGen lastNameGen;
    IntGen ageGen;

public:
    PersonGen() : firstNameGen(), lastNameGen(), ageGen(0, 100) {}

    Person generate() override {
        Person person;
        person.firstName = firstNameGen.generate();
        person.lastName = lastNameGen.generate();
        person.age = ageGen.generate();
        return person;
    }
};



// QuickCheck function
template<typename T>
bool quickCheckOO(GenOO<T>* g, bool (*p)(T), const size_t n = 20) {
    bool passed = true;

    for (size_t i = 0; i < n; ++i) {
        T value = g->generate();
        bool result = p(value);
        if (result) {
            std::cout << "[       OK ] value: " << value << std::endl;
        } else {
            std::cout << "[   Failed ] value: " << value << std::endl;
            passed = false;
        }
    }

    return passed;
}

#endif // GENOO_H

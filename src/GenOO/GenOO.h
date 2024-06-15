#ifndef GENOO_H
#define GENOO_H

#include "Person.h"

#include <random>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>

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
    size_t minLen = 1;
    size_t maxLen = 40;
    char minChar = 'a';
    char maxChar = 'z';
    size_t minSpaces = 0;
    size_t maxSpaces = 10;

public:
    StringGen() = default;
    StringGen(const size_t minLen, const size_t maxLen, const char minChar, const char maxChar) :
            minLen(minLen), maxLen(maxLen), minChar(minChar), maxChar(maxChar) {}

    std::string generate() override {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> len_dis(minLen, maxLen);
        static std::uniform_int_distribution<char> char_dis(minChar, maxChar);
        static std::uniform_int_distribution<size_t> space_dis(minSpaces, maxSpaces);

        size_t length = len_dis(gen);
        size_t numSpaces = 0;
        size_t numChars = 0;

        if(length > 0) {
            numSpaces = space_dis(gen);
            numSpaces = numSpaces >= length ? length - 1: numSpaces;
            numChars = length - numSpaces;
        }

        std::string str;
        str.reserve(length);

        // generate chars and append them
        for (size_t i = 0; i < numChars; ++i) {
            str.append(1, char_dis(gen));
        }

        // append spaces
        str.append(numSpaces, ' ');

        // shuffle the string to distribute spaces and characters more randomly
        std::shuffle(str.begin(), str.end(), gen);

        // replace spaces > 5
        size_t pos = 0;
        while ((pos = str.find("      ", pos)) != std::string::npos) {
            str.replace(pos, 6, "     "); // replace
            pos += 5;
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


class VectorStringGen : public GenOO<std::vector<std::string>> {
    StringGen stringGen = StringGen();
    uint16_t maxLen = 10;

public:
    VectorStringGen() = default;
    VectorStringGen(StringGen stringGen, const uint16_t maxLen) :
            stringGen(std::move(stringGen)), maxLen(maxLen) {};

    std::vector<std::string> generate() override {
        std::vector<std::string> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint16_t> lenDist(0, maxLen); // length

        int length = lenDist(gen);
        result.reserve(length); // reserve

        for (int i = 0; i < length; ++i) {
            result.push_back(stringGen.generate());
        }

        return result;
    }
};


// Person generator
class PersonGen : public GenOO<Person> {
private:
    StringGen firstNameGen;
    StringGen lastNameGen;
    IntGen ageGen;
    IntGen roleGen;

public:
    PersonGen() : firstNameGen(), lastNameGen(), ageGen(0, 100), roleGen(0, 1) {}

    Person generate() override {
        Person person;
        person.firstName = firstNameGen.generate();
        person.lastName = lastNameGen.generate();
        person.age = ageGen.generate();
        person.role = static_cast<Role>(roleGen.generate());
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

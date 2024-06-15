#ifndef GEN_H
#define GEN_H

#include "Person.h"

#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <algorithm>

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


// Integer generator
template<>
Gen<int> arbitrary<int>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(-100, 100);
        return dis(gen);
    }};
}

template<>
Gen<unsigned int> arbitrary<unsigned int>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned int> dis(0, 100);
        return dis(gen);
    }};
}

template<>
Gen<char> arbitrary<char>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<char> dis('a', 'z');
        return dis(gen);
    }};
}

// String generator
template<>
Gen<std::string> arbitrary<std::string>() {
    return {[]() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> len_dis(1, 40);
        static std::uniform_int_distribution<size_t> space_dis(0, 10);

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
            str.append(1, arbitrary<char>().generate());
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

// int list generator
template<>
Gen<std::vector<int>> arbitrary<std::vector<int>>() {
    return {[]() {
        std::vector<int> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> lenDist(0, 10); // length

        int length = lenDist(gen);
        result.reserve(length); // reserve

        for (int i = 0; i < length; ++i) {
            result.push_back(arbitrary<int>().generate());
        }

        return result;
    }};
}

// string list generator
template<>
Gen<std::vector<std::string>> arbitrary<std::vector<std::string>>() {
    return {[]() {
        std::vector<std::string> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> lenDist(0, 10); // length

        int length = lenDist(gen);
        result.reserve(length); // reserve

        for (int i = 0; i < length; ++i) {
            result.push_back(arbitrary<std::string>().generate());
        }

        return result;
    }};
}


// Person generator
template<>
Gen<Role> arbitrary<Role>() {
    return {[]() {
        Role role;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(0, 1);
        role = static_cast<Role>(dis(gen));
        return role;
    }};
}

template<>
Gen<Person> arbitrary<Person>() {
    return {[]() {
        Person person;
        person.firstName = arbitrary<std::string>().generate();
        person.lastName = arbitrary<std::string>().generate();
        person.age = arbitrary<unsigned int>().generate();
        person.role = arbitrary<Role>().generate();
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

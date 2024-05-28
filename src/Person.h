#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <ostream>

enum Role {
    STUDENT,
    TEACHER,
};

struct Person {
    std::string firstName;
    std::string lastName;
    int age;
    Role role;

public:

    bool validateAge() const {
        return age >= 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << "firstName: " << person.firstName;
        os << " lastName: " << person.lastName;
        os << " age: " << person.age;
        os << " role: " << person.role;
        return os << std::endl;
    }
};

#endif // PERSON_H

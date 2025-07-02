#include "build-your-own-http/models/StudentInfo.h"

// Constructor with all fields
StudentInfo::StudentInfo(const std::string& name, int age, const std::string& rollNumber,
                         const std::string& email, const std::string& address, const std::string& about)
    : name(name), age(age), rollNumber(rollNumber), email(email), address(address), about(about) {}

// Getters
std::string StudentInfo::getName() const {
    return name;
}

int StudentInfo::getAge() const {
    return age;
}

std::string StudentInfo::getRollNumber() const {
    return rollNumber;
}

std::string StudentInfo::getEmail() const {
    return email;
}

std::string StudentInfo::getAddress() const {
    return address;
}

std::string StudentInfo::getAbout() const {
    return about;
}

// Setters
void StudentInfo::setName(const std::string& name) {
    this->name = name;
}

void StudentInfo::setAge(int age) {
    this->age = age;
}

void StudentInfo::setRollNumber(const std::string& rollNumber) {
    this->rollNumber = rollNumber;
}

void StudentInfo::setEmail(const std::string& email) {
    this->email = email;
}

void StudentInfo::setAddress(const std::string& address) {
    this->address = address;
}

void StudentInfo::setAbout(const std::string& about) {
    this->about = about;
}
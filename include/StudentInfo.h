#pragma once

#include <string>

class StudentInfo {
public:
    // Constructors
    StudentInfo() = default;
    StudentInfo(const std::string& name, int age, const std::string& rollNumber,
                const std::string& email, const std::string& address, const std::string& about);

    // Getters
    std::string getName() const;
    int getAge() const;
    std::string getRollNumber() const;
    std::string getEmail() const;
    std::string getAddress() const;
    std::string getAbout() const;

    // Setters
    void setName(const std::string& name);
    void setAge(int age);
    void setRollNumber(const std::string& rollNumber);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);
    void setAbout(const std::string& about);

private:
    std::string name;
    int age;
    std::string rollNumber;
    std::string email;
    std::string address;
    std::string about;
};
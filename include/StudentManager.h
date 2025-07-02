#pragma once
#include "StudentInfo.h"
#include <vector>
#include <mutex>

class StudentManager {
public:
    static StudentManager& getInstance();

    void addStudent(const StudentInfo& student);
    std::vector<StudentInfo> getAllStudents() const;
    std::vector<StudentInfo> getStudentByName(std::string& studentName) const;

private:
    StudentManager() = default;
    std::vector<StudentInfo> students;
    mutable std::mutex mtx;
};
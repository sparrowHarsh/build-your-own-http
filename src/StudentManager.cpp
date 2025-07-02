#include "../include/StudentManager.h"
#include <vector>
#include <mutex>
#include <string>

StudentManager& StudentManager::getInstance() {
    static StudentManager instance;
    return instance;
}

void StudentManager::addStudent(const StudentInfo& student) {
    std::lock_guard<std::mutex> lock(mtx);
    students.push_back(student);
}

std::vector<StudentInfo> StudentManager::getAllStudents() const {
    std::lock_guard<std::mutex> lock(mtx);
    return students;
}

std::vector<StudentInfo> StudentManager::getStudentByName(std::string& studentName) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<StudentInfo> result;
    for (const auto& student : students) {
        if (student.getName() == studentName) {
            result.push_back(student);
        }
    }
    return result;
}

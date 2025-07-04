#include "build-your-own-http/models/StudentManager.h"
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>

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

bool StudentManager::removeStudentByRollNumber(const std::string& rollNumber){
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(students.begin(), students.end(),
        [&](const StudentInfo& s) { return s.getRollNumber() == rollNumber; });

    if (it != students.end()) {
        students.erase(it, students.end());
        return true;
    }
    return false;
}

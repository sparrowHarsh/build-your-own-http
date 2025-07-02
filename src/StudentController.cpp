#include "../include/StudentController.h"
#include "../include/StudentManager.h"
#include <sstream>

void StudentController::handle(HttpRequest& httpRequest, HttpResponse& httpResponse) {
    if (httpRequest.getMethod() == "POST") {
        // For simplicity, assume body is "name,age,roll,email,address,about"
        std::istringstream iss(httpRequest.getBody());
        std::string name, ageStr, roll, email, address, about;
        std::getline(iss, name, ',');
        std::getline(iss, ageStr, ',');
        std::getline(iss, roll, ',');
        std::getline(iss, email, ',');
        std::getline(iss, address, ',');
        std::getline(iss, about, ',');

        int age = std::stoi(ageStr);
        StudentInfo student(name, age, roll, email, address, about);
        StudentManager::getInstance().addStudent(student);

        httpResponse.setStatus(201, "Created");
        httpResponse.setBody("Student added successfully");
    } else if(httpRequest.getMethod() == "GET"){
        // Get all students and send as response
        const auto& students = StudentManager::getInstance().getAllStudents();
        std::ostringstream oss;
        for (const auto& student : students) {
            oss << "Name: " << student.getName()
            << ", Age: " << student.getAge()
            << ", Roll: " << student.getRoll()
            << ", Email: " << student.getEmail()
            << ", Address: " << student.getAddress()
            << ", About: " << student.getAbout()
            << "\n";
        }
        httpResponse.setStatus(200, "OK");
        httpResponse.setBody(oss.str());
    } else {
        httpResponse.setStatus(405, "Method Not Allowed");
        httpResponse.setBody("Method not allowed");
    }
}
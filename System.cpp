#include "System.h"

// -------------------- Supervisor class ----------------------
Supervisor::Supervisor(string id, int load) : id_(id), load_(load) {
}

Supervisor::~Supervisor() {
}

string Supervisor::getId() const {
    return id_;
}

int Supervisor::getLoad() const {
    return load_;
}

std::vector<int> Supervisor::getProjects() const {
    return proposedprojets_;
}

void Supervisor::setProjects(std::vector<int> projectlist) {
    proposedprojets_ = projectlist;
}

// -------------------- Student class ----------------------
Student::Student(string id, std::vector<int> projectchoices) : id_(id), projectchoices_(projectchoices), assignedproject_(false) {
}

Student::~Student() {
}

string Student::getId() const {
    return id_;
}

std::vector<int> Student::getProjectChoices() const {
    return projectchoices_;
}

int Student::getScore() const {
    return score_;
}

void Student::setScore(int value) {
    score_ = value;
}

bool Student::getAssignedProject() const {
    return assignedproject_;
}

void Student::setAssignedProject(bool value) {
    assignedproject_ = value;
}

// -------------------- Project class ----------------------
Project::Project(int id, string supervisorid, string title, int multiplcity) : id_(id), supervisorid_(supervisorid), 
title_(title), multiplicity_(multiplcity) {
}

Project::~Project() {
}

int Project::getId() const {
    return id_;
}

string Project::getSupervisorId() const {
    return supervisorid_;
}

string Project::getTitle() const {
    return title_;
}

int Project::getMultiplicity() const {
    return multiplicity_;
}

std::vector<Student> Project::getStudents() const {
    return allocatedstudents_;
}

void Project::setStudents(std::vector<Student> studentlist_) {
    allocatedstudents_ = studentlist_;
}
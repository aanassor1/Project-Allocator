#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <list>

using std::string;

class Supervisor {
public:
    // Supervisor constructor, specify id and load amount.
    Supervisor(string id, int load);

    // Destructor.
    ~Supervisor();

    // Return id of supervisor.
    string getId() const;

    // Return load amount of supervisor.
    int getLoad() const;

    // Return the proposed projects vector.
    std::vector<int> getProjects() const;

    // Set the proposed projects vector to the arg1.
    void setProjects(std::vector<int> projectlist);

protected:
    string id_;
    int load_;
    std::vector<int> proposedprojets_;

};

class Student {
public:

    // Student constructor, specify id and vector of project choices.
    Student(string id, std::vector<int> projectchoices);

    // Destructor.
    ~Student();

    // Return the id of the student.
    string getId() const;
    
    // Return the project choices of the student.
    std::vector<int> getProjectChoices() const;

    // Return the score of the student.
    int getScore() const;

    // Set the score of the student.
    void setScore(int);

    // Return the bool value of whether this student has been assigned a project
    bool getAssignedProject() const;

    // Set the bool value of whether this student has been assigned a project
    void setAssignedProject(bool);

protected:
    string id_;
    std::vector<int> projectchoices_;
    int score_;
    bool assignedproject_;
};

class Project {
public:

    // Project constructor, specify id, the supervisor id, title and multiplicity of the project.
    Project(int id, string supervisorid, string title, int multiplcity);

    // Destructor
    ~Project();
    
    // Return the id of the project.
    int getId() const;

    // Return the supervisor id of the project.
    string getSupervisorId() const;

    // Return the title of the project.
    string getTitle() const;

    // Return the multiplicity of the project.
    int getMultiplicity() const;

    // Return the allocated students vector of the project.
    std::vector<Student> getStudents() const;
    
    // Set the allocated students vector to arg 1
    void setStudents(std::vector<Student> studentlist);

protected:
    int id_;
    string supervisorid_;
    string title_;
    int multiplicity_;
    std::vector<Student> allocatedstudents_;

};

#endif
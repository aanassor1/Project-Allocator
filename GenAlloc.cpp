#include <iostream>
#include "System.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Open the file and return a vector of the file line by line
vector<string> readFileToVector(const string& filename)
{
    ifstream source;
    source.open(filename);
    vector<string> lines;
    string line;
    cout<<filename;
    while (getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}

// Seperate a single line by whitespace and turn it into a vector
vector<string> lineToVector(string line) {
    istringstream iss(line);
    vector<string> lineVector;
    for(string s; iss >> s;)
        lineVector.push_back(s);
    return lineVector;
}

// Turn the single line vector into a supervisor object and add it to the supervisor list
vector<unique_ptr<Supervisor>> vectorToStaff(std::vector<string> stafflist_) {
    vector<unique_ptr<Supervisor>> staffs;
    for (int i = 0; i != stafflist_.size(); ++i) {
        vector<string> staffVector = lineToVector(stafflist_[i]); //Line turned to vector (seperate whitespace)
        unique_ptr<Supervisor> staffObject_ = make_unique<Supervisor>(staffVector[0], stoi(staffVector[1]));
        staffs.push_back(std::move(staffObject_));
    }
    return staffs;
}

// Turn the single line vector into a project object and add it to the project list
vector<unique_ptr<Project>> vectorToProject(std::vector<string> projectlist_) {
    vector<unique_ptr<Project>> projects;
    for (int i = 0; i != projectlist_.size(); ++i) {
        vector<string> projectVector = lineToVector(projectlist_[i]); //Line turned to vector (seperate whitespace)
        unique_ptr<Project> projectObject_ = make_unique<Project>(stoi(projectVector[0]), projectVector[1], projectVector[3], 
        stoi(projectVector[2]));
        projects.push_back(std::move(projectObject_));
    }
    return projects;
}

// Turn the single line vector into a student object and add it to the student list
vector<unique_ptr<Student>> vectorToStudent(std::vector<string> studentlist_) {
    vector<unique_ptr<Student>> students;
    for (int i = 0; i != studentlist_.size(); ++i) {
        vector<string> studentVector = lineToVector(studentlist_[i]); //Line turned to vector (seperate whitespace)
        vector<int> projectpref_;
        for (int x = 1; x <= 4; x++) {
            projectpref_.push_back(stoi(studentVector[x]));
        };
        unique_ptr<Student> studentObject_ = make_unique<Student>(studentVector[0], projectpref_);
        students.push_back(std::move(studentObject_));
    }
    return students;
}

// Find the specific project object in a given vector of objects
Project* findProjectInVector(vector<unique_ptr<Project>>& projects_, int projectId_) {
    for (int i(0); i != projects_.size(); ++i) {
        if (projects_[i].get()->getId() == projectId_) {
            return projects_[i].get();
        }
    }
    return nullptr;
}

// Find the specific supervisor object in a given vector of objects
Supervisor* findSupervisorInVector(vector<unique_ptr<Supervisor>>& supervisors_, string supervisorId_) {
    for (int i(0); i != supervisors_.size(); ++i) {
        if (supervisors_[i].get()->getId() == supervisorId_) {
            return supervisors_[i].get();
        }
    }
    return nullptr;
}

// Custom compare function to compare student ids
bool compareFunction (unique_ptr<Student>& a, unique_ptr<Student>& b) {return a->getId()<b->getId();} 

int main(int argc, char** argv) {
    // Acquire the names of the file names
    string staffFileName_ = argv[1];
    string projectsFileName_ = argv[2];
    string studentsFileName_ = argv[3];
    string allocationFileName_ = argv[4];

    // Acquire the line by line vectors of each file
    vector<string> staffvector_ = readFileToVector(staffFileName_);
    vector<string> projectsvector_ = readFileToVector(projectsFileName_);
    vector<string> studentsvector_ = readFileToVector(studentsFileName_);

    // Acquire the vector objects of each class
    vector<unique_ptr<Supervisor>> staffs_ = vectorToStaff(staffvector_);
    vector<unique_ptr<Project>> projects_ = vectorToProject(projectsvector_);
    vector<unique_ptr<Student>> students_ = vectorToStudent(studentsvector_);
    
    for (int i = 0; i != projects_.size(); i++) { // Set up the projects that each supervisor has to supervise (set up the variable)
        Supervisor* supervisor_ = findSupervisorInVector(staffs_, projects_[i].get()->getSupervisorId());
        vector<int> supervisorprojects_ = supervisor_->getProjects();
        supervisorprojects_.push_back(projects_[i].get()->getId());
        supervisor_->setProjects(supervisorprojects_);
    }

    // Sorts the student list to be in alphabetical order
    std::sort(students_.begin(),students_.end(),compareFunction);

    for (int i(0); i != students_.size(); ++i) { //Go through each student and perform the algorithm to assigning projects
        //go through project preference
        vector<int> projectpref_ = students_[i].get()->getProjectChoices();
        for (int x = 0; x != projectpref_.size(); x++) {
            Project* projectinloop_ = findProjectInVector(projects_, projectpref_[x]);
            Supervisor* supervisor_ = findSupervisorInVector(staffs_, projectinloop_->getSupervisorId());
            int supervisorCurrentLoad_ = 0;

            for (int y = 0; y != supervisor_->getProjects().size(); y++) { //Calc the supervisor's current load
                Project* currentLoopProject_ = findProjectInVector(projects_, supervisor_->getProjects()[y]);
                supervisorCurrentLoad_ += currentLoopProject_->getStudents().size();
            }

            if (projectinloop_->getStudents().size() >= projectinloop_->getMultiplicity()) { //If the project is already full, skip
                continue;
            } else if (supervisorCurrentLoad_ < supervisor_->getLoad()) { //Else assign this project
                vector<Student> newstudentlist_ = projectinloop_->getStudents();
                newstudentlist_.push_back(*students_[i]);
                projectinloop_->setStudents(newstudentlist_);
                students_[i].get()->setAssignedProject(true);

                int calculatedscore_ = 0; //Calculate score and assign it
                bool breakloop_ = false;
                for (int z = 0; z != projectpref_.size(); z++) {
                    Project* projectinloop2_ = findProjectInVector(projects_, projectpref_[z]);
                    if (projectinloop_->getId() == projectinloop2_->getId()) {
                        calculatedscore_ = 4-z;
                        breakloop_ = true;
                        break;
                    }
                    if (breakloop_==true) {break;}
                }
                students_[i].get()->setScore(calculatedscore_);
                break;
            }
        }
        if (students_[i].get()->getAssignedProject() == false) { //If student still has no project, assign random
            for (int x = 0; x != projects_.size(); x++) { //Go through all projects to look for empty one
                Project projectinloop_ = *projects_[x];
                Supervisor* supervisor_ = findSupervisorInVector(staffs_, projectinloop_.getSupervisorId());

                int supervisorCurrentLoad_;
                for (int y = 0; y != supervisor_->getProjects().size(); y++) { //Calc the supervisor's current load
                    Project* currentLoopProject_ = findProjectInVector(projects_, supervisor_->getProjects()[y]);
                    supervisorCurrentLoad_ += currentLoopProject_->getStudents().size();
                }

                if (projectinloop_.getStudents().size() >= projectinloop_.getMultiplicity()) { //If the project is already full, skip
                    continue;
                } else if (supervisorCurrentLoad_ < supervisor_->getLoad()) { //Else assign this project
                    vector<Student> newstudentlist_ = projectinloop_.getStudents();
                    newstudentlist_.push_back(*students_[i]);
                    projectinloop_.setStudents(newstudentlist_);
                    students_[i].get()->setAssignedProject(true);

                    int calculatedscore_ = 0; //Calculate score and assign it
                    bool breakloop_ = false;
                    vector<int> projectpref_ = students_[i].get()->getProjectChoices();
                    for (int z = 0; z != projectpref_.size(); z++) {
                        Project* projectinloop2_ = findProjectInVector(projects_, projectpref_[z]);
                        if (projectinloop_.getId() == projectinloop2_->getId()) {
                            calculatedscore_ = 4-z;
                            breakloop_ = true;
                            break;
                        }
                        if (breakloop_==true) {break;}
                    }
                    students_[i].get()->setScore(calculatedscore_);
                    break;
                }
            }
        }
    }
    int totalscore_;
    string output_;
    for (int i(0); i != students_.size(); ++i) { //Assigns every student a project that has not gotten one yet
        output_ += students_[i].get()->getId() + " ";
        bool breakloop_ = false;
        for (int x = 0; x != projects_.size(); x++) { //Go through all projects and find the one the student has been assigned to
            Project projectinloop_ = *projects_[x];
            for (int y = 0; y != projectinloop_.getStudents().size(); y++) {
                if (projectinloop_.getStudents()[y].getId() == students_[i].get()->getId()) {
                    output_ += to_string(projectinloop_.getId()) + "\n";
                    breakloop_ = true;
                    break;
                }
            }
            if (breakloop_==true) {break;}
        }
        totalscore_ += students_[i].get()->getScore();
        if (breakloop_ == false) { //Student has no project
            output_ += "0\n";
        }
    }
    output_ += to_string(totalscore_);
    std::cout<<"\n"<<output_;
    
    //Write file out to the allocation txt file
    ofstream fw(allocationFileName_, std::ofstream::out);
    if (fw.is_open()) {
        fw << output_;
        fw.close();
    }
}
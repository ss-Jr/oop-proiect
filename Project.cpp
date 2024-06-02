#include "Project.h"
#include <iostream>

Project::Project(std::unique_ptr<Application> ptr_) : ptr(std::move(ptr_)) {}

void Project::start() const {
    ptr->run();
}

Project& Project::operator=(Project other) {
    swap(*this, other);
    return *this;
}

void swap(Project& p1, Project& p2) {
    std::swap(p1.ptr, p2.ptr);
}

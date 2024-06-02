#ifndef PROJECT_H
#define PROJECT_H

#include <memory>
#include "Application.h"

class Project {
private:
    std::unique_ptr<Application> ptr;
public:
    Project(std::unique_ptr<Application> ptr_);

    void start() const;

    Project& operator=(Project other);

    friend void swap(Project& p1, Project& p2);
};

#endif // PROJECT_H

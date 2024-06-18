/*
 * Author: Arvin Singh
 * Date: 06/04/2024
 * Description: This file contains the decleration of the To-do class.
 * Root object that holds all data for todo. This class contains Projects,
 * which in turn hold Tasks, which have a name, due date, completed status, and tags.
*/


#ifndef TODOLIST_H
#define TODOLIST_H

#include <fstream>
#include "project.h"

using ProjectContainer = std::vector<Project>;

class TodoList {

    public:
    explicit TodoList();
    ~TodoList() = default;
    
    unsigned int size() const;
    Project &newProject(const String &identifier);
    bool addProject(Project project);
    Project &getProject(const String &identifier);
    bool containsProject(const String &identifier) const;
    bool deleteProject(const String &identifier);
    void load(const String &fileName);
    void save(const String &fileName);
    const ProjectContainer &getProjects() const;
    String str() const;
    Json json() const;

    private:
    ProjectContainer projects;
};

#endif // TODOLIST_H

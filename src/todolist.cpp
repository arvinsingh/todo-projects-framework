/*
 * Author: Arvin Singh
 * Date: 06/04/2024
 * Description: This file contains the implementation of the TodoList class.
*/


#include "todolist.h"


// Constructor to create a TodoList object
TodoList::TodoList() {}


// Returns number of projects.
unsigned int TodoList::size() const {
    return projects.size();
}

/*
    * Function to create a new project in the TodoList object
    * @param &identifier: The identifier of the project to create
    * @return Project&: A reference to the created project
*/
Project &TodoList::newProject(const String &identifier) {
    for (auto &project : projects) {
        if (project.getIdent() == identifier) {
            return project;
        }
    }
    projects.emplace_back(identifier);
    return projects.back();
}

/*
    * Function to add a project in the TodoList object
    * @param project: The project to add
    * @return bool: True if the project was added, false otherwise
*/
bool TodoList::addProject(Project project) {
    for (auto &p : this->projects) {
        if (p.getIdent() == project.getIdent()) {
            for (const auto &t : project.getTasks()) {
                if(!p.containsTask(t.getIdent())){
                    p.addTask(t);
                }
            }
            return false;
        }
    }
    projects.push_back(project);
    return true;
}

/*
    * Function to get a project in the TodoList object
    * @param &identifier: The identifier of the project to get
    * @return Project&: A reference to the project
*/
Project &TodoList::getProject(const String &identifier) {
    for (auto &project : projects) {
        if (project.getIdent() == identifier) {
            return project;
        }
    }
    throw std::out_of_range("Project not found");
}

/*
    * Function to check if a project exists in the TodoList object
    * @param &identifier: The identifier of the project to check
    * @return bool: True if the project exists, false otherwise
*/
bool TodoList::containsProject(const String &identifier) const {
    for (const auto &project : projects) {
        if (project.getIdent() == identifier) {
            return true;
        }
    }
    return false;
}

/*
    * Function to delete a project in the TodoList object
    * @param &identifier: The identifier of the project to delete
    * @return bool: True if the project was deleted, false otherwise
*/
bool TodoList::deleteProject(const String &identifier) {
    for (auto it = projects.begin(); it != projects.end(); ++it) {
        if (it->getIdent() == identifier) {
            projects.erase(it);
            return true;
        }
    }
    throw std::runtime_error("Project not found");
}

/*
    * Function to load a database from a file
    * database file is in JSON format
    * therefore we parse the file and create the objects
    * @param &fileName: The name of the file to load
*/
void TodoList::load(const String &fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("File failed to open.");
    }
    Json j = Json::parse(file);

    for (auto &project : j.items()) {
        Project p(project.key());
        for (auto &task : project.value().items()) {
            Task t(task.key());
            if (task.value().contains("completed")) {
                t.setComplete(task.value()["completed"]);
            }
            if (task.value().contains("dueDate")) {
                Date date;
                date.setDateFromString(task.value()["dueDate"]);
                t.setDueDate(date);
            }
            if (task.value().contains("tags")) {
                for (auto &tag : task.value()["tags"]) {
                    t.addTag(tag);
                }
            }
            p.addTask(t);
        }
        projects.push_back(p);
    }
}

/*
    * Function to save a database to a file
    * database file is in JSON format
    * therefore we serialize the object to JSON
    * @param &fileName: The name of the file to save
*/
void TodoList::save(const String &fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    // serialize the object to JSON

    Json projectsJson;

    for (Project& project : projects) {
        Json projectJson;  
        for (const Task& task : project.getTasks()) {
            projectJson[task.getIdent()] = task.json();
        }
        projectsJson[project.getIdent()] = project.json();
    }

    file << projectsJson << std::endl;
    file.close();
}

/*
    * Function to compare two TodoList objects
    * @param &c1: The first TodoList object
    * @param &c2: The second TodoList object
    * @return bool: True if the objects are equal, false otherwise
*/
bool operator==(const TodoList &c1, const TodoList &c2) {
    
    if (c1.size() != c2.size()) {
        return false;
    }

    for (const Project& project : c1.getProjects()) {
        if (!(c2.containsProject(project.getIdent()))) {
            return false;
        }
    }

    for (const Project& project : c2.getProjects()) {
        if (!(c1.containsProject(project.getIdent()))) {
            return false;
        }
    }
    return true;
}

/*
    * Function to return the string representation of the TodoList object
    * @return String: The string representation of the TodoList object
*/
String TodoList::str() const {
    String s = "{";

    for (const Project& project : projects) {
        s += project.str() + ",";
    }
    s.pop_back();
    s += "}";
    return s;
}

/*
    * Function to return the JSON representation of the TodoList object
    * @return Json: The JSON representation of the TodoList object
*/
Json TodoList::json() const {
    Json j;
    for (const Project& project : projects) {
        j[project.getIdent()] = project.str();       
    }
    return j.dump(4);
}

// Returns the projects in the TodoList object
const ProjectContainer& TodoList::getProjects() const {
    return projects;
}

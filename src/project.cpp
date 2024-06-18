/*
 * Author: Arvin Singh
 * Date: 06/04/2024
 * Description: This file contains the implementation of the Project class.
 */


#include "project.h"

// Constructor to create a Project object with an identifier
Project::Project(String ident) : ident(ident) {}

// Function to return the number of tasks in the Project object
unsigned int Project::size() const noexcept{
    return tasks.size();
}

// Function to return the identifier of the Project object
const String& Project::getIdent() const noexcept {
    return ident;
}

// Function to set the identifier of the Project object
void Project::setIdent(String pIdent) noexcept {
    ident = pIdent;
}

/*
    * Function to create a new task in the Project object
    * @param tIdent: The identifier of the task to create
    * @return Task&: A reference to the created task
*/
Task &Project::newTask(const String &tIdent) {
    Task tObj{tIdent};
    if (containsTask(tIdent)) {
        return getTask(tIdent);
    }
    tasks.push_back(tObj);
    return tasks.back();
}

// Function to find a task in the Project object
const TaskContainer& Project::getTasks() const noexcept {
    return tasks;
}

/*
    * Function to add a task in the Project object
    * @param task: The task to add
    * @return bool: True if the task was added, false otherwise
*/
bool Project::addTask(Task task) {
    if (containsTask(task.getIdent())) {
        Task &tObj = getTask(task.getIdent());
        tObj.mergeTags(task.getTags());
        tObj.setComplete(task.isComplete()); // Fix the function name
        tObj.setDueDate(task.getDueDate());
        return false;
    }
    tasks.push_back(task);
    return true;
}

/*
    * Function to find a task in the Project object
    * @param tIdent: The identifier of the task to find
    * @return Task&: A reference to the task

*/
Task &Project::getTask(const String &tIdent) {
    for (Task &tObj : tasks) {
        if (tObj.getIdent() == tIdent) {
            return tObj;
        }
    }
    throw NoTaskError(tIdent);
}

/*
    * Function to delete a task in the Project object
    * @param tIdent: The identifier of the task to find
    * @return bool: True if the task was deleted, false otherwise

*/
bool Project::deleteTask(const String &tIdent) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getIdent() == tIdent) {
            tasks.erase(it);
            return true;
        }
    }
    throw NoTaskError(tIdent);
}

// Function to compare two Project objects
bool operator==(const Project &c1, const Project &c2) {
    return c1.getIdent() == c2.getIdent() && c1.getTasks() == c2.getTasks();
}

/*
    * Function to return the string representation of the Project object
    * @return String: The string representation of the Project object
*/
String Project::str() const {
    String s = "";
    s += "\"ident\":\"" + ident + "\":{";
    for (const Task &tObj : tasks) {
        s += tObj.str() + ",";
    }
    s.pop_back();
    s += "}";
    return s;
}

/*
    * Function to check if a task is in the Project object
    * @param tIdent: The identifier of the task to find
    * @return bool: True if the task is in the Project object, false otherwise
*/
bool Project::containsTask(const String &tIdent) const noexcept {
    for (const Task &tObj : tasks) {
        if (tObj.getIdent() == tIdent) {
            return true;
        }
    }
    return false;
}

/*
    * Function to return the JSON representation of the Project object
    * @return Json: The JSON representation of the Project object
*/
Json Project::json() const {
    Json j;
    for (const Task& task : tasks) {
        j[task.getIdent()] = task.json();
    }
    return j;
}

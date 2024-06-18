/*
 * Author: Arvin Singh
 * Date: 05/04/2024
 * Description: This file contains the implementation of the Task class.
*/


#include "task.h"
#include <sstream>


// Constructor to create a Task object with an identifier
Task::Task(const String& identifier) : identifier(identifier) {
    completed = false;
}

// Function to return the identifier of the Task object
const String Task::getIdent() const noexcept{
    return identifier;
}

// Function to set the identifier of the Task object
void Task::setIndent(String& identifier) {
    this->identifier = identifier;
}

// Function to add a tag to the Task object
bool Task::addTag(const String tag) {
    if (std::find(tags.begin(), tags.end(), tag) != tags.end()) {
        return false; // tag already exists
    } else {
        tags.push_back(tag);
        return true; // tag inserted into the container
    }
}

// Function to return the tags of the Task object
const TagContainer Task::getTags() const noexcept {
    return tags;
}

// Function to find a tag in the Task object tags
bool Task::deleteTag(const String tag) {
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end()) {
        tags.erase(it);
        return true;
    } else {
        throw std::out_of_range("Tag is empty.");
    }
}

// Merge the tags from another Task object into this one
void Task::mergeTags(const TagContainer& tags) {
    for (auto it = tags.begin(); it != tags.end(); it++) {
        addTag(*it);
    }
}

// Function to return the number of tags in the Task object
const unsigned int Task::numTags() const noexcept {
    return tags.size();
}

// Function to check if a tag is in the Task object
bool Task::containsTag(const String tag) const {
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

// Function to return the tags as a string
String Task::tagsString() const {
    std::stringstream sttr;
    sttr << "[";
    for (auto it = tags.begin(); it != tags.end(); it++) {
        sttr << "\"" << *it << "\"";
        if ((it + 1) != tags.end()) {
            sttr << ",";
        }
    }
    sttr << "]";
    return sttr.str();
}   

// Function to return the due date of the Task object
Date Task::getDueDate() const noexcept {
    return this->dueDate;
}


void Task::setDueDate(Date date) {
    dueDate = date;
}


void Task::setComplete(bool aBool) {
    completed = aBool;
}


const bool Task::isComplete() const noexcept {
    return completed;
}

const String Task::completeStr() const noexcept {
    return completed ? "true" : "false";
}


bool operator==(const Task& task1, const Task& task2) {
    return task1.getIdent() == task2.getIdent() &&
           task1.getDueDate() == task2.getDueDate() &&
           task1.isComplete() == task2.isComplete() &&
           task1.tags == task2.tags;
}


String Task::str() const {
    std::stringstream sttr;
    sttr << "\"" << identifier << "\""
        << ":{\"completed\":" << completeStr()
        << ",\"dueDate\":\"" << dueDate.str()
        << "\",\"tags\":" << tagsString() 
        << "}";
    return sttr.str();
}

Json Task::json() const {
    Json j;
    j["completed"] = completed;
    j["dueDate"] = dueDate.str();
    for (auto it = tags.begin(); it != tags.end(); it++) {
        j["tags"].push_back(*it);
    }
    return j;
}

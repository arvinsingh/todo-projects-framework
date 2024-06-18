/*
 * Author: Arvin Singh
 * Date: 04/04/2024
 * Description: This file contains the declaration of the Task class.
 * The Task class contains multiple 'tags' (e.g., a tag might be 'home' and
 * another tag could be 'uni'). A Task also has a identifier (e.g. 'Complete
 * Coursework') a completed status (completed or not) and due date. An
 * uninitialised due date (see date.cpp/date.h) means the task does not have a
 * due date.
*/


#ifndef TASK_H
#define TASK_H

#include "date.h"

using TagContainer = std::vector<String>;
using Json = nlohmann::json;

class Task {

    private:
    String identifier;
    TagContainer tags;
    Date dueDate;
    bool completed;

    public:
    explicit Task(const String& identifier);
    ~Task() = default;

    const String getIdent() const noexcept;
    void setIndent(String& identifier);
    bool addTag(const String tag);
    const TagContainer getTags() const noexcept;
    bool findTag(const String tag);
    bool deleteTag(String tag);
    void mergeTags(const TagContainer& tags);
    const unsigned int numTags() const noexcept;
    bool containsTag(String tag) const;
    String tagsString() const;

    Date getDueDate() const noexcept;
    void setDueDate(Date date);

    void setComplete(bool aBool);
    const bool isComplete() const noexcept;
    const String completeStr() const noexcept;

    friend bool operator==(const Task& task1, const Task& task2);
    String str() const;
    Json json() const;

};

#endif // TASK_Hs

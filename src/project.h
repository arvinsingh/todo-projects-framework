/*
 * Author: Arvin Singh
 * Date: 05/04/2024
 * Description: This file contains the declaration of the Project class.
 * A project contains one or more tasks, each with their own identifier ('ident').
*/


#ifndef PROJECT_H
#define PROJECT_H

#include <map>
#include <string>
#include <utility>

#include "lib_json.hpp"
#include "task.h"


using TaskContainer = std::vector<Task>;

class Project {
  String ident;
  TaskContainer tasks;

public:
  explicit Project(String ident);
  ~Project() = default;

  unsigned int size() const noexcept;
  const String &getIdent() const noexcept;

  void setIdent(String pIdent) noexcept;

  const TaskContainer &getTasks() const noexcept;
  Task &newTask(const String &tIdent);
  TaskContainer::iterator findTask(const String &tIdent);
  bool containsTask(const String &tIdent) const noexcept;

  bool addTask(Task task);
  Task &getTask(const String &tIdent);
  bool deleteTask(const String &tIdent);

  friend bool operator==(const Project &c1, const Project &c2);

  Json json() const;
  String str() const;

  // Wrappers for iterating over the nested container
  inline TaskContainer::iterator begin() { return tasks.begin(); }
  inline TaskContainer::const_iterator cbegin() const { return tasks.cbegin(); }

  inline TaskContainer::iterator end() { return tasks.end(); }
  inline TaskContainer::const_iterator cend() const { return tasks.cend(); }

  inline TaskContainer::reverse_iterator rbegin() { return tasks.rbegin(); }
  inline TaskContainer::const_reverse_iterator crbegin() const {
    return tasks.crbegin();
  }

  inline TaskContainer::reverse_iterator rend() { return tasks.rend(); }
  inline TaskContainer::const_reverse_iterator crend() const {
    return tasks.crend();
  }
};

struct AddTaskError : public std::runtime_error {
  explicit AddTaskError(const String &tIdent)
      : std::runtime_error("could not add task with identifier '" + tIdent +
                           "'") {
    /* do nothing */
  }

  ~AddTaskError() override = default;
};

struct NoTaskError : public std::out_of_range {
  explicit NoTaskError(const String &tIdent)
      : std::out_of_range("unknown task with identifier '" + tIdent + "'") {
    /* do nothing */
  }

  ~NoTaskError() override = default;
};

#endif // PROJECT_H

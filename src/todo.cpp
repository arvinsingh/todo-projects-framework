/*
 * Author: Arvin Singh
 * Date: 06/04/2024
 * Description: App namespace implementation for the helper functions
 * used for initialising and running todo.
*/


#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include "todo.h"
#include "lib_cxxopts.hpp"

/**
 * @brief Run the todo application.
 * 
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return int The exit code.
*/
int App::run(int argc, char *argv[]) {
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);

  // Print the help usage if requested
  if (args.count("help")) {
    std::cout << options.help() << '\n';
    return 0;
  }

  // Open the database and construct the TodoList
  const String db = args["db"].as<String>();
  TodoList tlObj{};

  tlObj.load(db);

  const Action a = parseActionArgument(args);

  switch (a) {

    case Action::CREATE: {
      // FOR CREATE ACTION
      if (args["project"].count()) {
        String projectIdent = args["project"].as<String>();
        if (!tlObj.containsProject(projectIdent)) {
          tlObj.newProject(projectIdent);
        }

        if (args["task"].count()) {
          String taskIdent = args["task"].as<String>();
          if (!tlObj.getProject(projectIdent).containsTask(taskIdent)) {
            tlObj.getProject(projectIdent).newTask(taskIdent);
          }
          if (args["tag"].count()) {
            std::vector<String> tags = splitTags(args["tag"].as<String>());
            for (String tag : tags) {
              if (!tlObj.getProject(projectIdent).getTask(taskIdent).containsTag(tag)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).addTag(tag);  
              }
            }
          }

          if (args["due"].count()) {
            String dateAsString = args["due"].as<String>();
            Date dueDate = Date();
            try {
              dueDate.setDateFromString(dateAsString);
            } catch (const std::invalid_argument& e) {
              std::cerr << "Invalid date: " << dateAsString << std::endl;
              return 1;
            }
            tlObj.getProject(projectIdent).getTask(taskIdent).setDueDate(dueDate);
          }

          if (args["completed"].count()) {
            tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(true);
          } 
          if (args["incomplete"].count()) {
            tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(false);
          }
        }    
        tlObj.save(db);      
      } else {
        std::cerr << "Error: missing project, task, tag, due, completed/incomplete argument(s)." << std::endl;
        return 1;
      }
      
      break;
    }

    case Action::JSON: {
      // FOR JSON ACTION

      if (args["project"].count()) {
        String projectIdent = args["project"].as<String>();

        if (tlObj.containsProject(projectIdent)) {
          auto& project = tlObj.getProject(projectIdent);
          if (args["task"].count()) {
            String taskIdent = args["task"].as<String>();
            
            if (project.containsTask(taskIdent)) {
              auto& task = tlObj.getProject(projectIdent).getTask(taskIdent);
              if (args["tag"].count()) {
                String tag = args["tag"].as<String>();
                if (task.containsTag(tag)) {
                  std::cout << tag << std::endl;
                } else {
                  std::cerr << "Error: invalid tag arguments(s)." << std::endl;
                  return 1;
                }
              } else {
                std::cout << task.json() << std::endl;
              }
            } else {
              std::cerr << "Error: invalid task argument(s)." << std::endl;
              return 1;
            }
          } else {
            std::cout << project.json() << std::endl;
          }
        } else {
          std::cerr << "Error: invalid project argument(s)." << std::endl;
          return 1;
        }
      } else if ((args.count("task") || args.count("tag")) && !args.count("project")) {
        std::cerr << "Error: missing project argument(s)." << std::endl;
        return 1;
      } else {
        std::cout << tlObj.json() << std::endl;
      }
      break;
    }

    case Action::UPDATE: {
      // FOR UPDATE ACTION

      if (args["project"].count() && args["task"].count()) {
        String projectIdent = args["project"].as<String>();
        
        
        if (projectIdent.find(':') != String::npos) {
          String oldIdent = projectIdent.substr(0, projectIdent.find(':'));
          String newIdent = projectIdent.substr(projectIdent.find(':') + 1);
          if (tlObj.containsProject(oldIdent)) {
            tlObj.getProject(oldIdent).setIdent(newIdent);
            projectIdent = newIdent;
          } else {
            std::cerr << "Project " << oldIdent << " not found." << std::endl;
            return 1;
          }
        }
        if (args["task"].count()) {
          String taskIdent = args["task"].as<String>();
          if (taskIdent.find(':') != String::npos) {
            String oldIdent = taskIdent.substr(0, taskIdent.find(':'));
            String newIdent = taskIdent.substr(taskIdent.find(':') + 1);
            if (tlObj.getProject(projectIdent).containsTask(oldIdent)) {
              tlObj.getProject(projectIdent).getTask(oldIdent).setIndent(newIdent);
              taskIdent = newIdent;
              
            } else {
              std::cerr << "Task " << oldIdent << " not found." << std::endl;
              return 1;
            }
          }
          if (args.count("completed")) {
              if (tlObj.containsProject(projectIdent) && tlObj.getProject(projectIdent).containsTask(taskIdent)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(true);
              } else {
                std::cerr << "Error project or task not found." << std::endl;
                return 1;
              }
          }
          if (args.count("incomplete")) {
              if (tlObj.containsProject(projectIdent) && tlObj.getProject(projectIdent).containsTask(taskIdent)) {
                tlObj.getProject(projectIdent).getTask(taskIdent).setComplete(false);
              } else {
                std::cerr << "Error project or task not found." << std::endl;
                return 1;
              }
          }
          if (args.count("due")) {
            String dueDateStr = args["due"].as<String>();
            tlObj.getProject(projectIdent).getTask(taskIdent).getDueDate().setDateFromString(dueDateStr);
          }
        }
        tlObj.save(db);
      }
      break;
    }

    case Action::DELETE: {
      // FOR DELETE ACTION

      if (args.count("project")) {
        String projectIdent = args["project"].as<String>();
        if (tlObj.containsProject(projectIdent)) {
          if (args.count("task")) {
            String taskIdent = args["task"].as<String>();
            if (tlObj.getProject(projectIdent).containsTask(taskIdent)) {
              if (args.count("tag")) {
                String tag = args["tag"].as<String>();
                if (!tlObj.getProject(projectIdent).getTask(taskIdent).deleteTag(tag)) {
                  std::cerr << "Tag " << tag << " not found in task " << taskIdent << " in project " 
                    << projectIdent << std::endl;
                  return 1;
                }
              } else if (args.count("due")) {
                tlObj.getProject(projectIdent).getTask(taskIdent).getDueDate().setInitialised(false);
              } else {
                tlObj.getProject(projectIdent).deleteTask(taskIdent);
              }
            } else {
              std::cerr << "Task " << taskIdent << " not found in Project " << projectIdent << std::endl;    
              return 1;
            }
          } else {
            tlObj.deleteProject(projectIdent);
          }
        } else {
          std::cerr << "Project " << projectIdent << "not found." << std::endl;
          return 1;
        }
        tlObj.save(db);
      }
      
      break;
    }
  }
  return 0;
}


/**
 * @brief Setup the cxxopts options.
 * print the usage if the help flag is set.
 * 
 * @return cxxopts::Options The cxxopts options.
*/
cxxopts::Options App::cxxoptsSetup() {
  cxxopts::Options cxxopts("Todo", "Coder " + PROGRAMMER + "\n");

  cxxopts.add_options()(
      "db", "Filename of the todo database",
      cxxopts::value<String>()->default_value("database.json"))(

      "action", "Action to take, can be: 'create', 'json', 'update', 'delete'.",
      cxxopts::value<String>())(

      "project",
      "Apply action (create, json, update, delete) to a project. If you want to "
      "add a project, set the action argument to 'create' and the project "
      "argument to your chosen project identifier.",
      cxxopts::value<String>())(

      "task",
      "Apply action (create, json, update, detele) to a task. If you want to add "
      "a task, set the action argument to 'create', the project argument to your "
      "chosen project identifier and the task argument to the task identifier).",

      cxxopts::value<String>())(

      "tag",
      "Apply action (create, json, delete) to a tag.  If you want to add an tag, "
      "set the action argument to 'create', the project argument to your chosen "
      "project identifier, the task argument to your chosen task identifier, and "
      "the tag argument to a single tag 'tag' or comma seperated list of tags: "
      "'tag1,tag2').  If you are simply retrieving a tag through the json action "
      "(and checking that it exists), set the tag argument to the tag name "
      "(e.g. 'example tag'). The action update is unsupported here.",
      cxxopts::value<String>())(

      "completed",
      "When creating or updating a task, set the completed flag to change the "
      "task's state to completed. This flag is not compatible in combination "
      "with the incomplete flag.",
      cxxopts::value<bool>())(

      "incomplete",
      "When creating or updating a task, set the incomplete flag to change the "
      "task's state to incomplete. This flag is not compatible in combination "
      "with the completed flag",
      cxxopts::value<bool>())(

      "due",
      "When creating or updating a task, set the due date flag to change the "
      "task's due date to the one specified as an argument (e.g. '2024-11-23')."
      "Ommitting the argument removes the due date from the task.",
      cxxopts::value<String>())(

      "h,help", "Print usage.");

  return cxxopts;
}

/**
 * @brief Parse the action argument.
 * 
 * @param args The cxxopts parse result.
 * @return App::Action The action.
*/
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
// enum Action { CREATE, JSON, DELETE, UPDATE };
  String input = args["action"].as<String>();
  // Convert the input to lowercase 
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);
  if(input == "create") {
    return Action::CREATE;
  } else if(input == "json") {
    return Action::JSON;
  } else if(input == "delete") {
    return Action::DELETE;
  } else if(input == "update") {
    return Action::UPDATE;
  }
  throw std::invalid_argument("action");
}

// Function to return the JSON representation of the TodoList object
String App::getJSON(TodoList &tlObj) {
  return tlObj.str();
}

// Function to return the JSON representation of a specific Project in a TodoList object
String App::getJSON(TodoList &tlObj, const String &p) {
  auto pObj = tlObj.getProject(p);
  return pObj.str();
}

// Function to return the JSON representation of a specific Task in a TodoList object
String App::getJSON(TodoList &tlObj, const String &p,
                         const String &t) {
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(t);
  return tObj.str();
}

// Function to return the JSON representation of a specific Tag in a TodoList object
String App::getJSON(TodoList &tlObj, const String &p,
                         const String &task, const String &tag) {
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(task);
  if (tObj.containsTag(tag)) {
    return tag;
  } else {
    return "";
  }
}

// Function to split the tags string into a vector of tags
std::vector<String> App::splitTags(const String& stringTags) {
  std::vector<String> tags;
  std::stringstream stringStream(stringTags);
  String tag;
  while (std::getline(stringStream, tag, ',')) {
      tags.push_back(tag);
  }
  return tags;
}

/*
 * Author: Arvin Singh
 * Date: 06/04/2024
 * Description: Declarations for the helper functions 
 * used for initialising and running todo.
*/


#ifndef _TODO_H
#define _TODO_H


#include "lib_cxxopts.hpp"
#include "todolist.h"

namespace App {

// Constant for User name
const String PROGRAMMER = "Arvin Singh";

// enum for the different actions that can be performed
enum Action { CREATE, JSON, DELETE, UPDATE };

int run(int argc, char *argv[]);

cxxopts::Options cxxoptsSetup();

App::Action parseActionArgument(cxxopts::ParseResult &args);

String getJSON(TodoList &tl);
String getJSON(TodoList &tl, const String &p);
String getJSON(TodoList &tl, const String &p, const String &t);
String getJSON(TodoList &tl, const String &p, const String &task,
                    const String &tag);

std::vector<String> splitTags(const String& stringTags);

} // namespace App

#endif // _TODO_H

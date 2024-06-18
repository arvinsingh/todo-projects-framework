
// -----------------------------------------------------
// Catch2 — https://github.com/catchorg/Catch2
// Catch2 is licensed under the BOOST license
// -----------------------------------------------------
// This file contains tests loading JSON files into
// the TodoList.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/todolist.h"

SCENARIO("A TodoList object can load from a JSON file", "[todolist]") {

  const std::string filePath = "./tests/testdatabasealt.json";

  auto fileExists = [](const std::string &path) {
    std::cout << path;
    return std::ifstream(path).is_open();
  };

  auto writeFileContents = [](const std::string &path,
                              const std::string &contents) {
    // Not a robust way to do this, but here it doesn't matter so much, if it
    // goes wrong we'll fail the test anyway…
    std::ofstream f{path};
    f << contents;
  };

  GIVEN("a valid path to a reset database JSON file") {

    // Reset the file...
    REQUIRE(fileExists(filePath));
    REQUIRE_NOTHROW(writeFileContents(
        filePath,
        "{ \"M02\": { \"Lab Assignment 1\": { \"completed\": true, "
        "\"dueDate\": \"2024-02-13\", \"tags\": [ \"uni\", \"c\", "
        "\"programming\" ] }, \"Lab Assignment 6\": { \"completed\": false, "
        "\"dueDate\": \"2024-04-23\", \"tags\": [ \"uni\", \"c++\", "
        "\"programming\", \"standard library\" ] } }, \"M118\": { \"Write "
        "Mobile App\": { \"completed\": true, \"dueDate\": \"2023-11-30\", "
        "\"tags\": [ \"uni\", \"programming\", \"android\" ] } } }"));

    WHEN("a new empty TodoList object is constructed") {

      TodoList tlObj1{};
      REQUIRE(tlObj1.size() == 0);

      AND_WHEN("the load function is called with the file path") {

        THEN("the file will be imported without an exception") {

          REQUIRE_NOTHROW(tlObj1.load(filePath));
          REQUIRE(tlObj1.size() == 2);

          REQUIRE_NOTHROW(tlObj1.getProject("M02"));
          REQUIRE(tlObj1.getProject("M02").size() == 2);

          REQUIRE_NOTHROW(
              tlObj1.getProject("M02").getTask("Lab Assignment 1"));
          REQUIRE(tlObj1.getProject("M02")
                      .getTask("Lab Assignment 1")
                      .numTags() == 3);

          REQUIRE(tlObj1.getProject("M02")
                      .getTask("Lab Assignment 1")
                      .containsTag("programming"));

          REQUIRE(tlObj1.getProject("M02")
                      .getTask("Lab Assignment 1")
                      .isComplete());
          Date d;
          REQUIRE_NOTHROW(d = tlObj1.getProject("M02")
                                  .getTask("Lab Assignment 1")
                                  .getDueDate());
          REQUIRE(d.getDay() == 13);
          REQUIRE(d.getMonth() == 2);
          REQUIRE(d.getYear() == 2024);

          REQUIRE_NOTHROW(
              tlObj1.getProject("M02").getTask("Lab Assignment 6"));
          REQUIRE(tlObj1.getProject("M02")
                      .getTask("Lab Assignment 6")
                      .numTags() == 4);
          REQUIRE_FALSE(tlObj1.getProject("M02")
                            .getTask("Lab Assignment 6")
                            .isComplete());

          REQUIRE_NOTHROW(tlObj1.getProject("M118"));
          REQUIRE(tlObj1.getProject("M118").size() == 1);
          REQUIRE_NOTHROW(
              tlObj1.getProject("M118").getTask("Write Mobile App"));
          REQUIRE(tlObj1.getProject("M118")
                      .getTask("Write Mobile App")
                      .numTags() == 3);
          REQUIRE(tlObj1.getProject("M118")
                      .getTask("Write Mobile App")
                      .isComplete());

        } // THEN

      } // AND_WHEN

    } // WHEN

  } // GIVEN

} // SCENARIO

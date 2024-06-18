# todo Projects framework in c++

#### Build script & Test suites

```> build.sh``` for shell script
```> build.bat``` for windows commandline

##### For testing
> build.sh testN
> build.bat testN

Replace `N` with test suite number or with `all` to run all tests.
Build without arguments to 

> [!WARNING]
> These test suites *do not provide complete coverage*.

Both scripts will build binaries to C++14 standard & will be placed in bin/ directory.

#### Program Arguments

    USAGE: > todo --action json
    This command will print the data stored in json file to the terminal.

  Todo [OPTION...]

      --db arg
      --action arg   Action to take, can be: 'create', 'json', 'update', 'delete'.

      --project arg  Apply action (create, json, update, delete) to a project. 
                     If you want to add a project, set the action argument to 
                     'create' and the project argument to your chosen project
                     identifier.

      --task arg     Apply action (create, json, update, detele) to a task.
                     If you want to add a task, set the action argument to '
                     create', the project argument to your chosen project id
                     entifier and the task argument to the task identifier).

      --tag arg      Apply action (create, json, delete) to a tag.  If you
                     want to add an tag, set the action argument to 'create',
                     the project argument to your chosen project identifier, 
                     the task argument to your chosen task identifier, and
                     the tag argument to a single tag 'tag' or comma sepera
                     ted list of tags: 'tag1,tag2').  If you are simply 
                     retrieving a tag through the json action (and checking 
                     that it exists), set the tag argument to the tag name 
                     (e.g.'example tag'). The action update is unsupported 
                     here.

      --completed    When creating or updating a task, set the completed flag 
                     to change the task's state to completed. This flag is not 
                     compatible in combination with the incomplete flag.

      --incomplete   When creating or updating a task, set the incomplete flag 
                     to change the task's state to incomplete. This flag is not 
                     compatible in combination with the completed flag.

      --due arg      When creating or updating a task, set the due date flag to 
                     change the task's due date to the one specified as an 
                     argument (e.g. '2024-11-23'). Ommitting the argument removes 
                     the due date from the task.

    -h, --help       To display the help options.

#### External libraries

> Catch2 unit testing framework used for test suites.

> CXXOpts used to assist parsing the command line arguments 

> JSON for Modern C++ used to help parse JSON files quickly and easily.


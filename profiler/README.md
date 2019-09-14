# Profiling Framework
This directory contains a script to run all profiling tests 
(```run_tests.sh```) and each sub-directory is the representation of such a 
test.  Adding new tests simply requires adding another subdirectory with the 
necessary files within it. The ```run_tests.sh``` script will automatically 
detect such a directory and run the corresponding tests. Timing information 
(time spent in user code and system code) are also produced for each test.

## Creating new tests
To create a new test, create a sub-directory within this directory that has a 
meaningful name. Within that subdirectory you must place at least the 
following files:

  * ```README.md``` - Describe your test, the format of the input, and the 
format of the output.

  * ```driver.cpp``` - This is the program to be compiled that defines and 
executes the streaming workflow. You must name such a file 
```driver.cpp``` although having additional and arbitrary header files is 
quite alright as well.

  * ```input.data``` - The data that will be consumed by your 
application. This file will be fed to the ```stdin``` of your 
application and therefore your ```driver.cpp``` file should plan to 
consume its input from ```stdin```.

  * ```output.data``` - The testing framework will inspect the output 
produced by the application and verify that it exactly matches the 
contents of this ```output.data``` file. Therefore ```output.data``` 
should be considered the gold-standard solution for the test.

You may include any additional files that you wish however it is an 
expectation that at least all of the above files be included in a new 
test's directory when it is added to the repository.

## Running profiling tests
To execute all tests in quick succession, return to the main directory of the 
repository and type: ```make profiler```

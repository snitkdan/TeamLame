# Project 2: Learning the Development Environment – The Next Step
This directory contains the source code for [Project 2](https://class.ee.washington.edu/474/peckol/assignments/lab2/lab2Summer2017.pdf)

A Makefile has been provided to run the necessary commands to compile the source files into binary executables for testing and deployment.

Within the a2/ directory, run the following command:
```
make
```
## Testing
This project utilizes the [Google Test C++ Testing Framework](https://github.com/google/googletest).

A test suite has been provided that performs unit tests
on the data intensive TCB task methods. (i.e. "satelliteComs",
"thrusterSubsystem", and "powerSubsystem"). The source files for said tests are found in the [*test*]test.cc files.

To use the test suite, follow the instructions below:

1. Compile the test suite within the a2/ directory
(either on a host machine or the Beaglebone) via
the following command:
```
make test_suite
```
2. Execute the test suite by running the following command
within the same directory:
```
./test_suite
```

_To add more unit tests, simply add the appropriate [*test*]test.cc
file to the a2/ directory and add the appropriate commands to the
Makefile to link it to the test_suite._

## Deployment
To deploy the main program on the Beaglebone, follow
the instructions below:

1. Establish a USB connection with
the Beaglebone Black and a host machine.
2. Transfer the a2/ directory
to a directory within the "/dev" folder
by running the following command
within the a2/ directory on the host machine:
```
make copy
```
3. Compile the main file on the Beaglebone,
using the following command:
```
make main
```
4. Execute the main file by running
the following command in the same directory:
```
./main
```


## Experiments
To reproduce the results for the TCB task execution, follow the
instructions below:
1. Edit the "timeTask" variable
to reflect the task to be timed (corresponding to the tasks
position in the task queue)
2. Connect a scope probe to GPIO48 on the Beaglebone.
(For set-up, reference the [lab manual](https://class.ee.washington.edu/474/peckol/assignments/lab2/lab2Summer2017.pdf))
3. Compile the experimental code via the following command:
```
make gpio
```
4. Execute the test via the following command:
```
./main
```

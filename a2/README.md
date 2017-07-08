# Project 2: Learning the Development Environment – The Next Step
This directory contains the source code for [Project 2](https://class.ee.washington.edu/474/peckol/assignments/lab2/)

A Makefile has been provided to run the necessary commands to compile the source files into binary executables for testing and deployment.

Within the a2/ directory, run the following command:

```
make
```

## Testing
This project utilizes the [Google Test C++ Testing Framework](https://github.com/google/googletest).

A test suite has been provided that performs unit tests
on all the TCB task methods. The source files for said tests
are found in the [*test*]test.cc files.

To re-compile the test suite, run
the following command within the a2/ directory:

```
make test_suite
```

To execute the test_suite, run the following command
within the same directory:
```
./test_suite
```

To add more unit tests, simply add the appropriate [*test*]test.cc
file to the a2/ directory and add the appropriate commands to the
Makefile to link it to the test_suite.

## Deployment
The deployment module utilizes the source code in "main.c"
to orchestrate the various TCBs.

To re-compile the main file, run the
following command within the a2/ directory:
```
make main
```

To execute the main file, run the following command
within the same directory:

```
./main
```

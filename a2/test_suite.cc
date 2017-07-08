/*
  test_suite.cc - This contains the main method
  that invokes all unit tests.
*/

#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

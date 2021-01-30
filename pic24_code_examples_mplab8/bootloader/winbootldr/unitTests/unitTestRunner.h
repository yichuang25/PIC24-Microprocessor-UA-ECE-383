// unitTests.h

/** \file
 *  \brief Runs the unit test on MSVC using xUnit.
 *
 *  This is implemented in C as much as possible to make these unit
 *  tests portable to other platforms. All tests are therefore
 *  implemented in unitTests.c.
 */

#pragma once

using namespace System;
using namespace Xunit;

#include "dataXfer.h"


void runAllTests();


namespace unitTests {

/** A simple test class. Since this code is targeted to run under C or
 *  C++, not only C++/CLI, put in the minimal structure here which will
 *  invoke the tests defined in unitTests.cpp. */
public ref class TestClass {
public:
  /// Make sure the test framework functions
  [Fact]
  void myTest() {
    Assert::Equal(2 + 2, 4);
  };

  [Fact]
  /// Run all the tests; written to make this portable to C
  /// and non .NET platforms.
  void testAll() {
    runAllTests();
  };
};
}



# Boot MySQL
add_test(NAME prepare
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/prep.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME cleanup COMMAND "${CMAKE_CURRENT_LIST_DIR}/cleanup.sh" "${CMAKE_BINARY_DIR}")
add_test(NAME test1 COMMAND "${CMAKE_CURRENT_LIST_DIR}/test1.sh" "${CMAKE_BINARY_DIR}")

set_tests_properties(test1 PROPERTIES ENVIRONMENT TEST_FOLDER=${CMAKE_CURRENT_LIST_DIR})
set_tests_properties(test1 PROPERTIES FIXTURES_REQUIRED Db)
set_tests_properties(prepare         PROPERTIES FIXTURES_SETUP    Db)
set_tests_properties(cleanup         PROPERTIES FIXTURES_CLEANUP  Db)

  #set_tests_properties(dependsTest12 PROPERTIES DEPENDS "baseTest1;baseTest2")
  # dependsTest12 runs after baseTest1 and baseTest2, even if they fail

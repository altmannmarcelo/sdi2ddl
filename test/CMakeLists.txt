SET(LOCAL_TESTS OFF CACHE BOOL "Flag to change the test to local mode")

IF(LOCAL_TESTS)
  MESSAGE("-- Running tests in local mode")
ENDIF()

# Boot MySQL
add_test(NAME prepare
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/prep.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME cleanup
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/cleanup.sh"
  "${CMAKE_BINARY_DIR}")

# Tests
add_test(NAME test1
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test1.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test2_generated_columns
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test2_generated_columns.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test3_json
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test3_json.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test4_functional_index
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test4_functional_index.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test5_fk
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test5_fk.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test6_prefix_index
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test6_prefix_index.sh"
  "${CMAKE_BINARY_DIR}")
add_test(NAME test7_gipk
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/test7_gipk.sh"
  "${CMAKE_BINARY_DIR}")
set_tests_properties(
  prepare
  test1
  test2_generated_columns
  test3_json
  test4_functional_index
  test5_fk
  test6_prefix_index
  test7_gipk
  PROPERTIES ENVIRONMENT "LOCAL_MODE=${LOCAL_TESTS};TEST_FOLDER=${CMAKE_CURRENT_LIST_DIR}")

set_tests_properties(
  test1
  test2_generated_columns
  test3_json
  test4_functional_index
  test5_fk
  test6_prefix_index
  test7_gipk
  PROPERTIES FIXTURES_REQUIRED Db)
set_tests_properties(prepare         PROPERTIES FIXTURES_SETUP    Db)
set_tests_properties(cleanup         PROPERTIES FIXTURES_CLEANUP  Db)

cmake_minimum_required(VERSION 3.16.3)

project(sdi2ddl)


MACRO (FIND_RAPIDJSON)
  MESSAGE("-- Searching for rapidjson/document.h")
  FIND_FILE(RAPIDJSON_INCLUDE_DIR NAMES rapidjson/document.h NO_CACHE)
  IF (RAPIDJSON_INCLUDE_DIR)
    MESSAGE("-- Searching for rapidjson/document.h - done")
  ELSE()
    MESSAGE(SEND_ERROR "Could not find rapidjson/document.h")
  ENDIF()
ENDMACRO()

set(CMAKE_BUILD_TYPE Debug)

FIND_RAPIDJSON()
add_executable ( sdi2ddl src/sdi2ddl.cc )

enable_testing()

add_subdirectory(test)

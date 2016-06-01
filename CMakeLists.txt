cmake_minimum_required (VERSION 2.8.7)

project (jill C)

option (LIBJILL_TESTS "Build and run libjill tests" ON)

add_definitions(-std=gnu11)
include_directories (src/3rdparty/cbitset/include)
include_directories (src)

if (LIBJILL_TESTS)
  enable_testing ()
  set (all_tests "")

  macro (add_libjill_test NAME TIMEOUT)
    list (APPEND all_tests ${NAME})
    add_executable (${NAME} tests/${NAME}.c)
    target_link_libraries (${NAME} ${PROJECT_NAME})
    add_test (NAME ${NAME} COMMAND ${NAME})
    set_tests_properties (${NAME} PROPERTIES TIMEOUT ${TIMEOUT})
  endmacro (add_libjill_test)

  #  Storage tests
  add_libjill_test (bitset 5)
  add_libjill_test (field 5)
  add_libjill_test (vallist 5)

  #  Build the performance tests.

  macro (add_libjill_perf NAME)
    add_executable (${NAME} perf/${NAME}.c)
    target_link_libraries (${NAME} ${PROJECT_NAME})
  endmacro (add_libjill_perf)

  add_libjill_perf (perf_jbitset)
  add_libjill_perf (perf_vallist)
endif ()

add_subdirectory (src)
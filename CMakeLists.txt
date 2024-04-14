cmake_minimum_required (VERSION 3.1)

project(test)

add_library(aparselib
    src/argparse/args.cpp
    src/argparse/args.h)
set_target_properties(aparselib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

add_library(algolib
    src/algo/word_automata.cpp
    src/algo/word_automata.h)
set_target_properties(algolib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

add_executable(test
    src/main.cpp)
set_target_properties(test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

add_dependencies(test
    aparselib
    algolib)
target_link_libraries(test
    aparselib
    algolib)


# Unit tests.
add_executable(unittest
    src/tests/main.cpp
    src/tests/aparse_unittest.cpp
    src/tests/word_unittest.cpp)
set_target_properties(unittest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_include_directories(unittest PRIVATE
    "${PROJECT_SOURCE_DIR}/src/algo"
    "${PROJECT_SOURCE_DIR}/src/argparse")
add_dependencies(unittest
    algolib
    aparselib)
target_link_libraries(unittest
    algolib
    aparselib)


# GoogleTest - find in packages or download from github and compile on-the-fly.

find_package(GTest)
if (GTest_FOUND)
  message(STATUS "Using installed googletest package.")

  target_include_directories(unittest PRIVATE
      ${GTEST_INCLUDE_DIRS})
  target_link_libraries(unittest
      ${GTEST_LIBRARIES})

else()
  message(STATUS "Fallback to googletest downloading on-the-fly.")

  # Download googletest
  configure_file(CMakeLists.txt.gtest.in googletest-download/CMakeLists.txt)

  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                  RESULT_VARIABLE result
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
                  RESULT_VARIABLE result
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Prevent overriding the parent project's compiler/linker on windows.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest to build. This defines the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                   ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                   EXCLUDE_FROM_ALL)
  target_include_directories(unittest PRIVATE
      "${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
      "${CMAKE_CURRENT_BINARY_DIR}/googletest-src/include")
  target_link_libraries(unittest
      gtest_main)
endif()

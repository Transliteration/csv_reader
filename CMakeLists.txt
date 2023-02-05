cmake_minimum_required(VERSION 3.22.0)
project(YadroTest VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(YadroTest src/main.cpp)

target_sources(YadroTest PRIVATE ./src/argParser/argParser.cpp)
target_sources(YadroTest PRIVATE ./src/util/util.cpp)
target_sources(YadroTest PRIVATE ./src/csvReader/csvReader.cpp)

add_executable(
  util_test
  ./test/util_test.cpp
  ./src/util/util.cpp
)
target_link_libraries(
  util_test
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(util_test)

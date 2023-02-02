cmake_minimum_required(VERSION 3.16.0)
project(YadroTest VERSION 0.1.0)
# set_property(TARGET YadroTest PROPERTY CXX_STANDARD `20`)
set (CMAKE_CXX_STANDARD 20)

# include(CTest)
# enable_testing()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(YadroTest)
# include_directories(src)
add_executable(YadroTest src/main.cpp )

target_sources(YadroTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/argParser/argParser.cpp)
target_sources(YadroTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/util/util.cpp)
target_sources(YadroTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/csvReader/csvReader.cpp)


# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

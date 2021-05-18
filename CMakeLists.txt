cmake_minimum_required(VERSION 3.1.3)
project(BicyclePowerPlan)

set(PROFILE_SUPPORT ON)

include_directories("./include")

if (PROFILE_SUPPORT)
    add_compile_definitions(PROFILE_PROJECT)
endif()

# include(FetchContent)

# FetchContent_Declare(json
#   GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
#   GIT_TAG v3.7.3)

# FetchContent_GetProperties(json)
# if(NOT json_POPULATED)
#   FetchContent_Populate(json)
#   add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()

set(SOURCE_LIST)
set(HEADER_LIST)

list(APPEND SOURCE_LIST
        src/Encoding.cpp
        src/EvolutionalStrategy.cpp
        src/BicyclePlan.cpp
        src/Util.cpp
)
list(APPEND HEADER_LIST
        include/Encoding.h
        include/EvolutionalStrategy.h
        include/BicyclePlan.h
        include/Util.H
)

add_executable(BicyclePowerPlan src/main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(BicyclePowerPlan) # PRIVATE nlohmann_json::nlohmann_json)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/965f8ecbfd8b91bbd4f5ee4914c028660bb89029.zip
#)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
#enable_testing()
#
#add_executable(
#  testbed
#   ${HEADER_LIST} ${SOURCE_LIST}
#  test/testbed.cc
#)
#target_link_libraries(
#  testbed
#  gtest_main
#)
#
#include(GoogleTest)
#gtest_discover_tests(testbed)
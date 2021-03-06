cmake_minimum_required(VERSION 3.1.3)
project(BicyclePowerPlan)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("release mode")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    message("debug mode")
    set(PROFILE_SUPPORT ON)
    if (PROFILE_SUPPORT)
        add_compile_definitions(PROFILE_PROJECT)
    endif()
endif()

include_directories("./include")

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
        src/Cyclist.cpp
        src/Config.cpp
        src/Track.cpp
        src/util.cpp
        src/timer.cpp
)
list(APPEND HEADER_LIST
        include/Encoding.h
        include/EvolutionalStrategy.h
        include/BicyclePlan.h
        include/Cyclist.h
        include/Config.h
        include/Track.h
        include/util.h
        include/timer.h
)

add_executable(BicyclePowerPlan src/main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(BicyclePowerPlan) # PRIVATE nlohmann_json::nlohmann_json)

add_executable(
  testbed
   ${HEADER_LIST} ${SOURCE_LIST}
  test/src/BicyclePlan_Test.cpp
)

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
cmake_minimum_required(VERSION 3.15)

project(MathFunctions VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DEMO "Build demo" ON)

add_library(MathFunctions
    src/MathFunctions.cpp
)

target_include_directories(MathFunctions
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(BUILD_DEMO)
    add_executable(demo main.cpp)
    target_link_libraries(demo PRIVATE MathFunctions)
endif()

if(BUILD_TESTS)
    include(FetchContent)
    
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    FetchContent_MakeAvailable(googletest)
    
    add_executable(tests tests/main_tests.cpp)
    
    target_link_libraries(tests
        PRIVATE
            MathFunctions
            GTest::gtest_main
            GTest::gmock
    )
    
    target_include_directories(tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    include(GoogleTest)
    
    gtest_discover_tests(tests)
endif()

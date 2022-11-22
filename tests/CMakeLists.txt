CPMFindPackage(
    NAME Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
    EXCLUDE_FROM_ALL YES
)

add_executable(tests)
target_sources(tests PRIVATE main.cpp tr-sort.cpp)
target_link_libraries(
    tests
    PRIVATE
        tr-sort-compiler-options  # tests use same compiler options as main project
        tr-sort
        Catch2::Catch2                         # unit testing framework
)

enable_testing()

# auto-discover and add Catch2 tests from unit tests program
include(CTest)
include(Catch)

catch_discover_tests(tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# begin basic metadata
# minimum CMake version required for C++20 support, among other things
cmake_minimum_required(VERSION 3.15)

# detect if Project is being used as a sub-project of another CMake project
if(NOT DEFINED PROJECT_NAME)
    set(TR_SORT_SUBPROJECT OFF)
else()
    set(TR_SORT_SUBPROJECT ON)
endif()

project(tr-sort VERSION 0.3.0 LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Set a default build type if none was specified
set(DEFAULT_BUILD_TYPE "Debug")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# set some handy custom variables to detect Release-type builds from Debug-type ones
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TR_SORT_BUILD_DEBUG ON)
    set(TR_SORT_BUILD_RELEASE OFF)
else()
    set(TR_SORT_BUILD_DEBUG OFF)
    set(TR_SORT_BUILD_RELEASE ON)
endif()

message(STATUS "[tr-sort] Build Mode: ${CMAKE_BUILD_TYPE}")

# set the C++ standard to use to C++20 always
set(TR_SORT_CXX_STANDARD "20")
message(STATUS "[tr-sort] C++ Standard set to C++${TR_SORT_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD ${TR_SORT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)
# if building in Release mode, provide an option to explicitly enable tests if desired (always ON for other builds, OFF by default for Release builds)
cmake_dependent_option(ENABLE_TESTS "Build the unit tests in release mode?" OFF TR_SORT_BUILD_RELEASE ON)

# Premature Optimisation causes problems. Commented out code below allows detection and enabling of LTO.
# It's not being used currently because it seems to cause linker errors with Clang++ on Ubuntu if the library
# is compiled with LTO but the unit tests are not. This suggests LTO may force some downstream software into
# using LTO also if it's enabled. The plan is to reënable LTO as an option in the future, possibly done much
# more optionally (and probably not by default).

# include(CheckIPOSupported)
# check_ipo_supported(RESULT IPO_SUPPORTED)
# # If we're in Release mode, set TR_SORT_USE_IPO to ON by default if it's detected as supported (user can always explicitly enable it in Release mode)
# cmake_dependent_option(TR_SORT_USE_IPO "Use Link-Time/Inter-Procedural Optimisation?" ${IPO_SUPPORTED} TR_SORT_BUILD_RELEASE OFF)
# if(TR_SORT_USE_IPO)
#     message(STATUS "[tr-sort] Link-Time-Optimisation Enabled")
# endif()

set(
    TR_SORT_VERSION_STRING
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)
set(TR_SORT_ESCAPED_VERSION_STRING "\"${TR_SORT_VERSION_STRING}\"")
# end basic metadata

# This is a special target which only exists to capture compilation options
# used for tr-sort and its tests. This is to avoid setting global compiler
# options which would be inherited by dependencies as well, which is bad
# because tr-sort uses strict compiler warning options which not all other
# tr-sorts can build successfully with.
# Any target linked with this one will inherit the compiler options used for
# tr-sort.
add_library(tr-sort-compiler-options INTERFACE)

# used for enabling additional compiler options if supported
include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    message(STATUS "[tr-sort] Checking if compiler supports warning flag '${flag}'")
    check_cxx_compiler_flag("${flag}" flag_supported)
    if(flag_supported)
        message(STATUS "[tr-sort] Enabling warning flag '${flag}'")
        target_compile_options(tr-sort-compiler-options INTERFACE "${flag}")
    endif()
    unset(flag_supported CACHE)
endfunction()

# enable a large amount of extra warnings, regardless of build mode
if (MSVC) # MSVC supports different warning options to GCC/Clang
    enable_cxx_compiler_flag_if_supported("/W3") # MSVC Warning level 3, 4 is too pedantic
    # if tests are enabled, enable converting all warnings to errors too
    if (ENABLE_TESTS)
        enable_cxx_compiler_flag_if_supported("/WX")
    endif()
else() # GCC/Clang warning option
    # NOTE: GCC and Clang support most of the same options, but neither supports all
    # of the others'. By only enabling them if supported, we get graceful failure
    # when trying to enable unsupported flags
    # e.g. at the time of writing, GCC does not support -Wdocumentation
    #
    # enable all warnings about 'questionable constructs'
    enable_cxx_compiler_flag_if_supported("-Wall")
    # issue 'pedantic' warnings for strict ISO compliance
    enable_cxx_compiler_flag_if_supported("-pedantic")
    # enable 'extra' strict warnings
    enable_cxx_compiler_flag_if_supported("-Wextra")
    # enable sign conversion warnings
    enable_cxx_compiler_flag_if_supported("-Wsign-conversion")
    # enable warnings about mistakes in Doxygen documentation
    enable_cxx_compiler_flag_if_supported("-Wdocumentation")
    # if tests are enabled, enable converting all warnings to errors too
    if (ENABLE_TESTS)
        enable_cxx_compiler_flag_if_supported("-Werror")
        # exclude the following kinds of warnings from being converted into errors
        # unknown-pragma is useful to have as a warning but not as an error, if you have
        # pragmas which are for the consumption of one compiler only
        enable_cxx_compiler_flag_if_supported("-Wno-error=unknown-pragmas")
        # unused variable and function warnings are helpful but we don't need them as errors
        enable_cxx_compiler_flag_if_supported("-Wno-error=unused-function")
        enable_cxx_compiler_flag_if_supported("-Wno-error=unused-variable")
        enable_cxx_compiler_flag_if_supported("-Wno-error=unused-parameter")
        enable_cxx_compiler_flag_if_supported("-Wno-error=unused-private-field")
    endif()
endif()

# add custom dependencies directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# a better way to load dependencies
include(CPM)

# library
add_subdirectory(tr-sort)
# unit tests --only enable if requested AND we're not building as a sub-project
if(ENABLE_TESTS AND NOT TR_SORT_SUBPROJECT)
    message(STATUS "[tr-sort] Unit Tests Enabled")
    add_subdirectory(tests)
    enable_testing()
endif()

add_executable(main main.cpp)
target_link_libraries(main tr-sort)

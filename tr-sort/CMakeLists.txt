include(GNUInstallDirs)

add_library(tr-sort STATIC)
# ALIAS target to export a namespaced target even when building in-tree
add_library(TrSort::tr-sort ALIAS tr-sort)
# add source files
add_subdirectory(src)
# library public header files location
target_include_directories(
    tr-sort PUBLIC
    # different include directory path depending on if using the local or installed version of library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# pass in version of tr-sort as preprocessor definitions
target_compile_definitions(
    tr-sort PRIVATE
    -DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DPROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DPROJECT_VERSION_STRING=${TR_SORT_ESCAPED_VERSION_STRING}
)
# set up version and soversion for the main library object
set_target_properties(
    tr-sort PROPERTIES
    VERSION ${TR_SORT_VERSION_STRING}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_TrSort_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_TrSort_MINOR_VERSION ${PROJECT_VERSION_MINOR}
)
set_property(
    TARGET tr-sort
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${TrSort_MAJOR_VERSION}.${TrSort_MINOR_VERSION}"
)
# inherit common tr-sort compiler options
target_link_libraries(
    tr-sort
        PRIVATE
            $<BUILD_INTERFACE:tr-sort-compiler-options>
)

# install if we're not being built as a sub-tr-sort
if (NOT TR_SORT_SUBPROJECT)
    # library
    install(
        TARGETS tr-sort
        EXPORT TrSortTargets
        # when a static library
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a shared library on UNIX
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a DLL on Windows
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    # public headers
    install(
        DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    # CMake export
    install(
        EXPORT TrSortTargets
        FILE TrSortTargets.cmake
        NAMESPACE TrSort::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/TrSort"
    )
    # CMake package
    include(CMakePackageConfigHelpers)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/TrSortConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/TrSort"
    )
    # generate the version file for the config file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TrSortConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY SameMinorVersion
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TrSortConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TrSortConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/TrSort"
    )
endif()

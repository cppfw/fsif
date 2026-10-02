cmake_minimum_required(VERSION 3.10)

project(fsif)

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)

# declare minizip library

set(minizip_srcs)
myci_add_source_files(minizip_srcs
    DIRECTORY
        ../../src_deps/minizip
    RECURSIVE
)

# WORKAROUND:
# ZLIB exports different named targets depending on VCPKG triplet (static/dynamic),
# So we need to check which ZLIB target we have.
find_package(ZLIB CONFIG REQUIRED)
if(TARGET ZLIB::ZLIB)
    set(zlib_dependency_target ZLIB::ZLIB)
elseif(TARGET ZLIB::ZLIBSTATIC)
    set(zlib_dependency_target ZLIB::ZLIBSTATIC)
else()
    message(FATAL_ERROR "No ZLIB target found")
endif()

myci_declare_library(minizip
    NO_WARNINGS_AS_ERRORS
    NO_ALL_WARNINGS
    SOURCES
        ${minizip_srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src_deps
    DEPENDENCIES
        ${zlib_dependency_target}
    NO_EXPORT
    NO_FORMAT
)

# declare the main library

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${PROJECT_NAME}
    RECURSIVE
)

myci_declare_library(${PROJECT_NAME}
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${PROJECT_NAME}
    DEPENDENCIES
        utki
        ${PROJECT_NAME}::minizip
    NO_EXPORT
)

# export both libraries

myci_export(
    TARGETS
        ${PROJECT_NAME}
        minizip
)

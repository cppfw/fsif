cmake_minimum_required(VERSION 3.10)

set(name fsif)
project(${name})

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${name}
    RECURSIVE
)

set(minizip_srcs)
myci_add_source_files(minizip_srcs
    DIRECTORY
        ../../src_deps/minizip
    RECURSIVE
)

myci_declare_library(minizip
    NO_WARNINGS_AS_ERRORS
    SOURCES
        ${minizip_srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src_deps
    DEPENDENCIES
        ZLIB
    NO_EXPORT
)

myci_declare_library(${name}
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${name}
    DEPENDENCIES
        utki
        ${name}::minizip
    NO_EXPORT
)

myci_export(
    TARGETS
        ${name}
        minizip
)

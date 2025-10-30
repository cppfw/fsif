cmake_minimum_required(VERSION 3.10)

set(name fsif)
project(${name})

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci
    CONFIG
    REQUIRED
    # For host VCPKG dependencies like myci, we need to explicitly tell where to find them,
    # because VCPKG adds only target triplet installed directory to the CMAKE_PREFIX_PATH.
    # If host==target the VCPKG_HOST_TRIPLET is not set, which is fine, because target dir is searched anyway.
    PATHS
        ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_HOST_TRIPLET}/share/myci
)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${name}
    RECURSIVE
)

set(deps_srcs)
myci_add_source_files(deps_srcs
    DIRECTORY
        ../../src_deps
    RECURSIVE
)

myci_declare_library(${name}
    SOURCES
        ${srcs}
        ${deps_srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    PRIVATE_INCLUDE_DIRECTORIES
        ../../src_deps
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${name}
    DEPENDENCIES
        utki
        ZLIB
)

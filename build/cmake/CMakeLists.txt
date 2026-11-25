cmake_minimum_required(VERSION 3.30)

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
# ZLIB CONFIG exports different named targets depending on VCPKG triplet (static/dynamic):
# - ZLIB::ZLIB for dynamically linked triplets
# - ZLIB::ZLIBSTATIC for statically linked triplets
# I.e. the target name differs depending on the environment.
# To avoid the mess we use find_package() for ZLIB in module mode.

# WORKAROUND:
# FindZLIB caches the resolved library files in the CMake cache. If the vcpkg registry
# updates the zlib port to a version that installs the library under different file names
# (e.g. "zd.lib" -> "libzd.lib"), the cached paths become stale and linking fails (LNK1104).
# Invalidate the cache entries that no longer point to existing files so that FindZLIB
# re-resolves them on reconfigure.
foreach(_zlib_cache_var ZLIB_LIBRARY ZLIB_LIBRARY_RELEASE ZLIB_LIBRARY_DEBUG)
    if(DEFINED "${_zlib_cache_var}" AND NOT EXISTS "${${_zlib_cache_var}}")
        unset(${_zlib_cache_var} CACHE)
    endif()
endforeach()

find_package(ZLIB REQUIRED)

myci_declare_library(minizip
    NO_WARNINGS_AS_ERRORS
    NO_ALL_WARNINGS
    SOURCES
        ${minizip_srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src_deps
    DEPENDENCIES
        ZLIB
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

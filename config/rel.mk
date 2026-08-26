include $(config_dir)base/base.mk

this_cxxflags += -O3

# WORKAROUND: on ubuntu jammy dpkg-buildpackage passes -ffat-lto-objects compilation flag
# which is not supported by clang and clang-tidy complains about it:
# error: optimization flag '-ffat-lto-objects' is not supported [clang-diagnostic-ignored-optimization-argument]
# Thus, suppress this warning.
this_cxxflags += -Wno-ignored-optimization-argument

# WORKAROUND: GCC 16 fails with:
# include/c++/16.2.0/bits/stl_algobase.h:424:32: error: 'void* __builtin_memmove(void*, const void*, long long unsigned int)' reading between 2 and 9223372036854775807 bytes from a region of size 0 [-Werror=stringop-overread]
# Which seems like a problem in GCC, so disable the error for now.
this_cxxflags += -Wno-error=stringop-overread

ifeq ($(os),macosx)
    # WORKAROUND:
    # clang-tidy on macos doesn't use /usr/local/include as default place to
    # search for header files, so we add it explicitly
    this_cxxflags += -isystem /usr/local/include
endif

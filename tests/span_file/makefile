include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs += $(call prorab-src-dir, .)

this_ldlibs += -l utki$(this_dbg)

this__libfsif := ../../src/out/$(c)/libfsif$(this_dbg)$(dot_so)

this_ldlibs += $(this__libfsif)

this_no_install := true

$(eval $(prorab-build-app))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name) $(this__libfsif)
this_test_ld_path := ../../src/out/$(c)

# TODO: is needed?
# for conan
this_test_ld_path += $(patsubst -L%,%,$(filter -L%,$(LDFLAGS)))

$(eval $(prorab-test))

# include makefile for building fsif
$(eval $(call prorab-include, ../../src/makefile))

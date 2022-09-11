default: release

.PHONY: default release debug all clean

MU_NOCOLOR=1
include make-utils/flags.mk
include make-utils/cpp-utils.mk

WARNING_FLAGS += -Wno-missing-field-initializers
CXX_FLAGS += -ICatch/include
$(eval $(call use_cpp11))

$(eval $(call src_folder_compile,/intrusive_list))

$(eval $(call add_src_executable,intrusive_list,intrusive_list/bench.cpp))

$(eval $(call add_executable_set,intrusive_list,intrusive_list))

release: release_intrusive_list
debug: debug_intrusive_list

all: release debug

clean:
	rm -rf release/
	rm -rf debug/

-include $(DEBUG_D_FILES)
-include $(RELEASE_D_FILES)

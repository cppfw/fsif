#include "main.hpp"

#include "../../src/fsif/native_file.hpp"

#ifdef assert
#	undef assert
#endif

// NOLINTNEXTLINE(bugprone-exception-escape, "we want uncaught exceptions to fail the tests")
int main(int /* argc */, const char** /* argv */){
	test_fsif_fs_file();

	// test file size
	{
		fsif::native_file f("test.file.txt");

		auto size = f.size();

		utki::assert(size == 66874, [&](auto&o){o << "size = " << size;}, SL);
	}

	return 0;
}

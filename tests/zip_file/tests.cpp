#include <utki/debug.hpp>
#include "../../src/fsif/zip_file.hpp"
#include "../../src/fsif/fs_file.hpp"

#include "tests.hpp"

namespace test_fsif_zip_file{
void run(){
	// list directory contents
	{
		fsif::zip_file zip_f(std::make_unique<fsif::fs_file>("test.zip"));
		utki::assert(!zip_f.is_dir(), SL);
		utki::assert(!zip_f.is_open(), SL);

		zip_f.set_path("./");
		utki::assert(zip_f.is_dir(), SL);
		utki::assert(!zip_f.is_open(), SL);

		{
			auto contents = zip_f.list_dir();

			utki::assert(contents.size() == 3, SL);
			
			utki::assert(contents[0] == "test1.txt", SL);
			utki::assert(contents[1] == "dir1/", SL);
			utki::assert(contents[2] == "dir2/", SL);
		}

		zip_f.set_path("dir1/");
		utki::assert(zip_f.is_dir(), SL);
		utki::assert(!zip_f.is_open(), SL);
		{
			auto contents = zip_f.list_dir();

			utki::assert(contents.size() == 1, SL);
			
			utki::assert(contents[0] == "test2.txt", SL);
		}

		zip_f.set_path("dir2/");
		utki::assert(zip_f.is_dir(), SL);
		utki::assert(!zip_f.is_open(), SL);
		{
			auto contents = zip_f.list_dir();

			utki::assert(contents.size() == 1, SL);
			
			utki::assert(contents[0] == "test3.txt", SL);
		}
	}

	// reading file
	{
		fsif::zip_file zip_f(std::make_unique<fsif::fs_file>("test.zip"), "dir1/test2.txt");
		utki::assert(!zip_f.is_dir(), SL);
		utki::assert(!zip_f.is_open(), SL);

		auto contents = zip_f.load();

		utki::assert(!contents.empty(), SL);

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		std::string str(reinterpret_cast<char*>(contents.data()), contents.size());

		utki::assert(str == "test file #2.\n", [&](auto&o){o << "str = " << str;}, SL);
	}
}
}

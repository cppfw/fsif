#include <utki/debug.hpp>
#include "../../src/fsif/vector_file.hpp"

#include "tests.hpp"




namespace test_basic_memory_file{
void run(){
	fsif::vector_file f;
	utki::assert(!f.is_dir(), SL);
	utki::assert(!f.is_open(), SL);
	utki::assert(f.size() == 0, SL);

	{
		std::array<uint8_t, 4> buf = {1, 2, 3, 4};
		auto b = utki::make_span(buf);
		
		fsif::file::guard file_guard(f, fsif::mode::create);
		
		f.write(b);
	}
	
	{
		std::array<uint8_t, 4> b{};
		
		fsif::file::guard file_guard(f, fsif::mode::read);
		
		f.read(utki::make_span(b));
		
		utki::assert(b[0] == 1, SL);
		utki::assert(b[1] == 2, SL);
		utki::assert(b[2] == 3, SL);
		utki::assert(b[3] == 4, SL);
	}
}
}

#include <fsif/native_file.hpp>

#include <iostream>

int main(int argc, const char** argv){
	fsif::native_file f("some.txt");

	std::cout << "Hello fsif!" << '\n';
}

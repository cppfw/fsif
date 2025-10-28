#include <fsif/fs_file.hpp>

#include <iostream>

int main(int argc, const char** argv){
	fsif::fs_file f("some.txt");

	std::cout << "Hello fsif!" << '\n';
}

#include <iostream>

#include <fsif/native_file.hpp>

int main(int argc, const char** argv){
    fsif::native_file dir("./");

    std::cout << "num files = " << dir.list_dir().size() << std::endl;

    return 0;
}

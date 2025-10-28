#include <fsif/fs_file.hpp>

int main(int argc, const char** argv){
    fsif::fs_file dir("./");

    std::cout << "num files = " << dir.list_dir().size() << std::endl;

    return 0;
}

#include "FileTape.h" 
#include "TapeSorter.h"
#include <iostream>
#include <string>

int main(int argc, char * argv[])
{
    if (argc != 3 && argc != 4) 
    {
        std::cout << "Usage:\n";
        std::cout << "./sorter input_file_tape output_file_tape\n";
        std::cout << "./sorter input_file_tape output_file_tape chunk_size\n";
        return 0;
    }

    size_t chunk_size = (argc == 4 ? std::stoi(argv[4]) : 1000);

    std::string in_path = argv[1];
    std::string out_path = argv[2];

    TapeSorter sorter(chunk_size);

    std::shared_ptr<Tape> in = FileTape::get_instance(in_path);
    std::shared_ptr<Tape> out = FileTape::get_instance(out_path, in->get_capacity());

    FileTape::read_configure_file("configure.txt");

    sorter.sort(in, out);
}
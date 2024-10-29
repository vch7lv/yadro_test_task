#include "FileTape.h"
#include <thread>



FileTape::~FileTape() = default;

std::unique_ptr<Tape> FileTape::get_instance(const std::string& file_path, size_t capacity) 
{
    auto ptr = static_cast<Tape*>( new FileTape(file_path, capacity) );
    return std::unique_ptr<Tape>(ptr);
}

std::unique_ptr<Tape> FileTape::get_instance(const std::string& file_path) 
{
    auto ptr = static_cast<Tape*>( new FileTape(file_path) );
    return std::unique_ptr<Tape>(ptr);
}


FileTape::FileTape(const std::string& file_path, size_t capacity_)  
    : stream(file_path, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc)
{
    capacity = capacity_;
    head_position = 0;
}

FileTape::FileTape(const std::string& file_path)
    : stream(file_path, std::ios::out | std::ios::in | std::ios::binary)
{
    stream.seekg(0, std::ios::end);
    capacity = stream.tellg() / 4;
    stream.seekg(0);

    head_position = 0;
}

void FileTape::rewind() const
{
    std::this_thread::sleep_for(rewind_delay);

    if (head_position != 0)
    {
        head_position = 0;
        stream.seekg(0);
    }
}


void FileTape::head_move_left() const
{
    std::this_thread::sleep_for(head_move_delay);

    if (head_position == 0) 
    {
        throw std::runtime_error("FileTape out of range");
    }
    
    --head_position;

    stream.seekg(head_position*4);
}

void FileTape::head_move_right() const
{
    std::this_thread::sleep_for(head_move_delay);

    if (head_position == capacity-1) 
    {
        throw std::runtime_error("FileTape out of range");
    }

    ++head_position;

    stream.seekg(head_position*4);
}


int FileTape::read_from_head() const
{
    std::this_thread::sleep_for(read_delay);

    char bytes[4];
    stream.read(bytes, 4);

    stream.seekg(head_position*4);

    return *reinterpret_cast<int*>(bytes);
}

void FileTape::write_on_head(int value) 
{
    std::this_thread::sleep_for(write_delay);

    stream.write((char*)&value, sizeof(int));

    stream.seekg(head_position*4);
}

void FileTape::read_configure_file(const std::string& file_path) 
{
    std::ifstream configure_file;
    configure_file.open(file_path);

    for (int i = 0; i < 4; ++i)
    {
        std::string name;
        unsigned value;

        configure_file >> name >> value;

        if (name == "rewind_delay") {
            rewind_delay = std::chrono::milliseconds(value);
        }
        if (name == "head_move_delay") {
            head_move_delay = std::chrono::milliseconds(value);
        }
        if (name == "read_delay") {
            read_delay = std::chrono::milliseconds(value);
        }
        if (name == "write_delay") {
            write_delay = std::chrono::milliseconds(value);
        }
    }
}


std::chrono::milliseconds FileTape::rewind_delay {0};
std::chrono::milliseconds FileTape::head_move_delay {0};
std::chrono::milliseconds FileTape::read_delay {0};
std::chrono::milliseconds FileTape::write_delay {0};